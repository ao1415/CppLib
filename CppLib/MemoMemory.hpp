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

namespace Search {

	using Int = int32_t;
	using UInt = uint32_t;

	constexpr UInt kbToB(unsigned long long kByte) noexcept { return narrow_cast<UInt>(kByte * 1024); }
	constexpr UInt mbToB(unsigned long long mByte) noexcept { return kbToB(mByte * 1024); }

#pragma warning(push)
#pragma warning(disable:26467)
	constexpr UInt kbToB(long double kByte) noexcept { return narrow_cast<UInt>(kByte * 1024); }
	constexpr UInt mbToB(long double mByte) noexcept { return kbToB(mByte * 1024); }
#pragma warning(pop)

	constexpr UInt operator"" _KB(unsigned long long kByte) noexcept { return kbToB(kByte); }
	constexpr UInt operator"" _MB(unsigned long long mByte) noexcept { return mbToB(mByte); }
	constexpr UInt operator"" _KB(long double kByte) noexcept { return kbToB(kByte); }
	constexpr UInt operator"" _MB(long double mByte) noexcept { return mbToB(mByte); }

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

		/** @brief �o�C�g�X�g���[�� */
		pointer stream;
		/** @brief �X�g���[���̒��� */
		UInt length;

		/** @brief �X�g���[�����̓ǂݎ��ʒu */
		UInt readPosition;

		/** @brief �X�g���[�����̏������݈ʒu */
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
#pragma warning(disable:26473 26474 26481 26490)
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

	class MemoryStreamPool {
	public:

		static constexpr UInt MaxMemsize = 1_MB;

	private:

		std::vector<std::shared_ptr<MemoryStream::value_type[]>> fragmentPools;
		MemoryStream fragment{ nullptr,0 };

		MemoryStream alloc() {
			fragmentPools.push_back(MemoryStream::MakeShared(MaxMemsize));
			return MemoryStream(fragmentPools.back().get(), MaxMemsize);
		}

	public:

		inline MemoryStream alloc(const UInt size) {
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
		 * @brief �o�b�t�@�X�g���[���ɃR�s�[����
		 * @tparam Class �R�s�[�Ώ�
		 * @param data �R�s�[�Ώ�
		*/
		template<class Class, typename std::enable_if_t<!std::is_const_v<Class>, std::nullptr_t> = nullptr>
		inline bool push(Class& data) noexcept {

			//���b�N���͎��s
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
		 * @brief �o�b�t�@�X�g���[������Ō���̃f�[�^�����o��
		 * @return �Ō���̃o�b�t�@�f�[�^<target, size, buffer>
		*/
		NODISCARD inline std::tuple<void*, UInt, void*> pop() noexcept {
			auto target = popBack<void*>();
			const auto size = popBack<UInt>();
			auto buffer = popBack(size);
			return std::make_tuple(target, size, buffer);
		}

		inline void free(const void*) const noexcept {}

		/**
		 * @brief ���b�N
		*/
		inline void lock() {
			locks.emplace_back(writePosition, redoSize);
			redoSize = 0;
		}
		/**
		 * @brief ���b�N����
		*/
		inline void unlock() noexcept {
			assert(!locks.empty());
			std::tie(writePosition, redoSize) = locks.back();
			locks.pop_back();
		}

		/**
		 * @brief ���b�N���
		 * @return true:���b�N�Ȃ�
		*/
		inline bool enabled() noexcept {
			return !locks.empty();
		}

		/**
		 * @brief �����߂�
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
		inline MemoryStream build(MemoryStreamPool pool) {
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
		 * @brief �����߂��p�b�`�K��
		 * @param patch �p�b�`
		*/
		inline void undo() noexcept {
			while (!endOfStream()) { adaptation(); }
		}

		/**
		 * @brief �����߂��p�b�`�K��
		 * @param patch �p�b�`
		*/
		inline void redo() noexcept {
			while (!endOfStream()) { adaptation(); }
		}

	public:

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

		MemoMemory() noexcept {}

		inline MemoryStream build() { return buffer.build(memoryPool); }

	};

}
