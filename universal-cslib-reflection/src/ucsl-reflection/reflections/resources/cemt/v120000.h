#pragma once
#include <ucsl/resources/cemt/v120000.h>
#include <simple-reflection/simple-reflection.h>
#include <ucsl-reflection/operators.h>
#include <ucsl-reflection/reflections/basic-types.h>

namespace ucsl::resources::cemt::v120000::reflections {
	using namespace simplerfl;
	using namespace ucsl::reflection;

	namespace impl = ::ucsl::resources::cemt::v120000;

	using JitteredValue = structure<impl::JitteredValue, "JitteredValue", void,
		field<float, "value">,
		field<float, "jitter">
	>;

	using JitteredVector3 = structure<impl::JitteredVector3, "JitteredVector3", void,
		field<JitteredValue, "x">,
		field<JitteredValue, "y">,
		field<JitteredValue, "z">
	>;

	using AnimationKeyframeParamInterpolationType = enumeration<impl::AnimationKeyframeParam::InterpolationType, "AnimationKeyframeParam::InterpolationType", unsigned short,
		option<"CONSTANT">,
		option<"LINEAR">,
		option<"HERMITE">
	>;

	using AnimationKeyframeParam = structure<impl::AnimationKeyframeParam, "AnimationKeyframeParam", void,
		field<unsigned short, "frame">,
		field<AnimationKeyframeParamInterpolationType, "interpolationType">,
		field<float, "derivativeIn">,
		field<float, "derivativeOut">,
		field<JitteredValue, "value">
	>;

	using AnimationTrackParam = structure<impl::AnimationTrackParam, "AnimationTrackParam", void,
		field<unsigned char, "index">,
		field<bitfield<unsigned char,
			component<bool, "disableRandomness", 0, 1>
		>, "randomnessFlag">,
		field<short, "keyframeCount">,
		field<dynamic_carray<AnimationKeyframeParam, field_resolver<short, "keyframeCount">>, "keyframes">
	>;

	using AnimationParamLoopType = enumeration<impl::AnimationParam::LoopType, "AnimationParam::LoopType", unsigned char,
		option<"NORMAL">,
		option<"BOUNCE">
	>;

	using AnimationParam = structure<impl::AnimationParam, "AnimationParam", void,
		field<bool, "useGlobalTime">,
		field<char, "unk1">,
		field<AnimationParamLoopType, "loopType">,
		field<bool, "timeScale">,
		field<int, "loopCount">,
		field<short, "totalFrameCount">,
		field<unsigned short, "trackCount">,
		field<AnimationTrackParam*[4], "tracks">
	>;

	using ChildEffectFlag = enumeration<impl::ChildEffect::Flag, "ChildEffect::Flag", unsigned char,
		option<"ENABLE_USING_ANIMATION">,
		option<"UNK1">,
		option<"INHERIT_ROTATION">
	>;

	using ChildEffect = structure<impl::ChildEffect, "ChildEffect", void,
		field<bitfield<unsigned char,
			component<bool, "enableUsingAnimation", 0, 1>,
			component<bool, "unk1", 1, 1>,
			component<bool, "inheritRotation", 2, 1>
		>, "flags">,
		field<char, "unkType">,
		field<unsigned char, "unk00">,
		field<float, "unk0">,
		field<float, "unk1">,
		field<float, "unk2">,
		field<float, "unk3">,
		field<float, "unk4">,
		field<AnimationParam*, "enableAnimation">,
		field<strbuf<128>, "name">,
		field<uint64_t, "param">
	>;

	using FieldParamType = enumeration<impl::FieldParam::Type, "FieldParam::Type", unsigned char,
		option<"GRAVITY">,
		option<"SPEED">,
		option<"MAGNET">,
		option<"NEWTON">,
		option<"VORTEX">,
		option<"SPIN">,
		option<"SPIN2">,
		option<"RANDOM">,
		option<"TAIL">,
		option<"FLUCTUATION">,
		option<"UNK0">
	>;

	using FieldParamOrigin = enumeration<impl::FieldParam::Origin, "FieldParam::Origin", unsigned char,
		option<"WORLD">,
		option<"EMISSION">,
		option<"INHERIT">,
		option<"UNK0">
	>;

	using FieldParamGravitySettings = structure<impl::FieldParam::GravitySettings, "FieldParam::GravitySettings", void,
		field<float, "scale">,
		field<math::Position, "rotation">,
		field<bool, "usesDirection">
	>;

	using FieldParamSpeedSettings = structure<impl::FieldParam::SpeedSettings, "FieldParam::SpeedSettings", void,
		field<float, "scale">
	>;

	using FieldParamMagnetSettings = structure<impl::FieldParam::MagnetSettings, "FieldParam::MagnetSettings", void,
		field<math::Position, "magnetPoint">,
		field<float, "scale">
	>;

	using FieldParamNewtonSettings = structure<impl::FieldParam::NewtonSettings, "FieldParam::NewtonSettings", void,
		field<math::Position, "targetPoint">,
		field<float, "force">,
		field<float, "falloffRadius">
	>;

	using FieldParamVortexSettings = structure<impl::FieldParam::VortexSettings, "FieldParam::VortexSettings", void,
		field<float, "max">,
		field<float, "min">,
		field<float, "radius">,
		field<math::Position, "rotation">,
		field<bool, "usesDirection">
	>;

	using FieldParamSpinSettings = structure<impl::FieldParam::SpinSettings, "FieldParam::SpinSettings", void,
		field<float, "scale">,
		field<math::Position, "rotation">,
		field<bool, "usesDirection">
	>;

	using FieldParamSpin2Settings = structure<impl::FieldParam::Spin2Settings, "FieldParam::Spin2Settings", void,
		field<float, "baseStrength">,
		field<float, "spinStrength">,
		field<float, "rotationAngle">,
		field<float, "axisFalloff">,
		field<math::Position, "axisVector">,
		field<bool, "useEulerRotation">
	>;

	using FieldParamRandomSettings = structure<impl::FieldParam::RandomSettings, "FieldParam::RandomSettings", void,
		field<float, "scale">,
		field<float, "spreadScale">,
		field<short, "updateInterval">,
		field<bitfield<unsigned short,
			component<bool, "randomizedScale", 0, 1>,
			component<bool, "x", 1, 1>,
			component<bool, "y", 2, 1>,
			component<bool, "z", 3, 1>
		>, "flags">,
		field<bool, "normalizedSpreadVector">,
		field<bool, "randomPerAxis">
	>;

	using FieldParamTailSettings = structure<impl::FieldParam::TailSettings, "FieldParam::TailSettings", void,
		field<float, "multiplier">
	>;

	using WaveformType = enumeration<impl::FieldParam::FluctuationSettings::WaveformType, "FieldParam::FluctuationSettings::WaveformType", unsigned char,
		option<"TRIANGLE">,
		option<"INVERSE_TRIANGLE">,
		option<"SAW">,
		option<"STEP">,
		option<"SINE">
	>;

	using FieldParamFluctuationSettings = structure<impl::FieldParam::FluctuationSettings, "FieldParam::FluctuationSettings", void,
		field<WaveformType, "waveformType">,
		field<bool, "unk2">,
		field<float, "baseOffset">,
		field<JitteredValue, "unk0">,
		field<JitteredValue, "unk1">
	>;

	using FieldParamUnkSettings = structure<impl::FieldParam::UnkSettings, "FieldParam::UnkSettings", void,
		field<float, "unk1">,
		field<float, "unk2">,
		field<bitfield<unsigned char,
			component<bool, "x", 0, 1>,
			component<bool, "y", 1, 1>,
			component<bool, "z", 2, 1>
		>, "axes">
	>;

	inline size_t get_field_param_settings_idx(const impl::FieldParam::Type& type) {
		switch (type) {
		case impl::FieldParam::Type::GRAVITY: return 0;
		case impl::FieldParam::Type::SPEED: return 1;
		case impl::FieldParam::Type::MAGNET: return 2;
		case impl::FieldParam::Type::NEWTON: return 3;
		case impl::FieldParam::Type::VORTEX: return 4;
		case impl::FieldParam::Type::SPIN: return 5;
		case impl::FieldParam::Type::SPIN2: return 6;
		case impl::FieldParam::Type::RANDOM: return 7;
		case impl::FieldParam::Type::TAIL: return 8;
		case impl::FieldParam::Type::FLUCTUATION: return 9;
		case impl::FieldParam::Type::UNK0: return 10;
		default: return 11;
		};
	}

	using FieldParamSettings = unionof<impl::FieldParam::Settings, "FieldParam::Settings", selector_resolver<size_t, field_resolver<impl::FieldParam::Type, "type">>::impl<get_field_param_settings_idx>,
		field<FieldParamGravitySettings, "gravity">,
		field<FieldParamSpeedSettings, "speed">,
		field<FieldParamMagnetSettings, "magnet">,
		field<FieldParamNewtonSettings, "newton">,
		field<FieldParamVortexSettings, "vortex">,
		field<FieldParamSpinSettings, "spin">,
		field<FieldParamSpin2Settings, "spin2">,
		field<FieldParamRandomSettings, "random">,
		field<FieldParamTailSettings, "tail">,
		field<FieldParamFluctuationSettings, "fluctuation">,
		field<FieldParamUnkSettings, "unk">,
		field<unsigned int[8], "maxSize">
	>;

	using FieldParam = structure<impl::FieldParam, "FieldParam", void,
		field<bool, "enabled">,
		field<FieldParamType, "type">,
		field<FieldParamOrigin, "origin">,
		field<char, "option2">,
		field<FieldParamSettings, "settings">
	>;

	using TextureParamDirectionFlag = enumeration<impl::TextureParam::DirectionFlag, "TextureParam::DirectionFlag", unsigned char,
		option<"INVERT_X">,
		option<"INVERT_Y">
	>;

	using TextureParamDirectionRandomizationFlag = enumeration<impl::TextureParam::DirectionRandomizationFlag, "TextureParam::DirectionRandomizationFlag", unsigned char,
		option<"RANDOMIZE_X">,
		option<"RANDOMIZE_Y">
	>;

	using TextureParamScrollColorParam = structure<impl::TextureParam::ScrollColorParam, "TextureParam::ScrollColorParam", void,
		field<math::Vector2, "scrollColor">,
		field<math::Vector2, "scrollStartJitter">
	>;

	using TextureParamUVAnimationParam = structure<impl::TextureParam::UVAnimationParam, "TextureParam::UVAnimationParam", void,
		field<math::Vector2, "scale">,
		field<float, "rotation">,
		field<math::Vector2, "translation">,
		field<AnimationParam*, "tilingAnimation">,
		field<AnimationParam*, "rotationAnimation">,
		field<AnimationParam*, "translationAnimation">
	>;

	using TextureParamPatternAnimationParamType = enumeration<impl::TextureParam::PatternAnimationParam::Type, "TextureParam::PatternAnimationParam::Type", unsigned char,
		option<"UNK0">,
		option<"UNK1">,
		option<"UNK2">,
		option<"UNK3">,
		option<"UNK4">,
		option<"UNK5">,
		option<"UNK6">,
		option<"UNK7">,
		option<"UNK8">,
		option<"UNK9">,
		option<"UNK10">
	>;

	using TextureParamPatternAnimationParam = structure<impl::TextureParam::PatternAnimationParam, "TextureParam::PatternAnimationParam", void,
		field<unsigned char, "startingFrame">,
		field<unsigned char, "stepCountX">,
		field<unsigned char, "stepCountY">,
		field<TextureParamPatternAnimationParamType, "type">,
		field<unsigned char, "frameRangeStart">,
		field<unsigned char, "frameRangeEnd">,
		field<unsigned char, "frameCount">,
		field<unsigned char, "repeating">,
		field<char, "unk6">,
		field<char, "unk6a">,
		field<bitfield<unsigned char,
			component<bool, "invertX", 0, 1>,
			component<bool, "invertY", 1, 1>
		>, "scrollDirectionFlags">,
		field<bitfield<unsigned char,
			component<bool, "randomizeX", 0, 1>,
			component<bool, "randomizeY", 1, 1>
		>, "scrollDirectionRandomizationFlags">,
		field<unsigned char, "unk9">,
		field<AnimationParam*, "patternAnimation">
	>;

	using TextureParam = structure<impl::TextureParam, "TextureParam", void,
		field<strbuf<128>, "name">,
		field<bitfield<unsigned int,
			component<bool, "textureType0", 0, 1>,
			component<bool, "textureType1", 1, 1>,
			component<bool, "textureType2", 2, 1>,
			component<bool, "textureType3", 3, 1>,
			component<bool, "textureType4", 4, 1>,
			component<bool, "textureType5", 5, 1>,
			component<bool, "textureType6", 6, 1>,
			component<bool, "textureType7", 7, 1>,
			component<bool, "textureType8", 8, 1>,
			component<bool, "textureType9", 9, 1>
		>, "textureFlags">,
		field<bitfield<unsigned int,
			component<bool, "animated", 0, 1>,
			component<bool, "unk1", 1, 1>,
			component<bool, "unk2", 2, 1>,
			component<bool, "stepwisePatternAnimation", 3, 1>,
			component<bool, "enableSecondaryScroll", 4, 1>,
			component<bool, "unk5", 5, 1>,
			component<bool, "randomizeStartingFrames", 6, 1>,
			component<bool, "enablePatternAnimation", 7, 1>,
			component<bool, "unk8", 8, 1>,
			component<bool, "scrollU", 9, 1>,
			component<bool, "scrollV", 10, 1>,
			component<bool, "unk11", 11, 1>,
			component<bool, "useMatricesOnTextureType0", 12, 1>,
			component<bool, "useMatricesOnTextureType1", 13, 1>,
			component<bool, "scaleU", 14, 1>,
			component<bool, "scaleV", 15, 1>
		>, "animationFlags">,
		field<TextureParamPatternAnimationParam, "patternAnimationParam">,
		field<TextureParamScrollColorParam[2], "scrollColorParams">,
		field<TextureParamUVAnimationParam[2], "uvAnimationParams">,
		field<math::Vector2, "uvScale">,
		field<int64_t, "unk22">
	>;

	using ColorRandomSet = structure<impl::ColorRandomSet, "ColorRandomSet", void,
		field<JitteredValue[3][32], "color1">,
		field<JitteredValue[3][32], "color2">,
		field<JitteredValue[32], "luminance1">,
		field<JitteredValue[32], "luminance2">,
		field<JitteredValue[32], "alpha1">,
		field<JitteredValue[32], "alpha2">,
		field<unsigned char, "color1Count">,
		field<unsigned char, "color2Count">,
		field<unsigned char, "luminance1Count">,
		field<unsigned char, "luminance2Count">,
		field<unsigned char, "alpha1Count">,
		field<unsigned char, "alpha2Count">
	>;

	using ColorParam = structure<impl::ColorParam, "ColorParam", void,
		field<JitteredValue[3], "color1">,
		field<JitteredValue, "alpha1">,
		field<JitteredValue[3], "color2">,
		field<JitteredValue, "alpha2">,
		field<JitteredValue, "luminance1">,
		field<JitteredValue, "luminance2">,
		field<AnimationParam*, "colorAnimation1">,
		field<AnimationParam*, "luminanceAnimation1">,
		field<AnimationParam*, "alphaAnimation1">,
		field<AnimationParam*, "colorAnimation2">,
		field<AnimationParam*, "luminanceAnimation2">,
		field<AnimationParam*, "alphaAnimation2">,
		field<ColorRandomSet, "colorRandomSet">,
		field<float, "unk7">,
		field<AnimationParam*, "unk7Animation">
	>;

	using BasicLODEffectParam = structure<impl::BasicLODEffectParam, "BasicLODEffectParam", void,
		field<strbuf<128>, "name">,
		field<float, "threshold">
	>;

	using BasicLODParamFlag = enumeration<impl::BasicLODParam::Flag, "BasicLODParam::Flag", unsigned char,
		option<"USE_NEGATIVE_ONE_UNK1864">
	>;

	using BasicLODParam = structure<impl::BasicLODParam, "BasicLODParam", void,
		field<unsigned char, "lodCount">,
		field<unsigned char, "flags">,
		field<BasicLODEffectParam[16], "lods">
	>;

	using RaycastLODEffectParam = structure<impl::RaycastLODEffectParam, "RaycastLODEffectParam", void,
		field<strbuf<128>, "name">,
		field<unsigned int, "raycastFlags">
	>;

	using RaycastLODParamRotationMode = enumeration<impl::RaycastLODParam::RotationMode, "RaycastLODParam::RotationMode", unsigned char,
		option<"DISABLE">,
		option<"ROTATE_TO_NORMAL">
	>;

	using RaycastLODParamFlag = enumeration<impl::RaycastLODParam::Flag, "RaycastLODParam::Flag", unsigned int,
		option<"UNK0">
	>;

	// 0x140FEBE70
	using RaycastLODParam = structure<impl::RaycastLODParam, "RaycastLODParam", void,
		field<float, "unkLifetime">,
		field<math::Position, "from">,
		field<math::Position, "to">,
		field<AnimationParam*, "fromAnimation">,
		field<AnimationParam*, "toAnimation">,
		field<RaycastLODParamRotationMode, "rotationMode">,
		field<RaycastLODParamRotationMode, "offsetRotationMode">,
		field<math::Position, "rotation">,
		field<math::Position, "offset">,
		field<AnimationParam*, "rotationAnimation">,
		field<AnimationParam*, "offsetAnimation">,
		field<unsigned int, "filterMask">,
		field<unsigned int, "flags">,
		field<unsigned char, "lodCount">,
		field<char[7], "gap12">,
		field<RaycastLODEffectParam[16], "lods">
	>;

	inline size_t get_lod_param_idx(const unsigned int& lodFlags) {
		if (lodFlags & 2)//lodFlags.test(impl::ElementParam::LODFlag::ANIMATED))
			return 1;
		else if (lodFlags & 1)//lodFlags.test(impl::ElementParam::LODFlag::BASIC))
			return 0;
		else
			return 0;
	}

	using LODParam = unionof<impl::LODParam, "LODParam", selector_resolver<size_t, field_resolver<unsigned int, "lodFlags">>::impl<get_lod_param_idx>,
		field<BasicLODParam, "basic">,
		field<RaycastLODParam, "raycast">
	>;

	using UserParameterType = enumeration<impl::UserParameter::Type, "UserParameter::Type", unsigned int,
		option<"UNK0">,
		option<"BOOL">,
		option<"INT">,
		option<"UINT">,
		option<"FLOAT">,
		option<"STRING">,
		option<"UNK1">,
		option<"VECTOR4">
	>;

	inline size_t get_user_parameter_data_ptr_idx(const impl::UserParameter::Type& type) {
		switch (type) {
		case impl::UserParameter::Type::UNK0: return 0;
		case impl::UserParameter::Type::BOOL: return 1;
		case impl::UserParameter::Type::INT: return 2;
		case impl::UserParameter::Type::UINT: return 3;
		case impl::UserParameter::Type::FLOAT: return 4;
		case impl::UserParameter::Type::STRING: return 5;
		case impl::UserParameter::Type::UNK1: return 0;
		case impl::UserParameter::Type::VECTOR4: return 6;
		default: return 0;
		}
	}

	using UserParameterDataPtr = unionof<impl::UserParameter::DataPtr, "UserParameter::DataPtr", selector_resolver<size_t, field_resolver<impl::UserParameter::Type, "type">>::impl<get_user_parameter_data_ptr_idx>,
		field<uint64_t, "unk0">,
		field<bool*, "boolean">,
		field<int*, "integer">,
		field<unsigned int*, "uinteger">,
		field<float*, "floating">,
		field<const char*, "string">,
		field<math::Position*, "vector">
	>;

	using UserParameter = structure<impl::UserParameter, "UserParameter", void,
		field<const char*, "name">,
		field<UserParameterType, "type">,
		field<unsigned int, "size">,
		field<UserParameterDataPtr, "data">
	>;

	using ElementParamLODFlag = enumeration<impl::ElementParam::LODFlag, "ElementParam::LODFlag", unsigned int,
		option<"BASIC">,
		option<"ANIMATED">
	>;

	using ElementParamParticleType = enumeration<impl::ElementParam::ParticleType, "ElementParam::ParticleType", unsigned char,
		option<"UNK0">,
		option<"UNK1">,
		option<"UNK2">,
		option<"UNK3">,
		option<"UNK4">,
		option<"UNK5_CPU">,
		option<"HISTORICAL_STRIPE">,
		option<"POINT_LIGHT">
	>;

	using ElementParamParticleUnkMode = enumeration<impl::ElementParam::ParticleUnkMode, "ElementParam::ParticleUnkMode", unsigned char,
		option<"UNK0">,
		option<"UNK1">
	>;

	using ElementParamUnk0ParticleParam = structure<impl::ElementParam::Unk0ParticleParam, "ElementParam::Unk0ParticleParam", void,
		field<unsigned int, "unk0">,
		field<float, "unk1">
	>;

	using ElementParamUnk1ParticleParam = structure<impl::ElementParam::Unk1ParticleParam, "ElementParam::Unk1ParticleParam", void,
		field<unsigned int, "unk0">,
		field<float, "unk1">
	>;

	using ElementParamUnk2ParticleParam = structure<impl::ElementParam::Unk2ParticleParam, "ElementParam::Unk2ParticleParam", void,
		field<unsigned int, "type">,
		field<char[4], "pad">,
		field<unsigned int, "unk0">,
		field<float, "unk1">
	>;

	using ElementParamUnk3ParticleParam = structure<impl::ElementParam::Unk3ParticleParam, "ElementParam::Unk3ParticleParam", void,
		field<unsigned int, "type">,
		field<bool, "unk2">,
		field<char[3], "pad">,
		field<unsigned int, "unk0">,
		field<float, "unk1">
	>;

	using ElementParamUnk4ParticleParam = structure<impl::ElementParam::Unk4ParticleParam, "ElementParam::Unk4ParticleParam", void,
		field<unsigned int, "type">,
		field<unsigned int, "unk3">,
		field<char, "pad1">,
		field<unsigned char, "unk2">,
		field<unsigned char, "gap3">,
		field<unsigned int, "unk0">,
		field<float, "unk1">
	>;

	using ElementParamUnk5ParticleParam = structure<impl::ElementParam::Unk5ParticleParam, "ElementParam::Unk5ParticleParam", void,
		field<unsigned int, "type">,
		field<unsigned char, "unk3">,
		field<char[3], "pad">,
		field<unsigned int, "unk1">,
		field<unsigned char, "unk4">,
		field<unsigned char, "unk5">,
		field<unsigned char, "unk6">,
		field<bool, "unk2">,
		field<char[0x8], "pad2">
	>;

	using ElementParamHistoricalStripeParticleParam = structure<impl::ElementParam::HistoricalStripeParticleParam, "ElementParam::HistoricalStripeParticleParam", void,
		field<unsigned int, "type">,
		field<unsigned int, "unkStride">,
		field<unsigned char, "unk1">,
		field<char[0x3], "pad1">,
		field<unsigned char, "unk4">,
		field<unsigned char, "unk3">,
		field<bool, "unk2">,
		field<char, "pad2">,
		field<unsigned int, "unkCount">,
		field<char[0x4], "pad3">
	>;

	using ElementParamPointLightParticleParam = structure<impl::ElementParam::PointLightParticleParam, "ElementParam::PointLightParticleParam", void,
		field<char[0x18], "pad">
	>;

	inline size_t get_element_param_particle_param_idx(const impl::ElementParam::ParticleType& particleType) {
		switch (particleType) {
		default: return 7;
		}
	}

	using ElementParamParticleParam = unionof<impl::ElementParam::ParticleParam, "ElementParam::ParticleParam", selector_resolver<size_t, field_resolver<impl::ElementParam::ParticleType, "particleType">>::impl<get_element_param_particle_param_idx>,
		field<ElementParamUnk0ParticleParam, "unk0">,
		field<ElementParamUnk1ParticleParam, "unk1">,
		field<ElementParamUnk2ParticleParam, "unk2">,
		field<ElementParamUnk3ParticleParam, "unk3">,
		field<ElementParamUnk4ParticleParam, "unk4">,
		field<ElementParamUnk5ParticleParam, "unk5">,
		field<ElementParamHistoricalStripeParticleParam, "historicalStripe">,
		field<ElementParamPointLightParticleParam, "pointLight">
	>;

	using ElementParamGpuParticleFlag = enumeration<impl::ElementParam::GpuParticleFlag, "ElementParam::GpuParticleFlag", unsigned int,
		option<"IS_GPU">,
		option<"UNK1">,
		option<"UNK2">
	>;

	using ElementParamTransformParamRandomAngleFlag = enumeration<impl::ElementParam::TransformParam::RandomAngleFlag, "ElementParam::TransformParam::RandomAngleFlag", unsigned char,
		option<"CCW_90">,
		option<"CW_90">,
		option<"CW_180">,
		option<"CW_0">
	>;

	using ElementParamTransformParam = structure<impl::ElementParam::TransformParam, "ElementParam::TransformParam", void,
		field<JitteredVector3, "rotation">,
		field<unsigned int, "rotationFlags">,
		field<JitteredVector3, "randomizedDirectionRotation">,
		field<unsigned int, "randomizedDirectionRotationFlags">,
		field<unsigned char[3], "rotationRandomInitialAngleFlags">,
		field<JitteredVector3, "angularVelocity">,
		field<unsigned int, "angularVelocityFlags">,
		field<JitteredVector3, "size">,
		field<unsigned int, "sizeFlags">,
		field<JitteredVector3, "scale">,
		field<unsigned int, "scaleFlags">
	>;

	using ElementParamUserParameterParam = structure<impl::ElementParam::UserParameterParam, "ElementParam::UserParameterParam", void,
		field<unsigned int, "userParameterCount">,
		field<dynamic_carray<UserParameter, field_resolver<unsigned int, "userParameterCount">>*, "userParameters">
	>;

	using ElementParamUpdateFlag = enumeration<impl::ElementParam::UpdateFlag, "ElementParam::UpdateFlag", unsigned int,
		option<"HAS_CHILDREN">,
		option<"HAS_RANDOMIZED_ROTATION_DIRECTION">,
		option<"HAS_FIXED_ANGULAR_VELOCITY">,
		option<"USE_GLOBAL_TIME">,
		option<"HAS_MODEL">,
		option<"UNK5">,
		option<"UNK6">,
		option<"UNK7">,
		option<"UNK8">,
		option<"UNK9">,
		option<"UNK10">,
		option<"DISABLE_ANIMATIONS">,
		option<"UNK12">,
		option<"DISABLE_FIELDS">,
		option<"UNK14">
	>;

	using ElementParamModelParam = structure<impl::ElementParam::ModelParam, "ElementParam::ModelParam", void,
		field<bool, "hasModel">,
		field<unsigned char, "modelSetting1">,
		field<strbuf<128>, "gap6b">,
		field<strbuf<128>, "modelName">,
		field<strbuf<128>, "skeletonName">,
		field<strbuf<128>, "nodeAnimName">
	>;

	using ElementParamDistanceScaleParam = structure<impl::ElementParam::DistanceScaleParam, "ElementParam::DistanceScaleParam", void,
		field<float, "min">,
		field<float, "max">,
		field<float, "multiplier">
	>;

	using ElementParam = structure<impl::ElementParam, "ElementParam", void,
		field<ElementParamTransformParam, "transformParam">,
		field<ColorParam[2], "colorParams">,
		field<JitteredValue, "lifetime">,
		field<AnimationParam*, "rotationAnimation">,
		field<AnimationParam*, "scaleAnimation">,
		field<AnimationParam*, "sizeAnimation">,
		field<char[32], "gap6aa">,
		field<ElementParamDistanceScaleParam, "distanceScaleParam">,
		field<ElementParamModelParam, "modelParam">,
		field<ElementParamParticleType, "particleType">,
		field<ElementParamParticleUnkMode, "particleUnkMode">,
		field<ElementParamParticleParam, "particleParam">,
		field<float, "unkScale">,
		field<TextureParam[4], "textures">,
		field<unsigned int, "textureCount">,
		field<ChildEffect[16], "childEffects">,
		field<FieldParam[8], "fields">,
		field<AnimationParam*[8][5], "fieldAnimations">,
		field<unsigned int, "updateFlags">,
		field<AnimationParam*, "uvAnimation">,
		field<unsigned int, "elementAnimationCount">,
		field<char[0x34], "gap7bb">,
		field<unsigned int, "gpuParticleFlags">,
		field<strbuf<128>, "vectorFieldName">,
		field<math::Position, "vectorFieldSize">,
		field<char[0x30], "gap7c2">,
		field<math::Position, "unkVector2">,
		field<math::Position, "vectorFieldParam">,
		field<math::Position, "unkVector1">,
		field<math::Position, "depthCollision">,
		field<char[8], "gap453543">,
		field<ElementParamUserParameterParam, "userParameterParam">,
		field<unsigned int, "emitterAnimationCount">,
		field<unsigned int, "unk7e">,
		field<uint64_t, "gpuElementParamTexture">,
		field<uint64_t, "gpuOtherTexture">,
		field<unsigned int, "lodFlags">,
		field<LODParam, "lodParam">
	>;

	using EmitterParamTransformParamTransformType = enumeration<impl::EmitterParam::TransformParam::TransformType, "EmitterParam::TransformParam::TransformType", unsigned int,
		option<"UNK0">,
		option<"UNK1">,
		option<"ROTATED">
	>;

	using EmitterParamTransformParam = structure<impl::EmitterParam::TransformParam, "EmitterParam::TransformParam", void,
		field<math::Position, "position">,
		field<math::Position, "rotation">,
		field<math::Position, "scale">,
		field<EmitterParamTransformParamTransformType, "type">,
		field<AnimationParam*, "positionAnimation">,
		field<AnimationParam*, "rotationAnimation">,
		field<AnimationParam*, "scaleAnimation">
	>;

	using EmitterParamEmissionParamShape = enumeration<impl::EmitterParam::EmissionParam::Shape, "EmitterParam::EmissionParam::Shape", unsigned int,
		option<"POINT">,
		option<"SPHERE">,
		option<"DISC">,
		option<"CYLINDER">,
		option<"LINE">,
		option<"TORUS">,
		option<"CUBE">,
		option<"MESH">,
		option<"FIXED">
	>;

	using EmitterParamEmissionParam = structure<impl::EmitterParam::EmissionParam, "EmitterParam::EmissionParam", void,
		field<EmitterParamEmissionParamShape, "shape">,
		field<math::Position, "positionJitter">,
		field<float, "spread">,
		field<float, "startAngle">,
		field<float, "endAngle">,
		field<bool, "useRadialDistribution">,
		field<bool, "consistentAngle">,
		field<bool, "useSubdivisions">,
		field<unsigned char, "subdivisionCount">,
		field<AnimationParam*, "positionJitterAnimation">,
		field<AnimationParam*, "spreadAnimation">,
		field<AnimationParam*, "startAngleAnimation">,
		field<AnimationParam*, "endAngleAnimation">
	>;

	using EmitterParamEmitParamEmitMode = enumeration<impl::EmitterParam::EmitParam::EmitMode, "EmitterParam::EmitParam::EmitMode", unsigned int,
		option<"CONTINUOUS">,
		option<"INTERVAL">
	>;

	using EmitterParamEmitParamAttenuationMode = enumeration<impl::EmitterParam::EmitParam::AttenuationMode, "EmitterParam::EmitParam::AttenuationMode", unsigned char,
		option<"FIXED">,
		option<"POSE_POSITION">
	>;

	using EmitterParamEmitParam = structure<impl::EmitterParam::EmitParam, "EmitterParam::EmitParam", void,
		field<EmitterParamEmitParamEmitMode, "emissionMode">,
		field<JitteredValue, "emitInterval">,
		field<JitteredValue, "emissionCount">,
		field<EmitterParamEmitParamAttenuationMode, "attenuationMode">,
		field<float, "minDistance">,
		field<float, "maxDistance">,
		field<float, "distanceAttenuation">,
		field<float, "duration">,
		field<float, "startDelay">,
		field<float, "fadeSpeed">,
		field<AnimationParam*, "emissionCountAnimation">
	>;

	using EmitterParamDeathParamDeathMode = enumeration<impl::EmitterParam::DeathParam::DeathMode, "EmitterParam::DeathParam::DeathMode", unsigned int,
		option<"WAIT_FOR_ELEMENT">,
		option<"KILL_AFTER_DELAY">,
		option<"WAIT_FOR_ELEMENTS_OR_KILL_AFTER_DELAY">,
		option<"IMMEDIATE">
	>;

	using EmitterParamDeathParam = structure<impl::EmitterParam::DeathParam, "EmitterParam::DeathParam", void,
		field<EmitterParamDeathParamDeathMode, "mode">,
		field<float, "killDelay">
	>;

	using EmitterParamEmissionDynamicsParamSpreadDirectionType = enumeration<impl::EmitterParam::EmissionDynamicsParam::Settings::SpreadDirectionType, "EmitterParam::EmissionDynamicsParam::Settings::SpreadDirectionType", unsigned int,
		option<"DIRECTION">,
		option<"ANGLES">
	>;
	
	using EmitterParamEmissionDynamicsParamSettings = structure<impl::EmitterParam::EmissionDynamicsParam::Settings, "EmitterParam::EmissionDynamicsParam::Settings", void,
		field<float, "direction1Velocity">,
		field<float, "direction2Velocity">,
		field<float, "randomDirectionVelocity">,
		field<float, "localSpreadVelocity">,
		field<float, "localSpread">,
		field<float, "worldSpreadVelocity">,
		field<float, "worldSpread">,
		field<EmitterParamEmissionDynamicsParamSpreadDirectionType, "worldSpreadDirectionType">,
		field<math::Position, "worldSpreadAnglesOrDirection">,
		field<float, "initialVelocityJitter">,
		field<float, "velocityJitter">
	>;

	using EmitterParamEmissionDynamicsParam = structure<impl::EmitterParam::EmissionDynamicsParam, "EmitterParam::EmissionDynamicsParam", void,
		field<EmitterParamEmissionDynamicsParamSettings, "settings">,
		field<AnimationParam*, "direction1VelocityAnimation">,
		field<AnimationParam*, "direction2VelocityAnimation">,
		field<AnimationParam*, "localSpreadVelocityAnimation">,
		field<AnimationParam*, "localSpreadAnimation">,
		field<AnimationParam*, "worldSpreadVelocityAnimation">,
		field<AnimationParam*, "worldSpreadAnimation">,
		field<AnimationParam*, "worldSpreadAnglesOrDirectionAnimation">
	>;

	using EmitterParamInheritFlag = enumeration<impl::EmitterParam::InheritFlag, "EmitterParam::InheritFlag", unsigned int,
		option<"TRANSLATION_RAW">,
		option<"ROTATION">,
		option<"TRANSLATION">,
		option<"TRANSLATION_RELATIVE">,
		option<"INHERIT_DIRECT ">
	>;

	using EmitterParamUnkFlags = enumeration<impl::EmitterParam::UnkFlags, "EmitterParam::UnkFlags", unsigned int,
		option<"UNK0">,
		option<"UNK1">,
		option<"UNK2">,
		option<"UNK3">,
		option<"USE_TEXTURES">,
		option<"USE_RENDER_PRIORITY">,
		option<"UNK6">,
		option<"USE_UNKFLOAT0_RANGE">,
		option<"USE_UNKFLOAT1_RANGE">,
		option<"UNK9">,
		option<"UNK10">,
		option<"UNK11">
	>;

	using EmitterParamCullMode = enumeration<impl::EmitterParam::CullMode, "EmitterParam::CullMode", unsigned int,
		option<"NONE">,
		option<"FRONT">,
		option<"BACK">
	>;

	using EmitterParamMaskType1 = enumeration<impl::EmitterParam::MaskType1, "EmitterParam::MaskType1", unsigned int,
		option<"UNK0">,
		option<"UNK1">,
		option<"UNK2">,
		option<"UNK3">
	>;

	using EmitterParamMaskType2 = enumeration<impl::EmitterParam::MaskType2, "EmitterParam::MaskType2", unsigned int,
		option<"UNK0">,
		option<"UNK1">,
		option<"UNK2">,
		option<"UNK3">,
		option<"UNK4">
	>;

	using EmitterParamMaskType3 = enumeration<impl::EmitterParam::MaskType3, "EmitterParam::MaskType3", unsigned int,
		option<"UNK0">,
		option<"UNK1">,
		option<"UNK2">
	>;

	using EmitterParamDepthMode = enumeration<impl::EmitterParam::DepthMode, "EmitterParam::DepthMode", unsigned int,
		option<"BLENDING_ENABLED_WITH_DEPTH_TEST">,
		option<"BLENDING_ENABLED">,
		option<"BLENDING_DISABLED_WITH_DEPTH_TEST_AND_DEPTH_WRITE">,
		option<"BLENDING_DISABLED_WITH_DEPTH_TEST_AND_DEPTH_WRITE_2">,
		option<"BLENDING_ENABLED_WITH_DEPTH_TEST_AND_DEPTH_WRITE">
	>;

	using EmitterParamBlendMode = enumeration<impl::EmitterParam::BlendMode, "EmitterParam::BlendMode", unsigned int,
		option<"MIX">,
		option<"ADD">,
		option<"MULTIPLY">,
		option<"SUBTRACT">
	>;

	using EmitterParamBillboardType = enumeration<impl::EmitterParam::BillboardType, "EmitterParam::BillboardType", unsigned char,
		option<"UNK0">,
		option<"UNK1">,
		option<"UNK2">
	>;

	using EmitterParamUnk0ParamFlag = enumeration<impl::EmitterParam::Unk0Param::Flag, "EmitterParam::Unk0Param::Flag", unsigned char,
		option<"UNK0">,
		option<"UNK1">
	>;

	using EmitterParamUnk0ParamMode = enumeration<impl::EmitterParam::Unk0Param::Mode, "EmitterParam::Unk0Param::Mode", unsigned char,
		option<"ANISOTROPIC">,
		option<"ISOTROPIC">
	>;

	using EmitterParamUnk0Param = structure<impl::EmitterParam::Unk0Param, "EmitterParam::Unk0Param", void,
		field<unsigned char, "flags">,
		field<EmitterParamUnk0ParamMode, "mode">,
		field<math::Position, "position">,
		field<math::Position, "scale">
	>;

	using EmitterParam = structure<impl::EmitterParam, "EmitterParam", void,
		field<EmitterParamTransformParam, "transformParam">,
		field<EmitterParamEmissionParam, "emissionParam">,
		field<EmitterParamEmitParam, "emitParam">,
		field<EmitterParamDeathParam, "deathParam">,
		field<EmitterParamEmissionDynamicsParam, "emissionDynamicsParam">,
		field<EmitterParamBlendMode, "blendMode">,
		field<EmitterParamDepthMode, "depthMode">,
		field<EmitterParamCullMode, "cullMode">,
		field<unsigned int, "inheritTransformFlags">,
		field<float, "inheritTransformRatio">,
		field<unsigned int, "inheritPoseFlags">,
		field<float, "inheritPoseRatio">,
		field<unsigned int, "unkFlags">,
		field<float, "unkFloat4">,
		field<float, "unkFloat5">,
		field<float, "unkFloat0RangeStart">,
		field<float, "unkFloat0RangeEnd">,
		field<float, "unkFloat1RangeStart">,
		field<float, "unkFloat1RangeEnd">,
		field<float, "unkFloat2RangeStart">,
		field<float, "unkFloat2RangeEnd">,
		field<float, "unkFloat6RangeEnd">,
		field<float, "unkFloat6RangeStart">,
		field<unsigned int, "gap6b">,
		field<bool, "unk6c">,
		field<float, "unkFloatFoo0">,
		field<float, "unkFloatFoo1">,
		field<float, "unkFloatFoo2">,
		field<unsigned int, "randomSeed">,
		field<char, "renderLayer">,
		field<unsigned char, "unk6b2">,
		field<unsigned char, "unk6b3">,
		field<unsigned char, "unk6b4">,
		field<unsigned char, "unkType5">,
		field<EmitterParamBillboardType, "billboardType">,
		field<bool, "enableUnkFloat2Range">,
		field<char, "gap6b2">,
		field<unsigned char, "unkAlpha">,
		field<EmitterParamUnk0Param, "unk0Param">,
		field<unsigned int, "gap6aa2">,
		field<EmitterParamMaskType1, "unkType0">,
		field<unsigned int, "unkType0a">,
		field<unsigned int, "gap6aa1a">,
		field<EmitterParamMaskType2, "unkType0c">,
		field<unsigned int, "unkType0d">,
		field<EmitterParamMaskType3, "unkType1">,
		field<EmitterParamMaskType1, "unkType1a">,
		field<unsigned int, "unkType1b">,
		field<EmitterParamMaskType3, "unkType2">,
		field<EmitterParamMaskType2, "unkType3">,
		field<unsigned int, "unkType4">,
		field<ElementParam, "elementParam">
	>;

	using EffectParam = structure<impl::EffectParam, "EffectParam", void,
		field<char[3], "magic">,
		field<bool, "resolved">,
		field<unsigned char, "majorVersion">,
		field<unsigned char, "minorVersion">,
		field<unsigned char, "patchVersion">,
		field<unsigned char, "buildVersion">,
		field<strbuf<128>, "name">,
		field<unsigned int, "animationBufferSize">,
		field<unsigned int, "emitterCount">,
		field<EmitterParam, "emitterParam">
	>;
}

namespace simplerfl {
	template<> struct canonical<ucsl::resources::cemt::v120000::EffectParam> { using type = ucsl::resources::cemt::v120000::reflections::EffectParam; };
}
