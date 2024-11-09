#pragma once
#include "../stdafx.hpp"

namespace ve
{
	class wrapper
	{
	public:
		wrapper(void* ptr = nullptr);
		explicit wrapper(std::uintptr_t ptr);

		template <typename T>
		std::enable_if_t<std::is_pointer_v<T>, T> as() const;

		template <typename T>
		std::enable_if_t<std::is_lvalue_reference_v<T>, T> as() const;

		template <typename T>
		std::enable_if_t<std::is_same_v<T, std::uintptr_t>, T> as() const;

		template <typename T>
		std::enable_if_t<std::is_same_v<T, std::shared_ptr<typename T::element_type>>, T> as() const;

		template <typename T>
		std::enable_if_t<std::is_same_v<T, std::unique_ptr<typename T::element_type>>, T> as() const;

		template <typename T>
		std::enable_if_t<std::is_integral_v<T>, wrapper> add(T offset) const;

		template <typename T>
		std::enable_if_t<std::is_integral_v<T>, wrapper> sub(T offset) const;

		wrapper rip() const;

		explicit operator bool() const;

		friend bool operator==(wrapper a, wrapper b);
		friend bool operator!=(wrapper a, wrapper b);

	private:
		void* ptr;
	};


	inline wrapper::wrapper(void* ptr) :
		ptr(ptr)
	{}

	inline wrapper::wrapper(std::uintptr_t ptr) :
		ptr(reinterpret_cast<void*>(ptr))
	{}

	template <typename T>
	inline std::enable_if_t<std::is_pointer_v<T>, T> wrapper::as() const
	{
		return static_cast<T>(ptr);
	}

	template <typename T>
	inline std::enable_if_t<std::is_lvalue_reference_v<T>, T> wrapper::as() const
	{
		if (!ptr)
			throw std::runtime_error("Null pointer dereferenced in wrapper::as.");
		return *static_cast<std::add_pointer_t<std::remove_reference_t<T>>>(ptr);
	}

	template <typename T>
	inline std::enable_if_t<std::is_same_v<T, std::uintptr_t>, T> wrapper::as() const
	{
		return reinterpret_cast<std::uintptr_t>(ptr);
	}

	template <typename T>
	inline std::enable_if_t<std::is_same_v<T, std::shared_ptr<typename T::element_type>>, T> wrapper::as() const
	{
		return std::static_pointer_cast<typename T::element_type>(std::shared_ptr<void>(ptr));
	}

	template <typename T>
	inline std::enable_if_t<std::is_same_v<T, std::unique_ptr<typename T::element_type>>, T> wrapper::as() const
	{
		return std::unique_ptr<typename T::element_type>(static_cast<typename T::element_type*>(ptr));
	}

	template <typename T>
	inline std::enable_if_t<std::is_integral_v<T>, wrapper> wrapper::add(T offset) const
	{
		return wrapper(as<std::uintptr_t>() + offset);
	}

	template <typename T>
	inline std::enable_if_t<std::is_integral_v<T>, wrapper> wrapper::sub(T offset) const
	{
		return wrapper(as<std::uintptr_t>() - offset);
	}

	inline wrapper wrapper::rip() const
	{
		return add(as<std::int32_t&>()).add(4);
	}

	inline bool operator==(wrapper a, wrapper b)
	{
		return a.ptr == b.ptr;
	}

	inline bool operator!=(wrapper a, wrapper b)
	{
		return a.ptr != b.ptr;
	}

	inline wrapper::operator bool() const
	{
		return ptr != nullptr;
	}
}
