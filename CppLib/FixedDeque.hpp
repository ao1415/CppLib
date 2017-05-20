#pragma once

#include <array>
#include "Basic.hpp"

namespace alib {

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

		void showArray() const {
			for (size_t i = 0; i < size(); i++) {
				cerr << i << ":" << m_data[i] << endl;
			}
		}

		FixedDeque() = default;
		FixedDeque(const FixedDeque& other) = default;
		FixedDeque(FixedDeque&& other) {
			begin = other.begin;
			end = other.end;
			element = other.element;
			m_data = std::move(other.m_data);
		}

		FixedDeque& operator=(const FixedDeque& other) = default;
		FixedDeque& operator=(FixedDeque&& other) = default;

		void push_front(const Type& val) {
			element++;
			begin = (begin - 1) & bit_mask();
			cerr << "begin:" << begin << endl;

			m_data[begin] = val;
		}
		void push_back(const Type& val) {
			m_data[end] = val;

			element++;
			end = (end + 1) & bit_mask();
			cerr << "end:" << end << endl;
		}

		void push_front(Type&& val) {
			element++;
			begin = (begin - 1) & bit_mask();
			cerr << "begin:" << begin << endl;

			m_data[begin] = std::move(val);
		}
		void push_back(Type&& val) {
			m_data[end] = std::move(val);

			element++;
			end = (end + 1) & bit_mask();
			cerr << "end:" << end << endl;
		}

		void pop_front() {
			if (empty())
			{
				_DEBUG_ERROR("FixedDeque empty before pop");
			}
			else
			{
				element--;
				begin = (begin + 1) & bit_mask();
			}
		}
		void pop_back() {
			if (empty())
			{
				_DEBUG_ERROR("FixedDeque empty before pop");
			}
			else
			{
				element--;
				end = (end - 1) & bit_mask();
			}
		}

		constexpr size_t size() const noexcept {
			return containerAllocator(Size);
		}
		size_t element_num() const noexcept {
			return element;
		}

		bool empty() const noexcept {
			return element == 0;
		}
		bool full() const noexcept {
			return element >= size();
		}

		const Type& at(size_t pos) const {
			if (pos >= element) throw std::out_of_range("FixedDeque::at");

			size_t n = begin + pos;
			n &= bit_mask();
			return m_data[n];
		}
		Type& at(size_t pos) {
			if (pos >= element) throw std::out_of_range("FixedDeque::at");

			size_t n = begin + pos;
			n &= bit_mask();
			return m_data[n];
		}

		const Type& operator[](size_t pos) const {
			size_t n = begin + pos;
			n &= bit_mask();
			return m_data[n];
		}
		Type& operator[](size_t pos) {
			size_t n = begin + pos;
			n &= bit_mask();
			return m_data[n];
		}

		const Type& front() const {
			return m_data[begin];
		}
		Type& front() {
			return m_data[begin];
		}

		const Type& back() const {
			return m_data[end];
		}
		Type& back() {
			return m_data[end];
		}

		void clear() noexcept {
			begin = 0;
			end = 0;
			element = 0;
		}

		void swap(FixedDeque& other) noexcept(_Is_nothrow_swappable<Type>::value)
		{
			m_data.swap(other.m_data);

			std::swap(begin, other.begin);
			std::swap(end, other.end);
			std::swap(element, other.element);
		}

	};

}
