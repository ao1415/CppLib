#pragma once

#include "Basic.hpp"
#include <vector>
#include <cassert>
#include <memory>
#include <cstdint>
#include <type_traits>
#include <string>
#include <sstream>
#include <iomanip>

#ifndef _MSC_VER
#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)
#else
#define likely(x) x
#define unlikely(x) x
#endif

class MemoryStream {
public:

	using value_type = std::byte;
	using pointer = value_type*;

	using Int = int32_t;
	using UInt = uint32_t;

protected:

	/** @brief バイトストリーム */
	pointer stream;
	/** @brief ストリームの長さ */
	UInt length;

	/** @brief ストリーム内の読み取り位置 */
	UInt readPosition;

	/** @brief ストリーム内の書き込み位置 */
	UInt writePosition;

	/** @brief ストリーム内の位置 */
	std::shared_ptr<value_type[]> garbage;

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
#pragma warning(disable:26474 26481 26490)
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

	NODISCARD MemoryStream(const UInt length) {

#pragma warning(push)
#pragma warning(disable:26409)
		this->garbage = std::shared_ptr<value_type[]>(new value_type[length]);
#pragma warning(pop)
		this->stream = garbage.get();
		this->length = length;
		this->readPosition = 0;
		this->writePosition = 0;
	}

	NODISCARD MemoryStream(const std::shared_ptr<value_type[]>& stream, const UInt length) noexcept {
		this->garbage = stream;
		this->stream = garbage.get();
		this->length = length;
		this->readPosition = 0;
		this->writePosition = 0;
	}

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
	 * @brief 指定されたバイト数をストリームから読み取る
	 * @param buffer 読み取り先ストリーム
	 * @param count 読み取りバイト数
	 * @return
	*/
	NODISCARD inline void read(MemoryStream& buffer, const UInt count) noexcept {
		assert(readPosition + count <= length);
		assert(readPosition + count <= writePosition);

		assert(buffer.writePosition + count <= buffer.length);

		std::memcpy(buffer.writePointer(), readPointer(), count);
		readPosition += count;
		buffer.writePosition += count;
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
	 * @brief 指定されたバイト数をストリームから読み取り削除する
	 * @param buffer 読み取り先ストリーム
	 * @param count 読み取りバイト数
	 * @return
	*/
	NODISCARD inline void popBack(MemoryStream& buffer, const UInt count) noexcept {
		assert(count <= writePosition);

		assert(buffer.writePosition + count <= buffer.length);

		writePosition -= count;
		readPosition = std::min(readPosition, writePosition);
		std::memcpy(buffer.writePointer(), writePointer(), count);
		buffer.writePosition += count;
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

	inline UInt getWritePosition() const noexcept { return writePosition; }
	inline void setWritePosition(const UInt position) noexcept {
		this->writePosition = position;
		readPosition = std::min(readPosition, writePosition);
	}

	inline bool endOfStream() const noexcept { return readPosition >= writePosition; }

	std::string toString() const {
		std::stringstream ss;
		ss << std::hex;

		forange(i, length) {
			ss << std::setw(2) << std::setfill('0') << static_cast<UInt>(at(i)) << " ";
		}

		return ss.str();
	}
};

class BufferMemoryStream : public MemoryStream {
public:

	struct Node {
		void* buffer;
		void* target;
		UInt size;
	};

	using MemoryStream::MemoryStream;

	/**
	 * @brief バッファストリームにコピーする
	 * @tparam Class コピー対象
	 * @param data コピー対象
	*/
	template<class Class, typename std::enable_if_t<!std::is_const_v<Class>, std::nullptr_t> = nullptr>
	inline void push(Class& data) noexcept {
		write(data);
		write(narrow_cast<UInt>(sizeof(Class)));
		write(std::addressof(data));
	}

	/**
	 * @brief バッファストリームから最後尾のデータを取り出す
	 * @return 最後尾のバッファデータ
	*/
	NODISCARD inline Node pop() noexcept {
		Node node{};

		node.target = popBack<void*>();
		node.size = popBack<UInt>();

		node.buffer = popBack(node.size);

		return node;
	}

};

class PatchMemoryStream : public MemoryStream {
public:

	struct Node {
		void* buffer;
		void* target;
		UInt size;
	};

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

	/**
	 * @brief バッファストリームから先頭のデータを取り出す
	 * @return 先頭のバッファデータ
	*/
	NODISCARD inline Node dequeue() noexcept {
		Node node{};

		node.target = read<void*>();
		node.size = read<UInt>();
		node.buffer = read(node.size);

		return node;
	}

};


struct MemoNodeFooter {
	using size_type = size_t;
	using index_type = size_t;

	MemoNodeFooter(size_type size, index_type pointerIndex) noexcept :
		size(size), pointerIndex(pointerIndex) {}

	const size_type size;
	const index_type pointerIndex;
};

class MemoMemory {
public:
	using UInt = MemoryStream::UInt;

private:
	static constexpr int MaxMemsize = 1048576 - 256;
	inline static UInt redoSize;
	inline static std::vector<std::tuple<UInt, UInt>> locks;
	inline static BufferMemoryStream buffer{ 262144 };
	std::vector<MemoryStream::pointer> mems;

	int usedMems = 0;
	int mymemsize = 0;
	MemoryStream::pointer mymem = 0;

public:
	inline static void free(const void*) noexcept {}

	/**
	 * @brief ロック
	*/
	inline static void lock() {
		locks.emplace_back(buffer.getWritePosition(), redoSize);
		redoSize = 0;
	}
	/**
	 * @brief ロック解除
	*/
	inline static void unlock() noexcept {
		assert(!locks.empty());
		MemoryStream::UInt bufferSize;
		std::tie(bufferSize, redoSize) = locks.back();
		locks.pop_back();
		buffer.setWritePosition(bufferSize);
	}

	/**
	 * @brief ロック状態
	 * @return true:ロックなし
	*/
	inline static bool enabled() noexcept {
		return !locks.empty();
	}

	template<class Class>
	inline static bool modify(const Class& data) noexcept {

		//ロック中は失敗
		if (!enabled()) {
			return false;
		}

		buffer.push(data);
		redoSize += sizeof(Class);

		return true;
	}

	/**
	 * @brief 巻き戻し
	*/
	inline static void undo() noexcept {

		assert(enabled());

		const UInt baseBufferSize = std::get<0>(locks.back());

		while (baseBufferSize < buffer.getWritePosition()) {
			const auto node = buffer.pop();
			std::memcpy(node.target, node.buffer, node.size);
		}

		assert(baseBufferSize == buffer.getWritePosition());
		redoSize = 0;
	}

	/**
	 * @brief 巻き戻しパッチ適応
	 * @param patch
	 * @return
	*/
	static inline void undo(PatchMemoryStream& patch) noexcept {

		while (!patch.endOfStream()) {
			const auto node = patch.dequeue();
			std::memcpy(node.target, node.buffer, node.size);
		}

	}

	static inline void redo(PatchMemoryStream& patch) noexcept {

		while (!patch.endOfStream()) {
			const auto node = patch.dequeue();
			std::memcpy(node.target, node.buffer, node.size);
		}

	}

	MemoMemory() noexcept {}

	/**
	 * @brief 初期化
	*/
	inline void clear() noexcept {
		usedMems = 0;
		mymemsize = 0;
	}

	/**
	 * @brief 領域確保
	 * @param size 領域サイズ
	 * @return ポインタ
	*/
	inline MemoryStream alloc(const UInt size) {
		// todo:毎回メモリ確保しないようにする
		return MemoryStream(size);
	}

	inline MemoryStream build() {

		assert(!locks.empty());

		const UInt base_buffer_size = std::get<0>(locks.back());

		const UInt buildSize = (buffer.getWritePosition() - base_buffer_size) + redoSize;

		MemoryStream ret = alloc(buildSize);

		while (base_buffer_size < buffer.getWritePosition()) {

			const auto node = buffer.pop();
			ret.write<void*>(node.target);
			ret.write<UInt>(node.size);
			ret.write(node.target, node.size);
			ret.write(node.buffer, node.size);
		}

		redoSize = 0;

		return ret;
	}

};
