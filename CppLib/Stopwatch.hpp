#pragma once

#include <chrono>
#include <string>

namespace alib
{

	/**
	 * @brief ���Ԍv�����s���N���X
	 * 
	 */
	class Stopwatch
	{
	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> s;
		std::chrono::time_point<std::chrono::high_resolution_clock> e;

	public:
		/**
		 * @brief �R���X�g���N�^
		 * 
		 * @param isStart �v���̊J�n�t���O
		 */
		Stopwatch(bool isStart = false) noexcept
		{
			if (isStart)
				start();
		}

		/**
		 * @brief �v�����J�n����
		 * 
		 */
		inline void start() noexcept
		{
			s = std::chrono::high_resolution_clock::now();
			e = s;
		}

		/**
		 * @brief �v�����~����
		 * 
		 */
		inline void stop() noexcept
		{
			e = std::chrono::high_resolution_clock::now();
		}

		/**
		 * @brief �v�����Ԃ��擾����i�i�m�b�j
		 * 
		 * @return const long long �v�����ԁi�i�m�b�j
		 */
		inline const long long nanoseconds() const noexcept { return std::chrono::duration_cast<std::chrono::nanoseconds>(e - s).count(); }
		/**
		 * @brief �v�����Ԃ��擾����i�}�C�N���b�j
		 * 
		 * @return const long long �v�����ԁi�}�C�N���b�j
		 */
		inline const long long microseconds() const noexcept { return std::chrono::duration_cast<std::chrono::microseconds>(e - s).count(); }
		/**
		 * @brief �v�����Ԃ��擾����i�~���b�j
		 * 
		 * @return const long long �v�����ԁi�~���b�j
		 */
		inline const long long millisecond() const noexcept { return std::chrono::duration_cast<std::chrono::milliseconds>(e - s).count(); }
		/**
		 * @brief �v�����Ԃ��擾����i�b�j
		 * 
		 * @return const long long �v�����ԁi�b�j
		 */
		inline const long long second() const noexcept { return std::chrono::duration_cast<std::chrono::seconds>(e - s).count(); }
		/**
		 * @brief �v�����Ԃ��擾����i���j
		 * 
		 * @return const long long �v�����ԁi���j
		 */
		inline const long long minutes() const noexcept { return std::chrono::duration_cast<std::chrono::minutes>(e - s).count(); }
		/**
		 * @brief �v�����Ԃ��擾����i���j
		 * 
		 * @return const long long �v�����ԁi���j
		 */
		inline const long long hours() const noexcept { return std::chrono::duration_cast<std::chrono::hours>(e - s).count(); }

		/**
		 * @brief �P�ʕt���̌v�����Ԃ̕�����𓾂�i�i�m�b�j
		 * 
		 * @return const std::string �v�����Ԃ̕�����i�i�m�b�j
		 */
		inline const std::string toString_ns() const { return std::to_string(nanoseconds()) + "ns"; }
		/**
		 * @brief �P�ʕt���̌v�����Ԃ̕�����𓾂�i�}�C�N���b�j
		 * 
		 * @return const std::string �v�����Ԃ̕�����i�}�C�N���b�j
		 */
		inline const std::string toString_us() const { return std::to_string(microseconds()) + "us"; }
		/**
		 * @brief �P�ʕt���̌v�����Ԃ̕�����𓾂�i�~���b�j
		 * 
		 * @return const std::string �v�����Ԃ̕�����i�~���b�j
		 */
		inline const std::string toString_ms() const { return std::to_string(millisecond()) + "ms"; }
		/**
		 * @brief �P�ʕt���̌v�����Ԃ̕�����𓾂�i�b�j
		 * 
		 * @return const std::string �v�����Ԃ̕�����i�b�j
		 */
		inline const std::string toString_s() const { return std::to_string(second()) + "s"; }
		/**
		 * @brief �P�ʕt���̌v�����Ԃ̕�����𓾂�i���j
		 * 
		 * @return const std::string �v�����Ԃ̕�����i���j
		 */
		inline const std::string toString_m() const { return std::to_string(minutes()) + "m"; }
		/**
		 * @brief �P�ʕt���̌v�����Ԃ̕�����𓾂�i���j
		 * 
		 * @return const std::string �v�����Ԃ̕�����i���j
		 */
		inline const std::string toString_h() const { return std::to_string(hours()) + "h"; }
	};

} // namespace alib
