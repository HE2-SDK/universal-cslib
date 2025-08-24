#pragma once

// reversed from files

namespace ucsl::resources::vibration::v21 {
    struct Sound { // unsure
        const char* name;
        char unk0; //soundChannelId?
        char unk1;
    };

    struct VibrationKeyframe {
        float time;
        float intensity;
        int flags;
        float unk1[2]; // maybe related to curve
        float unk2[2]; // maybe related to curve
    };

    struct VibrationMotor {
        char motorId;
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