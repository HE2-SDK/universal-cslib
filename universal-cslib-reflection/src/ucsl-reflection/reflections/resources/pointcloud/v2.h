#pragma once
#include <simple-reflection/simple-reflection.h>
#include <ucsl-reflection/operators.h>
#include <ucsl-reflection/reflections/basic-types.h>
#include <ucsl/resources/pointcloud/v2.h>

namespace ucsl::resources::pointcloud::v2::reflections {
	using namespace simplerfl;
	using namespace ucsl::reflection;

	namespace impl = ucsl::resources::pointcloud::v2;

	using RotationOrder = enumeration<impl::RotationOrder, "RotationOrder", unsigned int,
		option<"NONE">,
		option<"XYZ">,
		option<"YZX">,
		option<"ZXY">,
		option<"XZY">,
		option<"YXZ">,
		option<"ZYX">
	>;

	using InstanceData = structure<impl::InstanceData, "InstanceData", void,
		field<const char*, "name">,
		field<const char*, "resourceName">,
		field<ucsl::math::Position, "position">,
		field<ucsl::math::Position, "rotation">,
		field<RotationOrder, "rotationOrder">,
		field<ucsl::math::Position, "scale">,
		field<constant<unsigned int, 0>, "unk2">
	>;

	using PointcloudData = structure<impl::PointcloudData, "PointcloudData", void,
		field<constant<unsigned int, 0x43495043>, "magic">,
		field<constant<unsigned int, 2>, "version">,
		field<dynamic_carray<InstanceData, field_resolver<unsigned int, "instanceCount">>*, "instances">,
		field<size_t, "instanceCount">
	>;
}

namespace simplerfl {
	template<> struct canonical<ucsl::resources::pointcloud::v2::InstanceData> { using type = ucsl::resources::pointcloud::v2::reflections::InstanceData; };
	template<> struct canonical<ucsl::resources::pointcloud::v2::PointcloudData> { using type = ucsl::resources::pointcloud::v2::reflections::PointcloudData; };
}
