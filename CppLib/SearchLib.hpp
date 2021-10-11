#pragma once

#include "Basic.hpp"

#include <array>
#include <vector>
#include <unordered_set>

#include <cassert>

#include <memory>
#include <chrono>

namespace alib {

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
			DefaultSearchArgument(const ScoreType score, const ArgumentType& argument) : score(score), argument(argument) {}
			DefaultSearchArgument(const ScoreType score) noexcept : score(score) {}
			template<class... Args>
			DefaultSearchArgument(const ScoreType score, Args&& ...args) noexcept
				: score(score), argument(ArgumentType(std::forward<Args>(args)...)) {}
		};

		template<class Config>
		struct HashSearchArgument {
			inline static constexpr bool HasHash = true;

			using ScoreType = typename Config::ScoreType;
			using HashType = typename Config::HashType;
			using ArgumentType = typename Config::ArgumentType;

			HashType hash;
			ScoreType score;
			ArgumentType argument;

			HashSearchArgument() = default;
			HashSearchArgument(const HashType hash, const ScoreType score, const ArgumentType& argument) : hash(hash), score(score), argument(argument) {}
			HashSearchArgument(const HashType hash, const ScoreType score) : hash(hash), score(score), argument(ArgumentType()) {}
			template<class... Args>
			HashSearchArgument(const HashType hash, const ScoreType score, Args&& ...args) : hash(hash), score(score), argument(ArgumentType(std::forward<Args>(args)...)) {}
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
			inline void modify(const Class& o) noexcept {
				if (*ptr != o) { modify(); (*ptr) = o; }
			}
		public:
			Ref(Class* ptr) noexcept : ptr(ptr) {}

			inline void operator=(const Class& o) noexcept { modify(o); }
			inline void operator+=(const Class& o) { Class v = (*ptr) + o; modify(v); }
			inline void operator-=(const Class& o) { Class v = (*ptr) - o; modify(v); }
			inline void operator*=(const Class& o) { Class v = (*ptr) * o; modify(v); }
			inline void operator/=(const Class& o) { Class v = (*ptr) / o; modify(v); }
			inline void operator&=(const Class& o) { Class v = (*ptr) & o; modify(v); }
			inline void operator|=(const Class& o) { Class v = (*ptr) | o; modify(v); }
			inline void operator^=(const Class& o) { Class v = (*ptr) ^ o; modify(v); }

			inline Ref<Class>& operator++() { modify(); ++(*ptr); return *this; }
			inline Class operator++(int) { modify(); Class v = *ptr; ++(*ptr); return v; }
			inline Ref<Class>& operator--() { modify(); --(*ptr); return *this; }
			inline Class operator--(int) { modify(); Class v = *ptr; --(*ptr); return v; }

			NODISCARD inline Class value() const noexcept { return *ptr; }
			NODISCARD inline operator Class() const noexcept { return *ptr; }
		};

		template<typename Type>
		class Value : public Ref<Type> {
		private:
			Type val{};
		public:
			Value() noexcept : Ref<Type>(std::addressof(val)) {}
			Value(const Type val) noexcept : val(val), Ref<Type>(std::addressof(this->val)) {}
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

			inline void set(const size_type idx, const Type& o) noexcept { VersionControl::Modify(base::operator[](idx)); base::operator[](idx) = o; }
			NODISCARD inline const Type& get(const size_type idx) const noexcept { return base::operator[](idx); }

			NODISCARD inline const Type& operator[](const size_type idx) const noexcept { return base::operator[](idx); }
			NODISCARD inline Ref<Type> operator[](const size_type idx) noexcept { return Ref<Type>(std::addressof(base::operator[](idx))); }

			NODISCARD inline const Type& at(const size_type idx) const noexcept { return base::at(idx); }
			NODISCARD inline Ref<Type> at(const size_type idx) noexcept { return Ref<Type>(std::addressof(base::at(idx))); }
		};

		template<typename Type, SizeType Size>
		class Vector : public std::array<Type, Size> {
		public:
			using base = std::array<Type, Size>;
		private:
			size_type count = 0;

			NODISCARD inline void hasCapacity(const size_type n) noexcept { assert(0 < n && n <= Size); }
			NODISCARD inline void inside(const size_type idx) noexcept { assert(0 <= idx && idx < count); }
			NODISCARD inline void any() noexcept { assert(0 < count); }
		public:
			Vector() noexcept : base() {};
			Vector(const Vector& other) = default;
			Vector(Vector&& other) = default;
			Vector& operator=(const Vector& other) = default;
			Vector& operator=(Vector&& other) = default;

			inline void set(const size_type idx, const Type& o) noexcept {
				inside(idx);
				VersionControl::Modify(base::operator[](idx));
				base::operator[](idx) = o;
			}
			NODISCARD inline const Type& get(const size_type idx) const noexcept {
				inside(idx);
				return base::operator[](idx);
			}

			inline void clear() noexcept { VersionControl::Modify(count); count = 0; }
			NODISCARD inline bool empty() const noexcept { return count == 0; }
			NODISCARD inline bool full() const noexcept { return count == base::size(); }
			NODISCARD inline size_type full_size() const noexcept { return base::size(); }
			NODISCARD inline size_type size() const noexcept { return count; }

			inline void push_back(const Type& value) noexcept {
				hasCapacity(count + 1);
				VersionControl::Modify(count);
				WARN_PUSH(disable:26446);
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
				new(std::addressof(base::operator[](count))) Type(std::forward<TypeArgs>(args)...);
				++count;
			}

			inline void pop_back() noexcept {
				any();
				VersionControl::Modify(count);
				--count;
			}

			NODISCARD inline Ref<Type> front() noexcept {
				any();
				return RefMemory<Type>(std::addressof(base::front()));
			}
			NODISCARD inline const Type& front() const noexcept {
				any();
				return base::front();
			}

			WARN_PUSH(disable:26434);
			NODISCARD inline Ref<Type> back() noexcept {
				any();
				WARN_PUSH(disable:26446);
				return Ref<Type>(std::addressof(base::operator[](count - 1)));
				WARN_POP();
			}
			WARN_POP();

			NODISCARD inline const Type& back() const noexcept {
				any();
				return base::operator[](count - 1);
			}

			inline void resize(const SizeType n, const Type& value) noexcept {
				hasCapacity(n);
				VersionControl::Modify(count);
				if (count < n) {
					const auto addCount = n - count;
					VersionControl::Modify(std::addressof(base::operator[](count)), sizeof(Type) * addCount);
					forstep(idx, count, n) { base::operator[](i) = value; }
				}
				count = n;
			}
			inline void resize(int n) noexcept {
				hasCapacity(n);
				VersionControl::Modify(count);
				count = n;
			}

			NODISCARD inline const Type& operator[](const size_type idx) const noexcept {
				inside(idx);
				return base::operator[](idx);
			}
			NODISCARD inline Ref<Type> operator[](const size_type idx) noexcept {
				inside(idx);
				return Ref<Type>(std::addressof(base::operator[](idx)));
			}

			NODISCARD inline const Type& at(const size_type idx) const noexcept {
				inside(idx);
				return base::at(idx);
			}
			NODISCARD inline Ref<Type> at(const size_type idx) noexcept {
				inside(idx);
				return Ref<Type>(std::addressof(base::at(idx)));
			}
		};
	}
}
