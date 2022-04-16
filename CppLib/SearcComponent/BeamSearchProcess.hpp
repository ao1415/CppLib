#pragma once

#include "SearchConfig.hpp"
#include "SearchComponentLib.hpp"
#include "SearchNode.hpp"
#include "ExPriorityQueue.hpp"
#include "Timer.hpp"

namespace alib::Search::Lib {
	using DebugConfig = ConfigTemplate<10, 100, 1000, DefaultSearchMethod<int, double, size_t>>;

	/**
	 * @brief �r�[���T�[�`����
	 * @tparam Config
	*/
	template<class ConfigClass, class Space>
	class BeamSearchProcess {
	public:
		using Config = ConfigClass;
	private:
		using ArgumentType = typename Config::ArgumentType;
		using SearchMethod = typename Config::SearchMethod;
		using Node = SearchNode<Config>;
		using NodePointer = Node*;
		using RankingItem = std::pair<typename Config::ScoreType, NodePointer>;
		using Ranking = ExPriorityQueue<RankingItem>;
		using IMemo = MemoSingleton<Space>;

		/** @brief ���݂̒T���m�[�h */
		NodePointer currentNode = nullptr;
		/** @brief ���̒T���m�[�h */
		NodePointer nextNode = nullptr;

		/** @brief ���݂̒T���m�[�h�L���[ */
		Ranking currentRanking;
		/** @brief ���̂̒T���m�[�h�L���[ */
		Ranking nextRanking;
		int nodeWidthCount = 0;

		/** @brief �c�T���[�x */
		int remainDepth = narrow_cast<int>(Config::GetDepth());
		/** @brief ���[�x�̒T�����ԁims�j */
		double nextLimit = static_cast<double>(Config::GetLimit()) / Config::GetDepth();

		/** @brief �T���^�C�}�[ */
		Timer timer;
		/** @brief �ύX�Ǘ� */
		Memo version;
		/** @brief �m�[�h�Ǘ� */
		SearchNodePool<Config> nodePool;

		void destruction(const double t) {
			release(currentNode);
			IMemo::Get().unlock();
		}
		NODISCARD double getNextLimit(const double interval) const noexcept {
			return ((Config::GetLimit() - interval) / remainDepth) + interval;
		}

		/**
		 * @brief ��ԑJ��
		 * @param current �J�ڌ�
		 * @param next �J�ڐ�
		*/
		void transitin(NodePointer current, NodePointer next) noexcept {
			assert(next != nullptr);
			assert(next->parent != nullptr);
			assert(!next->patch.hasValue());

			NodePointer currentRoot = current;
			NodePointer nextRoot = next->parent;

			std::array<Memo::Patch, Config::GetDepth()> redos{};
			int stack = narrow_cast<decltype(stack)>(Config::GetDepth());

			while (currentRoot != nextRoot) {
				assert(currentRoot != nullptr);
				assert(nextRoot != nullptr);
				if (currentRoot->depth < nextRoot->depth) {
					if (nextRoot->patch.hasValue()) {
						stack--;
						WARN_PUSH_DISABLE(26446 26482);
						redos[stack] = nextRoot->patch;
						WARN_POP();
					}
					nextRoot = nextRoot->parent;
				}
				else {
					if (currentRoot->patch.hasValue()) {
						currentRoot->patch.undo();
					}
					currentRoot = currentRoot->parent;
				}
			}
			forstep(idx, stack, Config::GetDepth()) {
				WARN_PUSH_DISABLE(26446 26482);
				redos[idx].redo();
				WARN_POP();
			}
		}

		void clearRanking(Ranking& rank) {
			for (const auto& node : rank) {
				release(node.second);
			}
			rank.clear();
		}

		void release(NodePointer node) {
			assert(node != nullptr);
			assert(0 < node->ref);

			if (node->ref == 1) {
				if (node->parent != nullptr) { release(node->parent); }
				if (node->patch.hasValue()) { version.release(node->patch); }
				nodePool.release(node);
			}
			else {
				node->subRef();
			}
		}
	public:
		void timerStart() noexcept { timer.start(); }

		void init() {
			remainDepth = narrow_cast<decltype(remainDepth)>(Config::GetDepth());
			const auto interval = timer.interval();
			nextLimit = getNextLimit(interval);

			currentNode = new(nodePool.alloc()) Node();

			clearRanking(currentRanking);
			clearRanking(nextRanking);
			nodeWidthCount = 0;

			version.lock();
		}

		NODISCARD bool onloop() {
			if (nextNode != nullptr) {
				// �n�b�V����v�ŃX�L�b�v���ꂽ�ꍇ
				release(nextNode);
				nextNode = nullptr;
			}
			else {
				currentNode->patch = version.commit();
			}

			const double interval = timer.interval();
			if (nextLimit <= interval || currentRanking.empty() || nodeWidthCount >= Config::GetWidth()) {
				if (remainDepth <= 0) {
					destruction(interval);
					return false;
				}

				nextLimit = getNextLimit(interval);
				remainDepth--;
				clearRanking(currentRanking);
				std::swap(currentRanking, nextRanking);
				nodeWidthCount = 0;
			}

			if (currentRanking.empty()) {
				destruction(interval);
				return false;
			}

			nextNode = currentRanking.top().second;
			currentRanking.pop();

			return true;
		}

		void accept() {
			assert(nextNode != nullptr);
			nodeWidthCount++;
			transitin(currentNode, nextNode);
			release(currentNode);
			currentNode = nextNode;
			nextNode = nullptr;
		}

		void reserve(const SearchMethod& argument) {
			NodePointer ptr = new(nodePool.alloc()) Node(currentNode, argument.argument);
			nextRanking.emplace(argument.score, ptr);
		}

		NODISCARD const ArgumentType& getArgument() const noexcept {
			assert(nextNode != nullptr);
			return nextNode->argument;
		}
		NODISCARD bool endOfSearch() const noexcept {
			return remainDepth == 0;
		}
		NODISCARD int getDepth() const noexcept {
			return Config::GetDepth() - remainDepth - 2;
		}

	};
}
