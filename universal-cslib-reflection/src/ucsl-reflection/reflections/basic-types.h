#pragma once
#include <simple-reflection/simple-reflection.h>
#include <ucsl-reflection/operators.h>
#include <ucsl/memory/iallocator.h>
#include <ucsl/containers/arrays/array.h>
#include <ucsl/containers/arrays/tarray.h>
#include <ucsl/strings/variable-string.h>
#include <ucsl/math.h>
#include <ucsl/colors.h>
#include <ucsl/object-id.h>

namespace ucsl::reflections {
	using namespace simplerfl;
	using namespace ucsl::reflection;

	using Vector2 = structure<math::Vector2, "Vector2", void,
		field<float, "x">,
		field<float, "y">
	>;

	using Vector3 = aligned<16, structure<math::Vector3, "Vector3", void,
		field<float, "x">,
		field<float, "y">,
		field<float, "z">
	>>;

	using Vector4 = aligned<16, structure<math::Vector4, "Vector4", void,
		field<float, "x">,
		field<float, "y">,
		field<float, "z">,
		field<float, "w">
	>>;

	using Quaternion = aligned<16, structure<math::Quaternion, "Quaternion", void,
		field<float, "x">,
		field<float, "y">,
		field<float, "z">,
		field<float, "w">
	>>;

	using Matrix34 = aligned<16, structure<math::Matrix34, "Matrix34", void,
		field<Vector4, "t">,
		field<Vector4, "u">,
		field<Vector4, "v">,
		field<Vector4, "w">
	>>;

	using Matrix44 = aligned<16, structure<math::Matrix44, "Matrix44", void,
		field<Vector4, "t">,
		field<Vector4, "u">,
		field<Vector4, "v">,
		field<Vector4, "w">
	>>;

	using Position = structure<math::Position, "Position", void,
		field<float, "x">,
		field<float, "y">,
		field<float, "z">
	>;

	using Rotation = structure<math::Rotation, "Rotation", void,
		field<float, "x">,
		field<float, "y">,
		field<float, "z">,
		field<float, "w">
	>;

	//template<typename T, typename AllocatorSystem> size_t get_array_size(const containers::arrays::Array<T, AllocatorSystem>& arr) { return arr.size(); }
	//template<typename T, typename AllocatorSystem>
	//using Array = structure<containers::arrays::Array<T, AllocatorSystem>, "Array", void,
	//	field<dynamic_carray<T, containers::arrays::Array<T, AllocatorSystem>, get_array_size<T, AllocatorSystem>>*, "buffer">,
	//	field<size_t, "length">,
	//	field<size_t, "capacity">,
	//	field<erased<memory::IAllocator*>, "allocator">
	//>;

	//template<typename T, typename AllocatorSystem> size_t get_tarray_size(const containers::arrays::TArray<T, AllocatorSystem>& arr) { return arr.size(); }
	//template<typename T, typename AllocatorSystem>
	//using TArray = structure<containers::arrays::TArray<T, AllocatorSystem>, "TArray", void,
	//	field<dynamic_carray<T, containers::arrays::TArray<T, AllocatorSystem>, get_tarray_size<T, AllocatorSystem>>*, "buffer">,
	//	field<size_t, "length">,
	//	field<size_t, "capacity">
	//>;

	using VariableString = structure<strings::VariableString, "VariableString", void,
		field<const char*, "buffer">,
		field<erased<memory::IAllocator*>, "allocator">
	>;

	using ObjectIdV1 = structure<objectids::ObjectIdV1, "ObjectId", void,
		field<unsigned int, "id">
	>;

	using ObjectIdV2 = structure<objectids::ObjectIdV2, "ObjectId", void,
		field<unsigned long long, "groupId">,
		field<unsigned long long, "objectId">
	>;
}

namespace simplerfl {
	template<> struct canonical<ucsl::memory::IAllocator*> { using type = primitive<size_t>; };
	template<> struct canonical<ucsl::math::Vector2> { using type = primitive<ucsl::math::Vector2>; };
	template<> struct canonical<ucsl::math::Vector3> { using type = primitive<ucsl::math::Vector3>; };
	template<> struct canonical<ucsl::math::Vector4> { using type = primitive<ucsl::math::Vector4>; };
	template<> struct canonical<ucsl::math::Quaternion> { using type = primitive<ucsl::math::Quaternion>; };
	template<> struct canonical<ucsl::math::Matrix34> { using type = primitive<ucsl::math::Matrix34>; };
	template<> struct canonical<ucsl::math::Matrix44> { using type = primitive<ucsl::math::Matrix44>; };
	template<> struct canonical<ucsl::math::Position> { using type = primitive<ucsl::math::Position>; };
	template<> struct canonical<ucsl::math::Rotation> { using type = primitive<ucsl::math::Rotation>; };
	template<typename T, typename AllocatorSystem> struct canonical<ucsl::containers::arrays::Array<T, AllocatorSystem>> { using type = ucsl::reflection::array<T, AllocatorSystem>; };
	template<typename T, typename AllocatorSystem> struct canonical<ucsl::containers::arrays::TArray<T, AllocatorSystem>> { using type = ucsl::reflection::tarray<T, AllocatorSystem>; };
	template<> struct canonical<ucsl::strings::VariableString> { using type = primitive<ucsl::strings::VariableString>; };
	template<> struct canonical<ucsl::objectids::ObjectIdV1> { using type = primitive<ucsl::objectids::ObjectIdV1>; };
	template<> struct canonical<ucsl::objectids::ObjectIdV2> { using type = primitive<ucsl::objectids::ObjectIdV2>; };
	template<ucsl::colors::ChannelOrder order> struct canonical<ucsl::colors::Color8<order>> { using type = primitive<ucsl::colors::Color8<order>>; };
	template<ucsl::colors::ChannelOrder order> struct canonical<ucsl::colors::Colorf<order>> { using type = primitive<ucsl::colors::Colorf<order>>; };
}
