#pragma once

#include <memory>
#include <string>
#include <deque>

#include "FixedDeque.hpp"

namespace alib
{

	/**
	 * @brief メモリプールの管理クラス
	 * 
	 * @tparam Type プールする型
	 * @tparam Size プールサイズ
	 */
	template <class Type, size_t Size>
	class MemoryPool
	{
	private:
		using Deque = FastFixedDeque<Type *, Size / 16>;

		Type *m_data = nullptr;
		size_t pointer = 0;

		bool isRecycle = false;

		Deque addr;

		inline static std::shared_ptr<MemoryPool<Type, Size>> instance{};

		/**
		 * @brief コンストラクタ
		 * 
		 */
		MemoryPool() noexcept
		{
			pointer = 0;

			constexpr uint64_t memorySize = sizeof(Type) * Size;

			m_data = static_cast<Type *>(std::malloc(memorySize));
		}

	public:
		/**
		 * @brief メモリプールを作成する
		 * 
		 */
		static void Create()
		{
			instance.reset(new MemoryPool());
		}

		/**
		 * @brief メモリプールのインスタンスを取得する
		 * 
		 * @return MemoryPool<Type, Size>& インスタンス
		 */
		static MemoryPool<Type, Size> &Get()
		{
			return *instance;
		}

		/**
		 * @brief デストラクタ
		 * 
		 */
		~MemoryPool()
		{
			if (m_data != nullptr)
				std::free(m_data);
		}

		/**
		 * @brief プールにデータを作成する
		 * 
		 * @tparam Args コンストラクタ引数
		 * @param args コンストラクタ引数
		 * @return Type* 生成ポインタ
		 */
		template <class... Args>
		inline Type *push(Args... args)
		{
			if (UNLIKELY(isRecycle))
			{
				if (UNLIKELY(addr.empty()))
				{
					return nullptr;
				}

				const auto p = new (addr.back()) Type(std::forward<Args>(args)...);
				addr.pop_back();

				return p;
			}
			else
			{
				pointer++;
				if (pointer >= Size)
				{
					isRecycle = true;
				}

				const auto p = new (&m_data[pointer - 1]) Type(std::forward<Args>(args)...);
				return p;
			}
		}

		/**
		 * @brief 不要のポインタを返却する
		 * 
		 * @param p ポインタ
		 */
		inline void release(Type *p)
		{
			addr.push_back(p);
		}

		/**
		 * @brief メモリプールの使用状態をリセットする
		 * 
		 */
		void clear()
		{
			isRecycle = false;
			pointer = 0;
			addr.clear();
		}
	};
} // namespace alib
