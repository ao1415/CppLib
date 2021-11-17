#pragma once

#include "SearchLib.hpp"

namespace alib {

	namespace Search::Lib {

		template<bool IsDebug>
		struct Info {
			int acceptCount = 0;
			int depthAcceptCount = 0;
			int loopCount = 0;
			int depthLoopCount = 0;
			int depth = 0;
			double startTime = 0;
			double depthStartTime = 0;

			void init(const double t) noexcept {
				acceptCount = 0;
				depthAcceptCount = 0;
				loopCount = 0;
				depthLoopCount = 0;
				depth = 0;
				startTime = t;
				depthStartTime = t;
			}

			void nextDepth(const double t) {
				if constexpr (IsDebug) {
					if (1 <= loopCount - depthLoopCount) {
						std::cerr << "Info[" << depth - 1 << "]"
							<< depthStartTime << "->" << t << "(" << t - depthStartTime << ")"
							<< acceptCount - depthAcceptCount << "/" << loopCount - depthLoopCount << "..." << (static_cast<double>(acceptCount) - depthAcceptCount) / (t - startTime)
							<< std::endl;
					}
					depthStartTime = t;
					depthAcceptCount = acceptCount;
					depthLoopCount = loopCount;
					++depth;
				}
			}
		};

		template<class Config>
		class BeamSearchProcess {
		private:
			using SearchArgument = typename Config::SearchArgumentType;
			using Node = SearchNode<Config>;
			using NodePointer = Node*;
			using RankingItem = std::pair<double, NodePointer>;
			using Ranking = ExPriorityQueue<RankingItem>;

			/** @brief 現在の探査ノード */
			NodePointer currentNode = nullptr;
			/** @brief 次の探査ノード */
			NodePointer nextNode = nullptr;

			/** @brief 現在の探査ノードキュー */
			Ranking currentRanking;
			/** @brief 次のの探査ノードキュー */
			Ranking nextRanking;
			int nodeWidthCount = 0;

			/** @brief 残探査深度 */
			int remainDepth = Config::GetDepth();
			/** @brief 次深度の探査時間（ms） */
			double nextLimit = static_cast<double>(Config::GetLimit()) / Config::GetDepth();

			/** @brief 探査タイマー */
			Timer timer;
			/** @brief 変更管理 */
			VersionControl version;
			/** @brief ノード管理 */
			SearchNodePool<Config> nodePool;

			/** @brief 情報管理 */
			Info<Config::IsDebug()> info;

			void destruction(const double t) {
				release(currentNode);
				VersionControl::Unlock();
				info.nextDepth(t);
			}
			NODISCARD double getNextLimit(const double interval) const noexcept { return ((Config::GetLimit() - interval) / remainDepth) + interval; }

			/**
			 * @brief 状態遷移
			 * @param current 遷移元
			 * @param next 遷移先
			*/
			void transitin(NodePointer current, NodePointer next) noexcept {
				assert(next != nullptr);
				assert(next->parent != nullptr);
				assert(!next->patch.hasValue());

				NodePointer currentRoot = current;
				NodePointer nextRoot = next->parent;

				std::array<VersionControl::Patch, Config::GetDepth()> redos{};
				int stack = Config::GetDepth();

				while (currentRoot != nextRoot) {
					assert(currentRoot != nullptr);
					assert(nextRoot != nullptr);
					if (currentRoot->depth < nextRoot->depth) {
						if (nextRoot->patch.hasValue()) {
							stack--;
							WARN_PUSH(disable:26446 26482);
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
					WARN_PUSH(disable:26446 26482);
					redos[idx].redo();
					WARN_POP();
				}
			}

			void clearRanking(Ranking& rank) {
				const auto& container = rank.container();
				for (const auto& node : container) { release(node.second); }
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
				remainDepth = Config::GetDepth();
				const auto interval = timer.interval();
				nextLimit = getNextLimit(interval);
				info.init(interval);

				currentNode = new(nodePool.alloc()) Node();

				clearRanking(currentRanking);
				clearRanking(nextRanking);
				nodeWidthCount = 0;

				VersionControl::Lock();
			}

			NODISCARD bool onloop() {
				if (nextNode != nullptr) {
					// ハッシュ一致でスキップされた場合
					release(nextNode);
					nextNode = nullptr;
				}
				else {
					currentNode->patch = version.commit();
				}

				const double interval = timer.interval();
				if (nextLimit <= interval ||
					currentRanking.empty() ||
					nodeWidthCount >= Config::GetWidth()) {
					info.nextDepth(interval);
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

				info.loopCount++;
				return true;
			}

			void accept() {
				assert(nextNode != nullptr);
				info.acceptCount++;
				nodeWidthCount++;
				transitin(currentNode, nextNode);
				release(currentNode);
				currentNode = nextNode;
				nextNode = nullptr;
			}

			void reserve(const SearchArgument& argument) { nextRanking.emplace(argument.score, new(nodePool.alloc()) Node(currentNode, argument)); }

			NODISCARD inline const SearchArgument& getArgument() const noexcept { assert(nextNode != nullptr); return nextNode->argument; }
			NODISCARD inline bool endOfSearch() const noexcept { return remainDepth == 0; }
			NODISCARD inline int getDepth() const noexcept { return Config::GetDepth() - remainDepth - 2; }
		};

		/**
		 * @brief ビームサーチ用テンプレートクラス
		 * @tparam Config 設定
		*/
		template<class Config>
		class BeamSearchTemplate {
		public:
			using SearchArgument = typename Config::SearchArgumentType;

			BeamSearchTemplate() = default;
			virtual ~BeamSearchTemplate() = default;
			BeamSearchTemplate(BeamSearchTemplate&&) = default;
			BeamSearchTemplate& operator=(BeamSearchTemplate&&) = default;

			BeamSearchTemplate(const BeamSearchTemplate&) = delete;
			BeamSearchTemplate& operator=(const BeamSearchTemplate&) = delete;
		private:
			// 探査済みハッシュ
			// TODO:ハッシュ化した値を直接使用する方法
			std::unordered_set<size_t> visited{};
			BeamSearchProcess<Config> process{};

			void loop() {
				if (process.onloop()) {
					const auto& argument = process.getArgument();
					if constexpr (SearchArgument::HasHash) {
						visited.insert(argument.hash);
					}
					process.accept();
					init(argument);
				}
				while (process.onloop()) {
					const auto& argument = process.getArgument();
					if constexpr (SearchArgument::HasHash) {
						if (visited.find(argument.hash) != visited.end()) {
							continue;
						}
						visited.insert(argument.hash);
					}
					process.accept();
					search(argument);
				}
			}
		protected:
			/**
			 * @brief 次の状態処理
			 * @param args 次の状態処理の引数
			*/
			template<class... Args>
			inline void nextSearch(Args&& ...args) { nextSearch(SearchArgument(std::forward<Args>(args)...)); }

			/**
			 * @brief 次の状態処理
			 * @param arg 次の状態処理の引数
			*/
			inline void nextSearch(SearchArgument argument) { process.reserve(argument); }

			NODISCARD inline bool endOfSearch() const noexcept { return process.endOfSearch(); }
			NODISCARD inline int getDepth() const noexcept { return process.getDepth(); }
		public:
			void start(const SearchArgument& argument) {
				visited.clear();
				process.timerStart();
				process.init();
				process.reserve(argument);
				loop();
			}

			/**
			 * @brief 初回のパターンを生成する
			 * @param 引数
			*/
			virtual void init(const SearchArgument&) = 0;
			/**
			 * @brief 探査処理
			 * @param 引数
			*/
			virtual void search(const SearchArgument&) = 0;
		};
	}

	template<typename Argument>
	struct SearchArgumentConfig {
		using ArgumentType = Argument;
		using ScoreType = double;
		using HashType = uint64_t;
	};

	template<Search::Lib::SizeType Depth, Search::Lib::SizeType Width, Search::Lib::SizeType Limit, typename Argument, typename SearchArgument = Search::Lib::DefaultSearchArgument<SearchArgumentConfig<Argument>>, bool Debug = false>
	struct BeamSearchConfig {
		using BeamBase = Search::Lib::BeamSearchTemplate<BeamSearchConfig>;

		using ArgumentType = Argument;
		using SearchArgumentType = SearchArgument;

		NODISCARD static constexpr bool IsDebug() { return Debug; }

		NODISCARD static constexpr int GetDepth() { return narrow_cast<int>(Depth + 1); }
		NODISCARD static constexpr int GetWidth() { return narrow_cast<int>(Width); }
		NODISCARD static constexpr int GetLimit() { return narrow_cast<int>(Limit); }
	};
}
