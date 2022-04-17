#pragma once

#include <utility>

#include "SearchComponentLib.hpp"

namespace alib::Search {

	template<class Argument, typename Score = double, typename Hash = void>
	struct DefaultSearchMethod {
		using ArgumentType = typename Argument;
		using ScoreType = typename Score;
		using HashType = typename Hash;

		ArgumentType argument{};
		ScoreType score{};
		HashType hash{};

		DefaultSearchMethod() = default;
		DefaultSearchMethod(const ScoreType score, const HashType hash, const ArgumentType& argument) noexcept : score(score), hash(hash), argument(argument) {}
		DefaultSearchMethod(const ScoreType score, const HashType hash) noexcept : score(score), hash(hash) {}
		DefaultSearchMethod(const ScoreType score) noexcept : score(score) {}
		template<class... Args>
		DefaultSearchMethod(const ScoreType score, const HashType hash, Args&& ...args) noexcept : score(score), hash(hash), argument(ArgumentType(std::forward<Args>(args)...)) {}
	};

	template<class Argument, typename Score>
	struct DefaultSearchMethod<Argument, Score, void> {
		using ArgumentType = typename Argument;
		using ScoreType = typename Score;
		using HashType = void;

		ArgumentType argument{};
		ScoreType score{};

		DefaultSearchMethod() = default;
		DefaultSearchMethod(const ScoreType score, const ArgumentType& argument) noexcept : score(score), argument(argument) {}
		DefaultSearchMethod(const ScoreType score) noexcept : score(score) {}
		template<class... Args>
		DefaultSearchMethod(const ScoreType score, Args&& ...args) noexcept : score(score), argument(ArgumentType(std::forward<Args>(args)...)) {}
	};

	template <size_type Depth, size_type Width, size_type Limit, class Method, bool Debug = false>
	struct ConfigTemplate {
		using SearchMethod = Method;

		using ArgumentType = typename Method::ArgumentType;
		using ScoreType = typename Method::ScoreType;
		using HashType = typename Method::HashType;

		NODISCARD inline static constexpr bool HasHash() noexcept { return !std::is_void_v<HashType>; }

		NODISCARD inline static constexpr bool GetDebug() noexcept { return Debug; }

		NODISCARD inline static constexpr size_type GetDepth() noexcept { return Depth; }
		NODISCARD inline static constexpr size_type GetWidth() noexcept { return Width; }
		NODISCARD inline static constexpr size_type GetLimit() noexcept { return Limit; }
	};
}
