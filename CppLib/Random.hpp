#pragma once

#include <array>

namespace alib {

	struct XorShift {
		uint32_t x;
		XorShift() : x(2463534242U) {}
		uint32_t rand() {
			x ^= (x << 13);
			x ^= (x >> 17);
			x ^= (x << 5);
			return x;
		}

	};

	/**
	 * @brief xoshiro128++
	 *
	 */
	class XoShiro128
	{
	public:

		using value_type = uint32_t;

	private:

		value_type m_state0 = 723471715u;
		value_type m_state1 = 2497366906u;
		value_type m_state2 = 2064144800u;
		value_type m_state3 = 2008045182u;

	public:

		XoShiro128() {}

		[[nodiscard]]
		value_type next()
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

		[[nodiscard]]
		double nextDouble()
		{
			const value_type v1 = next();
			const value_type v2 = next();
			const uint64_t v = (static_cast<uint64_t>(v1) << 32) | static_cast<uint64_t>(v2);

			return static_cast<double>((v >> 11) * 0x1.0p-53);
		}

	};

}
