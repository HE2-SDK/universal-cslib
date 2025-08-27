#pragma once
#include <ucsl/math.h>

namespace ucsl::resources::shlightfield::v1{
    struct Node{
        const char* name;
        int probeCounts[3]; //x, y, z
        ucsl::math::Position position;
        ucsl::math::Position rotation;
        ucsl::math::Position scale;
    };

    struct SHLightFieldData{
        int version;
        float defaultProbeLightingData[36];
        unsigned int nodeCount;
        Node* nodes;
    };
};
