#pragma once
#define NOMINMAX
#include <Windows.h>
#include <cstdint>
#include <stdexcept>
#include <assert.h>

#include "../security/importer.h"

namespace detail
{
	class protect_guard
	{
	public:
		protect_guard(void* base, size_t len, std::uint32_t flags)
		{
			_base = base;
			_length = len;

			if (!LI_FN(VirtualProtect).cached()(base, len, flags, PDWORD(&_old)))
				throw std::runtime_error("Failed to protect region.");
		}
		~protect_guard()
		{
			LI_FN(VirtualProtect).cached()(_base, _length, _old, PDWORD(&_old));
		}

	private:
		void*         _base;
		size_t        _length;
		std::uint32_t _old;
	};

	template <typename Type, Type OffsetBasis, Type Prime>
	struct size_dependant_data
	{
		using type = Type;
		constexpr static auto k_offset_basis = OffsetBasis;
		constexpr static auto k_prime = Prime;
	};

	template <size_t Bits>
	struct size_selector;

	template <>
	struct size_selector<32>
	{
		using type = size_dependant_data<std::uint32_t, 0x811c9dc5ul, 16777619ul>;
	};

	template <>
	struct size_selector<64>
	{
		using type = size_dependant_data<std::uint64_t, 0xcbf29ce484222325ull, 1099511628211ull>;
	};

	// Implements FNV-1a hash algorithm
	template <std::size_t Size>
	class fnv_hash
	{
	private:
		using data_t = typename size_selector<Size>::type;

	public:
		using hash = typename data_t::type;

	private:
		constexpr static auto k_offset_basis = data_t::k_offset_basis;
		constexpr static auto k_prime = data_t::k_prime;

	public:
		template <std::size_t N>
		static __forceinline constexpr auto hash_constexpr(const char(&str)[N], const std::size_t size = N) -> hash
		{
			return static_cast<hash>(1ull * (size == 1
				? (k_offset_basis ^ str[0])
				: (hash_constexpr(str, size - 1) ^ str[size - 1])) * k_prime);
		}

		static auto __forceinline hash_runtime(const char* str) -> hash
		{
			auto result = k_offset_basis;
			do
			{
				result ^= *str++;
				result *= k_prime;
			} while (*(str - 1) != '\0');

			return result;
		}
	};
}

using fnv = ::detail::fnv_hash<sizeof(void*) * 8>;

#define FNV(str) (std::integral_constant<fnv::hash, fnv::hash_constexpr(str)>::value)

//--------------------------------------------------------------------------------

class vfunc_hook
{
public:
	uintptr_t* search_free_data_page(const char* module_name, const std::size_t vmt_size);
	vfunc_hook();
	vfunc_hook(void* base);
	~vfunc_hook();

	bool setup(void* base, const char* moduleName = nullptr);

	template<typename T>
	void hook_index(int index, T fun)
	{
		assert(index >= 0 && index <= (int)vftbl_len);
		new_vftbl[index + 1] = reinterpret_cast<std::uintptr_t>(fun);
	}
	void unhook_index(int index)
	{
		new_vftbl[index] = old_vftbl[index];
	}
	void unhook_all()
	{
		try {
			if (old_vftbl != nullptr) {
				auto guard = detail::protect_guard{ class_base, sizeof(std::uintptr_t), PAGE_READWRITE };
				*(std::uintptr_t**)class_base = old_vftbl;
				old_vftbl = nullptr;
			}
		}
		catch (...) {
		}
	}

	template<typename T>
	T get_original(int index)
	{
		return (T)old_vftbl[index];
	}

private:
	static inline std::size_t estimate_vftbl_length(std::uintptr_t* vftbl_start);

	void*           class_base;
	std::size_t     vftbl_len;
	std::uintptr_t* old_vftbl;
	std::uintptr_t* new_vftbl;
	LPCVOID search_base = nullptr;
	bool is_allocated = false;
};