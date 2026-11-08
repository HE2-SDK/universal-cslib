#pragma once
#include <ucsl/math.h>

namespace ucsl::resources::anmpxd::v200{
    template<typename T>
    struct TrackDataType {
        unsigned int count;
        unsigned short* frames;
        T* values;
    };

    struct TrackData {
        TrackDataType<ucsl::math::Vector3> positionData;
        TrackDataType<ucsl::math::Quaternion> rotationData;
        TrackDataType<ucsl::math::Vector3> scaleData;
    };

    struct AnimationData {
        float playrate;
        int frameLength;
        unsigned int trackCount;
        union {
            void* mainAnimACLData;
            TrackData* mainTracks;
        };
        union {
            void* rootAnimACLData;
            TrackData* rootTrack;
        };
    };
    
    struct AnimationPXDData{
        enum class Type : unsigned char {
            COMPRESSED,
            ACL = 8
        };

        unsigned int magic;
		unsigned int version;
        unsigned char flags;
        Type type;
        AnimationData* animData;
    };
};
