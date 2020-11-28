#pragma once

#include <array>
#include "Basic.hpp"

namespace alib
{

	/**
	 * @brief 要素数が固定のdeque
	 * 
	 * @tparam Type 格納する型
	 * @tparam Size 格納できる最大数
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
		 * @brief コンストラクタ
		 * 
		 */
		FixedDeque() = default;
		/**
		 * @brief コンストラクタ
		 * 
		 * @param other コピー対象
		 */
		FixedDeque(const FixedDeque &other) = default;
		/**
		 * @brief コンストラクタ
		 * 
		 * @param other ムーブ対象
		 */
		FixedDeque(FixedDeque &&other)
		{
			begin = other.begin;
			end = other.end;
			element = other.element;
			m_data = std::move(other.m_data);
		}

		/**
		 * @brief コピー演算子
		 * 
		 * @param other コピー対象
		 * @return FixedDeque& 自分の参照
		 */
		FixedDeque &operator=(const FixedDeque &other) = default;
		/**
		 * @brief ムーブ演算子
		 * 
		 * @param other ムーブ対象
		 * @return FixedDeque& 自分の参照
		 */
		FixedDeque &operator=(FixedDeque &&other) = default;

		/**
		 * @brief 先頭に要素を追加
		 * 
		 * @param val 要素
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
		 * @brief 終端に要素を追加
		 * 
		 * @param val 要素
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
		 * @brief 先頭に要素を追加
		 * 
		 * @param val 要素
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
		 * @brief 終端に要素を追加
		 * 
		 * @param val 要素
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
		 * @brief 先頭の要素を削除する
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
		 * @brief 先頭の要素を削除する
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
		 * @brief 最大要素数を返す
		 * 
		 * @return constexpr size_t 最大要素数
		 */
		constexpr size_t size() const noexcept
		{
			return containerAllocator(Size);
		}
		/**
		 * @brief 現在の要素数を返す
		 * 
		 * @return size_t 現在の要素数
		 */
		size_t element_num() const noexcept
		{
			return element;
		}

		/**
		 * @brief 容量が空かを返す
		 * 
		 * @return true 空
		 * @return false 要素あり
		 */
		inline bool empty() const noexcept
		{
			return element == 0;
		}
		/**
		 * @brief 容量がいっぱいかを返す
		 * 
		 * @return true いっぱい
		 * @return false 空きあり
		 */
		inline bool full() const noexcept
		{
			return element >= size();
		}

		/**
		 * @brief 要素にアクセスする
		 * 
		 * @param pos 先頭からの番地
		 * @return const Type& 指定された要素
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
		 * @brief 要素にアクセスする
		 * 
		 * @param pos 先頭からの番地
		 * @return Type& 指定された要素
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
		 * @brief 要素にアクセスする
		 * 
		 * @param pos 先頭からの番地
		 * @return const Type& 指定された要素
		 */
		const Type &operator[](size_t pos) const
		{
			size_t n = begin + pos;
			n &= bit_mask();
			return m_data[n];
		}
		/**
		 * @brief 要素にアクセスする
		 * 
		 * @param pos 先頭からの番地
		 * @return Type& 指定された要素
		 */
		Type &operator[](size_t pos)
		{
			size_t n = begin + pos;
			n &= bit_mask();
			return m_data[n];
		}

		/**
		 * @brief 先頭の要素を返す
		 * 
		 * @return const Type& 先頭の要素
		 */
		const Type &front() const
		{
			return m_data[begin];
		}
		/**
		 * @brief 先頭の要素を返す
		 * 
		 * @return Type& 先頭の要素
		 */
		Type &front()
		{
			return m_data[begin];
		}

		/**
		 * @brief 終端の要素を返す
		 * 
		 * @return const Type& 終端の要素
		 */
		const Type &back() const
		{
			return m_data[(end - 1) & bit_mask()];
		}
		/**
		 * @brief 終端の要素を返す
		 * 
		 * @return Type& 終端の要素
		 */
		Type &back()
		{
			return m_data[(end - 1) & bit_mask()];
		}

		/**
		 * @brief 要素を削除する
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
		 * @brief 別のDequeと交換する
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
	 * @brief 要素数が固定のdeque（エラーチェックなし）
	 * 
	 * @tparam Type 格納する型
	 * @tparam Size 格納できる最大数
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
		 * @brief コンストラクタ
		 * 
		 */
		FixedDeque() = default;
		/**
		 * @brief コンストラクタ
		 * 
		 * @param other コピー対象
		 */
		FixedDeque(const FixedDeque &other) = default;
		/**
		 * @brief コンストラクタ
		 * 
		 * @param other ムーブ対象
		 */
		FixedDeque(FixedDeque &&other)
		{
			begin = other.begin;
			end = other.end;
			element = other.element;
			m_data = std::move(other.m_data);
		}

		/**
		 * @brief コピー演算子
		 * 
		 * @param other コピー対象
		 * @return FixedDeque& 自分の参照
		 */
		FixedDeque &operator=(const FixedDeque &other) = default;
		/**
		 * @brief ムーブ演算子
		 * 
		 * @param other ムーブ対象
		 * @return FixedDeque& 自分の参照
		 */
		FixedDeque &operator=(FixedDeque &&other) = default;

		/**
		 * @brief 先頭に要素を追加
		 * 
		 * @param val 要素
		 */
		void push_front(const Type &val)
		{
			element++;
			begin = (begin - 1) & bit_mask();

			m_data[begin] = val;
		}
		/**
		 * @brief 先頭に要素を追加
		 * 
		 * @param val 要素
		 */
		void push_front(Type &&val)
		{
			element++;
			begin = (begin - 1) & bit_mask();

			m_data[begin] = std::move(val);
		}

		/**
		 * @brief 終端に要素を追加
		 * 
		 * @param val 要素
		 */
		void push_back(const Type &val)
		{
			m_data[end] = val;

			element++;
			end = (end + 1) & bit_mask();
		}
		/**
		 * @brief 終端に要素を追加
		 * 
		 * @param val 要素
		 */
		void push_back(Type &&val)
		{
			m_data[end] = std::move(val);

			element++;
			end = (end + 1) & bit_mask();
		}

		/**
		 * @brief 先頭の要素を削除する
		 * 
		 */
		void pop_front()
		{
			element--;
			begin = (begin + 1) & bit_mask();
		}
		/**
		 * @brief 先頭の要素を削除する
		 * 
		 */
		void pop_back()
		{
			element--;
			end = (end - 1) & bit_mask();
		}

		/**
		 * @brief 最大要素数を返す
		 * 
		 * @return constexpr size_t 最大要素数
		 */
		constexpr size_t size() const noexcept
		{
			return containerAllocator(Size);
		}
		/**
		 * @brief 現在の要素数を返す
		 * 
		 * @return size_t 現在の要素数
		 */
		size_t element_num() const noexcept
		{
			return element;
		}

		/**
		 * @brief 容量が空かを返す
		 * 
		 * @return true 空
		 * @return false 要素あり
		 */
		inline bool empty() const noexcept
		{
			return element == 0;
		}
		/**
		 * @brief 容量がいっぱいかを返す
		 * 
		 * @return true いっぱい
		 * @return false 空きあり
		 */
		inline bool full() const noexcept
		{
			return element >= size();
		}

		/**
		 * @brief 先頭の要素を返す
		 * 
		 * @return const Type& 先頭の要素
		 */
		const Type &front() const
		{
			return m_data[begin];
		}
		/**
		 * @brief 先頭の要素を返す
		 * 
		 * @return Type& 先頭の要素
		 */
		Type &front()
		{
			return m_data[begin];
		}

		/**
		 * @brief 終端の要素を返す
		 * 
		 * @return const Type& 終端の要素
		 */
		const Type &back() const
		{
			return m_data[(end - 1) & bit_mask()];
		}
		/**
		 * @brief 終端の要素を返す
		 * 
		 * @return Type& 終端の要素
		 */
		Type &back()
		{
			return m_data[(end - 1) & bit_mask()];
		}

		/**
		 * @brief 要素を削除する
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
