#pragma once

#include <memory>
#include <string>

namespace alib {
	template <class Type, size_t Size>
	class MemoryPool {
	private:

		Type* m_data = nullptr;
		uint64_t address = 0;

	public:

		MemoryPool() noexcept {
			address = 0;

			constexpr uint64_t memorySize = sizeof(Type) * Size;

			m_data = static_cast<Type*>(std::malloc(memorySize));
		}

		~MemoryPool() {

			if (m_data != nullptr)
				std::free(m_data);
		}

		uint64_t useRate() const {
			return address;
		}

		template<class... Args>
		Type* push(Args... args) {
			address++;

			if (address > Size)
			{
				return nullptr;
			}
			
			new(&m_data[address - 1]) Type(std::forward<Args>(args)...);

			return &m_data[address - 1];
		}
		void pop() {
			m_data[address - 1].~Type();
			address--;
		}

		void clear() {
			for (int i = 0; i < address; i++)
			{
				m_data[i].~Type();
			}
			address = 0;
		}

	};
}
