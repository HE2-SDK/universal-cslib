#pragma once

namespace ucsl::reflection::util {
	template<typename T, typename A>
	constexpr T align(T addr, A alignment) {
		return (T)(((size_t)addr + alignment - 1) & ~(alignment - 1));
	}

	template<typename T>
	constexpr T* addptr(T* addr, size_t off) {
		return reinterpret_cast<T*>(reinterpret_cast<size_t>(addr) + off);
	}
}
