#pragma once

#include <chrono>
#include <string>

namespace alib {

	/// <summary>
	/// 時間計測を行うクラス
	/// </summary>
	class Stopwatch {
	public:

		/// <summary>
		/// コンストラクタ
		/// </summary>
		Stopwatch() = default;

		/// <summary>
		/// 計測を開始する
		/// </summary>
		inline void start() noexcept {
			s = std::chrono::high_resolution_clock::now();
			e = s;
		}
		/// <summary>
		/// 計測を停止する
		/// </summary>
		inline void stop() noexcept {
			e = std::chrono::high_resolution_clock::now();
		}

		/// <summary>
		/// 計測時間を取得する(ナノ秒)
		/// </summary>
		/// <returns>計測時間(ナノ秒)</returns>
		inline const long long nanoseconds() const noexcept { return std::chrono::duration_cast<std::chrono::nanoseconds>(e - s).count(); }
		/// <summary>
		/// 計測時間を取得する(マイクロ秒)
		/// </summary>
		/// <returns>計測時間(マイクロ秒)</returns>
		inline const long long microseconds() const noexcept { return std::chrono::duration_cast<std::chrono::microseconds>(e - s).count(); }
		/// <summary>
		/// 計測時間を取得する(ミリ秒)
		/// </summary>
		/// <returns>計測時間(ミリ秒)</returns>
		inline const long long millisecond() const noexcept { return std::chrono::duration_cast<std::chrono::milliseconds>(e - s).count(); }
		/// <summary>
		/// 計測時間を取得する(秒)
		/// </summary>
		/// <returns>計測時間(秒)</returns>
		inline const long long second() const noexcept { return std::chrono::duration_cast<std::chrono::seconds>(e - s).count(); }
		/// <summary>
		/// 計測時間を取得する(分)
		/// </summary>
		/// <returns>計測時間(分)</returns>
		inline const long long minutes() const noexcept { return std::chrono::duration_cast<std::chrono::minutes>(e - s).count(); }
		/// <summary>
		/// 計測時間を取得する(時)
		/// </summary>
		/// <returns>計測時間(時)</returns>
		inline const long long hours() const noexcept { return std::chrono::duration_cast<std::chrono::hours>(e - s).count(); }

		/// <summary>
		/// 単位付きの計測時間の文字列を得る(ナノ秒)
		/// </summary>
		/// <returns>計測時間の文字列(ナノ秒)</returns>
		inline const std::string toString_ns() const { return std::to_string(nanoseconds()) + "ns"; }
		/// <summary>
		/// 単位付きの計測時間の文字列を得る(マイクロ秒)
		/// </summary>
		/// <returns>計測時間の文字列(マイクロ秒)</returns>
		inline const std::string toString_us() const { return std::to_string(microseconds()) + "us"; }
		/// <summary>
		/// 単位付きの計測時間の文字列を得る(ミリ秒)
		/// </summary>
		/// <returns>計測時間の文字列(ミリ秒)</returns>
		inline const std::string toString_ms() const { return std::to_string(millisecond()) + "ms"; }
		/// <summary>
		/// 単位付きの計測時間の文字列を得る(秒)
		/// </summary>
		/// <returns>計測時間の文字列(秒)</returns>
		inline const std::string toString_s() const { return std::to_string(second()) + "s"; }
		/// <summary>
		/// 単位付きの計測時間の文字列を得る(分)
		/// </summary>
		/// <returns>計測時間の文字列(分)</returns>
		inline const std::string toString_m() const { return std::to_string(minutes()) + "m"; }
		/// <summary>
		/// 単位付きの計測時間の文字列を得る(時)
		/// </summary>
		/// <returns>計測時間の文字列(時)</returns>
		inline const std::string toString_h() const { return std::to_string(hours()) + "h"; }

	private:

		std::chrono::time_point<std::chrono::high_resolution_clock> s;
		std::chrono::time_point<std::chrono::high_resolution_clock> e;

	};

}
