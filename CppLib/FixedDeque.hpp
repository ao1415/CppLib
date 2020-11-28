#pragma once

#include <array>
#include "Basic.hpp"

namespace alib
{

	/**
	 * @brief �v�f�����Œ��deque
	 * 
	 * @tparam Type �i�[����^
	 * @tparam Size �i�[�ł���ő吔
	 */
	template <typename Type, size_t Size>
	class FixedDeque
	{
	private:
		using ContainerType = std::array<Type, containerAllocator(Size)>;

		ContainerType m_data;

		size_t begin = 0;
		size_t end = 0;
		size_t element = 0;

		constexpr size_t bit_mask()
		{
			return size() - 1;
		}

	public:
		/**
		 * @brief �R���X�g���N�^
		 * 
		 */
		FixedDeque() = default;
		/**
		 * @brief �R���X�g���N�^
		 * 
		 * @param other �R�s�[�Ώ�
		 */
		FixedDeque(const FixedDeque &other) = default;
		/**
		 * @brief �R���X�g���N�^
		 * 
		 * @param other ���[�u�Ώ�
		 */
		FixedDeque(FixedDeque &&other)
		{
			begin = other.begin;
			end = other.end;
			element = other.element;
			m_data = std::move(other.m_data);
		}

		/**
		 * @brief �R�s�[���Z�q
		 * 
		 * @param other �R�s�[�Ώ�
		 * @return FixedDeque& �����̎Q��
		 */
		FixedDeque &operator=(const FixedDeque &other) = default;
		/**
		 * @brief ���[�u���Z�q
		 * 
		 * @param other ���[�u�Ώ�
		 * @return FixedDeque& �����̎Q��
		 */
		FixedDeque &operator=(FixedDeque &&other) = default;

		/**
		 * @brief �擪�ɗv�f��ǉ�
		 * 
		 * @param val �v�f
		 */
		void push_front(const Type &val)
		{
			if (full())
			{
				throw std::out_of_range("FixedDeque full before push");
			}
			else
			{
				element++;
				begin = (begin - 1) & bit_mask();

				m_data[begin] = val;
			}
		}
		/**
		 * @brief �I�[�ɗv�f��ǉ�
		 * 
		 * @param val �v�f
		 */
		void push_back(const Type &val)
		{
			if (full())
			{
				throw std::out_of_range("FixedDeque full back push");
			}
			else
			{
				m_data[end] = val;

				element++;
				end = (end + 1) & bit_mask();
			}
		}

		/**
		 * @brief �擪�ɗv�f��ǉ�
		 * 
		 * @param val �v�f
		 */
		void push_front(Type &&val)
		{
			if (full())
			{
				throw std::out_of_range("FixedDeque full before push");
			}
			else
			{
				element++;
				begin = (begin - 1) & bit_mask();

				m_data[begin] = std::move(val);
			}
		}
		/**
		 * @brief �I�[�ɗv�f��ǉ�
		 * 
		 * @param val �v�f
		 */
		void push_back(Type &&val)
		{
			if (full())
			{
				throw std::out_of_range("FixedDeque full back push");
			}
			else
			{
				m_data[end] = std::move(val);

				element++;
				end = (end + 1) & bit_mask();
			}
		}

		/**
		 * @brief �擪�̗v�f���폜����
		 * 
		 */
		void pop_front()
		{
			if (empty())
			{
				throw std::out_of_range("FixedDeque empty before push");
			}
			else
			{
				element--;
				begin = (begin + 1) & bit_mask();
			}
		}
		/**
		 * @brief �擪�̗v�f���폜����
		 * 
		 */
		void pop_back()
		{
			if (empty())
			{
				throw std::out_of_range("FixedDeque empty back push");
			}
			else
			{
				element--;
				end = (end - 1) & bit_mask();
			}
		}

		/**
		 * @brief �ő�v�f����Ԃ�
		 * 
		 * @return constexpr size_t �ő�v�f��
		 */
		constexpr size_t size() const noexcept
		{
			return containerAllocator(Size);
		}
		/**
		 * @brief ���݂̗v�f����Ԃ�
		 * 
		 * @return size_t ���݂̗v�f��
		 */
		size_t element_num() const noexcept
		{
			return element;
		}

		/**
		 * @brief �e�ʂ��󂩂�Ԃ�
		 * 
		 * @return true ��
		 * @return false �v�f����
		 */
		inline bool empty() const noexcept
		{
			return element == 0;
		}
		/**
		 * @brief �e�ʂ������ς�����Ԃ�
		 * 
		 * @return true �����ς�
		 * @return false �󂫂���
		 */
		inline bool full() const noexcept
		{
			return element >= size();
		}

		/**
		 * @brief �v�f�ɃA�N�Z�X����
		 * 
		 * @param pos �擪����̔Ԓn
		 * @return const Type& �w�肳�ꂽ�v�f
		 */
		const Type &at(size_t pos) const
		{
			if (pos >= element)
				throw std::out_of_range("FixedDeque::at");

			size_t n = begin + pos;
			n &= bit_mask();
			return m_data[n];
		}
		/**
		 * @brief �v�f�ɃA�N�Z�X����
		 * 
		 * @param pos �擪����̔Ԓn
		 * @return Type& �w�肳�ꂽ�v�f
		 */
		Type &at(size_t pos)
		{
			if (pos >= element)
				throw std::out_of_range("FixedDeque::at");

			size_t n = begin + pos;
			n &= bit_mask();
			return m_data[n];
		}

		/**
		 * @brief �v�f�ɃA�N�Z�X����
		 * 
		 * @param pos �擪����̔Ԓn
		 * @return const Type& �w�肳�ꂽ�v�f
		 */
		const Type &operator[](size_t pos) const
		{
			size_t n = begin + pos;
			n &= bit_mask();
			return m_data[n];
		}
		/**
		 * @brief �v�f�ɃA�N�Z�X����
		 * 
		 * @param pos �擪����̔Ԓn
		 * @return Type& �w�肳�ꂽ�v�f
		 */
		Type &operator[](size_t pos)
		{
			size_t n = begin + pos;
			n &= bit_mask();
			return m_data[n];
		}

		/**
		 * @brief �擪�̗v�f��Ԃ�
		 * 
		 * @return const Type& �擪�̗v�f
		 */
		const Type &front() const
		{
			return m_data[begin];
		}
		/**
		 * @brief �擪�̗v�f��Ԃ�
		 * 
		 * @return Type& �擪�̗v�f
		 */
		Type &front()
		{
			return m_data[begin];
		}

		/**
		 * @brief �I�[�̗v�f��Ԃ�
		 * 
		 * @return const Type& �I�[�̗v�f
		 */
		const Type &back() const
		{
			return m_data[(end - 1) & bit_mask()];
		}
		/**
		 * @brief �I�[�̗v�f��Ԃ�
		 * 
		 * @return Type& �I�[�̗v�f
		 */
		Type &back()
		{
			return m_data[(end - 1) & bit_mask()];
		}

		/**
		 * @brief �v�f���폜����
		 * 
		 */
		void clear() noexcept
		{
			begin = 0;
			end = 0;
			element = 0;
			m_data.swap(ContainerType());
		}

		/**
		 * @brief �ʂ�Deque�ƌ�������
		 * 
		 * @param other Deque
		 */
		void swap(FixedDeque &other) noexcept(_Is_nothrow_swappable<Type>::value)
		{
			m_data.swap(other.m_data);

			std::swap(begin, other.begin);
			std::swap(end, other.end);
			std::swap(element, other.element);
		}
	};

	/**
	 * @brief �v�f�����Œ��deque�i�G���[�`�F�b�N�Ȃ��j
	 * 
	 * @tparam Type �i�[����^
	 * @tparam Size �i�[�ł���ő吔
	 */
	template <typename Type, size_t Size>
	class FastFixedDeque
	{
	private:
		using ContainerType = std::array<Type, containerAllocator(Size)>;

		ContainerType m_data;

		size_t begin = 0;
		size_t end = 0;
		size_t element = 0;

		constexpr size_t bit_mask()
		{
			return size() - 1;
		}

	public:
		/**
		 * @brief �R���X�g���N�^
		 * 
		 */
		FixedDeque() = default;
		/**
		 * @brief �R���X�g���N�^
		 * 
		 * @param other �R�s�[�Ώ�
		 */
		FixedDeque(const FixedDeque &other) = default;
		/**
		 * @brief �R���X�g���N�^
		 * 
		 * @param other ���[�u�Ώ�
		 */
		FixedDeque(FixedDeque &&other)
		{
			begin = other.begin;
			end = other.end;
			element = other.element;
			m_data = std::move(other.m_data);
		}

		/**
		 * @brief �R�s�[���Z�q
		 * 
		 * @param other �R�s�[�Ώ�
		 * @return FixedDeque& �����̎Q��
		 */
		FixedDeque &operator=(const FixedDeque &other) = default;
		/**
		 * @brief ���[�u���Z�q
		 * 
		 * @param other ���[�u�Ώ�
		 * @return FixedDeque& �����̎Q��
		 */
		FixedDeque &operator=(FixedDeque &&other) = default;

		/**
		 * @brief �擪�ɗv�f��ǉ�
		 * 
		 * @param val �v�f
		 */
		void push_front(const Type &val)
		{
			element++;
			begin = (begin - 1) & bit_mask();

			m_data[begin] = val;
		}
		/**
		 * @brief �擪�ɗv�f��ǉ�
		 * 
		 * @param val �v�f
		 */
		void push_front(Type &&val)
		{
			element++;
			begin = (begin - 1) & bit_mask();

			m_data[begin] = std::move(val);
		}

		/**
		 * @brief �I�[�ɗv�f��ǉ�
		 * 
		 * @param val �v�f
		 */
		void push_back(const Type &val)
		{
			m_data[end] = val;

			element++;
			end = (end + 1) & bit_mask();
		}
		/**
		 * @brief �I�[�ɗv�f��ǉ�
		 * 
		 * @param val �v�f
		 */
		void push_back(Type &&val)
		{
			m_data[end] = std::move(val);

			element++;
			end = (end + 1) & bit_mask();
		}

		/**
		 * @brief �擪�̗v�f���폜����
		 * 
		 */
		void pop_front()
		{
			element--;
			begin = (begin + 1) & bit_mask();
		}
		/**
		 * @brief �擪�̗v�f���폜����
		 * 
		 */
		void pop_back()
		{
			element--;
			end = (end - 1) & bit_mask();
		}

		/**
		 * @brief �ő�v�f����Ԃ�
		 * 
		 * @return constexpr size_t �ő�v�f��
		 */
		constexpr size_t size() const noexcept
		{
			return containerAllocator(Size);
		}
		/**
		 * @brief ���݂̗v�f����Ԃ�
		 * 
		 * @return size_t ���݂̗v�f��
		 */
		size_t element_num() const noexcept
		{
			return element;
		}

		/**
		 * @brief �e�ʂ��󂩂�Ԃ�
		 * 
		 * @return true ��
		 * @return false �v�f����
		 */
		inline bool empty() const noexcept
		{
			return element == 0;
		}
		/**
		 * @brief �e�ʂ������ς�����Ԃ�
		 * 
		 * @return true �����ς�
		 * @return false �󂫂���
		 */
		inline bool full() const noexcept
		{
			return element >= size();
		}

		/**
		 * @brief �擪�̗v�f��Ԃ�
		 * 
		 * @return const Type& �擪�̗v�f
		 */
		const Type &front() const
		{
			return m_data[begin];
		}
		/**
		 * @brief �擪�̗v�f��Ԃ�
		 * 
		 * @return Type& �擪�̗v�f
		 */
		Type &front()
		{
			return m_data[begin];
		}

		/**
		 * @brief �I�[�̗v�f��Ԃ�
		 * 
		 * @return const Type& �I�[�̗v�f
		 */
		const Type &back() const
		{
			return m_data[(end - 1) & bit_mask()];
		}
		/**
		 * @brief �I�[�̗v�f��Ԃ�
		 * 
		 * @return Type& �I�[�̗v�f
		 */
		Type &back()
		{
			return m_data[(end - 1) & bit_mask()];
		}

		/**
		 * @brief �v�f���폜����
		 * 
		 */
		void clear() noexcept
		{
			begin = 0;
			end = 0;
			element = 0;
		}
	};
} // namespace alib
