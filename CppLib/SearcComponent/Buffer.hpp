#pragma once

#include <limits>

#include "SearchComponentLib.hpp"
#include "Patch.hpp"
#include "Stream.hpp"

namespace alib::Search::Lib {

	template<size_type Count, typename MemoSize = unsigned short>
	class Buffer {
	public:
		using value_type = Stream::value_type;
		using pointer = Stream::pointer;

		using MemoSizeType = MemoSize;
	private:
		/** @brief �ύX���e�������݃X�g���[�� */
		Stream stream{};

		/** @brief �ύX���e�������݃o�b�t�@ */
		value_type buffer[Count]{};

		/** @brief �L�^�J�n�t���O */
		bool isLocked = false;

		/** @brief �����߂��o�b�t�@�T�C�Y */
		size_type modiftSize = 0;

	public:
		Buffer() noexcept {
			WARN_PUSH_DISABLE(26485);
			stream = Stream(buffer, Count);
			WARN_POP();
		}

		void lock() noexcept { isLocked = true; }
		void unlock() noexcept { isLocked = false; }

		/**
		 * @brief �ύX�L�^
		 * @param data �ύX�Ώۃ|�C���^
		 * @param size �ύX�T�C�Y
		*/
		void modify(void* data, const size_type size) noexcept {
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

		/**
		 * @brief �p�b�`�T�C�Y�擾
		 * @return �p�b�`�T�C�Y
		*/
		size_type wholeSize() const noexcept {
			return stream.size() + modiftSize;
		}

		/**
		 * @brief �����p�b�`�쐬
		 * @param buf �������ݐ�̃o�b�t�@
		 * @return �����p�b�`
		*/
		Patch<MemoSizeType> commit(Stream buf) noexcept {
			assert(isLocked);

			while (buf.hasStream()) {
				// �L�^�Ώۂ̃|�C���^
				const auto ptr = stream.pop<void*>();
				buf.set<void*>(ptr);

				// �L�^�T�C�Y
				const auto size = stream.pop<MemoSizeType>();
				buf.set<MemoSizeType>(size);

				buf.copy(ptr, size);// �ύX��̒l
				stream.pop(buf, size);// �ύX�O�̒l
			}
			assert(buf.endOfStream());
			modiftSize = 0;

			auto [first, last] = buf.data();
			return Patch<MemoSizeType>(first, last);
		}
	};

}
