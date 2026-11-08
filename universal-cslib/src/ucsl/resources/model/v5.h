#pragma once
#include <ucsl/math.h>

namespace ucsl::resources::model::v5 {
    struct ReflectionInfo {
        enum class Format : unsigned int {
            FLOAT1 = 0x2C83A4,
            FLOAT2 = 0x2C23A5,
            FLOAT3 = 0x2A23B9,
            FLOAT4 = 0x1A23A6,
            INT1 = 0x2C83A1,
            INT2 = 0x2C23A2,
            INT4 = 0x1A23A3,
            UINT1 = 0x2C82A1,
            UINT2 = 0x2C22A2,
            UINT4 = 0x1A22A3,
            INT1_NORM = 0x2C81A1,
            INT2_NORM = 0x2C21A2,
            INT4_NORM = 0x1A21A3,
            UINT1_NORM = 0x2C80A1,
            UINT2_NORM = 0x2C20A2,
            UINT4_NORM = 0x1A20A3,
            D3D_COLOR = 0x182886,
            U_BYTE4 = 0x1A2286,
            BYTE4 = 0x1A2386,
            U_BYTE4_NORM = 0x1A2086,
            BYTE4_NORM = 0x1A2186,
            SHORT2 = 0x2C2359,
            SHORT4 = 0x1A235A,
            USHORT2 = 0x2C2259,
            USHORT4 = 0x1A225A,
            SHORT2_NORM = 0x2C2159,
            SHORT4_NORM = 0x1A215A,
            USHORT2_NORM = 0x2C2059,
            USHORT4_NORM = 0x1A205A,
            U_DEC3 = 0x2A2287,
            DEC3 = 0x2A2387,
            U_DEC3_NORM = 0x2A2087,
            DEC3_NORM = 0x2A2187,
            U_DEC4 = 0x1A2287,
            DEC4 = 0x1A2387,
            U_DEC4_NORM = 0x1A2087,
            DEC4_NORM = 0x1A2187,
            U_HEND3 = 0x2A2290,
            HEND3 = 0x2A2390,
            UHEND3_NORM = 0x2A2090,
            HEND3_NORM = 0x2A2190,
            UDHEN3 = 0x2A2291,
            DHEN3 = 0x2A2391,
            UDHEN3_NORM = 0x2A2091,
            DHEN3_NORM = 0x2A2191,
            FLOAT16_2 = 0x2C235F,
            FLOAT16_4 = 0x1A2360,
            INVALID = 0xFFFFFFFF
        };

        enum class Method : unsigned char {
            NORMAL,
            PARTIAL_U,
            PARTIAL_V,
            CROSS_UV,
            UV,
            LOOKUP,
            LOOKUP_PRE_SAMPLED,
        };

        enum class Type : unsigned char {
            POSITION,
            BLEND_WEIGHT,
            BLEND_INDICES,
            NORMAL,
            P_SIZE,
            TEX_COORD,
            TANGENT,
            BINORMAL,
            TESS_FACTOR,
            POSITION_T,
            COLOR,
            FOG,
            DEPTH,
            SAMPLE,
        };

        short index;
        unsigned short offset;
        Format format;
        Method method;
        Type type;
        unsigned char usageIndex;
    };

    struct Texture {
        const char* name;
        unsigned char index;
    };

    struct Mesh {
        const char* name;
        unsigned int indexCount;
        unsigned short* indices;
        unsigned int vertexCount;
        unsigned int vertexStride;
        unsigned char* vertexData;
        size_t reflectionInfo; //ReflectionInfo* reflectionInfo;
        unsigned int boneIndexCount;
        unsigned char* boneIndices;
        unsigned int textureCount;
        Texture** textures;
    };

    struct MeshGroup {
        unsigned int opaqueMeshCount;
        Mesh** opaqueMeshes;
        unsigned int transparentMeshCount;
        Mesh** transparentMeshes;
        unsigned int punchthroughMeshCount;
        Mesh** punchthroughMeshes;
        unsigned int customMeshTypeCount;
        const char** customMeshTypeNames;
        unsigned int* customMeshTypeMeshCounts;
        Mesh*** customMeshTypeMeshes;
    };

    //struct MorphTarget {
    //    const char* name;
    //    ucsl::math::Vector3* positions;
    //};

    //struct MorphModel {
    //    unsigned int vertexCount;
    //
    //};

    struct Node {
        unsigned int parentIdx;
        const char* name;
    };

	struct ModelContexts {
        unsigned int meshGroupCount;
        MeshGroup** meshGroups;
        unsigned int morphModelCount;
        size_t morphModels;
        unsigned int nodeCount;
        Node* nodes;
        ucsl::math::Matrix34* nodeTransforms;
        void* aabb;
	};
    
    struct TerrainModelContexts {
        unsigned int meshGroupCount;
        MeshGroup** meshGroups;
        const char* name;
        void* aabb;
    };
}
