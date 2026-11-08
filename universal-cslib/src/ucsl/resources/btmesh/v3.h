#pragma once
#include <ucsl/math.h>

namespace ucsl::resources::btmesh::v3 {
    struct Primitive {
        enum class Type : unsigned char {
            SPHERE,
            BOX,
            CAPSULE,
            CYLINDER
        };

        Type type;
        unsigned char surfaceLayer;
        unsigned int surfaceTypeFlag;
        math::Vector3 position;
        math::Quaternion rotation;
        math::Vector3 dimensions;
    };

    struct Shape {
        struct Face {
            unsigned short a;
            unsigned short b;
            unsigned short c;
        };

        enum class Flags : unsigned char {
            U16_FACES,
            CONVEX_SHAPE
        };

        Flags flags;
        unsigned int layer;
        unsigned int vertexCount;
        unsigned int faceCount;
        unsigned int bvhSize;
        unsigned int typeCount;
        unsigned int unk0;
        unsigned int unk1;
        math::Vector3* vertices;
        Face* faces;
        void* bvh;
        unsigned int* types;
    };

    struct BtMeshData {
        int unk0; // probably an empty signature
        unsigned int version;
        Shape* shapes;
        unsigned int shapeCount;
        unsigned int primitiveCount;
        Primitive* primitives;
    };
}
