#pragma once

// reversed from files

namespace ucsl::resources::vibration::v21 {
    struct Sound { // unsure
        const char* name;
        char unk0; //soundChannelId?
        char unk1;
    };

    enum class KeyframeType : unsigned char {
        LINEAR,
        CONSTANT,
        HERMITE
    };

    struct VibrationKeyframe {
        float time;
        float intensity;
        char flags;
        KeyframeType type;
        float in[2];
        float out[2];
    };

    enum class MotorInterpolationType : unsigned char {
        LINEAR,
        TANGENT
    };

    enum class MotorPreType : unsigned char {
        CLAMP,
        EXTRAPOLATE
    };

    enum class MotorPostType : unsigned char {
        HOLD,
        EXTRAPOLATE
    };

    struct VibrationMotor {
        char motorId;
        MotorInterpolationType interpolationType;
        MotorPreType preType;
        MotorPostType postType;
        unsigned int keyframeCount;
        VibrationKeyframe* keyframes;
    };

    struct Vibration {
        const char* name;
        char unk0;
        unsigned char motorCount;
        bool looping;
        bool unk1;
        VibrationMotor* motors;
        Sound* sound;
        int64_t unk4;
    };

    struct VibData {
        unsigned int magic;
        unsigned int version;
        unsigned int vibrationCount;
        Vibration* vibrations;
    };
}