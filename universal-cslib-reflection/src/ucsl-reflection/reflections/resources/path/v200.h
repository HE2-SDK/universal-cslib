#include <ucsl/resources/path/v200.h>
#include <simple-reflection/simple-reflection.h>
#include <ucsl-reflection/operators.h>
#include <ucsl-reflection/reflections/basic-types.h>
#include <ucsl-reflection/reflections/resources/kd-tree/v1.h>

namespace ucsl::resources::path::v200::reflections {
	using namespace simplerfl;
	using namespace ucsl::reflection;

	namespace impl = ucsl::resources::path::v200;

	using TagType = enumeration<impl::TagType, "TagType", unsigned int,
		option<"UINT">,
		option<"UNK1">,
		option<"STRING">
	>;

	inline size_t get_tag_value_idx(const impl::TagType& type) {
		switch (type) {
		case impl::TagType::UINT: return 0;
		case impl::TagType::STRING: return 1;
		}
	}

	using TagValue = unionof<impl::TagValue, "TagValue", selector_resolver<size_t, field_resolver<impl::TagType, "type">>::impl<get_tag_value_idx>,
		field<unsigned int, "uintValue">,
		field<const char*, "stringValue">
	>;

	using TagData = structure<impl::TagData, "TagData", void,
		field<const char*, "name">,
		field<TagType, "type">,
		field<TagValue, "value">
	>;

	using PathData = structure<impl::PathData, "PathData", void,
		field<const char*, "name">,
		field<bool, "unk1">,
		field<unsigned short, "nodeCount">,
		field<float, "length">,
		field<dynamic_carray<bool, field_resolver<unsigned short, "nodeCount">>*, "unk2">,
		field<dynamic_carray<float, field_resolver<unsigned short, "nodeCount">>*, "distances">,
		field<dynamic_carray<ucsl::math::Position, field_resolver<unsigned short, "nodeCount">>*, "positions">,
		field<dynamic_carray<ucsl::math::Position, field_resolver<unsigned short, "nodeCount">>*, "upVectors">,
		field<dynamic_carray<ucsl::math::Position, field_resolver<unsigned short, "nodeCount">>*, "forwardVectors">,
		field<unsigned short, "extraVertexCount">,
		field<dynamic_carray<ucsl::math::Position, field_resolver<unsigned short, "extraVertexCount">>*, "extraVertices">,
		field<ucsl::math::Position, "aabbMin">,
		field<ucsl::math::Position, "aabbMax">,
		field<unsigned short, "tagCount">,
		field<dynamic_carray<TagData, field_resolver<unsigned short, "tagCount">>*, "tags">,
		field<unsigned short, "unk3">,
		field<kd_tree::v1::reflections::ResKdTreeData*, "kdTree">
	>;

	using PathsData = structure<impl::PathsData, "PathsData", void,
		field<unsigned int, "magic">,
		field<unsigned int, "version">,
		field<unsigned int, "pathCount">,
		field<dynamic_carray<PathData, field_resolver<unsigned int, "pathCount">>*, "paths">
	>;
}

namespace simplerfl {
	template<> struct canonical<ucsl::resources::path::v200::TagType> { using type = ucsl::resources::path::v200::reflections::TagType; };
	template<> struct canonical<ucsl::resources::path::v200::TagValue> { using type = ucsl::resources::path::v200::reflections::TagValue; };
	template<> struct canonical<ucsl::resources::path::v200::TagData> { using type = ucsl::resources::path::v200::reflections::TagData; };
	template<> struct canonical<ucsl::resources::path::v200::PathData> { using type = ucsl::resources::path::v200::reflections::PathData; };
	template<> struct canonical<ucsl::resources::path::v200::PathsData> { using type = ucsl::resources::path::v200::reflections::PathsData; };
}
