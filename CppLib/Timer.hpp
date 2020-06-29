#pragma once

#include <chrono>

namespace alib {

	/// <summary>
	/// ��莞�Ԃ̌o�߂��m�F����N���X
	/// </summary>
	class Timer {
	private:

		enum class Type {
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

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		Timer() = default;
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="_time">�ݒ莞��(�i�m�b)</param>
		Timer(const std::chrono::nanoseconds& _time) noexcept { type = Type::nanoseconds; time = _time.count(); }
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="_time">�ݒ莞��(�}�C�N���b)</param>
		Timer(const std::chrono::microseconds& _time) noexcept { type = Type::microseconds; time = _time.count(); }
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="_time">�ݒ莞��(�~���b)</param>
		Timer(const std::chrono::milliseconds& _time) noexcept { type = Type::milliseconds; time = _time.count(); }
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="_time">�ݒ莞��(�b)</param>
		Timer(const std::chrono::seconds& _time) noexcept { type = Type::seconds; time = _time.count(); }
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="_time">�ݒ莞��(��)</param>
		Timer(const std::chrono::minutes& _time) noexcept { type = Type::minutes; time = _time.count(); }
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="_time">�ݒ莞��(��)</param>
		Timer(const std::chrono::hours& _time) noexcept { type = Type::hours; time = _time.count(); }

		/// <summary>
		/// ���Ԃ�ݒ肷��
		/// </summary>
		/// <param name="_time">�ݒ莞��(�i�m�b)</param>
		void set(const std::chrono::nanoseconds& _time) noexcept { type = Type::nanoseconds; time = _time.count(); }
		/// <summary>
		/// ���Ԃ�ݒ肷��
		/// </summary>
		/// <param name="_time">�ݒ莞��(�}�C�N���b)</param>
		void set(const std::chrono::microseconds& _time) noexcept { type = Type::microseconds; time = _time.count(); }
		/// <summary>
		/// ���Ԃ�ݒ肷��
		/// </summary>
		/// <param name="_time">�ݒ莞��(�~���b)</param>
		void set(const std::chrono::milliseconds& _time) noexcept { type = Type::milliseconds; time = _time.count(); }
		/// <summary>
		/// ���Ԃ�ݒ肷��
		/// </summary>
		/// <param name="_time">�ݒ莞��(�b)</param>
		void set(const std::chrono::seconds& _time) noexcept { type = Type::seconds; time = _time.count(); }
		/// <summary>
		/// ���Ԃ�ݒ肷��
		/// </summary>
		/// <param name="_time">�ݒ莞��(��</param>
		void set(const std::chrono::minutes& _time) noexcept { type = Type::minutes; time = _time.count(); }
		/// <summary>
		/// ���Ԃ�ݒ肷��
		/// </summary>
		/// <param name="_time">�ݒ莞��(��)</param>
		void set(const std::chrono::hours& _time) noexcept { type = Type::hours; time = _time.count(); }

		/// <summary>
		/// �^�C�}�[���J�n������
		/// </summary>
		void start() noexcept { s = std::chrono::high_resolution_clock::now(); }

		/// <summary>
		/// �ݒ莞�Ԍo�߂������𓾂�
		/// </summary>
		/// <returns>�o�߂��Ă���� true, ����ȊO�� false</returns>
		inline const bool check() const noexcept {
			const auto e = std::chrono::high_resolution_clock::now();
			switch (type)
			{
			case Type::nanoseconds: return std::chrono::duration_cast<std::chrono::nanoseconds>(e - s).count() >= time;
			case Type::microseconds: return std::chrono::duration_cast<std::chrono::microseconds>(e - s).count() >= time;
			case Type::milliseconds: return std::chrono::duration_cast<std::chrono::milliseconds>(e - s).count() >= time;
			case Type::seconds: return std::chrono::duration_cast<std::chrono::seconds>(e - s).count() >= time;
			case Type::minutes: return std::chrono::duration_cast<std::chrono::minutes>(e - s).count() >= time;
			case Type::hours: return std::chrono::duration_cast<std::chrono::hours>(e - s).count() >= time;
			default: return true;
			}
		}

		/// <summary>
		/// �ݒ莞�Ԍo�߂������𓾂�
		/// </summary>
		/// <returns>�o�߂��Ă���� true, ����ȊO�� false</returns>
		operator bool() const noexcept { return check(); }

	};

	/// <summary>
	/// ��莞�Ԃ̌o�߂��m�F����N���X
	/// </summary>
	class MilliSecTimer {
	private:

		std::chrono::time_point<std::chrono::high_resolution_clock> s;
		unsigned long long int startCycle = 0;
		long long time = 0;

		constexpr static double CyclePerMilliSec = 2794000.0;

#ifndef _MSC_VER
		unsigned long long int getCycle() const {
			unsigned int low, high;
			__asm__ volatile ("rdtsc" : "=a" (low), "=d" (high));
			return ((unsigned long long int)low) | ((unsigned long long int)high << 32);
		}
#endif // _MSC_VER

	public:

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		MilliSecTimer() = default;
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="_time">�ݒ莞��(�~���b)</param>
		MilliSecTimer(const std::chrono::milliseconds& _time) noexcept { time = _time.count(); }

		/// <summary>
		/// ���Ԃ�ݒ肷��
		/// </summary>
		/// <param name="_time">�ݒ莞��(�~���b)</param>
		void set(const std::chrono::milliseconds& _time) noexcept { time = _time.count(); }

		/// <summary>
		/// �^�C�}�[���J�n������
		/// </summary>
		void start() noexcept {
#ifdef _MSC_VER
			s = std::chrono::high_resolution_clock::now();
#else
			startCycle = getCycle();
#endif // _MSC_VER
		}

		/// <summary>
		/// �ݒ莞�Ԍo�߂������𓾂�
		/// </summary>
		/// <returns>�o�߂��Ă���� true, ����ȊO�� false</returns>
		inline const bool check() const noexcept {
#ifdef _MSC_VER
			const auto e = std::chrono::high_resolution_clock::now();
			return std::chrono::duration_cast<std::chrono::milliseconds>(e - s).count() >= time;
#else
			return (getCycle() - startCycle) / CyclePerMilliSec >= time;
#endif // _MSC_VER
		}

		/// <summary>
		/// �ݒ莞�Ԍo�߂������𓾂�
		/// </summary>
		/// <returns>�o�߂��Ă���� true, ����ȊO�� false</returns>
		operator bool() const noexcept { return check(); }

		/// <summary>
		/// �o�ߎ��Ԃ��擾����(�~���b)
		/// </summary>
		/// <returns>�v������(�~���b)</returns>
		inline const long long interval() const noexcept {
#ifdef _MSC_VER
			const auto e = std::chrono::high_resolution_clock::now();
			return std::chrono::duration_cast<std::chrono::milliseconds>(e - s).count();
#else
			return static_cast<long long int>((getCycle() - startCycle) / CyclePerMilliSec);
#endif // _MSC_VER
		}

	};

}
