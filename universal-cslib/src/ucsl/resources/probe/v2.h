#pragma once

namespace ucsl::resources::probe::v2 {
    enum class Shape {
        AABB,
        SPHERE
    };

    struct ProbeData {
        float matrix[12]; // Rotation and scale
        math::Position boxPosition;
        float unk0;
        math::Position spherePosition;
        float transitionFade; // Decides how hard the edge should be
        const char* textureName;
        Shape shape;
        void* texture; // needle::Texture loaded at runtime
        unsigned int type2;
        unsigned int type3;
        unsigned int parallax; // If 1, the reflection is parallax corrected
        unsigned int unk4;
    };

    struct ResProbeData {
        unsigned int magic;
        unsigned int version;
        ProbeData* probes;
        unsigned int probeCount;
    };
}
