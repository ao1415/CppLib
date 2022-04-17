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
#include "SearcComponent/Reference.hpp"
#include "SearcComponent/BeamSearchProcess.hpp"

namespace alib::Search {
	using DebugProcess = Lib::BeamSearchProcess<Lib::DebugConfig, int>;

	/**
	 * @brief SearchTemplate
	 * @tparam Config
	*/
	template<class ProcessType>
	class SearchTemplate {
	public:
		using Process = ProcessType;
		using Config = typename Process::Config;
		using SearchMethod = typename Config::SearchMethod;

		SearchTemplate() = default;
		virtual ~SearchTemplate() = default;
		SearchTemplate(SearchTemplate&&) = default;
		SearchTemplate& operator=(SearchTemplate&&) = default;

		SearchTemplate(const SearchTemplate&) = delete;
		SearchTemplate& operator=(const SearchTemplate&) = delete;
	private:
		// �T���ς݃n�b�V��
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
		 * @brief ���̏�ԏ���
		 * @param args ���̏�ԏ����̈���
		*/
		template<class... Args>
		void nextSearch(Args&& ...args) {
			nextSearch(SearchMethod(std::forward<Args>(args)...));
		}

		/**
		 * @brief ���̏�ԏ���
		 * @param arg ���̏�ԏ����̈���
		*/
		void nextSearch(SearchMethod argument) {
			process.reserve(argument);
		}

		NODISCARD bool endOfSearch() const noexcept {
			return process.endOfSearch();
		}
		NODISCARD int getDepth() const noexcept {
			return process.getDepth();
		}
	public:
		void start(const SearchMethod& argument) {
			visited.clear();
			process.timerStart();
			process.init();
			process.reserve(argument);
			loop();
		}

		NODISCARD auto getResultList() const {
			return process.getResultList();
		}

		/**
		 * @brief ����̃p�^�[���𐶐�����
		 * @param ����
		*/
		virtual void init(const SearchMethod&) = 0;
		/**
		 * @brief �T������
		 * @param ����
		*/
		virtual void search(const SearchMethod&) = 0;
	};

	template<class Config, class Space>
	using BeamSearchTemplate = SearchTemplate<Lib::BeamSearchProcess<Config, Space>>;
}
