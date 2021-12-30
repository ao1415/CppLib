#pragma once

#include <utility>
#include <type_traits>
#include <algorithm>

#include <array>
#include <vector>
#include <stack>
#include <map>
#include <unordered_set>

#include <memory>
#include <chrono>

#include <cassert>

#include "Basic.hpp"

namespace alib {

}

namespace Dev {
	using namespace alib;

	namespace Search {
		using size_type = size_t;

		namespace Lib {
			NODISCARD constexpr size_type operator"" _K(unsigned long long k) noexcept { return k << 10; }
			NODISCARD constexpr size_type operator"" _M(unsigned long long m) noexcept { return m << 20; }
		}

		template<class Argument, typename Score = double, typename Hash = void>
		struct DefaultSearchMethod {
			using ArgumentType = typename Argument;
			using ScoreType = typename Score;
			using HashType = typename Hash;

			ArgumentType argument{};
			ScoreType score{};
			HashType hash{};

			DefaultSearchMethod() = default;
			DefaultSearchMethod(const ScoreType score, const HashType hash, const ArgumentType& argument) noexcept : score(score), hash(hash), argument(argument) {}
			DefaultSearchMethod(const ScoreType score, const HashType hash) noexcept : score(score), hash(hash) {}
			DefaultSearchMethod(const ScoreType score) noexcept : score(score) {}
			template<class... Args>
			DefaultSearchMethod(const ScoreType score, const HashType hash, Args&& ...args) noexcept : score(score), argument(ArgumentType(std::forward<Args>(args)...)) {}
		};

		template<class Argument, typename Score>
		struct DefaultSearchMethod<Argument, Score, void> {
			using ArgumentType = typename Argument;
			using ScoreType = typename Score;
			using HashType = void;

			ArgumentType argument{};
			ScoreType score{};

			DefaultSearchMethod() = default;
			DefaultSearchMethod(const ScoreType score, const ArgumentType& argument) noexcept : score(score), argument(argument) {}
			DefaultSearchMethod(const ScoreType score) noexcept : score(score) {}
			template<class... Args>
			DefaultSearchMethod(const ScoreType score, Args&& ...args) noexcept : score(score), argument(ArgumentType(std::forward<Args>(args)...)) {}
		};

		template <size_type Depth, size_type Width, size_type Limit, class Method>
		struct ConfigTemplate {
			using SearchMethod = Method;

			using ArgumentType = typename Method::ArgumentType;
			using ScoreType = typename Method::ScoreType;
			using HashType = typename Method::HashType;

			NODISCARD inline static constexpr bool HasHash() noexcept { return std::is_void_v<HashType>; }

			NODISCARD inline static constexpr bool GetDebug() noexcept { return true; }

			NODISCARD inline static constexpr size_type GetDepth() noexcept { return Depth; }
			NODISCARD inline static constexpr size_type GetWidth() noexcept { return Width; }
			NODISCARD inline static constexpr size_type GetLimit() noexcept { return Limit; }
		};

		namespace Lib {

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
			class ExPriorityQueue {
			public:
				using container_type = std::vector<Type>;
				using value_compare = Compare;

				using value_type = typename container_type::value_type;
				using reference = typename container_type::reference;
				using const_reference = typename container_type::const_reference;
				using size_type = typename container_type::size_type;

				using iterator = typename container_type::iterator;
				using const_iterator = typename container_type::const_iterator;
				using reverse_iterator = typename container_type::reverse_iterator;
				using const_reverse_iterator = typename container_type::const_reverse_iterator;

				static_assert(std::is_same_v<Type, value_type>);
			protected:
				container_type c{};
				value_compare comp{};
			public:
				ExPriorityQueue() = default;
				explicit ExPriorityQueue(const value_compare& pred) noexcept(std::is_nothrow_default_constructible_v<container_type>&& std::is_nothrow_copy_constructible_v<value_compare>) : c(), comp(pred) {}
				ExPriorityQueue(const value_compare& pred, const container_type& cont) : c(cont), comp(pred) { std::make_heap(c.begin(), c.end(), comp); }
				ExPriorityQueue(const value_compare& pred, container_type&& cont) : c(std::move(cont)), comp(pred) { std::make_heap(c.begin(), c.end(), comp); }

				template <class Ite>
				ExPriorityQueue(Ite first, Ite last, const value_compare& pred, const container_type& cont) : c(cont), comp(pred) { c.insert(c.end(), first, last); std::make_heap(c.begin(), c.end(), comp); }
				template <class Ite>
				ExPriorityQueue(Ite first, Ite last) : c(first, last), comp() { std::make_heap(c.begin(), c.end(), comp); }
				template <class Ite>
				ExPriorityQueue(Ite first, Ite last, const value_compare& pred) : c(first, last), comp(pred) { std::make_heap(c.begin(), c.end(), comp); }
				template <class Ite>
				ExPriorityQueue(Ite first, Ite last, const value_compare& pred, container_type&& cont) : c(std::move(cont)), comp(pred) { c.insert(c.end(), first, last); std::make_heap(c.begin(), c.end(), comp); }

				template <class Alloc, std::enable_if_t<std::uses_allocator_v<container_type, Alloc>, std::nullptr_t> = nullptr>
				explicit ExPriorityQueue(const Alloc& alloc) noexcept(std::is_nothrow_constructible_v<container_type, const Alloc&>&& is_nothrow_default_constructible_v<value_compare>) : c(alloc), comp() {}
				template <class Alloc, std::enable_if_t<std::uses_allocator_v<container_type, Alloc>, std::nullptr_t> = nullptr>
				ExPriorityQueue(const value_compare& pred, const Alloc& alloc) noexcept(std::is_nothrow_constructible_v<container_type, const Alloc&>&& is_nothrow_copy_constructible_v<value_compare>) : c(alloc), comp(pred) {}
				template <class Alloc, std::enable_if_t<std::uses_allocator_v<container_type, Alloc>, std::nullptr_t> = nullptr>
				ExPriorityQueue(const value_compare& pred, const container_type& cont, const Alloc& alloc) : c(cont, alloc), comp(pred) { std::make_heap(c.begin(), c.end(), comp); }
				template <class Alloc, std::enable_if_t<std::uses_allocator_v<container_type, Alloc>, std::nullptr_t> = nullptr>
				ExPriorityQueue(const value_compare& pred, container_type&& cont, const Alloc& alloc) : c(std::move(cont), alloc), comp(pred) { std::make_heap(c.begin(), c.end(), comp); }
				template <class Alloc, std::enable_if_t<std::uses_allocator_v<container_type, Alloc>, std::nullptr_t> = nullptr>
				ExPriorityQueue(const ExPriorityQueue& right, const Alloc& alloc) : c(right.c, alloc), comp(right.comp) {}
				template <class Alloc, std::enable_if_t<std::uses_allocator_v<container_type, Alloc>, std::nullptr_t> = nullptr>
				ExPriorityQueue(ExPriorityQueue&& right, const Alloc& alloc) noexcept(std::is_nothrow_constructible_v<container_type, container_type, const Alloc&>&& std::is_nothrow_move_constructible_v<value_compare>) : c(std::move(right.c), alloc), comp(std::move(right.comp)) {}

				NODISCARD inline iterator begin() noexcept { return c.begin(); }
				NODISCARD inline iterator end() noexcept { return c.end(); }
				NODISCARD inline const_iterator begin() const noexcept { return c.begin(); }
				NODISCARD inline const_iterator end() const noexcept { return c.end(); }
				NODISCARD inline const_iterator cbegin() const noexcept { return c.cbegin(); }
				NODISCARD inline const_iterator cend() const noexcept { return c.cend(); }
				NODISCARD inline reverse_iterator rbegin() noexcept { return c.rbegin(); }
				NODISCARD inline reverse_iterator rend() noexcept { return c.rend(); }
				NODISCARD inline const_reverse_iterator rbegin() const noexcept { return c.rbegin(); }
				NODISCARD inline const_reverse_iterator rend() const noexcept { return c.rend(); }
				NODISCARD inline const_reverse_iterator crbegin() const noexcept { return c.crbegin(); }
				NODISCARD inline const_reverse_iterator crend() const noexcept { return c.crend(); }

				NODISCARD inline bool empty() const noexcept(noexcept(c.empty())) { return c.empty(); }
				NODISCARD inline size_type size() const noexcept(noexcept(c.size())) { return c.size(); }
				NODISCARD inline const_reference top() const noexcept(noexcept(c.front())) { return c.front(); }

				inline void push(const value_type& val) { c.push_back(val); std::push_heap(c.begin(), c.end(), comp); }
				inline void push(value_type&& val) { c.push_back(std::move(val)); std::push_heap(c.begin(), c.end(), comp); }
				template <class... Args>
				inline void emplace(Args&&... val) { c.emplace_back(std::forward<Args>(val)...); std::push_heap(c.begin(), c.end(), comp); }
				inline void pop() { std::pop_heap(c.begin(), c.end(), comp); c.pop_back(); }

				inline void reserve(const size_type n) { return c.reseve(n); }
				inline void clear() noexcept { c.clear(); }

				inline void swap(ExPriorityQueue& right) noexcept { using namespace std; swap(c, right.c); swap(comp, right.comp); }
			};

			template<class Type, size_type Count>
			class PagingMemory final {
			public:
				using value_type = Type;
				using allocator_type = std::allocator<value_type>;
				using traits = std::allocator_traits<allocator_type>;

				using pointer = typename traits::pointer;
				using const_pointer = typename traits::const_pointer;

				static_assert(std::is_same_v<value_type, traits::value_type>);
				static_assert(std::is_pod_v<value_type>);
			private:

				NODISCARD inline static constexpr size_type GetPagingCount() noexcept { return Count; }

				std::stack<pointer> unusedMemory;
				std::vector<std::pair<uintptr_t, size_type>> memory;

				pointer first = nullptr;
				pointer last = nullptr;
				pointer reserved = nullptr;

				allocator_type alloc;

				NODISCARD inline uintptr_t toInteger(const const_pointer p) const noexcept {
					WARN_PUSH_DISABLE(26490);
					return reinterpret_cast<uintptr_t>(p);
					WARN_POP();
				}
				NODISCARD inline pointer toPointer(const uintptr_t p) const noexcept {
					WARN_PUSH_DISABLE(26490);
					return reinterpret_cast<pointer>(p);
					WARN_POP();
				}

				inline void deploy(pointer p) noexcept {
					first = p;
					last = p;
					reserved = first + GetPagingCount();
				}
				inline void recall() noexcept {
					first = nullptr;
					last = nullptr;
					reserved = nullptr;
				}

			public:

				PagingMemory() noexcept = default;
				~PagingMemory() {
					for (auto rIt = memory.rbegin(), rend = memory.rbegin(); rIt != rend; ++rIt) {
						traits::deallocate(alloc, toPointer(rIt->first), GetPagingCount());
					}
				}

				NODISCARD pointer allocate(const size_type size) {
					assert(size <= GetPagingCount());

					if (first != nullptr) {
						const size_type capacity = std::distance(last, reserved);
						if (capacity < size) {
							recall();
						}
					}

					if (first == nullptr) {
						if (not unusedMemory.empty()) {
							deploy(unusedMemory.top());
							unusedMemory.pop();
						}
						else {
							pointer p = traits::allocate(alloc, GetPagingCount());
							memory.emplace_back(toInteger(p), 0);
							std::sort(memory.begin(), memory.end());
							deploy(p);
						}
					}

					pointer ret = last;
					last += size;

					return ret;
				}

				template <class... Args>
				NODISCARD pointer create(Args&&... val) {
					return traits::construct(alloc, allocate(1), std::forward<Args>(val)...);
				}

				void release(const const_pointer data) {
					using pair = decltype(memory)::value_type;
					const uintptr_t ptr = toInteger(data) - GetPagingCount();
					const auto it = std::upper_bound(memory.begin(), memory.end(), pair(ptr, 0));

					assert(0 < it->second);
					it->second--;
					if (it->second <= 0) {
						if (it->first != toInteger(first)) {
							unusedMemory.push(toPointer(it->first));
						}
					}
				}
			};

			class Stream {
			public:
				using value_type = std::byte;
				using pointer = value_type*;
			private:
				pointer first = nullptr;
				pointer last = nullptr;
				pointer reserved = nullptr;

				inline void add(const size_type n) noexcept {
					WARN_PUSH_DISABLE(26481);
					last += n;
					WARN_POP();
				}
				inline void sub(const size_type n) noexcept {
					WARN_PUSH_DISABLE(26481);
					last -= n;
					WARN_POP();
				}
			public:
				Stream() = default;
				Stream(pointer data, pointer reserved) noexcept : first(data), last(data), reserved(reserved) {}

				template<typename Ptr>
				NODISCARD inline Ptr reinterpret() const noexcept {
					WARN_PUSH_DISABLE(26490);
					return reinterpret_cast<Ptr>(last);
					WARN_POP();
				};

				template<typename Type>
				NODISCARD inline Type get() noexcept {
					assert(sizeof(Type) <= capacity());
					Type ret = *(reinterpret<Type*>());
					add(sizeof(Type));
					return ret;
				}

				template<typename Type>
				NODISCARD inline Type pop() noexcept {
					assert(sizeof(Type) <= size());
					sub(sizeof(Type));
					return *(reinterpret<Type*>());
				}

				inline void pop(Stream& stream, const size_type length) noexcept {
					assert(length <= size());
					assert(length <= stream.capacity());
					sub(length);
					std::memcpy(stream.last, last, length);
					stream.add(length);
				}

				template<typename Type>
				inline void set(const Type value) noexcept {
					assert(sizeof(Type) <= capacity());
					*(reinterpret<Type*>()) = value;
					add(sizeof(Type));
				}

				inline void copy(void* ptr, const size_type length) noexcept {
					assert(length <= capacity());
					std::memcpy(last, ptr, length);
					add(length);
				}

				inline void seek(const size_type length) noexcept {
					assert(length <= capacity());
					add(length);
				}

				inline void write(void* ptr, const size_type length) noexcept {
					assert(length <= capacity());
					std::memcpy(ptr, last, length);
					add(length);
				}

				NODISCARD inline size_type size() const noexcept { return std::distance(first, last); }
				NODISCARD inline size_type capacity() const noexcept { return std::distance(last, reserved); }

				NODISCARD inline bool endOfStream() const noexcept { return 0 == capacity(); }
				NODISCARD inline bool hasStream() const noexcept { return 0 < capacity(); }

				NODISCARD inline std::pair<pointer, pointer> data() const noexcept { return { first,last }; }
			};

			template<typename MemoSize>
			class Patch {
			public:
				using value_type = Stream::value_type;
				using pointer = Stream::pointer;

				using MemoSizeType = MemoSize;
			private:
				pointer first = nullptr;
				pointer last = nullptr;
			public:
				Patch() = default;
				Patch(pointer first, pointer last) noexcept : first(first), last(last) {}

				NODISCARD inline bool hasValue() const noexcept { return first != last; }
				NODISCARD inline pointer address() const noexcept { return first; }

				/** @brief 戻す */
				inline void undo() const noexcept {
					assert(data != nullptr);

					Stream stream(first, last);
					while (stream.hasStream()) {
						const auto ptr = stream.get<void*>();
						const auto size = stream.get<MemoSizeType>();
						stream.seek(size);
						stream.write(ptr, size);
					}
					assert(stream.endOfStream());
				}
				/** @brief 適応する */
				inline void redo() const noexcept {
					assert(data != nullptr);

					Stream stream(first, last);
					while (stream.hasStream()) {
						const auto ptr = stream.get<void*>();
						const auto size = stream.get<MemoSizeType>();
						stream.write(ptr, size);
						stream.seek(size);
					}
					assert(stream.endOfStream());
				}
			};

			template<size_type Count, typename MemoSize = unsigned short>
			class Buffer {
			public:
				using value_type = Stream::value_type;
				using pointer = Stream::pointer;

				using MemoSizeType = MemoSize;
			private:
				Stream stream{};
				value_type buffer[Count]{};
				bool isLocked = false;
				size_type modiftSize = 0;
			public:
				Buffer() noexcept {
					WARN_PUSH_DISABLE(26485);
					stream = Stream(buffer, 0, Count);
					WARN_POP();
				}

				inline void lock() noexcept { isLocked = true; }
				inline void unlock() noexcept { isLocked = false; }

				/** @brief 変更蓄積 */
				inline void modify(void* data, const size_type size) noexcept {
					if (not isLocked) { return; }

					const size_type writeSize = size + sizeof(MemoSizeType) + sizeof(void*);

					assert(0 < size);
					assert(size < std::numeric_limits<MemoSizeType>::max() - (sizeof(MemoSizeType) + sizeof(void*)));
					assert(writeSize <= stream.capacity());

					stream.copy(data, size);
					stream.set<MemoSizeType>(narrow_cast<MemoSizeType>(size));
					stream.set<void*>(data);

					modiftSize += size;
				}

				inline size_type wholeSize() const noexcept { return stream.size() + modiftSize; }
				inline Patch<MemoSizeType> commit(Stream buf) noexcept {
					assert(isLocked);

					while (buf.hasStream()) {
						const auto ptr = stream.pop<void*>();
						buf.set<void*>(ptr);
						const auto size = stream.pop<MemoSizeType>();
						buf.set<MemoSizeType>(size);
						buf.copy(ptr, size);// 変更後の値
						stream.pop(buf, size);// 変更前の値
					}
					assert(buf.endOfStream());
					modiftSize = 0;

					auto [first, last] = stream.data();
					return Patch<MemoSizeType>(first, last);
				}
			};

			class Memo {
			public:
				using value_type = Stream::value_type;
				using pointer = Stream::pointer;

				using MemoSizeType = unsigned short;
			private:
				Buffer<16_K, MemoSizeType> buffer{};
				PagingMemory<std::byte, 1_M> pool;
			public:
				inline void lock() { buffer.lock(); }
				inline void unlock() { buffer.unlock(); }

				inline void modify(void* data, const size_type size) noexcept { buffer.modify(data, size); }

				template<class Class>
				inline void modify(Class& data) noexcept {
					WARN_PUSH_DISABLE(26474);
					buffer.modify(reinterpret_cast<void*>(std::addressof(data)), sizeof(Class));
					WARN_POP();
				}

				/** @brief 差分パッチ作成 */
				NODISCARD inline Patch<MemoSizeType> commit() {
					const size_type wholeSize = buffer.wholeSize();
					pointer first = pool.allocate(wholeSize);
					const Stream buf(first, first + wholeSize);
					return buffer.commit(buf);
				}
				inline void release(const Patch<MemoSizeType>& patch) { pool.release(patch.address()); }
			};

			template<class Space>
			struct MemoSingleton {
				inline static Memo memo;

				inline static void Modify() noexcept { memo.modify(*ptr); }
				template<typename Type>
				inline static void Modify(Type* ptr, const Type& o) noexcept { if (*ptr != o) { Modify(); (*ptr) = o; } }
			};

			template<typename Type, class Space>
			class Ref {
			private:
				using IMemo = MemoSingleton<Space>;
				Type* ptr = nullptr;
			public:
				Ref(const Type* ptr) noexcept : ptr(ptr) {}

				NODISCARD inline const Ref& change(const Type& o) noexcept { IMemo::Modify(ptr, o)  return *this; }
				NODISCARD inline operator Type() const noexcept { return *ptr; }
			};
			template<typename Type, class Space>
			class Value {
			private:
				using IMemo = MemoSingleton<Space>;
				Type val{};
			public:
				Value() noexcept {}
				Value(const Type val) noexcept : val(val) {}

				NODISCARD inline const Value& change(const Type& o) noexcept { IMemo::Modify(&val, o)  return *this; }
				NODISCARD inline operator Type() const noexcept { return val; }
			};
			template<typename Type, size_type Size, class Space>
			class Array {
			private:
				using IMemo = MemoSingleton<Space>;
				std::array<Type, Size> elems;
			public:

				Array() noexcept {};
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
			template<typename Type, size_type Size, class Space>
			class Vector {};

			template<class Class>
			struct MemoType {
				template<typename Type>
				using Value = Value<Type, Class>;
				template<typename Type, size_type Size>
				using Array = Array<Type, Size, Class>;
				template<typename Type, size_type Size>
				using Vector = Vector<Type, Size, Class>;
			};

			template<class Config>
			struct SearchNode {};
			template<class Config>
			class SearchNodePool {};

			template<class Config>
			class SearchTemplate {};

			template<class Config>
			class BeamSearchTemplate {};

			template<class SearchClass>
			struct SearchManager {

			};

		}
	}


}
