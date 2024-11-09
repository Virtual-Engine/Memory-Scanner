#pragma once
#include "module.hpp"
#include "../stdafx.hpp"

namespace ve {

	module::module(HMODULE mod) : segment(mod, 0) {
		if (!mod) {
			throw std::invalid_argument("Invalid module handle.");
		}

		auto dosHeader = m_base.as<IMAGE_DOS_HEADER*>();
		assert(dosHeader->e_magic == IMAGE_DOS_SIGNATURE && "Invalid DOS header signature.");

		auto ntHeader = m_base.add(dosHeader->e_lfanew).as<IMAGE_NT_HEADERS*>();
		assert(ntHeader->Signature == IMAGE_NT_SIGNATURE && "Invalid NT header signature.");

		m_size = ntHeader->OptionalHeader.SizeOfImage;
	}

	module::module(std::nullptr_t) : module(GetModuleHandle(nullptr)) {}

	module::module(std::string_view name) : module(GetModuleHandleA(name.data())) {
		if (!m_base) {
			throw std::runtime_error("Failed to load module by name: " + std::string(name));
		}
	}

	module::module(std::wstring_view name) : module(GetModuleHandleW(name.data())) {
		if (!m_base) {
			throw std::runtime_error("Failed to load module by wide name.");
		}
	}

	wrapper module::get_export(std::string_view symbol_name) {
		if (!m_base) {
			throw std::runtime_error("Invalid module base.");
		}

		FARPROC symbol = GetProcAddress(m_base.as<HMODULE>(), symbol_name.data());
		if (!symbol) {
			throw std::runtime_error("Symbol not found: " + std::string(symbol_name));
		}

		return wrapper(symbol);
	}

	wrapper module::get_export(uint16_t ordinal) {
		if (!m_base) {
			throw std::runtime_error("Invalid module base.");
		}

		FARPROC symbol = GetProcAddress(m_base.as<HMODULE>(), MAKEINTRESOURCEA(ordinal));
		if (!symbol) {
			throw std::runtime_error("Symbol not found with ordinal: " + std::to_string(ordinal));
		}

		return wrapper(symbol);
	}

	wrapper module::base() const {
		return m_base;
	}

	std::size_t module::size() const {
		return m_size;
	}

	bool module::is_valid() const {
		return m_base && m_size > 0;
	}
}
