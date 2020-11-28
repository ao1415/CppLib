#pragma once

#include <cmath>

namespace alib
{

	/**
	 * @brief べき乗テーブルクラス
	 * 
	 * @tparam Size 
	 */
	template<size_t Size>
	class GeometricSequence
	{
	private:

		double m_data[Size];

	public:

		GeometricSequence(double radix)
		{
			static_assert(Size > 0);

			m_data[0] = 1.0;

			for (size_t i = 1; i < Size; i++)
			{
				m_data[i] = m_data[i - 1] * radix;
			}

		}

		inline double operator[](const size_t index) const { return m_data[index]; }

		constexpr size_t size() const { return Size; }

	};

}
