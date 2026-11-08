#pragma once
#include <ucsl/math.h>
#include <ucsl/resources/model/v5.h>

namespace ucsl::resources::model::v5::reflections {
	using namespace simplerfl;
	using namespace ucsl::reflection;

	namespace impl = ucsl::resources::model::v5;

	using Format = enumeration<impl::ReflectionInfo::Format, "Format", unsigned int,
		fixed_option<"FLOAT1", 0x2C83A4>,
		fixed_option<"FLOAT2", 0x2C23A5>,
		fixed_option<"FLOAT3", 0x2A23B9>,
		fixed_option<"FLOAT4", 0x1A23A6>,
		fixed_option<"INT1", 0x2C83A1>,
		fixed_option<"INT2", 0x2C23A2>,
		fixed_option<"INT4", 0x1A23A3>,
		fixed_option<"UINT1", 0x2C82A1>,
		fixed_option<"UINT2", 0x2C22A2>,
		fixed_option<"UINT4", 0x1A22A3>,
		fixed_option<"INT1_NORM", 0x2C81A1>,
		fixed_option<"INT2_NORM", 0x2C21A2>,
		fixed_option<"INT4_NORM", 0x1A21A3>,
		fixed_option<"UINT1_NORM", 0x2C80A1>,
		fixed_option<"UINT2_NORM", 0x2C20A2>,
		fixed_option<"UINT4_NORM", 0x1A20A3>,
		fixed_option<"D3D_COLOR", 0x182886>,
		fixed_option<"U_BYTE4", 0x1A2286>,
		fixed_option<"BYTE4", 0x1A2386>,
		fixed_option<"U_BYTE4_NORM", 0x1A2086>,
		fixed_option<"BYTE4_NORM", 0x1A2186>,
		fixed_option<"SHORT2", 0x2C2359>,
		fixed_option<"SHORT4", 0x1A235A>,
		fixed_option<"USHORT2", 0x2C2259>,
		fixed_option<"USHORT4", 0x1A225A>,
		fixed_option<"SHORT2_NORM", 0x2C2159>,
		fixed_option<"SHORT4_NORM", 0x1A215A>,
		fixed_option<"USHORT2_NORM", 0x2C2059>,
		fixed_option<"USHORT4_NORM", 0x1A205A>,
		fixed_option<"U_DEC3", 0x2A2287>,
		fixed_option<"DEC3", 0x2A2387>,
		fixed_option<"U_DEC3_NORM", 0x2A2087>,
		fixed_option<"DEC3_NORM", 0x2A2187>,
		fixed_option<"U_DEC4", 0x1A2287>,
		fixed_option<"DEC4", 0x1A2387>,
		fixed_option<"U_DEC4_NORM", 0x1A2087>,
		fixed_option<"DEC4_NORM", 0x1A2187>,
		fixed_option<"U_HEND3", 0x2A2290>,
		fixed_option<"HEND3", 0x2A2390>,
		fixed_option<"UHEND3_NORM", 0x2A2090>,
		fixed_option<"HEND3_NORM", 0x2A2190>,
		fixed_option<"UDHEN3", 0x2A2291>,
		fixed_option<"DHEN3", 0x2A2391>,
		fixed_option<"UDHEN3_NORM", 0x2A2091>,
		fixed_option<"DHEN3_NORM", 0x2A2191>,
		fixed_option<"FLOAT16_2", 0x2C235F>,
		fixed_option<"FLOAT16_4", 0x1A2360>,
		fixed_option<"INVALID", 0xFFFFFFFF>
	>;

	using Method = enumeration<impl::ReflectionInfo::Method, "Method", unsigned char,
		option<"NORMAL">,
		option<"PARTIAL_U">,
		option<"PARTIAL_V">,
		option<"CROSS_UV">,
		option<"UV">,
		option<"LOOKUP">,
		option<"LOOKUP_PRE_SAMPLED">
	>;

	using Type = enumeration<impl::ReflectionInfo::Type, "Type", unsigned char,
		option<"POSITION">,
		option<"BLEND_WEIGHT">,
		option<"BLEND_INDICES">,
		option<"NORMAL">,
		option<"P_SIZE">,
		option<"TEX_COORD">,
		option<"TANGENT">,
		option<"BINORMAL">,
		option<"TESS_FACTOR">,
		option<"POSITION_T">,
		option<"COLOR">,
		option<"FOG">,
		option<"DEPTH">,
		option<"SAMPLE">
	>;

	using ReflectionInfo = structure<impl::ReflectionInfo, "ReflectionInfo", void,
		field<short, "index">,
		field<unsigned short, "offset">,
		field<Format, "format">,
		field<Method, "method">,
		field<Type, "type">,
		field<unsigned char, "usageIndex">
	>;

	using Texture = structure<impl::Texture, "Texture", void,
		field<const char*, "name">,
		field<unsigned char, "index">
	>;

	using Mesh = structure<impl::Mesh, "Mesh", void,
		field<const char*, "name">,
		field<unsigned int, "indexCount">,
		field<dynamic_carray<unsigned short, field_resolver<unsigned int, "indexCount">>*, "indices">,
		field<unsigned int, "vertexCount">,
		field<unsigned int, "vertexStride">,
		field<dynamic_carray<unsigned char, selector_resolver<unsigned int, field_resolver<unsigned int, "vertexCount">, field_resolver<unsigned int, "vertexStride">>::impl<[](const unsigned int& vertexCount, const unsigned int& vertexStride) -> unsigned int { return vertexCount * vertexStride; }>>*, "vertexData" > ,
		field<unsigned int, "reflectionInfo">,
		field<unsigned int, "boneIndexCount">,
		field<dynamic_carray<unsigned char, field_resolver<unsigned int, "boneIndexCount">>*, "boneIndices">,
		field<unsigned int, "textureCount">,
		field<dynamic_carray<Texture*, field_resolver<unsigned int, "textureCount">>*, "textures">
	>;

	using MeshGroup = structure<impl::MeshGroup, "MeshGroup", void,
		field<unsigned int, "opaqueMeshCount">,
		field<dynamic_carray<Mesh*, field_resolver<unsigned int, "opaqueMeshCount">>*, "opaqueMeshes">,
		field<unsigned int, "transparentMeshCount">,
		field<dynamic_carray<Mesh*, field_resolver<unsigned int, "transparentMeshCount">>*, "transparentMeshes">,
		field<unsigned int, "punchthroughMeshCount">,
		field<dynamic_carray<Mesh*, field_resolver<unsigned int, "punchthroughMeshCount">>*, "punchthroughMeshes">,
		field<unsigned int, "customMeshTypeCount">,
		field<unsigned int, "customMeshTypeNames">,//dynamic_carray<const char*, field_resolver<unsigned int, "customMeshTypeCount">>*, "customMeshTypeNames">,
		field<unsigned int, "customMeshTypeMeshCounts">,//dynamic_carray<unsigned int, field_resolver<unsigned int, "customMeshTypeCount">>*, "customMeshTypeMeshCounts">,
		field<unsigned int, "customMeshTypeMeshes">
	>;

	//using MorphTarget = structure<impl::MorphTarget, "MorphTarget", void,
	//    const char* name;
	//    ucsl::math::Vector3* positions;
	//>;

	//using MorphModel = structure<impl::MorphModel, "MorphModel", void,
	//    unsigned int vertexCount;
	//
	//>;

	using Node = structure<impl::Node, "Node", void,
		field<unsigned int, "parentIdx">,
		field<const char*, "name">
	>;

	using ModelContexts = structure<impl::ModelContexts, "ModelContexts", void,
		field<unsigned int, "meshGroupCount">,
		field<dynamic_carray<MeshGroup*, field_resolver<unsigned int, "meshGroupCount">>*, "meshGroups">,
		field<unsigned int, "morphModelCount">,
		field<unsigned int, "morphModels">,
		field<unsigned int, "nodeCount">,
		field<dynamic_carray<Node*, field_resolver<unsigned int, "nodeCount">>*, "nodes">,
		field<unsigned int, "nodeTransforms">,//dynamic_carray<ucsl::math::Matrix34, impl::ModelContexts, [](const impl::ModelContexts& parent) -> size_t { return parent.nodeCount; }>*, "nodeTransforms">
		field<unsigned int, "aabb">
	>;

	using TerrainModelContexts = structure<impl::TerrainModelContexts, "TerrainModelContexts", void,
		field<unsigned int, "meshGroupCount">,
		field<dynamic_carray<MeshGroup*, field_resolver<unsigned int, "meshGroupCount">>*, "meshGroups">,
		field<const char*, "name">,
		field<unsigned int, "aabb">
	>;
}

namespace simplerfl {
	template<> struct canonical<ucsl::resources::model::v5::ModelContexts> { using type = ucsl::resources::model::v5::reflections::ModelContexts; };
}
