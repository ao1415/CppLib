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

	/** @brief �o�C�g�X�g���[�� */
	pointer stream;
	/** @brief �X�g���[���̒��� */
	UInt length;

	/** @brief �X�g���[�����̓ǂݎ��ʒu */
	UInt readPosition;

	/** @brief �X�g���[�����̏������݈ʒu */
	UInt writePosition;

	/** @brief �X�g���[�����̈ʒu */
	std::shared_ptr<value_type[]> garbage;

	/**
	 * @brief �X�g���[���̌��݈ʒu�̃|�C���^���擾����
	 * @tparam Type�i�Z�p�^�A�|�C���^�^�j
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
	 * @brief �X�g���[���̌��݈ʒu�̃|�C���^���擾����
	 * @tparam Type�i�Z�p�^�A�|�C���^�^�j
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
	 * @brief �X�g���[���̌��݈ʒu�̃|�C���^���擾����
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
	 * @brief �w�肳�ꂽ�^�ŃX�g���[������ǂݎ��
	 * @tparam Type �ǂݎ��^�i�Z�p�^�j
	 * @return �l
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
	 * @brief �w�肳�ꂽ�T�C�Y�ŃX�g���[������ǂݎ��
	 * @param count �ǂݎ��o�C�g��
	 * @return void�|�C���^
	*/
	NODISCARD inline void* read(const UInt count) noexcept {
		assert(readPosition + count <= length);
		assert(readPosition + count <= writePosition);

		void* p = readPointer<void>();
		readPosition += count;
		return p;
	}

	/**
	 * @brief �w�肳�ꂽ�o�C�g�����X�g���[������ǂݎ��
	 * @param buffer �ǂݎ���X�g���[��
	 * @param count �ǂݎ��o�C�g��
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
	 * @brief �w�肳�ꂽ�^�ŃX�g���[������ǂݎ��폜����
	 * @tparam Type �ǂݎ��^�i�Z�p�^�j
	 * @return �l
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
	 * @brief �w�肳�ꂽ�T�C�Y�ŃX�g���[������ǂݎ��폜����
	 * @tparam count �ǂݎ��o�C�g��
	 * @return void�|�C���^
	*/
	NODISCARD inline void* popBack(const UInt count) noexcept {
		assert(count <= writePosition);

		writePosition -= count;
		readPosition = std::min(readPosition, writePosition);
		return writePointer<void*>();
	}

	/**
	 * @brief �w�肳�ꂽ�o�C�g�����X�g���[������ǂݎ��폜����
	 * @param buffer �ǂݎ���X�g���[��
	 * @param count �ǂݎ��o�C�g��
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
	 * @brief �w�肳�ꂽ�^�ŃX�g���[���ɏ�������
	 * @tparam Type �������݌^�i�Z�p�^�j
	 * @param value �l
	*/
	template<typename Type, typename std::enable_if_t<std::is_arithmetic_v<Type> || std::is_pointer_v<Type>, std::nullptr_t> = nullptr>
	NODISCARD inline void write(const Type value) noexcept {
		assert(writePosition + sizeof(Type) <= length);

		*writePointer<Type>() = value;
		writePosition += sizeof(Type);
	}

	/**
	 * @brief �w�肳�ꂽ�o�C�g�����X�g���[���֏�������
	 * @param buffer �������ރX�g���[��
	 * @param count �������݃o�C�g��
	 * @return
	*/
	NODISCARD inline void write(const void* buffer, const UInt count) noexcept {
		assert(writePosition + count <= length);

		std::memcpy(writePointer(), buffer, count);
		writePosition += count;
	}

	/**
	 * @brief �w�肳�ꂽ�f�[�^���X�g���[���֏�������
	 * @param data �������ݑΏ�
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
	 * @brief �o�b�t�@�X�g���[���ɃR�s�[����
	 * @tparam Class �R�s�[�Ώ�
	 * @param data �R�s�[�Ώ�
	*/
	template<class Class, typename std::enable_if_t<!std::is_const_v<Class>, std::nullptr_t> = nullptr>
	inline void push(Class& data) noexcept {
		write(data);
		write(narrow_cast<UInt>(sizeof(Class)));
		write(std::addressof(data));
	}

	/**
	 * @brief �o�b�t�@�X�g���[������Ō���̃f�[�^�����o��
	 * @return �Ō���̃o�b�t�@�f�[�^
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
	 * @brief �o�b�t�@�X�g���[���ɃR�s�[����
	 * @tparam Class �R�s�[�Ώ�
	 * @param data �R�s�[�Ώ�
	*/
	template<class Class, typename std::enable_if_t<!std::is_const_v<Class>, std::nullptr_t> = nullptr>
	inline void push(Class& data) noexcept {
		write(std::addressof(data));
		write(narrow_cast<UInt>(sizeof(Class)));
		write(data);
	}

	/**
	 * @brief �o�b�t�@�X�g���[������擪�̃f�[�^�����o��
	 * @return �擪�̃o�b�t�@�f�[�^
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
	 * @brief ���b�N
	*/
	inline static void lock() {
		locks.emplace_back(buffer.getWritePosition(), redoSize);
		redoSize = 0;
	}
	/**
	 * @brief ���b�N����
	*/
	inline static void unlock() noexcept {
		assert(!locks.empty());
		MemoryStream::UInt bufferSize;
		std::tie(bufferSize, redoSize) = locks.back();
		locks.pop_back();
		buffer.setWritePosition(bufferSize);
	}

	/**
	 * @brief ���b�N���
	 * @return true:���b�N�Ȃ�
	*/
	inline static bool enabled() noexcept {
		return !locks.empty();
	}

	template<class Class>
	inline static bool modify(const Class& data) noexcept {

		//���b�N���͎��s
		if (!enabled()) {
			return false;
		}

		buffer.push(data);
		redoSize += sizeof(Class);

		return true;
	}

	/**
	 * @brief �����߂�
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
	 * @brief �����߂��p�b�`�K��
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
	 * @brief ������
	*/
	inline void clear() noexcept {
		usedMems = 0;
		mymemsize = 0;
	}

	/**
	 * @brief �̈�m��
	 * @param size �̈�T�C�Y
	 * @return �|�C���^
	*/
	inline MemoryStream alloc(const UInt size) {
		// todo:���񃁃����m�ۂ��Ȃ��悤�ɂ���
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
