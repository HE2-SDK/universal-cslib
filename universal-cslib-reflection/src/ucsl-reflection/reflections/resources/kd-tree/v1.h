#include <ucsl/resources/kd-tree/v1.h>
#include <simple-reflection/simple-reflection.h>
#include <ucsl-reflection/operators.h>
#include <ucsl-reflection/reflections/basic-types.h>

namespace ucsl::resources::kd_tree::v1::reflections {
	using namespace simplerfl;
	using namespace ucsl::reflection;

	namespace impl = ucsl::resources::kd_tree::v1;

	using ResKdTreeNodeType = enumeration<impl::ResKdTreeNodeType, "ResKdTreeNodeType",
		option<"SPLIT_X">,
		option<"SPLIT_Y">,
		option<"SPLIT_Z">,
		option<"LEAF">
	>;

	using ResKdTreeNodeData = structure<impl::ResKdTreeNodeData, "ResKdTreeNodeData", void,
		field<unsigned int, "deadZoneStartCoordOrLeafIndexAndNodeType">,
		field<float, "deadZoneEndCoord">
	>;

	using ResKdTreeLeafNodeData = structure<impl::ResKdTreeLeafNodeData, "ResKdTreeLeafNodeData", void,
		field<unsigned int, "objectCount">,
		field<unsigned int, "objectsOffset">
	>;

	using ResKdTreeData = structure<impl::ResKdTreeData, "ResKdTreeData", void,
		field<int, "depth">,
		field<int, "nodeCount">,
		field<dynamic_carray<ResKdTreeNodeData, field_resolver<int, "nodeCount">>*, "nodes">,
		field<int, "leafCount">,
		field<dynamic_carray<ResKdTreeLeafNodeData, field_resolver<int, "leafCount">>*, "leaves">,
		field<int, "objectCount">,
		field<dynamic_carray<unsigned int, field_resolver<int, "objectCount">>*, "objects">
	>;
}

namespace simplerfl {
	template<> struct canonical<ucsl::resources::kd_tree::v1::ResKdTreeNodeType> { using type = ucsl::resources::kd_tree::v1::reflections::ResKdTreeNodeType; };
	template<> struct canonical<ucsl::resources::kd_tree::v1::ResKdTreeNodeData> { using type = ucsl::resources::kd_tree::v1::reflections::ResKdTreeNodeData; };
	template<> struct canonical<ucsl::resources::kd_tree::v1::ResKdTreeLeafNodeData> { using type = ucsl::resources::kd_tree::v1::reflections::ResKdTreeLeafNodeData; };
	template<> struct canonical<ucsl::resources::kd_tree::v1::ResKdTreeData> { using type = ucsl::resources::kd_tree::v1::reflections::ResKdTreeLeafNodeData; };
}
