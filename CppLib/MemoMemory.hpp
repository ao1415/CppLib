#pragma once

#include "Basic.hpp"
#include <array>
#include <vector>
#include <cassert>
#include <memory>
#include <cstdint>
#include <type_traits>
#include <string>
#include <sstream>
#include <iomanip>

namespace Search {
	using Int = int32_t;
	using UInt = uint32_t;
}

namespace Search::Memory {

	NODISCARD constexpr UInt kbToB(unsigned long long kByte) noexcept { return narrow_cast<UInt>(kByte * 1024); }
	NODISCARD constexpr UInt mbToB(unsigned long long mByte) noexcept { return kbToB(mByte * 1024); }

#pragma warning(push)
#pragma warning(disable:26467)
	NODISCARD constexpr UInt kbToB(long double kByte) noexcept { return narrow_cast<UInt>(kByte * 1024); }
	NODISCARD constexpr UInt mbToB(long double mByte) noexcept { return kbToB(mByte * 1024); }
#pragma warning(pop)

	NODISCARD constexpr UInt operator"" _KB(unsigned long long kByte) noexcept { return kbToB(kByte); }
	NODISCARD constexpr UInt operator"" _MB(unsigned long long mByte) noexcept { return mbToB(mByte); }
	NODISCARD constexpr UInt operator"" _KB(long double kByte) noexcept { return kbToB(kByte); }
	NODISCARD constexpr UInt operator"" _MB(long double mByte) noexcept { return mbToB(mByte); }

	class MemoMemory;
	class MemoryStreamPool;

	class MemoryStream {
	public:

		using value_type = std::byte;
		using pointer = value_type*;

		using shared_pointer = std::shared_ptr<MemoryStream::value_type[]>;

		inline static shared_pointer MakeShared(const UInt size) {
#pragma warning(push)
#pragma warning(disable:26409)
			return std::shared_ptr<MemoryStream::value_type[]>(new MemoryStream::value_type[size]);
#pragma warning(pop)
		}

	protected:

		friend MemoryStreamPool;

		/** @brief バイトストリーム */
		pointer stream;
		/** @brief ストリームの長さ */
		UInt length;

		/** @brief ストリーム内の読み取り位置 */
		UInt readPosition;

		/** @brief ストリーム内の書き込み位置 */
		UInt writePosition;

		NODISCARD MemoryStream(MemoryStream& stream, const UInt length) noexcept {
			assert(stream.writePosition + length < stream.length);
			this->stream = stream.writePointer<value_type>();
			this->length = length;
			this->readPosition = 0;
			this->writePosition = 0;

			stream.writePosition += length;
		}

		NODISCARD MemoryStream(pointer stream, const UInt length) noexcept {
			this->stream = stream;
			this->length = length;
			this->readPosition = 0;
			this->writePosition = 0;
		}

		/**
		 * @brief ストリームの現在位置のポインタを取得する
		 * @tparam Type（算術型、ポインタ型）
		 * @return
		*/
		template<typename Type = void>
		NODISCARD inline Type* readPointer() noexcept {
#pragma warning(push)
#pragma warning(disable:26474 26481 26490)
			return reinterpret_cast<Type*>(std::addressof(stream[readPosition]));
#pragma warning(pop)
		}

		/**
		 * @brief ストリームの現在位置のポインタを取得する
		 * @tparam Type（算術型、ポインタ型）
		 * @return
		*/
		template<typename Type = void>
		NODISCARD inline Type* writePointer() noexcept {
#pragma warning(push)
#pragma warning(disable:26473 26474 26481 26490)
			return reinterpret_cast<Type*>(std::addressof(stream[writePosition]));
#pragma warning(pop)
		}

		/**
		 * @brief ストリームの現在位置のポインタを取得する
		 * @return
		*/
		NODISCARD inline value_type at(const UInt index) const noexcept {
			assert(index < length);
#pragma warning(push)
#pragma warning(disable:26481)
			return stream[index];
#pragma warning(pop)
		}

	public:

		/**
		 * @brief 指定された型でストリームから読み取る
		 * @tparam Type 読み取り型（算術型）
		 * @return 値
		*/
		template<typename Type, typename std::enable_if_t<std::is_arithmetic_v<Type> || std::is_pointer_v<Type>, std::nullptr_t> = nullptr>
		NODISCARD inline Type read() noexcept {
			assert(readPosition + sizeof(Type) <= length);
			assert(readPosition + sizeof(Type) <= writePosition);

			Type t = *readPointer<Type>();
			readPosition += sizeof(Type);
			return t;
		}

		/**
		 * @brief 指定されたサイズでストリームから読み取る
		 * @param count 読み取りバイト数
		 * @return voidポインタ
		*/
		NODISCARD inline void* read(const UInt count) noexcept {
			assert(readPosition + count <= length);
			assert(readPosition + count <= writePosition);

			void* p = readPointer<void>();
			readPosition += count;
			return p;
		}

		/**
		 * @brief 指定された型でストリームから読み取り削除する
		 * @tparam Type 読み取り型（算術型）
		 * @return 値
		*/
		template<typename Type, typename std::enable_if_t<std::is_arithmetic_v<Type> || std::is_pointer_v<Type>, std::nullptr_t> = nullptr>
		NODISCARD inline Type popBack() noexcept {
			assert(sizeof(Type) <= writePosition);

			writePosition -= sizeof(Type);
			readPosition = std::min(readPosition, writePosition);
			Type t = *writePointer<Type>();
			return t;
		}

		/**
		 * @brief 指定されたサイズでストリームから読み取り削除する
		 * @tparam count 読み取りバイト数
		 * @return voidポインタ
		*/
		NODISCARD inline void* popBack(const UInt count) noexcept {
			assert(count <= writePosition);

			writePosition -= count;
			readPosition = std::min(readPosition, writePosition);
			return writePointer<void*>();
		}

		/**
		 * @brief 指定された型でストリームに書き込む
		 * @tparam Type 書き込み型（算術型）
		 * @param value 値
		*/
		template<typename Type, typename std::enable_if_t<std::is_arithmetic_v<Type> || std::is_pointer_v<Type>, std::nullptr_t> = nullptr>
		NODISCARD inline void write(const Type value) noexcept {
			assert(writePosition + sizeof(Type) <= length);

			*writePointer<Type>() = value;
			writePosition += sizeof(Type);
		}

		/**
		 * @brief 指定されたバイト数をストリームへ書き込む
		 * @param buffer 書き込むストリーム
		 * @param count 書き込みバイト数
		 * @return
		*/
		NODISCARD inline void write(const void* buffer, const UInt count) noexcept {
			assert(writePosition + count <= length);

			std::memcpy(writePointer(), buffer, count);
			writePosition += count;
		}

		/**
		 * @brief 指定されたデータをストリームへ書き込む
		 * @param data 書き込み対象
		*/
		template<class Class, typename std::enable_if_t<!std::is_arithmetic_v<Class> && !std::is_pointer_v<Class>, std::nullptr_t> = nullptr>
		inline void write(const Class& data) noexcept {
			assert(writePosition + sizeof(Class) <= length);

			std::memcpy(writePointer(), std::addressof(data), sizeof(Class));
			writePosition += sizeof(Class);
		}

		NODISCARD inline bool endOfStream() const noexcept { return readPosition >= writePosition; }

		NODISCARD std::string toString() const {
			std::stringstream ss;
			ss << std::hex;

			forange(i, length) {
				ss << std::setw(2) << std::setfill('0') << static_cast<UInt>(at(i)) << " ";
			}

			return ss.str();
		}
	};

	class MemoryStreamPool {
	public:

		static constexpr UInt MaxMemsize = 1_MB;

	private:

		std::vector<std::shared_ptr<MemoryStream::value_type[]>> fragmentPools;
		MemoryStream fragment{ nullptr,0 };

		NODISCARD MemoryStream alloc() {
			fragmentPools.push_back(MemoryStream::MakeShared(MaxMemsize));
			return MemoryStream(fragmentPools.back().get(), MaxMemsize);
		}

	public:

		NODISCARD inline MemoryStream alloc(const UInt size) {
			assert(size <= MaxMemsize);

			if (fragment.writePosition + size >= fragment.length) {
				fragment = alloc();
			}

			return MemoryStream(fragment);
		}

	};

	template<UInt Size>
	class BufferMemoryStream : public MemoryStream {
	private:

		UInt redoSize = 0;
		std::vector<std::tuple<UInt, UInt>> locks{};
		value_type buffer[Size] = {};

	public:

		using MemoryStream::MemoryStream;

#pragma warning(push)
#pragma warning(disable:26485)
		NODISCARD BufferMemoryStream() noexcept : MemoryStream(buffer, Size) {}
#pragma warning(pop)

		/**
		 * @brief バッファストリームにコピーする
		 * @tparam Class コピー対象
		 * @param data コピー対象
		*/
		template<class Class, typename std::enable_if_t<!std::is_const_v<Class>, std::nullptr_t> = nullptr>
		NODISCARD inline bool push(Class& data) noexcept {

			//ロック中は失敗
			if (!enabled()) {
				return false;
			}

			write(data);
			write(narrow_cast<UInt>(sizeof(Class)));
			write(std::addressof(data));

			redoSize += sizeof(Class);

			return true;
		}

		/**
		 * @brief バッファストリームから最後尾のデータを取り出す
		 * @return 最後尾のバッファデータ<target, size, buffer>
		*/
		NODISCARD inline std::tuple<void*, UInt, void*> pop() noexcept {
			auto target = popBack<void*>();
			const auto size = popBack<UInt>();
			auto buffer = popBack(size);
			return std::make_tuple(target, size, buffer);
		}

		inline void free(const void*) const noexcept {}

		/**
		 * @brief ロック
		*/
		inline void lock() {
			locks.emplace_back(writePosition, redoSize);
			redoSize = 0;
		}
		/**
		 * @brief ロック解除
		*/
		inline void unlock() noexcept {
			assert(!locks.empty());
			std::tie(writePosition, redoSize) = locks.back();
			locks.pop_back();
		}

		/**
		 * @brief ロック状態
		 * @return true:ロックなし
		*/
		NODISCARD inline bool enabled() noexcept { return !locks.empty(); }

		/**
		 * @brief 巻き戻し
		*/
		inline void undo() noexcept {

			assert(enabled());

			const UInt baseBufferSize = std::get<0>(locks.back());

			while (baseBufferSize < writePosition) {
				auto [target, size, buffer] = pop();
				std::memcpy(target, buffer, size);
			}

			assert(baseBufferSize == writePosition);

			redoSize = 0;
		}

		/**
		 * @brief
		 * @param pool
		 * @return
		*/
		NODISCARD inline MemoryStream build(MemoryStreamPool pool) {
			assert(!locks.empty());
			const UInt baseBufferSize = std::get<0>(locks.back());

			MemoryStream ret = pool.alloc((writePosition - baseBufferSize) + redoSize);
			while (baseBufferSize < writePosition) {
				auto [target, size, buffer] = pop();
				ret.write<void*>(target);
				ret.write<UInt>(size);
				ret.write(target, size);
				ret.write(buffer, size);
			}
			redoSize = 0;

			return ret;
		}


	};

	class PatchMemoryStream : public MemoryStream {
	private:

		friend MemoMemory;

		inline void adaptation() noexcept {
			void* target = read<void*>();
			const UInt size = read<UInt>();
			const void* buffer = read(size);

			std::memcpy(target, buffer, size);
		}

		/**
		 * @brief 巻き戻しパッチ適応
		 * @param patch パッチ
		*/
		inline void undo() noexcept {
			while (!endOfStream()) { adaptation(); }
		}

		/**
		 * @brief 巻き戻しパッチ適応
		 * @param patch パッチ
		*/
		inline void redo() noexcept {
			while (!endOfStream()) { adaptation(); }
		}

	public:

		using MemoryStream::MemoryStream;

		/**
		 * @brief バッファストリームにコピーする
		 * @tparam Class コピー対象
		 * @param data コピー対象
		*/
		template<class Class, typename std::enable_if_t<!std::is_const_v<Class>, std::nullptr_t> = nullptr>
		inline void push(Class& data) noexcept {
			write(std::addressof(data));
			write(narrow_cast<UInt>(sizeof(Class)));
			write(data);
		}

	};

	class MemoMemory final {
	private:
		inline static BufferMemoryStream<MemoryStreamPool::MaxMemsize> buffer;
		MemoryStreamPool memoryPool;

	public:

		template<class Class>
		inline static bool modify(const Class& data) noexcept { return buffer.push(data); }

		inline static void undo() noexcept { buffer.undo(); }
		inline static void undo(PatchMemoryStream& patch) noexcept { patch.undo(); }
		inline static void redo(PatchMemoryStream& patch) noexcept { patch.redo(); }

		NODISCARD MemoMemory() noexcept {}

		inline MemoryStream build() { return buffer.build(memoryPool); }

	};

}

namespace Search::Ref {

	template<class Class>
	class RefMemory {
	private:

		Class* pointer = nullptr;

		inline void modify() { Memory::MemoMemory::modify(*pointer); }
		inline void modify(const Class& o) {
			if (*pointer != o) {
				modify();
				*pointer = o;
			}
		}

	public:
		inline RefMemory(Class* p) : pointer(p) {}

		inline void operator=(const Class& o) { modify(o); }
		inline void operator+=(const Class& o) { Class v = *pointer + o; modify(v); }
		inline void operator-=(const Class& o) { Class v = *pointer - o; modify(v); }
		inline void operator*=(const Class& o) { Class v = *pointer * o; modify(v); }
		inline void operator/=(const Class& o) { Class v = *pointer / o; modify(v); }
		inline void operator&=(const Class& o) { Class v = *pointer & o; modify(v); }
		inline void operator|=(const Class& o) { Class v = *pointer | o; modify(v); }
		inline void operator^=(const Class& o) { Class v = *pointer ^ o; modify(v); }

		inline RefMemory<Class>& operator++() { modify(); ++(*pointer); return *this; }
		inline Class operator++(int) { modify(); Class v = *pointer; ++(*pointer); return v; }
		inline RefMemory<Class>& operator--() { modify(); --(*pointer); return *this; }
		inline Class operator--(int) { modify(); Class v = *pointer; --(*pointer); return v; }

		NODISCARD inline Class value() const { return *pointer; }
		NODISCARD inline operator Class() const { return *pointer; }
	};

	template<typename Type, UInt Size>
	class RefArray : public std::array<Type, Size> {
	public:

		using size_type = UInt;
		using base = std::array<Type, Size>;

		RefArray() noexcept : base() {};
		RefArray(const RefArray& other) = default;
		RefArray(RefArray&& other) = default;
		RefArray& operator=(const RefArray& other) = default;
		RefArray& operator=(RefArray&& other) = default;

		inline void set(const size_type idx, const Type& o) noexcept { Memory::MemoMemory::modify(base::operator[](idx)); base::operator[](idx) = o; }
		NODISCARD inline const Type& get(const size_type idx) const noexcept { return base::operator[](idx); }

		NODISCARD inline const Type& operator[](const size_type idx) const noexcept { return base::operator[](idx); }
		NODISCARD inline RefMemory<Type> operator[](const size_type idx) noexcept { return RefMemory<Type>(std::addressof(base::operator[](idx))); }

		NODISCARD inline const Type& at(const size_type idx) const noexcept { return base::at(idx); }
		NODISCARD inline RefMemory<Type> at(const size_type idx) noexcept { return RefMemory<Type>(std::addressof(base::at(idx))); }
	};

	template<typename Type, UInt Size>
	class RefVector : public std::array<Type, Size> {
	public:

		using size_type = UInt;
		using base = std::array<Type, Size>;

	private:

		size_type count = 0;

		NODISCARD inline void hasCapacity(const size_type n) { assert(0 <= n && n < Size); }
		NODISCARD inline void inside(const size_type idx) { assert(0 <= idx && idx < count); }
		NODISCARD inline void any() { assert(0 < count); }

	public:

		RefVector() noexcept : base() {};
		RefVector(const RefVector& other) = default;
		RefVector(RefVector&& other) = default;
		RefVector& operator=(const RefVector& other) = default;
		RefVector& operator=(RefVector&& other) = default;

		inline void set(const size_type idx, const Type& o) noexcept {
			inside(idx);
			Memory::MemoMemory::modify(base::operator[](idx));
			base::operator[](idx) = o;
		}
		NODISCARD inline const Type& get(const size_type idx) const noexcept {
			inside(idx);
			return base::operator[](idx);
		}

		inline void clear() noexcept { Memory::MemoMemory::modify(count); count = 0; }
		NODISCARD inline bool empty() const noexcept { return count == 0; }
		NODISCARD inline bool full() const noexcept { return count == base::size(); }
		NODISCARD inline size_type full_size() const noexcept { return base::size(); }
		NODISCARD inline size_type size() const noexcept { return count; }

		inline void push_back(const Type& value) noexcept {
			inside(idx);
			Memory::MemoMemory::modify(count);
			Memory::MemoMemory::modify(base::oprator[](count));
			base::oprator[](count) = value;
			++count;
		}
		template<class ...Args>
		inline void emplace_back(Args&& ...args) noexcept {
			inside(idx);
			Memory::MemoMemory::modify(count);
			Memory::MemoMemory::modify(base::oprator[](count));
			new(std::addressof(base::oprator[](count))) Type(std::forward<TyArgs>(args)...);
			++count;
		}

		inline void pop_back() noexcept {
			any();
			Memory::MemoMemory::modify(count);
			--count;
		}

		NODISCARD inline RefMemory<Type> front() noexcept { any(); return RefMemory<Type>(std::addressof(base::front())); }
		NODISCARD inline const Type& front() const noexcept { any(); return base::front(); }

		NODISCARD inline RefMemory<Type> back() noexcept { any(); return RefMemory<Type>(std::addressof(base::back())); }
		NODISCARD inline const Type& back() const noexcept { any(); return base::back(); }

		inline void resize(UInt n, const Type& value) noexcept {
			hasCapacity(n);
			Memory::MemoMemory::modify(count);
			if (count < n) {
				const auto addCount = n - count;
				Memory::MemoMemory::modify(std::addressof(base::oprator[](count)), sizeof(Type) * addCount);
				forstep(idx, count, n) { base::oprator[](i) = value; }
			}
			count = n;
		}
		inline void resize(int n) noexcept {
			hasCapacity(n);
			Memory::MemoMemory::modify(count);
			count = n;
		}

		NODISCARD inline const Type& operator[](const size_type idx) const noexcept { inside(idx); return base::operator[](idx); }
		NODISCARD inline RefMemory<Type> operator[](const size_type idx) noexcept { inside(idx); return RefMemory<Type>(std::addressof(base::operator[](idx))); }

		NODISCARD inline const Type& at(const size_type idx) const noexcept { inside(idx); return base::at(idx); }
		NODISCARD inline RefMemory<Type> at(const size_type idx) noexcept { inside(idx); return RefMemory<Type>(std::addressof(base::at(idx))); }

	};

}
