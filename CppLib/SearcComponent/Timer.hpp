#pragma once

#include <chrono>
#include "../Basic.hpp"

namespace alib::Search::Lib {

	class Timer {
	private:
		using Rep = std::chrono::nanoseconds;
		using Duration = std::chrono::microseconds;
		using Clock = std::chrono::high_resolution_clock;

		std::chrono::time_point<Clock> startPoint;
	public:
		/** @brief �R���X�g���N�^ */
		Timer() = default;

		/** @brief �^�C�}�[�J�n */
		void start() noexcept { startPoint = Clock::now(); }

		/** @brief �o�ߎ��ԁims�j */
		NODISCARD double interval() const noexcept {
			constexpr auto basicDuration = std::chrono::milliseconds(1);
			constexpr auto duration = std::chrono::duration_cast<Rep>(basicDuration).count();
			return std::chrono::duration_cast<Rep>(Clock::now() - startPoint).count() / static_cast<double>(duration);
		}
	};

}
