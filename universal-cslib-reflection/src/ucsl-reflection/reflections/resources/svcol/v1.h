#include <ucsl/resources/svcol/v1.h>
#include <simple-reflection/simple-reflection.h>
#include <ucsl-reflection/operators.h>
#include <ucsl-reflection/reflections/basic-types.h>

namespace ucsl::resources::svcol::v1::reflections {
	using namespace simplerfl;
	using namespace ucsl::reflection;

	namespace impl = ucsl::resources::svcol::v1;

	using Shape = enumeration<impl::Shape, "Shape", unsigned char,
		option<"SPHERE">,
		option<"CYLINDER">,
		option<"OBB">
	>;

	using SectorFilterData = structure<impl::SectorFilterData, "SectorFilterData", void,
		field<unsigned char, "sectorId">,
		field<bool, "enabled">
	>;

	using ShapeData = structure<impl::ShapeData, "ShapeData", void,
		field<const char*, "name">,
		field<unsigned char, "priority">,
		field<Shape, "type">,
		field<float, "depth">,
		field<float, "width">,
		field<float, "height">,
		field<math::Position, "position">,
		field<math::Rotation, "rotation">,
		field<math::Position, "aabbMin">,
		field<math::Position, "aabbMax">,
		field<unsigned char, "skySectorId">,
		field<unsigned int, "groundSectorFilterCount">,
		field<dynamic_carray<SectorFilterData, field_resolver<size_t, "groundSectorFilterCount">>*, "groundSectorFilters">
	>;

	using KdTreeNodeType = enumeration<impl::KdTreeNodeType, "KdTreeNodeType", unsigned char,
		option<"SPLIT_X">,
		option<"SPLIT_Y">,
		option<"SPLIT_Z">,
		option<"LEAF">
	>;

	using KdTreeNodeData = structure<impl::KdTreeNodeData, "KdTreeNodeData", void,
		field<int, "deadZoneStartCoordOrLeafIndexAndNodeType">,
		field<float, "deadZoneEndCoord">
	>;

	using KdTreeLeafData = structure<impl::KdTreeLeafData, "KdTreeLeafData", void,
		field<unsigned int, "shapeCount">,
		field<int, "shapeOffset">,
		field<math::Position, "aabbMin">,
		field<math::Position, "aabbMax">
	>;

	using SvColData = structure<impl::SvColData, "SvColData", void,
		field<constant<unsigned int, 0x5356434F>, "magic">,
		field<constant<unsigned int, 1>, "version">,
		field<unsigned int, "shapeCount">,
		field<dynamic_carray<ShapeData, field_resolver<size_t, "shapeCount">>*, "shapes"> // maybe todo: add reflections for KDtree data
	>;
}

namespace simplerfl {
	template<> struct canonical<ucsl::resources::svcol::v1::Shape> { using type = ucsl::resources::svcol::v1::reflections::Shape; };
	template<> struct canonical<ucsl::resources::svcol::v1::SectorFilterData> { using type = ucsl::resources::svcol::v1::reflections::SectorFilterData; };
	template<> struct canonical<ucsl::resources::svcol::v1::ShapeData> { using type = ucsl::resources::svcol::v1::reflections::ShapeData; };
	template<> struct canonical<ucsl::resources::svcol::v1::KdTreeNodeType> { using type = ucsl::resources::svcol::v1::reflections::KdTreeNodeType; };
	template<> struct canonical<ucsl::resources::svcol::v1::KdTreeNodeData> { using type = ucsl::resources::svcol::v1::reflections::KdTreeNodeData; };
	template<> struct canonical<ucsl::resources::svcol::v1::KdTreeLeafData> { using type = ucsl::resources::svcol::v1::reflections::KdTreeLeafData; };
	template<> struct canonical<ucsl::resources::svcol::v1::SvColData> { using type = ucsl::resources::svcol::v1::reflections::SvColData; };
}
