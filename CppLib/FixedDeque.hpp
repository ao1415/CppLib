#pragma once

#include <array>
#include "Basic.hpp"

namespace alib {

	/// <summary>
	/// 要素数が固定のdeque
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
		/// コンストラクタ
		/// </summary>
		FixedDeque() = default;
		/// <summary>
		/// コンストラクタ
		/// </summary>
		FixedDeque(const FixedDeque& other) = default;
		/// <summary>
		/// コンストラクタ
		/// </summary>
		FixedDeque(FixedDeque&& other) {
			begin = other.begin;
			end = other.end;
			element = other.element;
			m_data = std::move(other.m_data);
		}

		/// <summary>
		/// コピー演算子
		/// </summary>
		/// <param name="other">Deque</param>
		/// <returns>自分の参照</returns>
		FixedDeque& operator=(const FixedDeque& other) = default;
		/// <summary>
		/// ムーブ演算子
		/// </summary>
		/// <param name="other">Deque</param>
		/// <returns>自分の参照</returns>
		FixedDeque& operator=(FixedDeque&& other) = default;

		/// <summary>
		/// 先頭に要素を追加
		/// </summary>
		/// <param name="val">要素</param>
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
		/// 終端に要素を追加
		/// </summary>
		/// <param name="val">要素</param>
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
		/// 先頭に要素を追加
		/// </summary>
		/// <param name="val">要素</param>
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
		/// 終端に要素を追加
		/// </summary>
		/// <param name="val">要素</param>
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
		/// 先頭の要素を削除する
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
		/// 終端の要素を削除する
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
		/// 最大要素数を返す
		/// </summary>
		/// <returns>最大要素数</returns>
		constexpr size_t size() const noexcept {
			return containerAllocator(Size);
		}
		/// <summary>
		/// 現在の要素数を返す
		/// </summary>
		/// <returns>現在の要素数</returns>
		size_t element_num() const noexcept {
			return element;
		}

		/// <summary>
		/// 追加された要素がないかを返す
		/// </summary>
		/// <returns>要素がなければばtrue, それ以外ならfalse</returns>
		bool empty() const noexcept {
			return element == 0;
		}
		/// <summary>
		/// 要素がいっぱいかを返す
		/// </summary>
		/// <returns>いっぱいならばtrue, それ以外ならfalse</returns>
		bool full() const noexcept {
			return element >= size();
		}

		/// <summary>
		/// 要素にアクセスする
		/// </summary>
		/// <param name="pos">先頭からの番地</param>
		/// <returns>指定された要素</returns>
		const Type& at(size_t pos) const {
			if (pos >= element) throw std::out_of_range("FixedDeque::at");

			size_t n = begin + pos;
			n &= bit_mask();
			return m_data[n];
		}
		/// <summary>
		/// 要素にアクセスする
		/// </summary>
		/// <param name="pos">先頭からの番地</param>
		/// <returns>指定された要素</returns>
		Type& at(size_t pos) {
			if (pos >= element) throw std::out_of_range("FixedDeque::at");

			size_t n = begin + pos;
			n &= bit_mask();
			return m_data[n];
		}

		/// <summary>
		/// 要素にアクセスする
		/// </summary>
		/// <param name="pos">先頭からの番地</param>
		/// <returns>指定された要素</returns>
		const Type& operator[](size_t pos) const {
			size_t n = begin + pos;
			n &= bit_mask();
			return m_data[n];
		}
		/// <summary>
		/// 要素にアクセスする
		/// </summary>
		/// <param name="pos">先頭からの番地</param>
		/// <returns>指定された要素</returns>
		Type& operator[](size_t pos) {
			size_t n = begin + pos;
			n &= bit_mask();
			return m_data[n];
		}

		/// <summary>
		/// 先頭の要素を返す
		/// </summary>
		/// <returns>先頭の要素</returns>
		const Type& front() const {
			return m_data[begin];
		}
		/// <summary>
		/// 先頭の要素を返す
		/// </summary>
		/// <returns>先頭の要素</returns>
		Type& front() {
			return m_data[begin];
		}

		/// <summary>
		/// 終端の要素を返す
		/// </summary>
		/// <returns>終端の要素</returns>
		const Type& back() const {
			return m_data[(end - 1) & bit_mask()];
		}
		/// <summary>
		/// 終端の要素を返す
		/// </summary>
		/// <returns>終端の要素</returns>
		Type& back() {
			return m_data[(end - 1) & bit_mask()];
		}

		/// <summary>
		/// 要素を削除する
		/// </summary>
		void clear() noexcept {
			begin = 0;
			end = 0;
			element = 0;
			m_data.swap(ContainerType());
		}

		/// <summary>
		/// 別のDequeと交換する
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
