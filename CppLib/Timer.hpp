#pragma once

#include <chrono>

namespace alib
{

	/**
	 * @brief ��莞�Ԃ̌o�߂��m�F����N���X
	 * 
	 */
	class Timer
	{
	private:
		enum class Type
		{
			nanoseconds,
			microseconds,
			milliseconds,
			seconds,
			minutes,
			hours
		};

		std::chrono::time_point<std::chrono::high_resolution_clock> s;
		long long time;
		Type type;

	public:
		Timer() = default;
		/**
		 * @brief �R���X�g���N�^
		 * 
		 * @param _time �ݒ莞�ԁi�i�m�b�j
		 */
		Timer(const std::chrono::nanoseconds &_time) noexcept
		{
			type = Type::nanoseconds;
			time = _time.count();
		}
		/**
		 * @brief �R���X�g���N�^
		 * 
		 * @param _time �ݒ莞�ԁi�}�C�N���b�j
		 */
		Timer(const std::chrono::microseconds &_time) noexcept
		{
			type = Type::microseconds;
			time = _time.count();
		}
		/**
		 * @brief �R���X�g���N�^
		 * 
		 * @param _time �ݒ莞�ԁi�~���b�j
		 */
		Timer(const std::chrono::milliseconds &_time) noexcept
		{
			type = Type::milliseconds;
			time = _time.count();
		}
		/**
		 * @brief �R���X�g���N�^
		 * 
		 * @param _time �ݒ莞�ԁi�b�j
		 */
		Timer(const std::chrono::seconds &_time) noexcept
		{
			type = Type::seconds;
			time = _time.count();
		}
		/**
		 * @brief �R���X�g���N�^
		 * 
		 * @param _time �ݒ莞�ԁi���j
		 */
		Timer(const std::chrono::minutes &_time) noexcept
		{
			type = Type::minutes;
			time = _time.count();
		}
		/**
		 * @brief �R���X�g���N�^
		 * 
		 * @param _time �ݒ莞�ԁi���j
		 */
		Timer(const std::chrono::hours &_time) noexcept
		{
			type = Type::hours;
			time = _time.count();
		}

		/**
		 * @brief ���Ԃ�ݒ肷��
		 * 
		 * @param _time �ݒ莞�ԁi�i�m�b�j
		 */
		void set(const std::chrono::nanoseconds &_time) noexcept
		{
			type = Type::nanoseconds;
			time = _time.count();
		}
		/**
		 * @brief ���Ԃ�ݒ肷��
		 * 
		 * @param _time �ݒ莞�ԁi�}�C�N���b�j
		 */
		void set(const std::chrono::microseconds &_time) noexcept
		{
			type = Type::microseconds;
			time = _time.count();
		}
		/**
		 * @brief ���Ԃ�ݒ肷��
		 * 
		 * @param _time �ݒ莞�ԁi�~���b�j
		 */
		void set(const std::chrono::milliseconds &_time) noexcept
		{
			type = Type::milliseconds;
			time = _time.count();
		}
		/**
		 * @brief ���Ԃ�ݒ肷��
		 * 
		 * @param _time �ݒ莞�ԁi�b�j
		 */
		void set(const std::chrono::seconds &_time) noexcept
		{
			type = Type::seconds;
			time = _time.count();
		}
		/**
		 * @brief ���Ԃ�ݒ肷��
		 * 
		 * @param _time �ݒ莞�ԁi���j
		 */
		void set(const std::chrono::minutes &_time) noexcept
		{
			type = Type::minutes;
			time = _time.count();
		}
		/**
		 * @brief ���Ԃ�ݒ肷��
		 * 
		 * @param _time �ݒ莞�ԁi���j
		 */
		void set(const std::chrono::hours &_time) noexcept
		{
			type = Type::hours;
			time = _time.count();
		}

		/**
		 * @brief �^�C�}�[���J�n������
		 * 
		 */
		void start() noexcept { s = std::chrono::high_resolution_clock::now(); }

		/**
		 * @brief �ݒ莞�Ԍo�߂������𓾂�
		 * 
		 * @return true �ݒ莞�Ԃ��o�߂��Ă���
		 * @return false �ݒ莞�Ԗ���
		 */
		inline const bool check() const noexcept
		{
			const auto e = std::chrono::high_resolution_clock::now();
			switch (type)
			{
			case Type::nanoseconds:
				return std::chrono::duration_cast<std::chrono::nanoseconds>(e - s).count() >= time;
			case Type::microseconds:
				return std::chrono::duration_cast<std::chrono::microseconds>(e - s).count() >= time;
			case Type::milliseconds:
				return std::chrono::duration_cast<std::chrono::milliseconds>(e - s).count() >= time;
			case Type::seconds:
				return std::chrono::duration_cast<std::chrono::seconds>(e - s).count() >= time;
			case Type::minutes:
				return std::chrono::duration_cast<std::chrono::minutes>(e - s).count() >= time;
			case Type::hours:
				return std::chrono::duration_cast<std::chrono::hours>(e - s).count() >= time;
			default:
				return true;
			}
		}

		/**
		 * @brief �ݒ莞�Ԍo�߂������𓾂�
		 * 
		 * @return true �ݒ莞�Ԃ��o�߂��Ă���
		 * @return false �ݒ莞�Ԗ���
		 */
		operator bool() const noexcept { return check(); }
	};

	/**
	 * @brief ��莞�Ԃ̌o�߂��m�F����N���X
	 * 
	 */
	class MilliSecTimer
	{
	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> s;
		long long time = 0;

	public:
		/**
		 * @brief �R���X�g���N�^
		 * 
		 */
		MilliSecTimer() = default;
		/**
		 * @brief �R���X�g���N�^
		 * 
		 * @param _time �ݒ莞��
		 */
		MilliSecTimer(const std::chrono::milliseconds &_time) noexcept { time = _time.count(); }

		/**
		 * @brief ���Ԃ�ݒ肷��
		 * 
		 * @param _time �ݒ莞��
		 */
		void set(const std::chrono::milliseconds &_time) noexcept { time = _time.count(); }

		/**
		 * @brief �^�C�}�[���J�n������
		 * 
		 */
		void start() noexcept
		{
			s = std::chrono::high_resolution_clock::now();
		}

		/**
		 * @brief �ݒ莞�Ԗ������𓾂�
		 * 
		 * @return true �ݒ莞�Ԗ���
		 * @return false �ݒ莞�Ԓ���
		 */
		inline const bool within() const noexcept
		{
			const auto e = std::chrono::high_resolution_clock::now();
			return std::chrono::duration_cast<std::chrono::milliseconds>(e - s).count() < time;
		}

		/**
		 * @brief �ݒ莞�Ԗ������𓾂�
		 * 
		 * @return true �ݒ莞�Ԗ���
		 * @return false �ݒ莞�Ԓ���
		 */
		operator bool() const noexcept { return within(); }

		/**
		 * @brief �o�ߎ��Ԃ��擾����
		 * 
		 * @return const long long �v������
		 */
		inline const long long interval() const noexcept
		{
			const auto e = std::chrono::high_resolution_clock::now();
			return std::chrono::duration_cast<std::chrono::milliseconds>(e - s).count();
		}
	};

} // namespace alib
