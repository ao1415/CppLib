#pragma once

#include "Basic.hpp"

#include <array>
#include <vector>
#include <unordered_set>
#include <unordered_map>

#include <stack>

#include <cstdint>
#include <cassert>

#include <string>
#include <sstream>
#include <iomanip>

#include <memory>
#include <chrono>

namespace alib {

	namespace BeamSearch::Lib {

		using SizeType = size_t;

		NODISCARD constexpr SizeType kbToB(unsigned long long kByte) noexcept { return narrow_cast<SizeType>(kByte * 1024); }
		NODISCARD constexpr SizeType mbToB(unsigned long long mByte) noexcept { return kbToB(mByte * 1024); }

		NODISCARD constexpr SizeType operator"" _KB(unsigned long long kByte) noexcept { return kbToB(kByte); }
		NODISCARD constexpr SizeType operator"" _MB(unsigned long long mByte) noexcept { return mbToB(mByte); }

		class Timer {
		private:
			using Rep = std::chrono::nanoseconds;
			using Duration = std::chrono::microseconds;
			using Clock = std::chrono::high_resolution_clock;
			std::chrono::time_point<Clock> startPoint;
		public:
			/** @brief コンストラクタ */
			Timer() = default;

			/** @brief タイマー開始 */
			void start() noexcept { startPoint = Clock::now(); }
			/** @brief 経過時間（ms） */
			inline double interval() const noexcept {
				constexpr auto basicDuration = std::chrono::milliseconds(1);
				constexpr auto duration = std::chrono::duration_cast<Rep>(basicDuration).count();
				return std::chrono::duration_cast<Rep>(Clock::now() - startPoint).count() / static_cast<double>(duration);
			}
		};

		template <class Type, class Compare = std::less<typename std::vector<Type>::value_type>>
		class ExPriorityQueue : public std::priority_queue<Type, std::vector<Type>, Compare> {
		public:
			auto& container() noexcept { return this->c; }
			const auto& container() const noexcept { return this->c; }
			void clear() noexcept { this->c.clear(); }
		};

		template<SizeType MaxMemorySize = 1_MB>
		class MemoryPool {
		public:

			using ValueType = std::byte;
			using Pointer = ValueType*;
			using PointerConst = ValueType const*;

			using SharedPtr = std::shared_ptr<ValueType[]>;

		private:

			struct Ref {
				uintptr_t basePtr = 0;
				int count = 0;
				SharedPtr memory{};
			};

			SharedPtr memory;
			SizeType memorySize = 0;

			std::list<SharedPtr> unusedMemory;
			std::list<Ref> refMemory;

			inline uintptr_t toInteger(const PointerConst p) const noexcept {
				WARN_PUSH(disable:26490);
				return reinterpret_cast<uintptr_t>(p);
				WARN_POP();
			}

		public:

			inline Pointer alloc(const SizeType size) {
				assert(size < MaxMemorySize);

				if (memorySize + size >= MaxMemorySize) {
					if (unusedMemory.empty()) {
						WARN_PUSH(disable:26409);
						memory = SharedPtr(new ValueType[MaxMemorySize]);
						WARN_POP();
					}
					else {
						memory = unusedMemory.front();
						unusedMemory.pop_front();
					}
					refMemory.emplace_back(Ref{ toInteger(memory.get()), 0,memory });
					memorySize = 0;
				}

				WARN_PUSH(disable:26446);
				Pointer ret = std::addressof(memory[memorySize]);
				WARN_POP();
				memorySize += size;
				refMemory.back().count++;
				return ret;
			}

			inline void release(const Pointer data) {

				const auto ptr = toInteger(data);
				for (auto ite = refMemory.begin(); ite != refMemory.end();) {
					if (ite->basePtr <= ptr && ptr < ite->basePtr + MaxMemorySize) {
						ite->count--;

						if (ite->count <= 0) {
							unusedMemory.push_back(ite->memory);
							ite = refMemory.erase(ite);
						}
						return;
					}
					++ite;
				}
				assert(false);
			}

		};

		class VersionControl {
		public:

			using ValueType = std::byte;
			using Pointer = ValueType*;

			using VersionSizeType = unsigned short;
		private:

			struct Stream {
				Pointer data = nullptr;
				SizeType pos = 0;
				SizeType length = 0;

				Stream() = default;
				Stream(Pointer data, const SizeType pos, const SizeType length) noexcept
					: data(data), pos(pos), length(length) {}

				inline Pointer address() const noexcept {
					WARN_PUSH(disable:26481);
					return std::addressof(data[pos]);
					WARN_POP();
				}

				template<typename Ptr>
				inline Ptr reinterpret() const noexcept {
					WARN_PUSH(disable:26490);
					return reinterpret_cast<Ptr>(address());
					WARN_POP();
				};

				template<typename Type>
				inline Type get() noexcept {
					assert(pos + sizeof(Type) <= length);
					Type ret = *(reinterpret<Type*>());
					pos += sizeof(Type);
					return ret;
				}

				template<typename Type>
				inline Type pop() noexcept {
					assert(sizeof(Type) <= pos);
					pos -= sizeof(Type);
					return *(reinterpret<Type*>());
				}

				inline void pop(Stream& stream, const SizeType size) noexcept {
					assert(size <= pos);
					assert(stream.pos + size <= stream.length);
					pos -= size;
					std::memcpy(stream.address(), address(), size);
					stream.pos += size;
				}

				template<typename Type>
				inline void set(const Type value) noexcept {
					assert(pos + sizeof(Type) <= length);
					*(reinterpret<Type*>()) = value;
					pos += sizeof(Type);
				}

				inline void copy(void* ptr, const SizeType size) noexcept {
					assert(pos + size <= length);
					std::memcpy(address(), ptr, size);
					pos += size;
				}

				inline void seek(const SizeType size) noexcept {
					assert(pos + size <= length);
					pos += size;
				}

				inline void write(void* ptr, const SizeType size) noexcept {
					assert(pos + size <= length);
					std::memcpy(ptr, address(), size);
					pos += size;
				}

				inline bool endOfStream() const noexcept {
					return pos == length;
				}

				inline bool hasStream() const noexcept {
					return pos < length;
				}

			};

			template<SizeType Length>
			struct Buffer {

				/** @brief バッファストリーム */
				Stream stream;

				/** @brief 書き込まれたデータサイズ */
				SizeType redoSize = 0;

				/** @brief ロック */
				std::vector<std::pair<SizeType, SizeType>> locks{};
				/** @brief 変更記録バッファ */
				ValueType buffer[Length]{};

				Buffer() noexcept {
					WARN_PUSH(disable:26485);
					stream = Stream(buffer, 0, Length);
					WARN_POP();
				}

				inline bool isEnabled() noexcept { return !locks.empty(); }

				inline void lock() {
					locks.emplace_back(stream.pos, redoSize);
					redoSize = 0;
				}

				inline void unlock() noexcept {
					assert(isEnabled());
					std::tie(stream.pos, redoSize) = locks.back();
					locks.pop_back();
				}

				/** @brief 変更蓄積 */
				inline void modify(void* data, const VersionSizeType size) noexcept {
					if (!isEnabled()) {
						return;
					}

					const SizeType writeSize = size + sizeof(VersionSizeType) + sizeof(void*);

					assert(1 <= size);
					assert(size <= std::numeric_limits<VersionSizeType>::max());
					assert(stream.pos + writeSize <= Length);

					stream.copy(data, size);
					stream.set<VersionSizeType>(size);
					stream.set<void*>(data);

					redoSize += size;
				}

				inline SizeType getBufferSize() const noexcept { return stream.pos - locks.back().first; }
			};

			inline static Buffer<256_KB> buffer{};
			MemoryPool<> pool;

		public:

			class Patch {
			private:
				Pointer data = nullptr;
				SizeType length = 0;
			public:

				Patch() = default;
				Patch(Pointer data, const SizeType length) noexcept
					: data(data), length(length) {}

				inline bool hasValue() const noexcept { return length != 0; }

				/** @brief 戻す */
				inline void undo() const noexcept {
					assert(data != nullptr);

					Stream stream(data, 0, length);
					while (stream.hasStream()) {
						const auto ptr = stream.get<void*>();
						const auto size = stream.get<VersionSizeType>();
						stream.seek(size);
						stream.write(ptr, size);
					}
					assert(stream.endOfStream());
				}
				/** @brief 進める */
				inline void redo() const noexcept {
					assert(data != nullptr);

					Stream stream(data, 0, length);
					while (stream.hasStream()) {
						const auto ptr = stream.get<void*>();
						const auto size = stream.get<VersionSizeType>();
						stream.write(ptr, size);
						stream.seek(size);
					}
					assert(stream.endOfStream());
				}
			};

			inline static void lock() { buffer.lock(); }
			inline static void unlock() noexcept { buffer.unlock(); }

			inline static void modify(void* mem, const SizeType size) noexcept {
				buffer.modify(mem, narrow_cast<VersionSizeType>(size));
			}

			/** @brief 差分パッチ作成 */
			inline Patch commit() {
				assert(buffer.isEnabled());

				const SizeType wholeSize = buffer.getBufferSize() + buffer.redoSize;
				Stream stream(pool.alloc(wholeSize), 0, wholeSize);

				while (stream.hasStream()) {

					const auto ptr = buffer.stream.pop<void*>();
					stream.set<void*>(ptr);

					const auto size = buffer.stream.pop<VersionSizeType>();
					stream.set<VersionSizeType>(size);

					// 変更後の値
					stream.copy(ptr, size);

					// 変更前の値
					buffer.stream.pop(stream, size);
				}
				assert(stream.endOfStream());
				buffer.redoSize = 0;

				return Patch(stream.data, stream.length);
			}

		};

		template<typename Argument, typename ScoreTy = double>
		struct DefaultSearchArgument {
			inline static constexpr bool HasHash = false;

			using ScoreType = ScoreTy;

			ScoreType score{};
			Argument argument{};

			DefaultSearchArgument() = default;
			DefaultSearchArgument(const ScoreType score, const Argument& argument) : score(score), argument(argument) {}
			DefaultSearchArgument(const ScoreType score) noexcept : score(score) {}
			template<class... Args>
			DefaultSearchArgument(const ScoreType score, Args&& ...args) : score(score), argument(Argument(std::forward<Args>(args)...)) {}
		};

		template<typename Argument, typename HashTy = size_t, typename ScoreTy = double>
		struct HashSearchArgument {
			inline static constexpr bool HasHash = true;

			using ScoreType = ScoreTy;
			using HashType = HashTy;

			HashType hash;
			ScoreType score;
			Argument argument;

			HashSearchArgument() = default;
			HashSearchArgument(const HashType hash, const ScoreType score, const Argument& argument) : hash(hash), score(score), argument(argument) {}
			HashSearchArgument(const HashType hash, const ScoreType score) : hash(hash), score(score), argument(Argument()) {}
			template<class... Args>
			HashSearchArgument(const HashType hash, const ScoreType score, Args&& ...args) : hash(hash), score(score), argument(Argument(std::forward<Args>(args)...)) {}
		};

		/**
		 * @brief 探査ノードの構造体
		 * @tparam Argument 探索引数
		*/
		template<typename Argument>
		struct SearchNode {

			using Pointer = SearchNode*;

			// TODO:実装
			static Pointer Create() noexcept {
				return nullptr;
			}

			// TODO:実装
			static Pointer Create(Pointer parent, const Argument& arg) noexcept {
				return nullptr;
			}

			/** @brief 親ノードのポインタ */
			Pointer parent = nullptr;
			/** @brief 探査引数 */
			Argument argument;
			/** @brief ノード遷移パッチ */
			VersionControl::Patch patch;
			/** @brief ノード深度 */
			int depth;

			// TODO:実装
			void release() const noexcept {

			}

		};

		template<SizeType Depth, SizeType Witdh, SizeType Limit, typename Argument, typename SearchArgument>
		class BeamSearchProcess {
		private:

			using Node = SearchNode<SearchArgument>;
			using NodePointer = typename Node::Pointer;

			using RankingItem = std::pair<double, NodePointer>;
			using Ranking = ExPriorityQueue<RankingItem>;

			// TODO:初期化方法
			inline static constexpr bool enabelDebug = false;
			inline static Timer timer;

			/** @brief 現在の探査ノード */
			NodePointer currentNode = nullptr;
			/** @brief 次の探査ノード */
			NodePointer nextNode = nullptr;

			/** @brief 現在の探査ノードキュー */
			Ranking currentRanking;
			/** @brief 次のの探査ノードキュー */
			Ranking nextRanking;
			/** @brief 現在の探査ノードキューの低スコアキュー */
			std::priority_queue<double, std::vector<double>, std::greater<>> currentScoreRinking;

			/** @brief 残探査深度 */
			int remainDepth = Depth;
			/** @brief 次深度の探査時間（ms） */
			double nextLimit = static_cast<double>(Limit) / Depth;

			VersionControl version;

			/** @brief 探査終了処理 */
			void destruction() noexcept {
				VersionControl::unlock();
			}

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

				std::array<VersionControl::Patch, Depth> redos{};
				int stack = Depth;

				while (currentRoot != nextRoot) {
					assert(currentRoot != nullptr);
					assert(nextRoot != nullptr);
					if (currentRoot->depth < nextRoot->depth) {
						assert(nextRoot->patch.hasValue());
						stack--;
						WARN_PUSH(disable:26446 26482);
						redos[stack] = nextRoot->patch;
						WARN_POP();
						nextRoot = nextRoot->parent;
					}
					else {
						assert(currentRoot->patch.hasValue());
						currentRoot->patch.undo();
						currentRoot = currentRoot->parent;
					}
				}
				forstep(idx, stack, Depth) {
					WARN_PUSH(disable:26446 26482);
					redos[idx].redo();
					WARN_POP();
				}
			}

			void clearRanking(Ranking& rank) const noexcept {
				const auto& container = rank.container();
				for (const auto& node : container) {
					node.second->release();
				}
				rank.clear();
			}

			double getNextLimit(const double interval) const noexcept {
				return ((Limit - interval) / remainDepth) + interval;
			}

		public:

			BeamSearchProcess() noexcept = default;

			void timerStart() noexcept { timer.start(); }

			void init() {
				remainDepth = Depth;

				nextLimit = getNextLimit(timer.interval());

				currentNode = Node::Create();

				clearRanking(currentRanking);
				clearRanking(nextRanking);

				VersionControl::lock();
			}

			bool onloop() {
				if (nextNode != nullptr) {
					nextNode->release();
					nextNode = nullptr;
				}
				else {
					currentNode->patch = version.commit();
				}

				const double interval = timer.interval();
				if (nextLimit <= interval || currentRanking.empty()) {
					if (remainDepth <= 0) {
						destruction();
						return false;
					}

					nextLimit = getNextLimit(interval);
					remainDepth--;
					clearRanking(currentRanking);
					std::swap(currentRanking, nextRanking);
				}

				if (currentRanking.empty()) {
					destruction();
					return false;
				}

				nextNode = currentRanking.top().second;
				currentRanking.pop();

				return true;
			}

			void accept() noexcept {
				assert(nextNode != nullptr);
				transitin(currentNode, nextNode);
				currentNode->release();
				currentNode = nextNode;
				nextNode = nullptr;
			}

			void reserve(const SearchArgument& arg) {
				if (Witdh <= nextRanking.size()) {
					if (arg.score <= currentScoreRinking.top()) {
						return;
					}
					else {
						currentScoreRinking.pop();
					}
				}
				nextRanking.emplace(arg.score, Node::Create(currentNode, arg));
				currentScoreRinking.push(arg.score);
			}

			SearchArgument getArgument() const noexcept {
				assert(nextNode != nullptr);
				return nextNode->argument;
			}
		};
	}

	/**
	 * @brief ビームサーチ用テンプレートクラス
	 * @tparam Depth ビーム深度
	 * @tparam Witdh ビーム幅
	 * @tparam Limit サーチ時間（ms）
	 * @tparam Argument 状態引数
	 * @tparam SearchArgument 探査タイプ
	*/
	template<BeamSearch::Lib::SizeType Depth, BeamSearch::Lib::SizeType Witdh, BeamSearch::Lib::SizeType Limit, typename Argument, typename SearchArgument = BeamSearch::Lib::DefaultSearchArgument<Argument>>
	class BeamSearchTemplate {
	public:

		using ArgumentType = SearchArgument;

		BeamSearchTemplate() noexcept = default;
		BeamSearchTemplate(BeamSearchTemplate&&) = default;
		BeamSearchTemplate& operator=(BeamSearchTemplate&&) = default;
		virtual ~BeamSearchTemplate() = default;

		// コピー禁止
		BeamSearchTemplate(const BeamSearchTemplate&) = delete;
		BeamSearchTemplate& operator=(const BeamSearchTemplate&) = delete;

	private:

		// 探査済みハッシュ
		// TODO:ハッシュ化した値を直接使用する方法
		std::unordered_set<size_t> visited;

		// ビームサーチ処理
		BeamSearch::Lib::BeamSearchProcess<Depth, Witdh, Limit, Argument, SearchArgument> process;

		/**
		 * @brief ループ処理
		 * @param process 次の状態処理の引数
		*/
		void loop() {
			while (process.onloop()) {
				if constexpr (SearchArgument::HasHash) {
					const auto& arg = process.getArgument();
					if (visited.find(arg.hash) != visited.end()) { continue; }
					visited.insert(arg.hash);
					process.accept();
					search(arg);
				}
				else {
					const auto& arg = process.getArgument();
					process.accept();
					search(arg);
				}
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
		inline void nextSearch(SearchArgument arg) { process.reserve(arg); }

	public:

		void start(const ArgumentType& arg) {
			process.timerStart();
			process.init();
			process.reserve(arg);
			loop();
		}

		virtual void search(ArgumentType) = 0;

	};

}
