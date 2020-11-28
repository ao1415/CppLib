#pragma once

#include <array>

namespace alib
{

	class Random
	{
	public:
		using value_type = uint32_t;

		/**
		 * @brief 32bitの乱数値を取得する
		 *
		 * @return value_type
		 */
		[[nodiscard]] inline virtual value_type next() = 0;

		/**
		 * @brief [0.0, 1.0)の範囲の乱数値を取得する
		 *
		 * @return float [0.0, 1.0)
		 */
		[[nodiscard]] inline float nextFloat()
		{
			const value_type v = (next() >> 9) | 0b0'01111111'00000000000000000000000;

			return (*((float *)(&v))) - 1.0f;
		}
		/**
		 * @brief [0.0, 1.0)の範囲の乱数値を取得する
		 *
		 * @return double [0.0, 1.0)
		 */
		[[nodiscard]] inline double nextDouble()
		{
			const uint64_t v = (static_cast<uint64_t>(next()) << (32 - 12)) | 0b0'01111111111'0000000000000000000000000000000000000000000000000000ull;

			return (*((double *)(&v))) - 1.0;
		}
	};

	class XorShift : public Random
	{
	private:
		value_type m_state = 2463534242u;

	public:
		/**
		 * @brief 32bitの乱数値を取得する
		 *
		 * @return value_type
		 */
		[[nodiscard]] inline value_type next()
		{
			m_state ^= (m_state << 13);
			m_state ^= (m_state >> 17);
			m_state ^= (m_state << 5);

			return m_state;
		}
	};

	/**
	 * @brief xoshiro128++
	 *
	 */
	class XoShiro128 : public Random
	{
	private:
		value_type m_state0 = 723471715u;
		value_type m_state1 = 2497366906u;
		value_type m_state2 = 2064144800u;
		value_type m_state3 = 2008045182u;

	public:
		/**
		 * @brief 32bitの乱数値を取得する
		 *
		 * @return value_type
	 	*/
		[[nodiscard]] inline value_type next()
		{
			const value_type rotl_tmp = m_state0 + m_state3;
			const value_type result = ((rotl_tmp << 7) | (rotl_tmp >> (32 - 7))) + m_state0;

			const value_type t = m_state1 << 9;

			m_state2 ^= m_state0;
			m_state3 ^= m_state1;
			m_state1 ^= m_state2;
			m_state0 ^= m_state3;

			m_state2 ^= t;

			m_state3 = ((m_state3 << 11) | (m_state3 >> (32 - 11)));

			return result;
		}
	};

} // namespace alib
