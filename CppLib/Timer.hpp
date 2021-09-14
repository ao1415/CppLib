#pragma once

#include <chrono>

namespace alib
{

	/**
	 * @brief 一定時間の経過を確認するクラス
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
		 * @brief コンストラクタ
		 * 
		 * @param _time 設定時間（ナノ秒）
		 */
		Timer(const std::chrono::nanoseconds &_time) noexcept
		{
			type = Type::nanoseconds;
			time = _time.count();
		}
		/**
		 * @brief コンストラクタ
		 * 
		 * @param _time 設定時間（マイクロ秒）
		 */
		Timer(const std::chrono::microseconds &_time) noexcept
		{
			type = Type::microseconds;
			time = _time.count();
		}
		/**
		 * @brief コンストラクタ
		 * 
		 * @param _time 設定時間（ミリ秒）
		 */
		Timer(const std::chrono::milliseconds &_time) noexcept
		{
			type = Type::milliseconds;
			time = _time.count();
		}
		/**
		 * @brief コンストラクタ
		 * 
		 * @param _time 設定時間（秒）
		 */
		Timer(const std::chrono::seconds &_time) noexcept
		{
			type = Type::seconds;
			time = _time.count();
		}
		/**
		 * @brief コンストラクタ
		 * 
		 * @param _time 設定時間（分）
		 */
		Timer(const std::chrono::minutes &_time) noexcept
		{
			type = Type::minutes;
			time = _time.count();
		}
		/**
		 * @brief コンストラクタ
		 * 
		 * @param _time 設定時間（時）
		 */
		Timer(const std::chrono::hours &_time) noexcept
		{
			type = Type::hours;
			time = _time.count();
		}

		/**
		 * @brief 時間を設定する
		 * 
		 * @param _time 設定時間（ナノ秒）
		 */
		void set(const std::chrono::nanoseconds &_time) noexcept
		{
			type = Type::nanoseconds;
			time = _time.count();
		}
		/**
		 * @brief 時間を設定する
		 * 
		 * @param _time 設定時間（マイクロ秒）
		 */
		void set(const std::chrono::microseconds &_time) noexcept
		{
			type = Type::microseconds;
			time = _time.count();
		}
		/**
		 * @brief 時間を設定する
		 * 
		 * @param _time 設定時間（ミリ秒）
		 */
		void set(const std::chrono::milliseconds &_time) noexcept
		{
			type = Type::milliseconds;
			time = _time.count();
		}
		/**
		 * @brief 時間を設定する
		 * 
		 * @param _time 設定時間（秒）
		 */
		void set(const std::chrono::seconds &_time) noexcept
		{
			type = Type::seconds;
			time = _time.count();
		}
		/**
		 * @brief 時間を設定する
		 * 
		 * @param _time 設定時間（分）
		 */
		void set(const std::chrono::minutes &_time) noexcept
		{
			type = Type::minutes;
			time = _time.count();
		}
		/**
		 * @brief 時間を設定する
		 * 
		 * @param _time 設定時間（時）
		 */
		void set(const std::chrono::hours &_time) noexcept
		{
			type = Type::hours;
			time = _time.count();
		}

		/**
		 * @brief タイマーを開始させる
		 * 
		 */
		void start() noexcept { s = std::chrono::high_resolution_clock::now(); }

		/**
		 * @brief 設定時間経過したかを得る
		 * 
		 * @return true 設定時間を経過している
		 * @return false 設定時間未満
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
		 * @brief 設定時間経過したかを得る
		 * 
		 * @return true 設定時間を経過している
		 * @return false 設定時間未満
		 */
		operator bool() const noexcept { return check(); }
	};

	/**
	 * @brief 一定時間の経過を確認するクラス
	 * 
	 */
	class MilliSecTimer
	{
	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> s;
		long long time = 0;

	public:
		/**
		 * @brief コンストラクタ
		 * 
		 */
		MilliSecTimer() = default;
		/**
		 * @brief コンストラクタ
		 * 
		 * @param _time 設定時間
		 */
		MilliSecTimer(const std::chrono::milliseconds &_time) noexcept { time = _time.count(); }

		/**
		 * @brief 時間を設定する
		 * 
		 * @param _time 設定時間
		 */
		void set(const std::chrono::milliseconds &_time) noexcept { time = _time.count(); }

		/**
		 * @brief タイマーを開始させる
		 * 
		 */
		void start() noexcept
		{
			s = std::chrono::high_resolution_clock::now();
		}

		/**
		 * @brief 設定時間未満かを得る
		 * 
		 * @return true 設定時間未満
		 * @return false 設定時間超過
		 */
		inline const bool within() const noexcept
		{
			const auto e = std::chrono::high_resolution_clock::now();
			return std::chrono::duration_cast<std::chrono::milliseconds>(e - s).count() < time;
		}

		/**
		 * @brief 設定時間未満かを得る
		 * 
		 * @return true 設定時間未満
		 * @return false 設定時間超過
		 */
		operator bool() const noexcept { return within(); }

		/**
		 * @brief 経過時間を取得する
		 * 
		 * @return const long long 計測時間
		 */
		inline const long long interval() const noexcept
		{
			const auto e = std::chrono::high_resolution_clock::now();
			return std::chrono::duration_cast<std::chrono::milliseconds>(e - s).count();
		}
	};

} // namespace alib
