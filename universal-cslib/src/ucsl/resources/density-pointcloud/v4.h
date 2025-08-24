#pragma once

namespace ucsl::resources::density_pointcloud::v4 {
    struct InstanceData {
        csl::math::Position position;
        int unk0;
        csl::math::Position scale;
        int unk1;
        csl::math::Rotation rotation;
        int unk2[4];
        int unk3;
        int densitySettingReferenceId;
        int unk4;
        int unk5[2];
    };
    
    struct DensityPointCloudData {
        unsigned int magic;
        unsigned int version;
        long long unk0;
        InstanceData* instances;
        unsigned int instanceCount;
    };
}