#pragma once

#include <memory>
#include <iostream>
#include <unordered_map>
#include <map>
#include "misc/Recv.hpp"
#include "../helpers/vfunc_hook.h"

class netvar_manager
{
private:
	struct stored_data
	{
		RecvProp* prop_ptr;
		uint16_t class_relative_offset;
	};

public:
	// Return a const instance, others shouldnt modify this.
	static auto get() -> const netvar_manager&
	{
		static netvar_manager instance;
		return instance;
	}

	auto get_offset(const fnv::hash hash) const -> std::uint16_t
	{
		return m_props.at(hash).class_relative_offset;
	}

	auto get_prop(const fnv::hash hash) const -> RecvProp*
	{
		return m_props.at(hash).prop_ptr;
	}

	__declspec(noinline) static auto get_offset_by_hash(const fnv::hash hash) -> std::uint16_t
	{
		return get().get_offset(hash);
	}

	template<fnv::hash Hash>
	static auto get_offset_by_hash_cached() -> std::uint16_t
	{
		static auto offset = std::uint16_t(0);
		if (!offset)
			offset = get_offset_by_hash(Hash);
		return offset;
	}

private:
	netvar_manager();
	auto dump_recursive(const char* base_class, RecvTable* table, std::uint16_t offset) -> void;

private:
	std::map<fnv::hash, stored_data> m_props;
};
