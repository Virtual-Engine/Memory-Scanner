#pragma once
#include "../stdafx.hpp"

namespace ve {
	class module : public segment {
	public:
		explicit module(HMODULE mod);
		module(std::nullptr_t);
		explicit module(std::string_view name);
		explicit module(std::wstring_view name);

		wrapper get_export(std::string_view symbol_name);

		wrapper get_export(uint16_t ordinal);

		wrapper base() const;

		std::size_t size() const;

		bool is_valid() const;

	private:
		wrapper m_base;
		std::size_t m_size;
	};

}
