#pragma once

#include <array>
#include "Basic.hpp"

namespace alib {

	/// <summary>
	/// �v�f�����Œ��deque
	/// </summary>
	template<typename Type, size_t Size>
	class FixedDeque {
	private:

		using ContainerType = std::array<Type, containerAllocator(Size)>;

		ContainerType m_data;

		int begin = 0;
		int end = 0;
		int element = 0;

		constexpr size_t bit_mask() {
			return size() - 1;
		}

	public:

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		FixedDeque() = default;
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		FixedDeque(const FixedDeque& other) = default;
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		FixedDeque(FixedDeque&& other) {
			begin = other.begin;
			end = other.end;
			element = other.element;
			m_data = std::move(other.m_data);
		}

		/// <summary>
		/// �R�s�[���Z�q
		/// </summary>
		/// <param name="other">Deque</param>
		/// <returns>�����̎Q��</returns>
		FixedDeque& operator=(const FixedDeque& other) = default;
		/// <summary>
		/// ���[�u���Z�q
		/// </summary>
		/// <param name="other">Deque</param>
		/// <returns>�����̎Q��</returns>
		FixedDeque& operator=(FixedDeque&& other) = default;

		/// <summary>
		/// �擪�ɗv�f��ǉ�
		/// </summary>
		/// <param name="val">�v�f</param>
		void push_front(const Type& val) {
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
		/// <summary>
		/// �I�[�ɗv�f��ǉ�
		/// </summary>
		/// <param name="val">�v�f</param>
		void push_back(const Type& val) {
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

		/// <summary>
		/// �擪�ɗv�f��ǉ�
		/// </summary>
		/// <param name="val">�v�f</param>
		void push_front(Type&& val) {
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
		/// <summary>
		/// �I�[�ɗv�f��ǉ�
		/// </summary>
		/// <param name="val">�v�f</param>
		void push_back(Type&& val) {
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

		/// <summary>
		/// �擪�̗v�f���폜����
		/// </summary>
		void pop_front() {
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
		/// <summary>
		/// �I�[�̗v�f���폜����
		/// </summary>
		void pop_back() {
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

		/// <summary>
		/// �ő�v�f����Ԃ�
		/// </summary>
		/// <returns>�ő�v�f��</returns>
		constexpr size_t size() const noexcept {
			return containerAllocator(Size);
		}
		/// <summary>
		/// ���݂̗v�f����Ԃ�
		/// </summary>
		/// <returns>���݂̗v�f��</returns>
		size_t element_num() const noexcept {
			return element;
		}

		/// <summary>
		/// �ǉ����ꂽ�v�f���Ȃ�����Ԃ�
		/// </summary>
		/// <returns>�v�f���Ȃ���΂�true, ����ȊO�Ȃ�false</returns>
		bool empty() const noexcept {
			return element == 0;
		}
		/// <summary>
		/// �v�f�������ς�����Ԃ�
		/// </summary>
		/// <returns>�����ς��Ȃ��true, ����ȊO�Ȃ�false</returns>
		bool full() const noexcept {
			return element >= size();
		}

		/// <summary>
		/// �v�f�ɃA�N�Z�X����
		/// </summary>
		/// <param name="pos">�擪����̔Ԓn</param>
		/// <returns>�w�肳�ꂽ�v�f</returns>
		const Type& at(size_t pos) const {
			if (pos >= element) throw std::out_of_range("FixedDeque::at");

			size_t n = begin + pos;
			n &= bit_mask();
			return m_data[n];
		}
		/// <summary>
		/// �v�f�ɃA�N�Z�X����
		/// </summary>
		/// <param name="pos">�擪����̔Ԓn</param>
		/// <returns>�w�肳�ꂽ�v�f</returns>
		Type& at(size_t pos) {
			if (pos >= element) throw std::out_of_range("FixedDeque::at");

			size_t n = begin + pos;
			n &= bit_mask();
			return m_data[n];
		}

		/// <summary>
		/// �v�f�ɃA�N�Z�X����
		/// </summary>
		/// <param name="pos">�擪����̔Ԓn</param>
		/// <returns>�w�肳�ꂽ�v�f</returns>
		const Type& operator[](size_t pos) const {
			size_t n = begin + pos;
			n &= bit_mask();
			return m_data[n];
		}
		/// <summary>
		/// �v�f�ɃA�N�Z�X����
		/// </summary>
		/// <param name="pos">�擪����̔Ԓn</param>
		/// <returns>�w�肳�ꂽ�v�f</returns>
		Type& operator[](size_t pos) {
			size_t n = begin + pos;
			n &= bit_mask();
			return m_data[n];
		}

		/// <summary>
		/// �擪�̗v�f��Ԃ�
		/// </summary>
		/// <returns>�擪�̗v�f</returns>
		const Type& front() const {
			return m_data[begin];
		}
		/// <summary>
		/// �擪�̗v�f��Ԃ�
		/// </summary>
		/// <returns>�擪�̗v�f</returns>
		Type& front() {
			return m_data[begin];
		}

		/// <summary>
		/// �I�[�̗v�f��Ԃ�
		/// </summary>
		/// <returns>�I�[�̗v�f</returns>
		const Type& back() const {
			return m_data[(end - 1) & bit_mask()];
		}
		/// <summary>
		/// �I�[�̗v�f��Ԃ�
		/// </summary>
		/// <returns>�I�[�̗v�f</returns>
		Type& back() {
			return m_data[(end - 1) & bit_mask()];
		}

		/// <summary>
		/// �v�f���폜����
		/// </summary>
		void clear() noexcept {
			begin = 0;
			end = 0;
			element = 0;
			m_data.swap(ContainerType());
		}

		/// <summary>
		/// �ʂ�Deque�ƌ�������
		/// </summary>
		/// <param name="other">Deque</param>
		void swap(FixedDeque& other) noexcept(_Is_nothrow_swappable<Type>::value)
		{
			m_data.swap(other.m_data);

			std::swap(begin, other.begin);
			std::swap(end, other.end);
			std::swap(element, other.element);
		}

	};

}
