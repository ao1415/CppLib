#pragma once

#include <chrono>
#include <string>

namespace alib
{

	/**
	 * @brief 時間計測を行うクラス
	 * 
	 */
	class Stopwatch
	{
	private:
		std::chrono::time_point<std::chrono::high_resolution_clock> s;
		std::chrono::time_point<std::chrono::high_resolution_clock> e;

	public:
		/**
		 * @brief コンストラクタ
		 * 
		 * @param isStart 計測の開始フラグ
		 */
		Stopwatch(bool isStart = false) noexcept
		{
			if (isStart)
				start();
		}

		/**
		 * @brief 計測を開始する
		 * 
		 */
		inline void start() noexcept
		{
			s = std::chrono::high_resolution_clock::now();
			e = s;
		}

		/**
		 * @brief 計測を停止する
		 * 
		 */
		inline void stop() noexcept
		{
			e = std::chrono::high_resolution_clock::now();
		}

		/**
		 * @brief 計測時間を取得する（ナノ秒）
		 * 
		 * @return const long long 計測時間（ナノ秒）
		 */
		inline const long long nanoseconds() const noexcept { return std::chrono::duration_cast<std::chrono::nanoseconds>(e - s).count(); }
		/**
		 * @brief 計測時間を取得する（マイクロ秒）
		 * 
		 * @return const long long 計測時間（マイクロ秒）
		 */
		inline const long long microseconds() const noexcept { return std::chrono::duration_cast<std::chrono::microseconds>(e - s).count(); }
		/**
		 * @brief 計測時間を取得する（ミリ秒）
		 * 
		 * @return const long long 計測時間（ミリ秒）
		 */
		inline const long long millisecond() const noexcept { return std::chrono::duration_cast<std::chrono::milliseconds>(e - s).count(); }
		/**
		 * @brief 計測時間を取得する（秒）
		 * 
		 * @return const long long 計測時間（秒）
		 */
		inline const long long second() const noexcept { return std::chrono::duration_cast<std::chrono::seconds>(e - s).count(); }
		/**
		 * @brief 計測時間を取得する（分）
		 * 
		 * @return const long long 計測時間（分）
		 */
		inline const long long minutes() const noexcept { return std::chrono::duration_cast<std::chrono::minutes>(e - s).count(); }
		/**
		 * @brief 計測時間を取得する（時）
		 * 
		 * @return const long long 計測時間（時）
		 */
		inline const long long hours() const noexcept { return std::chrono::duration_cast<std::chrono::hours>(e - s).count(); }

		/**
		 * @brief 単位付きの計測時間の文字列を得る（ナノ秒）
		 * 
		 * @return const std::string 計測時間の文字列（ナノ秒）
		 */
		inline const std::string toString_ns() const { return std::to_string(nanoseconds()) + "ns"; }
		/**
		 * @brief 単位付きの計測時間の文字列を得る（マイクロ秒）
		 * 
		 * @return const std::string 計測時間の文字列（マイクロ秒）
		 */
		inline const std::string toString_us() const { return std::to_string(microseconds()) + "us"; }
		/**
		 * @brief 単位付きの計測時間の文字列を得る（ミリ秒）
		 * 
		 * @return const std::string 計測時間の文字列（ミリ秒）
		 */
		inline const std::string toString_ms() const { return std::to_string(millisecond()) + "ms"; }
		/**
		 * @brief 単位付きの計測時間の文字列を得る（秒）
		 * 
		 * @return const std::string 計測時間の文字列（秒）
		 */
		inline const std::string toString_s() const { return std::to_string(second()) + "s"; }
		/**
		 * @brief 単位付きの計測時間の文字列を得る（分）
		 * 
		 * @return const std::string 計測時間の文字列（分）
		 */
		inline const std::string toString_m() const { return std::to_string(minutes()) + "m"; }
		/**
		 * @brief 単位付きの計測時間の文字列を得る（時）
		 * 
		 * @return const std::string 計測時間の文字列（時）
		 */
		inline const std::string toString_h() const { return std::to_string(hours()) + "h"; }
	};

} // namespace alib
