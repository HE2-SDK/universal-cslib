#pragma once
#include <ucsl/bitset.h>
#include <ucsl/math.h>

// reversed from files

namespace ucsl::resources::effdb::v100 {
    struct Emitter {
        enum class TransType : unsigned int {
            NODE,
            NODE_AND_FRAME,
            NODE_POSITION,
        };

        unsigned int flags;
        unsigned int attachTimeInFrames;
        const char* emitterName;
        const char* boneName;
        ucsl::math::Vector3 position;
        ucsl::math::Quaternion rotation;
        ucsl::math::Vector3 scale;

        bool GetIsAttachedFlag() const {
            return flags & (1 << 0);
        }

        void SetIsAttachedFlag(bool value) {
            flags = (flags & ~(1 << 0)) | (value ? (1 << 0) : 0);
        }

        bool GetDisableBoneScaleFlag() const {
            return flags & (1 << 1);
        }

        void SetDisableBoneScaleFlag(bool value) {
            flags = (flags & ~(1 << 1)) | (value ? (1 << 1) : 0);
        }

        TransType GetTransType() const {
            return static_cast<TransType>((flags >> 7) & 3);
        }

        void SetTransType(TransType transType) {
            flags = (flags & 0xFFFFFE7F) | (static_cast<unsigned int>(transType) << 7);
        }

        bool GetUsePositionFlag() const {
            return flags & (1 << 9);
        }

        void SetUsePositionFlag(bool value) {
            flags = (flags & ~(1 << 9)) | (value ? (1 << 9) : 0);
        }

        bool GetUseRotationFlag() const {
            return flags & (1 << 10);
        }

        void SetUseRotationFlag(bool value) {
            flags = (flags & ~(1 << 10)) | (value ? (1 << 10) : 0);
        }

        bool GetUseScaleFlag() const {
            return flags & (1 << 11);
        }

        void SetUseScaleFlag(bool value) {
            flags = (flags & ~(1 << 11)) | (value ? (1 << 11) : 0);
        }
    };

    struct EmitterSet {
        const char* clipName;
        unsigned int emitterCount;
        Emitter* emitters;
        unsigned int soundNameCount;
        const char** soundNames;
    };

    struct EffdbData{
        unsigned int magic;
        unsigned int version;
        unsigned int emitterSetCount;
        EmitterSet* emitterSets;
    };
}