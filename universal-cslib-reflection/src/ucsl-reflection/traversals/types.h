#pragma once
#include <optional>
#include <ucsl/rfl/ranges.h>

namespace ucsl::reflection::traversals {
	struct RootInfo {
		size_t alignment{};
		size_t size{};
	};

	struct StructureInfo {
		const char* name{};
		const void* rflClass{};
	};

	struct UnionInfo {};

	struct FieldInfo {
		const char* name{};
		bool erased{};
	};

	struct ArrayFieldInfo : FieldInfo {
		size_t size{};
	};

	struct ArrayFieldItemInfo : ArrayFieldInfo {
		size_t index{};
	};

	struct TypeInfo {
		size_t alignment{};
		size_t size{};
	};

	template<typename A, typename S>
	struct PointerInfo {
		A getTargetAlignment;
		S getTargetSize;
		bool isWeak{};
	};

	//template<typename A, typename S>
	//struct OffsetInfo {
	//	A getTargetAlignment;
	//	S getTargetSize;
	//};

	struct CArrayInfo {
		size_t size{};
		size_t stride{};
	};

	struct ArrayInfo {
		size_t itemAlignment{};
		size_t itemSize{};
	};

	template<typename T> struct PrimitiveInfo {
		bool erased{};
		std::optional<T> constantValue{};
		const ucsl::rfl::ranges::range_t<T>* range{};
	};

	template<typename T>
	struct EnumInfo {
		const T& options{};
	};

	template<typename T>
	struct FlagsInfo {
		const T& flags{};
	};

	//template<typename T>
	//concept Algorithm = requires (T t) {
	//	typename T::result_type;

	//	{ t.visit_primitive()
	//}
}
