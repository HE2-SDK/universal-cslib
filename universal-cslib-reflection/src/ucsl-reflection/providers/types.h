#pragma once
#include <ucsl/rfl/ranges.h>

namespace ucsl::reflection::providers {
	enum class TypeKind {
		PRIMITIVE,
		ENUM,
		BITFIELD,
		ARRAY,
		TARRAY,
		POINTER,
		CARRAY,
		UNION,
		STRUCTURE,
		TYPE,
	};

	template<typename R> struct PrimitiveData {
		using repr = R;

		bool is_erased{};
		std::optional<R> constant_value{};
		const ucsl::rfl::ranges::range_t<R>* range{};
	};
	template<size_t size> struct PrimitiveData<char[size]> {
		using repr = char[size];

		bool is_erased{};
	};

	template<TypeKind K> struct Kind {

	};
}
