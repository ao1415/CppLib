#pragma once

#include "Basic.hpp"

#include <array>
#include <vector>
#include <unordered_set>

#include <cassert>

#include <memory>
#include <chrono>

namespace alib_del {

	namespace Search::Lib {
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

		template<typename Type, SizeType MaxMemorySize>
		class MemoryPool {
		public:
			using ValueType = Type;
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
				WARN_PUSH_DISABLE(26490);
				return reinterpret_cast<uintptr_t>(p);
				WARN_POP();
			}
		public:
			NODISCARD inline Pointer alloc(const SizeType size) {
				assert(size < MaxMemorySize);
				if (size == 0)
					return nullptr;

				if (memorySize + size >= MaxMemorySize) {
					if (unusedMemory.empty()) {
						WARN_PUSH_DISABLE(26409);
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

				WARN_PUSH_DISABLE(26446);
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
							if (ite->memory != memory) {
								unusedMemory.push_back(ite->memory);
								ite = refMemory.erase(ite);
							}
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
					WARN_PUSH_DISABLE(26481);
					return std::addressof(data[pos]);
					WARN_POP();
				}

				template<typename Ptr>
				NODISCARD inline Ptr reinterpret() const noexcept {
					WARN_PUSH_DISABLE(26490);
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

				NODISCARD inline bool endOfStream() const noexcept { return pos == length; }
				NODISCARD inline bool hasStream() const noexcept { return pos < length; }
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
					WARN_PUSH_DISABLE(26485);
					stream = Stream(buffer, 0, Length);
					WARN_POP();
				}
				NODISCARD inline bool isEnabled() noexcept { return !locks.empty(); }
				NODISCARD inline SizeType getBufferSize() const noexcept { return stream.pos - locks.back().first; }

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
					if (!isEnabled()) { return; }

					const SizeType writeSize = size + sizeof(VersionSizeType) + sizeof(void*);

					assert(1 <= size);
					assert(size <= std::numeric_limits<VersionSizeType>::max());
					assert(stream.pos + writeSize <= Length);

					stream.copy(data, size);
					stream.set<VersionSizeType>(size);
					stream.set<void*>(data);

					redoSize += size;
				}
			};

			inline static Buffer<256_KB> buffer{};

			MemoryPool<std::byte, 1_MB> pool;
		public:
			class Patch {
			private:
				Pointer data = nullptr;
				SizeType length = 0;
			public:
				Patch() = default;
				Patch(Pointer data, const SizeType length) noexcept : data(data), length(length) {}

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
				assert(size <= std::numeric_limits<VersionSizeType>::max());
				buffer.modify(mem, narrow_cast<VersionSizeType>(size));
			}
			template<class Class>
			inline static void Modify(Class& mem) noexcept {
				static_assert(sizeof(Class) <= std::numeric_limits<VersionSizeType>::max());
				WARN_PUSH_DISABLE(26474);
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
					stream.copy(ptr, size);// 変更後の値
					buffer.stream.pop(stream, size);// 変更前の値
				}
				assert(stream.endOfStream());
				buffer.redoSize = 0;

				return Patch(stream.data, stream.length);
			}

			inline void release(const Patch& patch) { pool.release(patch.address()); }
		};

		template<class Config>
		struct DefaultSearchArgument {
			inline static constexpr bool HasHash = false;

			using ScoreType = typename Config::ScoreType;
			using ArgumentType = typename Config::ArgumentType;

			ScoreType score{};
			ArgumentType argument{};

			DefaultSearchArgument() = default;
			DefaultSearchArgument(const ScoreType score, const ArgumentType& argument) noexcept : score(score), argument(argument) {}
			DefaultSearchArgument(const ScoreType score) noexcept : score(score) {}
			template<class... Args>
			DefaultSearchArgument(const ScoreType score, Args&& ...args) noexcept : score(score), argument(ArgumentType(std::forward<Args>(args)...)) {}
		};

		template<class Config>
		struct HashSearchArgument {
			inline static constexpr bool HasHash = true;

			using ScoreType = typename Config::ScoreType;
			using HashType = typename Config::HashType;
			using ArgumentType = typename Config::ArgumentType;

			HashType hash{};
			ScoreType score{};
			ArgumentType argument{};

			HashSearchArgument() = default;
			HashSearchArgument(const HashType hash, const ScoreType score, const ArgumentType& argument) noexcept : hash(hash), score(score), argument(argument) {}
			HashSearchArgument(const HashType hash, const ScoreType score) noexcept : hash(hash), score(score), argument(ArgumentType()) {}
			template<class... Args>
			HashSearchArgument(const HashType hash, const ScoreType score, Args&& ...args) noexcept : hash(hash), score(score), argument(ArgumentType(std::forward<Args>(args)...)) {}
		};

		/**
		 * @brief 探査ノードの構造体
		 * @tparam Argument 探索引数
		*/
		template<class Config>
		struct SearchNode {
			using ArgumentType = typename Config::SearchArgumentType;
			using Pointer = SearchNode*;

			SearchNode() noexcept = default;
			SearchNode(Pointer parent, const ArgumentType& argument) noexcept : argument(argument) {
				assert(parent != nullptr);
				this->parent = parent;
				parent->addRef();
				depth = parent->depth + 1;
			}

			/** @brief 親ノードのポインタ */
			Pointer parent = nullptr;
			/** @brief 探査引数 */
			ArgumentType argument{};
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
		class SearchNodePool {
		public:
			using Pointer = typename SearchNode<Config>::Pointer;
		private:
			MemoryPool<SearchNode<Config>, 16_KB> pool{};
			Pointer top = nullptr;
		public:
			NODISCARD inline Pointer alloc() {
				if (top != nullptr) {
					Pointer ptr = top;
					top = ptr->parent;
					return ptr;
				}
				else {
					return pool.alloc(1);
				}
			}
			inline void release(Pointer ptr) noexcept {
				ptr->parent = top;
				top = ptr;
			}
		};
	}

	namespace VersionRef {
		using namespace Search::Lib;

		template<class Class>
		class Ref {
		private:
			Class* ptr = nullptr;

			inline void modify() noexcept { VersionControl::Modify(*ptr); }
			inline void modify(const Class& o) noexcept { if (*ptr != o) { modify(); (*ptr) = o; } }
		public:
			Ref(Class* ptr) noexcept : ptr(ptr) {}

			inline void operator=(const Class& o) noexcept { modify(o); }
			inline void operator+=(const Class& o) noexcept { Class v = (*ptr) + o; modify(v); }
			inline void operator-=(const Class& o) noexcept { Class v = (*ptr) - o; modify(v); }
			inline void operator*=(const Class& o) noexcept { Class v = (*ptr) * o; modify(v); }
			inline void operator/=(const Class& o) noexcept { Class v = (*ptr) / o; modify(v); }
			inline void operator&=(const Class& o) noexcept { Class v = (*ptr) & o; modify(v); }
			inline void operator|=(const Class& o) noexcept { Class v = (*ptr) | o; modify(v); }
			inline void operator^=(const Class& o) noexcept { Class v = (*ptr) ^ o; modify(v); }

			inline Ref<Class>& operator++() noexcept { modify(); ++(*ptr); return *this; }
			inline Class operator++(int) noexcept { modify(); Class v = *ptr; ++(*ptr); return v; }
			inline Ref<Class>& operator--() noexcept { modify(); --(*ptr); return *this; }
			inline Class operator--(int) noexcept { modify(); Class v = *ptr; --(*ptr); return v; }

			inline bool operator==(const Class& o) const noexcept { return (*ptr) == o; }
			inline bool operator!=(const Class& o) const noexcept { return (*ptr) != o; }

			NODISCARD inline Class value() const noexcept { return *ptr; }
			NODISCARD inline operator Class() const noexcept { return *ptr; }
		};

		template<typename Type>
		class Value {
		private:
			Type val{};
		public:
			Value() noexcept {}
			Value(const Type val) noexcept : val(val) {}

			NODISCARD inline const Type& operator()() const noexcept { return val; }
			NODISCARD inline Ref<Type> operator()() noexcept { return Ref<Type>(std::addressof(val)); }

			NODISCARD inline const Type& value() const noexcept { return val; }
		};

		template<typename Type, SizeType Size>
		class Array : public std::array<Type, Size> {
		public:
			using base = std::array<Type, Size>;

			Array() noexcept : base() {};
			Array(const Array& other) = default;
			Array(Array&& other) = default;
			Array& operator=(const Array& other) = default;
			Array& operator=(Array&& other) = default;

			inline void set(const SizeType idx, const Type& o) noexcept { VersionControl::Modify(base::operator[](idx)); base::operator[](idx) = o; }
			NODISCARD inline const Type& get(const SizeType idx) const noexcept { return base::operator[](idx); }

			NODISCARD inline const Type& operator[](const SizeType idx) const noexcept { return base::operator[](idx); }
			NODISCARD inline Ref<Type> operator[](const SizeType idx) noexcept { return Ref<Type>(std::addressof(base::operator[](idx))); }

			NODISCARD inline const Type& at(const SizeType idx) const { return base::at(idx); }
			NODISCARD inline Ref<Type> at(const SizeType idx) { return Ref<Type>(std::addressof(base::at(idx))); }
		};

		template<typename Type, SizeType Size>
		class Vector : public std::array<Type, Size> {
		public:
			using base = std::array<Type, Size>;
		private:
			SizeType count = 0;

			inline void hasCapacity(const SizeType n) noexcept { assert(0 < n && n <= Size); }
			inline void inside(const SizeType idx) noexcept { assert(0 <= idx && idx < count); }
			inline void any() noexcept { assert(0 < count); }
		public:
			Vector() noexcept : base() {};
			Vector(const Vector& other) = default;
			Vector(Vector&& other) = default;
			Vector& operator=(const Vector& other) = default;
			Vector& operator=(Vector&& other) = default;

			inline void set(const SizeType idx, const Type& o) noexcept {
				inside(idx);
				VersionControl::Modify(base::operator[](idx));
				base::operator[](idx) = o;
			}
			NODISCARD inline const Type& get(const SizeType idx) const noexcept {
				inside(idx);
				return base::operator[](idx);
			}

			inline void clear() noexcept { VersionControl::Modify(count); count = 0; }
			NODISCARD inline bool empty() const noexcept { return count == 0; }
			NODISCARD inline bool full() const noexcept { return count == base::size(); }
			NODISCARD inline SizeType full_size() const noexcept { return base::size(); }
			NODISCARD inline SizeType size() const noexcept { return count; }

			inline void push_back(const Type& value) noexcept {
				hasCapacity(count + 1);
				VersionControl::Modify(count);
				WARN_PUSH_DISABLE(26446);
				VersionControl::Modify(base::operator[](count));
				base::operator[](count) = value;
				WARN_POP();
				++count;
			}
			template<class ...Args>
			inline void emplace_back(Args&& ...args) noexcept {
				hasCapacity(count + 1);
				VersionControl::Modify(count);
				VersionControl::Modify(base::operator[](count));
				new(std::addressof(base::operator[](count))) Type(std::forward<Args>(args)...);
				++count;
			}

			inline void pop_back() noexcept { any(); VersionControl::Modify(count); --count; }

			WARN_PUSH_DISABLE(26434 26446);
			NODISCARD inline Ref<Type> front() noexcept { any(); return Ref<Type>(std::addressof(base::front())); }
			NODISCARD inline const Type& front() const noexcept { any(); return base::front(); }

			NODISCARD inline Ref<Type> back() noexcept { any(); return Ref<Type>(std::addressof(base::operator[](count - 1))); }
			NODISCARD inline const Type& back() const noexcept { any(); return base::operator[](count - 1); }
			WARN_POP();

			inline void resize(const SizeType n, const Type& value) noexcept {
				hasCapacity(n);
				VersionControl::Modify(count);
				if (count < n) {
					const auto addCount = n - count;
					VersionControl::Modify(std::addressof(base::operator[](count)), sizeof(Type) * addCount);
					forstep(idx, count, n) { base::operator[](idx) = value; }
				}
				count = n;
			}
			inline void resize(int n) noexcept {
				hasCapacity(n);
				VersionControl::Modify(count);
				count = n;
			}

			NODISCARD inline const Type& operator[](const SizeType idx) const noexcept { inside(idx); return base::operator[](idx); }
			NODISCARD inline Ref<Type> operator[](const SizeType idx) noexcept { inside(idx); return Ref<Type>(std::addressof(base::operator[](idx))); }

			NODISCARD inline const Type& at(const SizeType idx) const { inside(idx); return base::at(idx); }
			NODISCARD inline Ref<Type> at(const SizeType idx) { inside(idx); return Ref<Type>(std::addressof(base::at(idx))); }
		};
	}

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
			using ArgumentType = typename Config::SearchArgumentType;
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

			void reserve(const ArgumentType& argument) { nextRanking.emplace(argument.score, new(nodePool.alloc()) Node(currentNode, argument)); }

			NODISCARD inline const ArgumentType& getArgument() const noexcept { assert(nextNode != nullptr); return nextNode->argument; }
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
			using ArgumentType = typename Config::SearchArgumentType;

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
					if constexpr (ArgumentType::HasHash) {
						visited.insert(argument.hash);
					}
					process.accept();
					init(argument);
				}
				while (process.onloop()) {
					const auto& argument = process.getArgument();
					if constexpr (ArgumentType::HasHash) {
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
			inline void nextSearch(Args&& ...args) { nextSearch(ArgumentType(std::forward<Args>(args)...)); }

			/**
			 * @brief 次の状態処理
			 * @param arg 次の状態処理の引数
			*/
			inline void nextSearch(ArgumentType argument) { process.reserve(argument); }

			NODISCARD inline bool endOfSearch() const noexcept { return process.endOfSearch(); }
			NODISCARD inline int getDepth() const noexcept { return process.getDepth(); }
		public:
			void start(const ArgumentType& argument) {
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
			virtual void init(const ArgumentType&) = 0;
			/**
			 * @brief 探査処理
			 * @param 引数
			*/
			virtual void search(const ArgumentType&) = 0;
		};
	}

	template<typename Argument>
	struct SearchArgumentConfig {
		using ArgumentType = Argument;
		using ScoreType = double;
		using HashType = uint64_t;
	};

	template<Search::Lib::SizeType Depth, Search::Lib::SizeType Width, Search::Lib::SizeType Limit, typename Argument, typename ArgumentType = Search::Lib::DefaultSearchArgument<SearchArgumentConfig<Argument>>, bool Debug = false>
	struct BeamSearchConfig {
		using BeamBase = Search::Lib::BeamSearchTemplate<BeamSearchConfig>;

		using ArgumentType = Argument;
		using SearchArgumentType = ArgumentType;

		NODISCARD static constexpr bool IsDebug() { return Debug; }

		NODISCARD static constexpr int GetDepth() { return narrow_cast<int>(Depth + 1); }
		NODISCARD static constexpr int GetWidth() { return narrow_cast<int>(Width); }
		NODISCARD static constexpr int GetLimit() { return narrow_cast<int>(Limit); }
	};
}
