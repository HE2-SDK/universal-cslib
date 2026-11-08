#pragma once
#include <ucsl/resources/path/v1.h>
#include <simple-reflection/simple-reflection.h>
#include <ucsl-reflection/operators.h>
#include <ucsl-reflection/reflections/basic-types.h>

namespace ucsl::resources::path::v1::reflections {
	using namespace simplerfl;
	using namespace ucsl::reflection;

	namespace impl = ucsl::resources::path::v1;

	using PathIdData = structure<impl::PathIdData, "PathIdData", void,
		field<const char*, "name">,
		field<unsigned int, "id">
	>;

	using NodeData = structure<impl::NodeData, "NodeData", void,
		field<float, "distance">,
		field<unsigned int, "unk2">,
		field<ucsl::math::Position, "position">,
		field<ucsl::math::Position, "normal">,
		field<ucsl::math::Position, "tangent">,
		field<float, "unk3">
	>;

	using PathData = structure<impl::PathData, "PathData", void,
		field<const char*, "name">,
		field<unsigned int, "id">,
		field<unsigned int, "unk2">,
		field<float, "unk3">,
		field<unsigned int, "nodeCount">,
		field<dynamic_carray<NodeData, field_resolver<unsigned int, "nodeCount">>*, "nodes">,
		field<unsigned int, "extraVertexCount">,
		field<dynamic_carray<ucsl::math::Position, field_resolver<unsigned int, "extraVertexCount">>*, "extraVertices">
	>;

	using PathsData = structure<impl::PathsData, "PathsData", void,
		field<unsigned int, "magic">,
		field<unsigned int, "version">,
		field<unsigned int, "pathIdCount">,
		field<dynamic_carray<PathIdData, field_resolver<unsigned int, "pathIdCount">>*, "pathIds">,
		field<unsigned int, "pathCount">,
		field<dynamic_carray<PathData, field_resolver<unsigned int, "pathCount">>*, "paths">,
		field<weak<PathData*>, "stageGuidePath">,
		field<unsigned int, "objPathSize">,
		field<dynamic_carray<weak<PathData*>, field_resolver<unsigned int, "objPathSize">>*, "objPaths">,
		field<unsigned int, "qsPathSize">,
		field<dynamic_carray<weak<PathData*>, field_resolver<unsigned int, "qsPathSize">>*, "qsPaths">,
		field<unsigned int, "svPathSize">,
		field<dynamic_carray<weak<PathData*>, field_resolver<unsigned int, "svPathSize">>*, "svPaths">,
		field<unsigned int, "grPathSize">,
		field<dynamic_carray<weak<PathData*>, field_resolver<unsigned int, "grPathSize">>*, "grPaths">,
		field<unsigned int, "unkPathSize">,
		field<dynamic_carray<weak<PathData*>, field_resolver<unsigned int, "unkPathSize">>*, "unkPaths">
	>;
}

namespace simplerfl {
	template<> struct canonical<ucsl::resources::path::v1::PathIdData> { using type = ucsl::resources::path::v1::reflections::PathIdData; };
	template<> struct canonical<ucsl::resources::path::v1::NodeData> { using type = ucsl::resources::path::v1::reflections::NodeData; };
	template<> struct canonical<ucsl::resources::path::v1::PathData> { using type = ucsl::resources::path::v1::reflections::PathData; };
	template<> struct canonical<ucsl::resources::path::v1::PathsData> { using type = ucsl::resources::path::v1::reflections::PathsData; };
}
