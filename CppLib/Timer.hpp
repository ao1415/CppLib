#pragma once

#include <chrono>

namespace alib {

	/// <summary>
	/// 一定時間の経過を確認するクラス
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
		/// コンストラクタ
		/// </summary>
		Timer() = default;
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="_time">設定時間(ナノ秒)</param>
		Timer(const std::chrono::nanoseconds& _time) noexcept { type = Type::nanoseconds; time = _time.count(); }
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="_time">設定時間(マイクロ秒)</param>
		Timer(const std::chrono::microseconds& _time) noexcept { type = Type::microseconds; time = _time.count(); }
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="_time">設定時間(ミリ秒)</param>
		Timer(const std::chrono::milliseconds& _time) noexcept { type = Type::milliseconds; time = _time.count(); }
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="_time">設定時間(秒)</param>
		Timer(const std::chrono::seconds& _time) noexcept { type = Type::seconds; time = _time.count(); }
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="_time">設定時間(分)</param>
		Timer(const std::chrono::minutes& _time) noexcept { type = Type::minutes; time = _time.count(); }
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="_time">設定時間(時)</param>
		Timer(const std::chrono::hours& _time) noexcept { type = Type::hours; time = _time.count(); }

		/// <summary>
		/// 時間を設定する
		/// </summary>
		/// <param name="_time">設定時間(ナノ秒)</param>
		void set(const std::chrono::nanoseconds& _time) noexcept { type = Type::nanoseconds; time = _time.count(); }
		/// <summary>
		/// 時間を設定する
		/// </summary>
		/// <param name="_time">設定時間(マイクロ秒)</param>
		void set(const std::chrono::microseconds& _time) noexcept { type = Type::microseconds; time = _time.count(); }
		/// <summary>
		/// 時間を設定する
		/// </summary>
		/// <param name="_time">設定時間(ミリ秒)</param>
		void set(const std::chrono::milliseconds& _time) noexcept { type = Type::milliseconds; time = _time.count(); }
		/// <summary>
		/// 時間を設定する
		/// </summary>
		/// <param name="_time">設定時間(秒)</param>
		void set(const std::chrono::seconds& _time) noexcept { type = Type::seconds; time = _time.count(); }
		/// <summary>
		/// 時間を設定する
		/// </summary>
		/// <param name="_time">設定時間(分</param>
		void set(const std::chrono::minutes& _time) noexcept { type = Type::minutes; time = _time.count(); }
		/// <summary>
		/// 時間を設定する
		/// </summary>
		/// <param name="_time">設定時間(時)</param>
		void set(const std::chrono::hours& _time) noexcept { type = Type::hours; time = _time.count(); }

		/// <summary>
		/// タイマーを開始させる
		/// </summary>
		void start() noexcept { s = std::chrono::high_resolution_clock::now(); }

		/// <summary>
		/// 設定時間経過したかを得る
		/// </summary>
		/// <returns>経過していれば true, それ以外は false</returns>
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
		/// 設定時間経過したかを得る
		/// </summary>
		/// <returns>経過していれば true, それ以外は false</returns>
		operator bool() const noexcept { return check(); }

	};

	/// <summary>
	/// 一定時間の経過を確認するクラス
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
		/// コンストラクタ
		/// </summary>
		MilliSecTimer() = default;
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="_time">設定時間(ミリ秒)</param>
		MilliSecTimer(const std::chrono::milliseconds& _time) noexcept { time = _time.count(); }

		/// <summary>
		/// 時間を設定する
		/// </summary>
		/// <param name="_time">設定時間(ミリ秒)</param>
		void set(const std::chrono::milliseconds& _time) noexcept { time = _time.count(); }

		/// <summary>
		/// タイマーを開始させる
		/// </summary>
		void start() noexcept {
#ifdef _MSC_VER
			s = std::chrono::high_resolution_clock::now();
#else
			startCycle = getCycle();
#endif // _MSC_VER
		}

		/// <summary>
		/// 設定時間経過したかを得る
		/// </summary>
		/// <returns>経過していれば true, それ以外は false</returns>
		inline const bool check() const noexcept {
#ifdef _MSC_VER
			const auto e = std::chrono::high_resolution_clock::now();
			return std::chrono::duration_cast<std::chrono::milliseconds>(e - s).count() >= time;
#else
			return (getCycle() - startCycle) / CyclePerMilliSec >= time;
#endif // _MSC_VER
		}

		/// <summary>
		/// 設定時間経過したかを得る
		/// </summary>
		/// <returns>経過していれば true, それ以外は false</returns>
		operator bool() const noexcept { return check(); }

		/// <summary>
		/// 経過時間を取得する(ミリ秒)
		/// </summary>
		/// <returns>計測時間(ミリ秒)</returns>
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
