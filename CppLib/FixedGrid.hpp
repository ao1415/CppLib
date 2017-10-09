#pragma once
#include <array>

namespace alib {

	template<typename Type, size_t Width, size_t Height>
	class FixedGrid {
	private:

		using ContainerType = std::array<std::array<Type, Width>, Height>;
		ContainerType m_data;

	public:

		FixedGrid() = default;
		FixedGrid(const Type& v) { fill(v); }
		FixedGrid(const FixedGrid& other) = default;
		FixedGrid(FixedGrid&& other) {
			data = std::move(other);
		}

		FixedGrid& operator=(const FixedGrid& other) = default;
		FixedGrid& operator=(FixedGrid&& other) = default;

		const std::array<Type, Width>& operator[](size_t y) const { return m_data[y]; }
		std::array<Type, Width>& operator[](size_t y) { return m_data[y]; }

		const std::array<Type, Width>& at(size_t y) const { return m_data.at(y); }
		std::array<Type, Width>& at(size_t y) { return m_data.at(y); }

		const Type& at(size_t x, size_t y) const { return m_data.at(y).at(x); }
		Type& at(size_t x, size_t y) { return m_data.at(y).at(x); }

		constexpr size_t width() const {
			return Width;
		}
		constexpr size_t height() const {
			return Height;
		}

		void fill(const Type& v) noexcept {
			for (auto& line : m_data)
				line.fill(v);
		}

		void clear() {
			m_data.swap(ContainerType());
		}

	};

}
