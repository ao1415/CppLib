#pragma once

#include "Basic.hpp"

#include <array>
#include <vector>
#include <unordered_set>

#include <cassert>

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
			NODISCARD inline double interval() const noexcept {
				constexpr auto basicDuration = std::chrono::milliseconds(1);
				constexpr auto duration = std::chrono::duration_cast<Rep>(basicDuration).count();
				return std::chrono::duration_cast<Rep>(Clock::now() - startPoint).count() / static_cast<double>(duration);
			}
		};

		template <class Type, class Compare = std::less<typename std::vector<Type>::value_type>>
		class ExPriorityQueue : public std::priority_queue<Type, std::vector<Type>, Compare> {
		public:
			NODISCARD auto& container() noexcept { return this->c; }
			NODISCARD const auto& container() const noexcept { return this->c; }
			void clear() noexcept { this->c.clear(); }
		};

		template<class Class = std::byte, SizeType MaxMemorySize = 1_MB>
		class MemoryPool {
		public:

			using ValueType = Class;
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
			SizeType memorySize = MaxMemorySize;

			std::list<SharedPtr> unusedMemory;
			std::list<Ref> refMemory;

			NODISCARD inline uintptr_t toInteger(const PointerConst p) const noexcept {
				WARN_PUSH(disable:26490);
				return reinterpret_cast<uintptr_t>(p);
				WARN_POP();
			}

		public:

			NODISCARD inline Pointer alloc(const SizeType size) {
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

			inline void release(const PointerConst data) {

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

				NODISCARD inline Pointer address() const noexcept {
					WARN_PUSH(disable:26481);
					return std::addressof(data[pos]);
					WARN_POP();
				}

				template<typename Ptr>
				NODISCARD inline Ptr reinterpret() const noexcept {
					WARN_PUSH(disable:26490);
					return reinterpret_cast<Ptr>(address());
					WARN_POP();
				};

				template<typename Type>
				NODISCARD inline Type get() noexcept {
					assert(pos + sizeof(Type) <= length);
					Type ret = *(reinterpret<Type*>());
					pos += sizeof(Type);
					return ret;
				}

				template<typename Type>
				NODISCARD inline Type pop() noexcept {
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

				NODISCARD inline bool endOfStream() const noexcept {
					return pos == length;
				}

				NODISCARD inline bool hasStream() const noexcept {
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

				NODISCARD inline bool isEnabled() noexcept { return !locks.empty(); }

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

				NODISCARD inline SizeType getBufferSize() const noexcept { return stream.pos - locks.back().first; }
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

				NODISCARD inline bool hasValue() const noexcept { return length != 0; }
				NODISCARD inline Pointer address() const noexcept { return data; }

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

			inline static void Lock() { buffer.lock(); }
			inline static void Unlock() noexcept { buffer.unlock(); }

			inline static void Modify(void* mem, const SizeType size) noexcept {
				buffer.modify(mem, narrow_cast<VersionSizeType>(size));
			}

			template<class Class>
			inline static void Modify(Class& mem) noexcept {
				WARN_PUSH(disable:26474);
				buffer.modify(reinterpret_cast<void*>(std::addressof(mem)), narrow_cast<VersionSizeType>(sizeof(Class)));
				WARN_POP();
			}

			/** @brief 差分パッチ作成 */
			NODISCARD inline Patch commit() {
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

			inline void release(const Patch& patch) { pool.release(patch.address()); }

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
			DefaultSearchArgument(const ScoreType score, Args&& ...args) noexcept
				: score(score), argument(Argument(std::forward<Args>(args)...)) {}
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
		template<class Config>
		struct SearchNode {

			using SearchArgument = typename Config::SearchArgumentType;
			using Pointer = SearchNode*;

			SearchNode() noexcept = default;
			SearchNode(Pointer parent, const SearchArgument& argument) noexcept : argument(argument) {
				assert(parent != nullptr);
				this->parent = parent;
				parent->addRef();
				depth = parent->depth + 1;
			}

			/** @brief 親ノードのポインタ */
			Pointer parent = nullptr;
			/** @brief 探査引数 */
			SearchArgument argument{};
			/** @brief ノード遷移パッチ */
			VersionControl::Patch patch{};
			/** @brief ノード深度 */
			int depth = 0;
			/** @brief 参照カウント */
			int ref = 1;

			inline void addRef() noexcept { ref++; }
			inline void subRef() noexcept { ref--; }
		};

		template<class Config>
		class BeamSearchProcess {
		private:

			using SearchArgument = typename Config::SearchArgumentType;

			using Node = SearchNode<Config>;
			using NodePointer = Node*;

			using RankingItem = std::pair<double, NodePointer>;
			using Ranking = ExPriorityQueue<RankingItem>;

			class SearchNodePool {
			private:
				MemoryPool<Node, 16_KB> pool{};
				NodePointer top = nullptr;
			public:
				NODISCARD inline NodePointer alloc() {
					if (top != nullptr) {
						NodePointer ptr = top;
						top = ptr->parent;
						return ptr;
					}
					else {
						return pool.alloc(1);
					}
				}
				inline void release(NodePointer ptr) noexcept {
					ptr->parent = top;
					top = ptr;
				}
			};

			// TODO:初期化方法
			inline static constexpr bool EnabelDebug = false;

			/** @brief 現在の探査ノード */
			NodePointer currentNode = nullptr;
			/** @brief 次の探査ノード */
			NodePointer nextNode = nullptr;

			/** @brief 現在の探査ノードキュー */
			Ranking currentRanking;
			/** @brief 次のの探査ノードキュー */
			Ranking nextRanking;
			/** @brief 現在の探査ノードキューの低スコアキュー */
			ExPriorityQueue<double, std::greater<double>> nextScoreRinking;
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
			SearchNodePool nodePool;

			/** @brief 探査終了処理 */
			void destruction() noexcept {
				VersionControl::Unlock();
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

				std::array<VersionControl::Patch, Config::GetDepth()> redos{};
				int stack = Config::GetDepth();

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
				forstep(idx, stack, Config::GetDepth()) {
					WARN_PUSH(disable:26446 26482);
					redos[idx].redo();
					WARN_POP();
				}
			}

			void clearRanking(Ranking& rank) {
				const auto& container = rank.container();
				for (const auto& node : container) {
					release(node.second);
				}
				rank.clear();
			}

			NODISCARD double getNextLimit(const double interval) const noexcept {
				return ((Config::GetLimit() - interval) / remainDepth) + interval;
			}

			void release(NodePointer node) {
				assert(node != nullptr);
				assert(0 < node->ref);

				if (node->ref == 1) {
					if (node->parent != nullptr) {
						release(node->parent);
					}
					if (node->patch.address() != nullptr) {
						version.release(node->patch);
					}
					nodePool.release(node);
				}
				else {
					node->subRef();
				}

			}

		public:

			BeamSearchProcess() noexcept = default;

			void timerStart() noexcept { timer.start(); }

			void init() {
				remainDepth = Config::GetDepth();

				nextLimit = getNextLimit(timer.interval());

				currentNode = new(nodePool.alloc()) Node();

				clearRanking(currentRanking);
				clearRanking(nextRanking);
				nextScoreRinking.clear();
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
					if (remainDepth <= 0) {
						destruction();
						return false;
					}

					nextLimit = getNextLimit(interval);
					remainDepth--;
					clearRanking(currentRanking);
					std::swap(currentRanking, nextRanking);
					nextScoreRinking.clear();
					nodeWidthCount = 0;
				}

				if (currentRanking.empty()) {
					destruction();
					return false;
				}

				nextNode = currentRanking.top().second;
				currentRanking.pop();
				nodeWidthCount++;

				return true;
			}

			void accept() {
				assert(nextNode != nullptr);
				transitin(currentNode, nextNode);
				release(currentNode);
				currentNode = nextNode;
				nextNode = nullptr;
			}

			void reserve(const SearchArgument& argument) {
				if (Config::GetWidth() <= nextRanking.size()) {
					if (argument.score <= nextScoreRinking.top()) {
						return;
					}
					else {
						nextScoreRinking.pop();
					}
				}

				nextRanking.emplace(argument.score, new(nodePool.alloc()) Node(currentNode, argument));
				nextScoreRinking.push(argument.score);
			}

			NODISCARD SearchArgument getArgument() const noexcept {
				assert(nextNode != nullptr);
				return nextNode->argument;
			}

			NODISCARD inline bool endOfSearch() const noexcept { return remainDepth == 0; }
			NODISCARD inline int getDepth() const noexcept { return Config::GetDepth() - remainDepth - 1; }

		};

		/**
		 * @brief ビームサーチ用テンプレートクラス
		 * @tparam Config 設定
		*/
		template<class Config>
		class BeamSearchTemplate {
		public:

			using Argument = typename Config::ArgumentType;
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
				while (process.onloop()) {
					if constexpr (SearchArgument::HasHash) {
						const auto& argument = process.getArgument();
						if (visited.find(argument.hash) != visited.end()) { continue; }
						visited.insert(argument.hash);
						process.accept();
						search(argument);
					}
					else {
						const auto& argument = process.getArgument();
						process.accept();
						search(argument);
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
			inline void nextSearch(SearchArgument argument) { process.reserve(argument); }

			NODISCARD inline bool endOfSearch() const { return process.endOfSearch(); }
			NODISCARD inline int getDepth() const { return process.getDepth(); }

		public:

			void start(const SearchArgument& argument) {
				process.timerStart();
				process.init();
				process.reserve(argument);
				loop();
			}

			virtual void search(const SearchArgument&) = 0;

		};
	}

	namespace BeamSearch {

		template<class Class>
		class RefValue {
		private:
			Class* ptr = nullptr;

			inline void modify() noexcept { Lib::VersionControl::Modify(*ptr); }
			inline void modify(const Class& o) noexcept {
				if (*ptr != o) {
					modify();
					(*ptr) = o;
				}
			}

		public:
			RefValue(Class* ptr) noexcept : ptr(ptr) {}

			inline void operator=(const Class& o) noexcept { modify(o); }
			inline void operator+=(const Class& o) { Class v = (*ptr) + o; modify(v); }
			inline void operator-=(const Class& o) { Class v = (*ptr) - o; modify(v); }
			inline void operator*=(const Class& o) { Class v = (*ptr) * o; modify(v); }
			inline void operator/=(const Class& o) { Class v = (*ptr) / o; modify(v); }
			inline void operator&=(const Class& o) { Class v = (*ptr) & o; modify(v); }
			inline void operator|=(const Class& o) { Class v = (*ptr) | o; modify(v); }
			inline void operator^=(const Class& o) { Class v = (*ptr) ^ o; modify(v); }

			inline RefValue<Class>& operator++() { modify(); ++(*ptr); return *this; }
			inline Class operator++(int) { modify(); Class v = *ptr; ++(*ptr); return v; }
			inline RefValue<Class>& operator--() { modify(); --(*ptr); return *this; }
			inline Class operator--(int) { modify(); Class v = *ptr; --(*ptr); return v; }

			NODISCARD inline Class value() const { return *ptr; }
			NODISCARD inline operator Class() const { return *ptr; }
		};

		template<typename Type, Lib::SizeType Size>
		class RefArray : public std::array<Type, Size> {
		public:
			using base = std::array<Type, Size>;

			RefArray() noexcept : base() {};
			RefArray(const RefArray& other) = default;
			RefArray(RefArray&& other) = default;
			RefArray& operator=(const RefArray& other) = default;
			RefArray& operator=(RefArray&& other) = default;

			inline void set(const size_type idx, const Type& o) noexcept { Lib::VersionControl::Modify(base::operator[](idx)); base::operator[](idx) = o; }
			NODISCARD inline const Type& get(const size_type idx) const noexcept { return base::operator[](idx); }

			NODISCARD inline const Type& operator[](const size_type idx) const noexcept { return base::operator[](idx); }
			NODISCARD inline RefValue<Type> operator[](const size_type idx) noexcept { return RefValue<Type>(std::addressof(base::operator[](idx))); }

			NODISCARD inline const Type& at(const size_type idx) const noexcept { return base::at(idx); }
			NODISCARD inline RefValue<Type> at(const size_type idx) noexcept { return RefValue<Type>(std::addressof(base::at(idx))); }
		};

		template<typename Type, Lib::SizeType Size>
		class RefVector : public std::array<Type, Size> {
		public:
			using base = std::array<Type, Size>;
		private:

			size_type count = 0;

			NODISCARD inline void hasCapacity(const size_type n) noexcept { assert(0 < n && n <= Size); }
			NODISCARD inline void inside(const size_type idx) noexcept { assert(0 <= idx && idx < count); }
			NODISCARD inline void any() noexcept { assert(0 < count); }

		public:

			RefVector() noexcept : base() {};
			RefVector(const RefVector& other) = default;
			RefVector(RefVector&& other) = default;
			RefVector& operator=(const RefVector& other) = default;
			RefVector& operator=(RefVector&& other) = default;

			inline void set(const size_type idx, const Type& o) noexcept {
				inside(idx);
				Lib::VersionControl::Modify(base::operator[](idx));
				base::operator[](idx) = o;
			}
			NODISCARD inline const Type& get(const size_type idx) const noexcept {
				inside(idx);
				return base::operator[](idx);
			}

			inline void clear() noexcept { Lib::VersionControl::Modify(count); count = 0; }
			NODISCARD inline bool empty() const noexcept { return count == 0; }
			NODISCARD inline bool full() const noexcept { return count == base::size(); }
			NODISCARD inline size_type full_size() const noexcept { return base::size(); }
			NODISCARD inline size_type size() const noexcept { return count; }

			inline void push_back(const Type& value) noexcept {
				hasCapacity(count + 1);
				Lib::VersionControl::Modify(count);
				WARN_PUSH(disable:26446);
				Lib::VersionControl::Modify(base::operator[](count));
				base::operator[](count) = value;
				WARN_POP();
				++count;
			}
			template<class ...Args>
			inline void emplace_back(Args&& ...args) noexcept {
				hasCapacity(count + 1);
				Lib::VersionControl::Modify(count);
				Lib::VersionControl::Modify(base::operator[](count));
				new(std::addressof(base::operator[](count))) Type(std::forward<TypeArgs>(args)...);
				++count;
			}

			inline void pop_back() noexcept {
				any();
				Lib::VersionControl::Modify(count);
				--count;
			}

			NODISCARD inline RefValue<Type> front() noexcept {
				any();
				return RefMemory<Type>(std::addressof(base::front()));
			}
			NODISCARD inline const Type& front() const noexcept {
				any();
				return base::front();
			}

			WARN_PUSH(disable:26434);
			NODISCARD inline RefValue<Type> back() noexcept {
				any();
				WARN_PUSH(disable:26446);
				return RefValue<Type>(std::addressof(base::operator[](count - 1)));
				WARN_POP();
			}
			WARN_POP();

			NODISCARD inline const Type& back() const noexcept {
				any();
				return base::operator[](count - 1);
			}

			inline void resize(const Lib::SizeType n, const Type& value) noexcept {
				hasCapacity(n);
				Lib::VersionControl::Modify(count);
				if (count < n) {
					const auto addCount = n - count;
					Lib::VersionControl::Modify(std::addressof(base::operator[](count)), sizeof(Type) * addCount);
					forstep(idx, count, n) { base::operator[](i) = value; }
				}
				count = n;
			}
			inline void resize(int n) noexcept {
				hasCapacity(n);
				Lib::VersionControl::Modify(count);
				count = n;
			}

			NODISCARD inline const Type& operator[](const size_type idx) const noexcept {
				inside(idx);
				return base::operator[](idx);
			}
			NODISCARD inline RefValue<Type> operator[](const size_type idx) noexcept {
				inside(idx);
				return RefValue<Type>(std::addressof(base::operator[](idx)));
			}

			NODISCARD inline const Type& at(const size_type idx) const noexcept {
				inside(idx);
				return base::at(idx);
			}
			NODISCARD inline RefValue<Type> at(const size_type idx) noexcept {
				inside(idx);
				return RefValue<Type>(std::addressof(base::at(idx)));
			}

		};

	}

	template<BeamSearch::Lib::SizeType Depth, BeamSearch::Lib::SizeType Width, BeamSearch::Lib::SizeType Limit, typename Argument, typename SearchArgument = BeamSearch::Lib::DefaultSearchArgument<Argument>>
	struct BeamSearchConfig {

		using BeamBase = BeamSearch::Lib::BeamSearchTemplate<BeamSearchConfig>;

		using ArgumentType = Argument;
		using SearchArgumentType = SearchArgument;

		NODISCARD static constexpr int GetDepth() { return narrow_cast<int>(Depth); }
		NODISCARD static constexpr int GetWidth() { return narrow_cast<int>(Width); }
		NODISCARD static constexpr int GetLimit() { return narrow_cast<int>(Limit); }
	};

}
