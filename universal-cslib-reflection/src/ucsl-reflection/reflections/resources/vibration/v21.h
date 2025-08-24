#pragma once
#include <ucsl/resources/vibration/v21.h>
#include <simple-reflection/simple-reflection.h>
#include <ucsl-reflection/operators.h>
#include <ucsl-reflection/reflections/basic-types.h>

namespace ucsl::resources::vibration::v21::reflections {
	using namespace simplerfl;
	using namespace ucsl::reflection;

	namespace impl = ucsl::resources::vibration::v21;

    using Sound = structure<impl::Sound, "Sound", void,
        field<const char*, "name">,
        field<char, "unk0">,
        field<char, "unk1">
    >;

    using VibrationKeyframe = structure<impl::VibrationKeyframe, "VibrationKeyframe", void,
        field<float, "time">,
        field<float, "intensity">,
        field<int, "flags">,
        field<float[2], "unk1">,
        field<float[2], "unk2">
    >;

    using VibrationMotor = structure<impl::VibrationMotor, "VibrationMotor", void,
        field<char, "motorId">,
        field<unsigned int, "keyframeCount">,
        field<dynamic_carray<VibrationKeyframe, impl::VibrationMotor, [](const impl::VibrationMotor& parent) -> size_t { return parent.keyframeCount; }>*, "keyframes">
    >;

    using Vibration = structure<impl::Vibration, "Vibration", void,
        field<const char*, "name">,
        field<char, "unk0">,
        field<unsigned char, "motorCount">,
        field<bool, "looping">,
        field<bool, "unk1">,
        field<dynamic_carray<VibrationMotor, impl::Vibration, [](const impl::Vibration& parent) -> size_t { return parent.motorCount; }>*, "motors">,
        field<Sound*, "sound">,
        field<int64_t, "unk4">
    >;

    using VibData = structure<impl::VibData, "VibData", void,
        field<unsigned int, "magic">,
        field<unsigned int, "version">,
        field<unsigned int, "vibrationCount">,
        field<dynamic_carray<Vibration, impl::VibData, [](const impl::VibData& parent) -> size_t { return parent.vibrationCount; }>*, "vibrations">
    >;
}

namespace simplerfl {
	template<> struct canonical<ucsl::resources::vibration::v21::Sound> { using type = ucsl::resources::vibration::v21::reflections::Sound; };
	template<> struct canonical<ucsl::resources::vibration::v21::VibrationKeyframe> { using type = ucsl::resources::vibration::v21::reflections::VibrationKeyframe; };
	template<> struct canonical<ucsl::resources::vibration::v21::VibrationMotor> { using type = ucsl::resources::vibration::v21::reflections::VibrationMotor; };
	template<> struct canonical<ucsl::resources::vibration::v21::Vibration> { using type = ucsl::resources::vibration::v21::reflections::Vibration; };
	template<> struct canonical<ucsl::resources::vibration::v21::VibData> { using type = ucsl::resources::vibration::v21::reflections::VibData; };
}
