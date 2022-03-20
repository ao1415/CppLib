#pragma once

#include <utility>
#include <type_traits>
#include <algorithm>

#include <array>
#include <vector>
#include <stack>
#include <map>
#include <unordered_set>

#include <memory>
#include <chrono>

#include <cassert>

#include "Basic.hpp"
#include "SearcComponent/BeamSearchProcess.hpp"

namespace alib::Search {
	using DebugProcess = Lib::BeamSearchProcess<Lib::DebugConfig, int>;

	/**
	 * @brief SearchTemplate
	 * @tparam Config
	*/
	template<class Process>
	class SearchTemplate {
	public:
		using Config = typename Process::Config;
		using ArgumentType = typename Config::SearchMethod;

		SearchTemplate() = default;
		virtual ~SearchTemplate() = default;
		SearchTemplate(SearchTemplate&&) = default;
		SearchTemplate& operator=(SearchTemplate&&) = default;

		SearchTemplate(const SearchTemplate&) = delete;
		SearchTemplate& operator=(const SearchTemplate&) = delete;
	private:
		// 探査済みハッシュ
		std::unordered_set<size_t> visited{};
		Process process{};

		void loop() {
			if (process.onloop()) {
				const auto& argument = process.getArgument();
				if constexpr (Config::HasHash()) {
					visited.insert(argument.hash);
				}
				process.accept();
				init(argument);
			}
			while (process.onloop()) {
				const auto& argument = process.getArgument();
				if constexpr (Config::HasHash()) {
					if (visited.find(argument.hash) != visited.end()) {
						continue;
					}
					visited.insert(argument.hash);
				}
				process.accept();
				search(argument);
			}
		}
	protected:
		/**
		 * @brief 次の状態処理
		 * @param args 次の状態処理の引数
		*/
		template<class... Args>
		inline void nextSearch(Args&& ...args) {
			nextSearch(ArgumentType(std::forward<Args>(args)...));
		}

		/**
		 * @brief 次の状態処理
		 * @param arg 次の状態処理の引数
		*/
		inline void nextSearch(ArgumentType argument) { process.reserve(argument); }

		NODISCARD inline bool endOfSearch() const noexcept { return process.endOfSearch(); }
		NODISCARD inline int getDepth() const noexcept { return process.getDepth(); }
	public:
		void start(const ArgumentType& argument) {
			visited.clear();
			process.timerStart();
			process.init();
			process.reserve(argument);
			loop();
		}

		/**
		 * @brief 初回のパターンを生成する
		 * @param 引数
		*/
		virtual void init(const ArgumentType&) = 0;
		/**
		 * @brief 探査処理
		 * @param 引数
		*/
		virtual void search(const ArgumentType&) = 0;
	};
}
