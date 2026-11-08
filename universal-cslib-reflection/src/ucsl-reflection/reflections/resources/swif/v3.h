#pragma once
#include <cassert>
#include <ucsl/resources/swif/v3.h>
#include <simple-reflection/simple-reflection.h>
#include <ucsl-reflection/operators.h>
#include <ucsl-reflection/reflections/basic-types.h>

namespace ucsl::resources::swif::v3::reflections {
	using namespace simplerfl;
	using namespace ucsl::reflection;

	namespace impl = ::ucsl::resources::swif::v3;

	using Vector2 = math::Vector2;
	using Vector3 = math::Position;
	using Matrix34 = math::Matrix34;
	using Matrix44 = math::Matrix44;
	using Rotation3 = structure<impl::Rotation3, "Rotation3", void,
		field<int, "x">,
		field<int, "y">,
		field<int, "z">
	>;

	using Color = colors::Color8RGBA;

	// User data
	using EDataType = enumeration<impl::SRS_DATA::Type, "EDataType", unsigned int,
		option<"BOOL">,
		option<"INT">,
		option<"UINT">,
		option<"FLOAT">,
		option<"UNK">,
		option<"STRING">
	>;

	inline size_t get_data_value_ptr_idx(const impl::SRS_DATA::Type& type) {
		switch (type) {
		case impl::SRS_DATA::Type::BOOL: return 0;
		case impl::SRS_DATA::Type::INT: return 1;
		case impl::SRS_DATA::Type::UINT: return 2;
		case impl::SRS_DATA::Type::FLOAT: return 3;
		case impl::SRS_DATA::Type::STRING: return 4;
		default: assert(false && "Unknown data type"); return 0;
		}
	}

	using SRS_DATA_VALUE_PTR = unionof<impl::SRS_DATA_VALUE_PTR, "SRS_CASTNODE_PTR", selector_resolver<size_t, field_resolver<impl::SRS_DATA::Type, "type">>::impl<get_data_value_ptr_idx>,
		field<bool*, "b">,
		field<int*, "i32">,
		field<unsigned int*, "u32">,
		field<float*, "float">,
		field<const char*, "u32">
	>;

	using SRS_DATA = structure<impl::SRS_DATA, "SRS_DATA", void,
		field<const char*, "name">,
		field<EDataType, "type">,
		field<SRS_DATA_VALUE_PTR, "value">
	>;

	using SRS_USERDATA = structure<impl::SRS_USERDATA, "SRS_USERDATA", void,
		field<unsigned int, "count">,
		field<dynamic_carray<SRS_DATA, field_resolver<unsigned int, "count">>*, "items">
	>;

	using ECurveType = enumeration<impl::ECurveType, "ECurveType", unsigned short,
		option<"TranslationX">,
		option<"TranslationY">,
		option<"TranslationZ">,
		option<"RotationX">,
		option<"RotationY">,
		option<"RotationZ">,
		option<"ScaleX">,
		option<"ScaleY">,
		option<"ScaleZ">,
		option<"MaterialColor">,
		option<"Display">,
		option<"Width">,
		option<"Height">,
		option<"VertexColorTopLeft">,
		option<"VertexColorTopRight">,
		option<"VertexColorBottomLeft">,
		option<"VertexColorBottomRight">,
		option<"CropIndex0">,
		option<"CropIndex1">,
		option<"Unknown">,
		option<"IlluminationColor">,
		option<"MaterialColorR">,
		option<"MaterialColorG">,
		option<"MaterialColorB">,
		option<"MaterialColorA">,
		option<"VertexColorTopLeftR">,
		option<"VertexColorTopLeftG">,
		option<"VertexColorTopLeftB">,
		option<"VertexColorTopLeftA">,
		option<"VertexColorTopRightR">,
		option<"VertexColorTopRightG">,
		option<"VertexColorTopRightB">,
		option<"VertexColorTopRightA">,
		option<"VertexColorBottomLeftR">,
		option<"VertexColorBottomLeftG">,
		option<"VertexColorBottomLeftB">,
		option<"VertexColorBottomLeftA">,
		option<"VertexColorBottomRightR">,
		option<"VertexColorBottomRightG">,
		option<"VertexColorBottomRightB">,
		option<"VertexColorBottomRightA">,
		option<"IlluminationColorR">,
		option<"IlluminationColorG">,
		option<"IlluminationColorB">,
		option<"IlluminationColorA">
	>;

	using EInterpolationType = enumeration<impl::EInterpolationType, "EInterpolationType", unsigned short,
		option<"CONSTANT">,
		option<"LINEAR">,
		option<"HERMITE">,
		option<"INDIVIDUAL">
	>;

	using ETrackDataType = enumeration<impl::ETrackDataType, "ETrackDataType", unsigned int,
		option<"UNKNOWN">,
		option<"FLOAT">,
		option<"INDEX">,
		option<"BOOL">,
		option<"INT">,
		option<"COLOR">,
		option<"UNK_FLOAT">,
		option<"UNK_DOUBLE">,
		option<"UNK_CHAR">
	>;

	using ETrackRepeatType = enumeration < impl::ETrackRepeatType, "ETrackRepeatType", unsigned int,
		option<"ONCE">,
		option<"REPEAT">
	>;

	using SRS_KEYFRAME = structure<impl::SRS_KEYFRAME, "SRS_KEYFRAME", void,
		field<unsigned int, "frame">
	>;

	template<typename T>
	using Key = structure<impl::Key<T>, "Key", SRS_KEYFRAME,
		field<T, "value">
	>;

	template<typename T>
	using KeyLinear = structure<impl::KeyLinear<T>, "KeyLinear", Key<T>>;

	template<typename T>
	using KeyHermite = structure<impl::KeyHermite<T>, "KeyHermite", KeyLinear<T>,
		field<T, "derivativeIn">,
		field<T, "derivativeOut">
	>;

	template<typename T>
	using KeyIndividual = structure<impl::KeyIndividual<T>, "KeyIndividual", KeyHermite<T>,
		field<EInterpolationType, "interpolationType">
	>;

	inline size_t get_keyframe_ptr_idx(const unsigned int& flags) {
		switch (static_cast<impl::EInterpolationType>(flags & 0x3)) {
		case impl::EInterpolationType::CONSTANT:
			switch (static_cast<impl::ETrackDataType>((flags >> 4) & 0xF)) {
			case impl::ETrackDataType::FLOAT: return 0;
			case impl::ETrackDataType::INDEX: return 1;
			case impl::ETrackDataType::INT: return 2;
			case impl::ETrackDataType::BOOL: return 3;
			case impl::ETrackDataType::COLOR: return 4;
			default: assert(false && "Invalid track flags"); return 0;
			}
		case impl::EInterpolationType::LINEAR:
			switch (static_cast<impl::ETrackDataType>((flags >> 4) & 0xF)) {
			case impl::ETrackDataType::FLOAT: return 5;
			case impl::ETrackDataType::INDEX: return 6;
			case impl::ETrackDataType::INT: return 7;
			case impl::ETrackDataType::BOOL: return 8;
			case impl::ETrackDataType::COLOR: return 9;
			default: assert(false && "Invalid track flags"); return 0;
			}
		case impl::EInterpolationType::HERMITE:
			switch (static_cast<impl::ETrackDataType>((flags >> 4) & 0xF)) {
			case impl::ETrackDataType::FLOAT: return 10;
			case impl::ETrackDataType::INDEX: return 11;
			case impl::ETrackDataType::INT: return 12;
			case impl::ETrackDataType::BOOL: return 13;
			case impl::ETrackDataType::COLOR: return 14;
			default: assert(false && "Invalid track flags"); return 0;
			}
		case impl::EInterpolationType::INDIVIDUAL:
			switch (static_cast<impl::ETrackDataType>((flags >> 4) & 0xF)) {
			case impl::ETrackDataType::FLOAT: return 15;
			case impl::ETrackDataType::INDEX: return 16;
			case impl::ETrackDataType::INT: return 17;
			case impl::ETrackDataType::BOOL: return 18;
			case impl::ETrackDataType::COLOR: return 19;
			default: assert(false && "Invalid track flags"); return 0;
			}
		default: assert(false && "Invalid track flags"); return 0;
		}
	}

	using SRS_KEYFRAME_PTR = unionof<impl::SRS_KEYFRAME_PTR, "SRS_KEYFRAME_PTR", selector_resolver<size_t, field_resolver<unsigned int, "flags">>::impl<get_keyframe_ptr_idx>,
        field<dynamic_carray<Key<float>, field_resolver<unsigned short, "keyCount">>*, "constantFloat">,
        field<dynamic_carray<Key<int>, field_resolver<unsigned short, "keyCount">>*, "constantIndex">,
        field<dynamic_carray<Key<int>, field_resolver<unsigned short, "keyCount">>*, "constantInt">,
        field<dynamic_carray<Key<bool>, field_resolver<unsigned short, "keyCount">>*, "constantBool">,
        field<dynamic_carray<Key<Color>, field_resolver<unsigned short, "keyCount">>*, "constantColor">,

        field<dynamic_carray<KeyLinear<float>, field_resolver<unsigned short, "keyCount">>*, "linearFloat">,
        field<dynamic_carray<KeyLinear<int>, field_resolver<unsigned short, "keyCount">>*, "linearIndex">,
        field<dynamic_carray<KeyLinear<int>, field_resolver<unsigned short, "keyCount">>*, "linearInt">,
        field<dynamic_carray<KeyLinear<bool>, field_resolver<unsigned short, "keyCount">>*, "linearBool">,
        field<dynamic_carray<KeyLinear<Color>, field_resolver<unsigned short, "keyCount">>*, "linearColor">,

        field<dynamic_carray<KeyHermite<float>, field_resolver<unsigned short, "keyCount">>*, "hermiteFloat">,
        field<dynamic_carray<KeyHermite<int>, field_resolver<unsigned short, "keyCount">>*, "hermiteIndex">,
        field<dynamic_carray<KeyHermite<int>, field_resolver<unsigned short, "keyCount">>*, "hermiteInt">,
        field<dynamic_carray<KeyHermite<bool>, field_resolver<unsigned short, "keyCount">>*, "hermiteBool">,
        field<dynamic_carray<KeyHermite<Color>, field_resolver<unsigned short, "keyCount">>*, "hermiteColor">,

        field<dynamic_carray<KeyIndividual<float>, field_resolver<unsigned short, "keyCount">>*, "individualFloat">,
        field<dynamic_carray<KeyIndividual<int>, field_resolver<unsigned short, "keyCount">>*, "individualIndex">,
        field<dynamic_carray<KeyIndividual<int>, field_resolver<unsigned short, "keyCount">>*, "individualInt">,
        field<dynamic_carray<KeyIndividual<bool>, field_resolver<unsigned short, "keyCount">>*, "individualBool">,
        field<dynamic_carray<KeyIndividual<Color>, field_resolver<unsigned short, "keyCount">>*, "individualColor">
	>;

	using SRS_TRACK = structure<impl::SRS_TRACK, "SRS_TRACK", void,
		field<ECurveType, "trackType">,
		field<unsigned short, "keyCount">,
		field<bitfield<unsigned int,
			component<EInterpolationType, "interpolationType", 0, 2>,
			component<ETrackDataType, "dataType", 4, 4>,
			component<ETrackRepeatType, "repeatType", 8, 2>
		>, "flags">,
		field<unsigned int, "firstFrame">,
		field<unsigned int, "lastFrame">,
		field<SRS_KEYFRAME_PTR, "keyFrames">
	>;

	using SRS_MOTION = structure<impl::SRS_MOTION, "SRS_MOTION", void,
		field<unsigned short, "castId">,
		field<unsigned short, "trackCount">,
		field<dynamic_carray<SRS_TRACK, field_resolver<unsigned short, "trackCount">>*, "tracks">
	>;

	using SRS_ANIMATION = structure<impl::SRS_ANIMATION, "SRS_ANIMATION", void,
		field<const char*, "name">,
		field<unsigned int, "id">,
		field<unsigned int, "motionCount">,
		field<unsigned int, "frameCount">,
		field<dynamic_carray<SRS_MOTION, field_resolver<unsigned int, "motionCount">>*, "motions">,
		field<SRS_USERDATA*, "userData">,
		field<bool, "isLooping">
	>;

	// Crops
	using SRS_CROP = structure<impl::SRS_CROP, "SRS_CROP", void,
		field<float, "left">,
		field<float, "top">,
		field<float, "right">,
		field<float, "bottom">
	>;

	using SRS_CROPREF = structure<impl::SRS_CROPREF, "SRS_CROPREF", void,
		field<short, "textureListIndex">,
		field<short, "textureIndex">,
		field<short, "cropIndex">
	>;

	// Textures
	using SRS_TEXTURE = structure<impl::SRS_TEXTURE, "SRS_TEXTURE", void,
		field<const char*, "filename">,
		field<unsigned int, "id">,
		field<unsigned short, "width">,
		field<unsigned short, "height">,
		field<unsigned int, "flags">,
		field<unsigned int, "cropCount">,
		field<dynamic_carray<SRS_CROP, field_resolver<unsigned int, "cropCount">>*, "crops">,
		field<SRS_USERDATA*, "userData">
	>;

	using SRS_TEXTURELIST = structure<impl::SRS_TEXTURELIST, "SRS_TEXTURELIST", void,
		field<const char*, "name">,
		field<unsigned int, "textureCount">,
		field<dynamic_carray<SRS_TEXTURE, field_resolver<unsigned int, "textureCount">>*, "textures">,
		field<SRS_USERDATA*, "userData">
	>;

	// Fonts
	using SRS_CHARACTER_MAPPING = structure<impl::SRS_CHARACTER_MAPPING, "SRS_CHARACTER_MAPPING", void,
		field<unsigned short, "character">,
		field<SRS_CROPREF, "cropRef">
	>;

	using SRS_FONT = structure<impl::SRS_FONT, "SRS_FONT", void,
		field<const char*, "name">,
		field<unsigned int, "id">,
		field<unsigned int, "unk1">,
		field<unsigned short, "characterCount">,
		field<unsigned short, "unk3">,
		field<dynamic_carray<SRS_CHARACTER_MAPPING, field_resolver<unsigned short, "characterCount">>*, "characters">,
		field<unsigned long long, "unk4">,
		field<SRS_USERDATA*, "userData">
	>;

	// Text
	using ETextDataVerticalAlignment = enumeration<impl::SRS_TEXTDATA::VerticalAlignment, "ETextDataVerticalAlignment", unsigned int,
		option<"TOP">,
		option<"CENTER">,
		option<"BOTTOM">
	>;

	using SRS_TEXTDATA = structure<impl::SRS_TEXTDATA, "SRS_TEXTDATA", void,
		field<bitfield<unsigned int,
			component<bool, "wrap", 0, 1>,
			component<bool, "disableLineHeight", 1, 1>,
			component<bool, "unk0", 2, 1>,
			component<bool, "unk1", 3, 1>,
			component<ETextDataVerticalAlignment, "verticalAlignment", 4, 4>
		>, "flags">,
		field<unsigned int, "fontIndex">,
		field<const char*, "text">,
		field<Vector2, "scale">,
		field<short, "paddingLeft">,
		field<short, "paddingRight">,
		field<short, "paddingTop">,
		field<short, "paddingBottom">,
		field<short, "tracking">,
		field<short, "lineHeight">,
		field<weak<SRS_FONT*>, "font">
	>;

	// Effects
	using EEffectType = enumeration<impl::EEffectType, "EEffectType", unsigned int,
		option<"NONE">,
		option<"BLUR">,
		option<"REFLECT">
	>;

	using ERenderMode = enumeration<impl::ERenderMode, "ERenderMode", unsigned int,
		option<"UNK0">,
		option<"OVERRIDE">,
		option<"UNK1">,
		option<"UNK2">
	>;

	using ECropBlendMode = enumeration<impl::ECropBlendMode, "ECropBlendMode", unsigned int,
		option<"CROP0">,
		option<"CROP1">,
		option<"MODULATE">,
		option<"ALPHA_BLEND">
	>;

	using EBlendMode = enumeration<impl::EBlendMode, "EBlendMode", unsigned int,
		option<"DEFAULT">,
		option<"ADD">,
		option<"SUBTRACT">,
		option<"MULTIPLY">,
		option<"DEFAULT_NO_ALPHA">,
		option<"OVERRIDE">
	>;

	using EPivotType = enumeration<impl::EPivotType, "EPivotType", unsigned int,
		option<"TOP_LEFT">,
		option<"TOP_CENTER">,
		option<"TOP_RIGHT">,
		option<"CENTER_LEFT">,
		option<"CENTER_CENTER">,
		option<"CENTER_RIGHT">,
		option<"BOTTOM_LEFT">,
		option<"BOTTOM_CENTER">,
		option<"BOTTOM_RIGHT">,
		option<"CUSTOM">
	>;

	using EOrientation = enumeration<impl::EOrientation, "EOrientation", unsigned int,
		option<"UP">,
		option<"LEFT">,
		option<"DOWN">,
		option<"RIGHT">
	>;

	using SRS_EFFECT = structure<impl::SRS_EFFECT, "SRS_EFFECT", void>;

	using SRS_BLUR = structure<impl::SRS_BLUR, "SRS_BLUR", SRS_EFFECT,
		field<unsigned int, "unk1">,
		field<unsigned int, "unk2">,
		field<unsigned int, "cropCount">,
		field<unsigned int, "steps">,
		field<int, "duration">,
		field<bitfield<unsigned int,
			component<EBlendMode, "blendMode", 0, 4>,
			component<bool, "hide", 12, 1>
		>, "flags">,
		field<Color, "color">
	>;

	using SRS_BLUR3D = structure<impl::SRS_BLUR3D, "SRS_BLUR3D", SRS_BLUR>;

	using SRS_REFLECT = structure<impl::SRS_REFLECT, "SRS_REFLECT", SRS_EFFECT,
		field<unsigned int, "unk1">,
		field<unsigned int, "unk2">,
		field<unsigned int, "unk3">,
		field<float, "unk4">,
		field<float, "unk5">,
		field<float, "unk6">,
		field<float, "unk7">,
		field<bitfield<unsigned int,
			component<EBlendMode, "blendMode", 0, 4>,
			component<bool, "hide", 12, 1>
		>, "flags">,
		field<Color, "color">
	>;

	using SRS_REFLECT3D = structure<impl::SRS_REFLECT3D, "SRS_REFLECT3D", SRS_REFLECT>;

	// Transforms
	using SRS_TRS_BASE = structure<impl::SRS_TRS_BASE, "SRS_TRS_BASE", void,
		field<Color, "materialColor">,
		field<Color, "illuminationColor">,
		field<bool, "display">
	>;

	using SRS_TRS2D = structure<impl::SRS_TRS2D, "SRS_TRS2D", SRS_TRS_BASE,
		field<Vector2, "position">,
		field<int, "rotation">,
		field<Vector2, "scale">,
		field<unsigned int, "unkX">,
		field<unsigned int, "unkY">
	>;

	using SRS_TRS3D = structure<impl::SRS_TRS3D, "SRS_TRS3D", SRS_TRS_BASE,
		field<Vector3, "position">,
		field<Rotation3, "rotation">,
		field<Vector3, "scale">
	>;

	inline size_t get_effect_ptr_idx(const unsigned int& effectType) {
		switch (static_cast<impl::EEffectType>(effectType & 0xF)) {
		case impl::EEffectType::NONE: return 0;
		case impl::EEffectType::BLUR: return 1;
		case impl::EEffectType::REFLECT: return 2;
		default: assert(false && "invalid type"); return 0;
		}
	}

	// Casts
	using SRS_EFFECT_PTR = unionof<impl::SRS_EFFECT_PTR, "SRS_EFFECT_PTR", selector_resolver<size_t, field_resolver<unsigned int, "effectType">>::impl<get_effect_ptr_idx>,
		field<SRS_BLUR3D*, "none">,
		field<SRS_BLUR3D*, "blur">,
		field<SRS_REFLECT3D*, "reflect">
	>;

	using SRS_IMAGECAST = structure<impl::SRS_IMAGECAST, "SRS_IMAGECAST", void,
		field<bitfield<unsigned int,
			component<EBlendMode, "blendMode", 0, 4>,
			component<bool, "mirrorHorizontally", 4, 1>,
			component<bool, "mirrorVertically", 5, 1>,
			component<EOrientation, "orientation", 6, 2>,
			component<bool, "isText", 8, 1>,
			component<ERenderMode, "renderMode", 9, 2>,
			component<ECropBlendMode, "cropBlendMode", 11, 2>,
			component<bool, "unkFlag1", 13, 1>,
			component<EPivotType, "pivotType", 19, 4>,
			component<bool, "useCrop0Transparency", 29, 1>
		>, "flags">,
		field<Vector2, "size">,
		field<Vector2, "pivot">,
		field<Color, "vertexColorTopLeft">,
		field<Color, "vertexColorBottomLeft">,
		field<Color, "vertexColorTopRight">,
		field<Color, "vertexColorBottomRight">,
		field<short, "cropIndex0">,
		field<short, "cropIndex1">,
		field<short, "cropRef0Count">,
		field<short, "cropRef1Count">,
		field<dynamic_carray<SRS_CROPREF, field_resolver<short, "cropRef0Count">>*, "cropRefs0">,
		field<dynamic_carray<SRS_CROPREF, field_resolver<short, "cropRef1Count">>*, "cropRefs1">,
		field<SRS_TEXTDATA*, "textData">,
		field<bitfield<unsigned int,
			component<EEffectType, "effectType", 0, 4>
		>, "effectType">,
		field<SRS_EFFECT_PTR, "effectData">
	>;

	using ESliceConstraint = enumeration<impl::ESliceConstraint, "ESliceConstraint", unsigned int,
		option<"CENTER">,
		option<"UNK1">,
		option<"UNK2">,
		option<"UNK3">,
		option<"UNK4">,
		option<"UNK5">,
		option<"UNK6">,
		option<"UNK7">,
		option<"UNK8">,
		option<"UNK9">
	>;

	using SRS_SLICE = structure<impl::SRS_SLICE, "SRS_SLICE", void,
		field<unsigned int, "flags">,
		field<Vector2, "fixedSize">,
		field<Color, "materialColor">,
		field<Color, "illuminationColor">,
		field<Color, "vertexColorTopLeft">,
		field<Color, "vertexColorBottomLeft">,
		field<Color, "vertexColorTopRight">,
		field<Color, "vertexColorBottomRight">,
		field<short, "cropIndex0">
	>;

	inline size_t get_slice_count(const short& h, const short& v) {
		return h * v;
	}

	using SRS_SLICECAST = structure<impl::SRS_SLICECAST, "SRS_SLICECAST", void,
		field<bitfield<unsigned int,
			component<EBlendMode, "blendMode", 0, 4>,
			component<bool, "mirrorHorizontally", 4, 1>,
			component<bool, "mirrorVertically", 5, 1>,
			component<EOrientation, "orientation", 6, 2>,
			component<bool, "isText", 8, 1>,
			component<ERenderMode, "renderMode", 9, 2>,
			component<ECropBlendMode, "cropBlendMode", 11, 2>,
			component<bool, "unkFlag1", 13, 1>,
			component<EPivotType, "pivotType", 19, 4>,
			component<bool, "useCrop0Transparency", 29, 1>
		>, "flags">,
		field<Vector2, "size">,
		field<Vector2, "pivot">,
		field<Color, "vertexColorTopLeft">,
		field<Color, "vertexColorBottomLeft">,
		field<Color, "vertexColorTopRight">,
		field<Color, "vertexColorBottomRight">,
		field<Vector2, "fixedSize">,
		field<short, "sliceHorizontalCount">,
		field<short, "sliceVerticalCount">,
		field<short, "horizontalFixedCount">,
		field<short, "verticalFixedCount">,
		field<short, "cropRef0Count">,
		field<dynamic_carray<SRS_CROPREF, field_resolver<short, "cropRef0Count">>*, "cropRefs0">,
		field<bitfield<unsigned int,
			component<EEffectType, "effectType", 0, 4>
		>, "effectType">,
		field<SRS_EFFECT_PTR, "effectData">,
		field<dynamic_carray<SRS_SLICE, selector_resolver<size_t, field_resolver<short, "sliceHorizontalCount">, field_resolver<short, "sliceVerticalCount">>::impl<get_slice_count>>, "slices">
	>;

	using SRS_REFERENCECAST = structure<impl::SRS_REFERENCECAST, "SRS_REFERENCECAST", void,
		field<weak<deferred<impl::SRS_LAYER>*>, "layer">,
		field<unsigned int, "flags">,
		field<unsigned int, "animationId">,
		field<unsigned int, "animationFrame">,
		field<int, "unk2">
	>;

	inline size_t get_cast_ptr_idx(const unsigned int& flags) {
		switch (static_cast<impl::SRS_CASTNODE::Type>(flags & 0xF)) {
		case impl::SRS_CASTNODE::Type::NORMAL: return 0;
		case impl::SRS_CASTNODE::Type::IMAGE: return 1;
		case impl::SRS_CASTNODE::Type::SLICE: return 2;
		case impl::SRS_CASTNODE::Type::REFERENCE: return 3;
		default: assert(false && "invalid type"); return 0;
		}
	}

	using SRS_CAST_PTR = unionof<impl::SRS_CAST_PTR, "SRS_CASTNODE_PTR", selector_resolver<size_t, field_resolver<unsigned int, "flags">>::impl<get_cast_ptr_idx>,
		field<SRS_IMAGECAST*, "none">,
		field<SRS_IMAGECAST*, "image">,
		field<SRS_SLICECAST*, "slice">,
		field<SRS_REFERENCECAST*, "reference">
	>;

	using ECastNodeType = enumeration<impl::SRS_CASTNODE::Type, "ECastNodeType", unsigned int,
		option<"NORMAL">,
		option<"IMAGE">,
		option<"SLICE">,
		option<"REFERENCE">
	>;

	using SRS_CASTNODE = structure<impl::SRS_CASTNODE, "SRS_CASTNODE", void,
		field<const char*, "name">,
		field<unsigned int, "id">,
		field<bitfield<unsigned int,
			component<ECastNodeType, "type", 0, 4>,
			component<bool, "unk0", 4, 1>,
			component<bool, "transformMaterialColor", 5, 1>,
			component<bool, "transformDisplay", 6, 1>,
			component<bool, "transformIlluminationColor", 7, 1>,
			component<bool, "disableTranslation", 8, 1>,
			component<bool, "disableRotation", 9, 1>,
			component<bool, "disableScale", 10, 1>,
			component<bool, "hide", 12, 1>,
			component<bool, "unk1", 13, 1>,
			component<bool, "transformCropIndex0", 14, 1>,
			component<bool, "transformCropIndex1", 15, 1>,
			component<bool, "transformTranslation", 16, 1>,
			component<bool, "transformRotation", 17, 1>,
			component<bool, "transformScale", 18, 1>,
			component<ESliceConstraint, "sliceConstraint", 19, 4>,
			component<bool, "unk2", 23, 1>,
			component<unsigned char, "unk3", 24, 8>
		>, "flags">,
		field<SRS_CAST_PTR, "data">,
		field<short, "childIndex">,
		field<short, "siblingIndex">,
		field<SRS_USERDATA*, "userData">
	>;

	inline size_t get_trs_ptr_idx(const unsigned int& flags) {
		return flags & 0x1 ? 1 : 0;
	}

	// Layers
	using SRS_TRS_PTR = unionof<impl::SRS_TRS_PTR, "SRS_TRS_PTR", selector_resolver<size_t, field_resolver<unsigned int, "flags">>::impl<get_trs_ptr_idx>,
		field<dynamic_carray<SRS_TRS2D, field_resolver<int, "castCount">>*, "transforms2d">,
		field<dynamic_carray<SRS_TRS3D, field_resolver<int, "castCount">>*, "transforms3d">
	>;

	using EResolutionMode = enumeration<impl::EResolutionMode, "EResolutionMode", unsigned int,
		option<"DIRECT">,
		option<"RESOLUTION_INDEPENDENT">
	>;

	using SRS_LAYER = structure<impl::SRS_LAYER, "SRS_LAYER", void,
		field<const char*, "name">,
		field<unsigned int, "id">,
		field<bitfield<unsigned int,
			component<bool, "is3D", 0, 1>,
			component<unsigned char, "unk0", 1, 3>,
			component<EResolutionMode, "resolutionMode", 4, 4>,
			component<bool, "hide", 8, 1>,
			component<unsigned char, "unk3", 9, 7>,
			component<bool, "unk4", 16, 1>,
			component<unsigned char, "unk5", 17, 7>,
			component<bool, "unk6", 24, 1>,
			component<unsigned char, "unk7", 25, 7>
		>, "flags">,
		field<int, "castCount">,
		field<dynamic_carray<SRS_CASTNODE, field_resolver<int, "castCount">>*, "casts">,
		field<SRS_TRS_PTR, "transforms">,
		field<int, "animationCount">,
		field<dynamic_carray<SRS_ANIMATION, field_resolver<int, "animationCount">>*, "animations">,
		field<int, "currentAnimationIndex">,
		field<SRS_USERDATA*, "userData">
	>;

	// Cameras
	using SRS_CAMERA = structure<impl::SRS_CAMERA, "SRS_CAMERA", void,
		field<const char*, "name">,
		field<unsigned int, "id">,
		field<Vector3, "position">,
		field<Vector3, "target">,
		field<int, "fov">,
		field<float, "nearPlane">,
		field<float, "farPlane">,
		field<float, "unk1">,
		field<SRS_USERDATA*, "userData">
	>;

	// Scenes
	using SRS_SCENE = structure<impl::SRS_SCENE, "SRS_SCENE", void,
		field<const char*, "name">,
		field<unsigned int, "id">,
		field<bitfield<unsigned int,
			component<bool, "hide", 0, 1>
		>, "flags">,
		field<int, "layerCount">,
		field<dynamic_carray<SRS_LAYER, field_resolver<int, "layerCount">>*, "layers">,
		field<unsigned short, "cameraCount">,
		field<unsigned short, "currentCameraIndex">,
		field<dynamic_carray<SRS_CAMERA, field_resolver<unsigned short, "cameraCount">>*, "cameras">,
		field<Color, "backgroundColor">,
		field<Vector2, "resolution">,
		field<SRS_USERDATA*, "userData">
	>;

	// Projects
	using SRS_PROJECT = structure<impl::SRS_PROJECT, "SRS_PROJECT", void,
		field<const char*, "name">,
		field<unsigned short, "sceneCount">,
		field<unsigned short, "unk1">,
		field<unsigned short, "textureListCount">,
		field<unsigned short, "fontCount">,
		field<dynamic_carray<SRS_SCENE, field_resolver<unsigned short, "sceneCount">>*, "scenes">,
		field<dynamic_carray<SRS_TEXTURELIST, field_resolver<unsigned short, "textureListCount">>*, "textureLists">,
		field<dynamic_carray<SRS_FONT, field_resolver<unsigned short, "fontCount">>*, "fonts">,
		field<SRS_CAMERA, "camera">,
		field<int, "startFrame">,
		field<int, "endFrame">,
		field<float, "frameRate">,
		field<SRS_USERDATA*, "userData">
	>;
}

namespace simplerfl {
	template<> struct canonical<ucsl::resources::swif::v3::Rotation3> { using type = ucsl::resources::swif::v3::reflections::Rotation3; };
	template<> struct canonical<ucsl::resources::swif::v3::Color> { using type = ucsl::resources::swif::v3::reflections::Color; };
	template<> struct canonical<ucsl::resources::swif::v3::ECurveType> { using type = ucsl::resources::swif::v3::reflections::ECurveType; };
	template<> struct canonical<ucsl::resources::swif::v3::EInterpolationType> { using type = ucsl::resources::swif::v3::reflections::EInterpolationType; };
	template<> struct canonical<ucsl::resources::swif::v3::SRS_KEYFRAME> { using type = ucsl::resources::swif::v3::reflections::SRS_KEYFRAME; };
	template<> struct canonical<ucsl::resources::swif::v3::Key<float>> { using type = ucsl::resources::swif::v3::reflections::Key<float>; };
	template<> struct canonical<ucsl::resources::swif::v3::Key<int>> { using type = ucsl::resources::swif::v3::reflections::Key<int>; };
	template<> struct canonical<ucsl::resources::swif::v3::Key<bool>> { using type = ucsl::resources::swif::v3::reflections::Key<bool>; };
	template<> struct canonical<ucsl::resources::swif::v3::Key<ucsl::resources::swif::v3::Color>> { using type = ucsl::resources::swif::v3::reflections::Key<ucsl::resources::swif::v3::Color>; };
	template<> struct canonical<ucsl::resources::swif::v3::KeyLinear<float>> { using type = ucsl::resources::swif::v3::reflections::KeyLinear<float>; };
	template<> struct canonical<ucsl::resources::swif::v3::KeyLinear<int>> { using type = ucsl::resources::swif::v3::reflections::KeyLinear<int>; };
	template<> struct canonical<ucsl::resources::swif::v3::KeyLinear<bool>> { using type = ucsl::resources::swif::v3::reflections::KeyLinear<bool>; };
	template<> struct canonical<ucsl::resources::swif::v3::KeyLinear<ucsl::resources::swif::v3::Color>> { using type = ucsl::resources::swif::v3::reflections::KeyLinear<ucsl::resources::swif::v3::Color>; };
	template<> struct canonical<ucsl::resources::swif::v3::KeyHermite<float>> { using type = ucsl::resources::swif::v3::reflections::KeyHermite<float>; };
	template<> struct canonical<ucsl::resources::swif::v3::KeyHermite<int>> { using type = ucsl::resources::swif::v3::reflections::KeyHermite<int>; };
	template<> struct canonical<ucsl::resources::swif::v3::KeyHermite<bool>> { using type = ucsl::resources::swif::v3::reflections::KeyHermite<bool>; };
	template<> struct canonical<ucsl::resources::swif::v3::KeyHermite<ucsl::resources::swif::v3::Color>> { using type = ucsl::resources::swif::v3::reflections::KeyHermite<ucsl::resources::swif::v3::Color>; };
	template<> struct canonical<ucsl::resources::swif::v3::KeyIndividual<float>> { using type = ucsl::resources::swif::v3::reflections::KeyIndividual<float>; };
	template<> struct canonical<ucsl::resources::swif::v3::KeyIndividual<int>> { using type = ucsl::resources::swif::v3::reflections::KeyIndividual<int>; };
	template<> struct canonical<ucsl::resources::swif::v3::KeyIndividual<bool>> { using type = ucsl::resources::swif::v3::reflections::KeyIndividual<bool>; };
	template<> struct canonical<ucsl::resources::swif::v3::KeyIndividual<ucsl::resources::swif::v3::Color>> { using type = ucsl::resources::swif::v3::reflections::KeyIndividual<ucsl::resources::swif::v3::Color>; };
	template<> struct canonical<ucsl::resources::swif::v3::SRS_KEYFRAME_PTR> { using type = ucsl::resources::swif::v3::reflections::SRS_KEYFRAME_PTR; };
	template<> struct canonical<ucsl::resources::swif::v3::SRS_TRACK> { using type = ucsl::resources::swif::v3::reflections::SRS_TRACK; };
	template<> struct canonical<ucsl::resources::swif::v3::SRS_MOTION> { using type = ucsl::resources::swif::v3::reflections::SRS_MOTION; };
	template<> struct canonical<ucsl::resources::swif::v3::SRS_ANIMATION> { using type = ucsl::resources::swif::v3::reflections::SRS_ANIMATION; };
	template<> struct canonical<ucsl::resources::swif::v3::SRS_CROP> { using type = ucsl::resources::swif::v3::reflections::SRS_CROP; };
	template<> struct canonical<ucsl::resources::swif::v3::SRS_CROPREF> { using type = ucsl::resources::swif::v3::reflections::SRS_CROPREF; };
	template<> struct canonical<ucsl::resources::swif::v3::SRS_DATA::Type> { using type = ucsl::resources::swif::v3::reflections::EDataType; };
	template<> struct canonical<ucsl::resources::swif::v3::SRS_DATA_VALUE_PTR> { using type = ucsl::resources::swif::v3::reflections::SRS_DATA_VALUE_PTR; };
	template<> struct canonical<ucsl::resources::swif::v3::SRS_DATA> { using type = ucsl::resources::swif::v3::reflections::SRS_DATA; };
	template<> struct canonical<ucsl::resources::swif::v3::SRS_USERDATA> { using type = ucsl::resources::swif::v3::reflections::SRS_USERDATA; };
	template<> struct canonical<ucsl::resources::swif::v3::SRS_TEXTURE> { using type = ucsl::resources::swif::v3::reflections::SRS_TEXTURE; };
	template<> struct canonical<ucsl::resources::swif::v3::SRS_TEXTURELIST> { using type = ucsl::resources::swif::v3::reflections::SRS_TEXTURELIST; };
	template<> struct canonical<ucsl::resources::swif::v3::SRS_CHARACTER_MAPPING> { using type = ucsl::resources::swif::v3::reflections::SRS_CHARACTER_MAPPING; };
	template<> struct canonical<ucsl::resources::swif::v3::SRS_FONT> { using type = ucsl::resources::swif::v3::reflections::SRS_FONT; };
	template<> struct canonical<ucsl::resources::swif::v3::SRS_TEXTDATA> { using type = ucsl::resources::swif::v3::reflections::SRS_TEXTDATA; };
	template<> struct canonical<ucsl::resources::swif::v3::SRS_EFFECT> { using type = ucsl::resources::swif::v3::reflections::SRS_EFFECT; };
	template<> struct canonical<ucsl::resources::swif::v3::SRS_BLUR> { using type = ucsl::resources::swif::v3::reflections::SRS_BLUR; };
	template<> struct canonical<ucsl::resources::swif::v3::SRS_BLUR3D> { using type = ucsl::resources::swif::v3::reflections::SRS_BLUR3D; };
	template<> struct canonical<ucsl::resources::swif::v3::SRS_REFLECT> { using type = ucsl::resources::swif::v3::reflections::SRS_REFLECT; };
	template<> struct canonical<ucsl::resources::swif::v3::SRS_REFLECT3D> { using type = ucsl::resources::swif::v3::reflections::SRS_REFLECT3D; };
	template<> struct canonical<ucsl::resources::swif::v3::SRS_TRS_BASE> { using type = ucsl::resources::swif::v3::reflections::SRS_TRS_BASE; };
	template<> struct canonical<ucsl::resources::swif::v3::SRS_TRS2D> { using type = ucsl::resources::swif::v3::reflections::SRS_TRS2D; };
	template<> struct canonical<ucsl::resources::swif::v3::SRS_TRS3D> { using type = ucsl::resources::swif::v3::reflections::SRS_TRS3D; };
	//template<> struct canonical<ucsl::resources::swif::v3::SRS_EFFECT_PTR> { using type = ucsl::resources::swif::v3::reflections::SRS_EFFECT_PTR; };
	template<> struct canonical<ucsl::resources::swif::v3::SRS_IMAGECAST> { using type = ucsl::resources::swif::v3::reflections::SRS_IMAGECAST; };
	template<> struct canonical<ucsl::resources::swif::v3::SRS_SLICE> { using type = ucsl::resources::swif::v3::reflections::SRS_SLICE; };
	template<> struct canonical<ucsl::resources::swif::v3::SRS_SLICECAST> { using type = ucsl::resources::swif::v3::reflections::SRS_SLICECAST; };
	template<> struct canonical<ucsl::resources::swif::v3::SRS_REFERENCECAST> { using type = ucsl::resources::swif::v3::reflections::SRS_REFERENCECAST; };
	template<> struct canonical<ucsl::resources::swif::v3::SRS_CAST_PTR> { using type = ucsl::resources::swif::v3::reflections::SRS_CAST_PTR; };
	template<> struct canonical<ucsl::resources::swif::v3::SRS_CASTNODE> { using type = ucsl::resources::swif::v3::reflections::SRS_CASTNODE; };
	template<> struct canonical<ucsl::resources::swif::v3::SRS_TRS_PTR> { using type = ucsl::resources::swif::v3::reflections::SRS_TRS_PTR; };
	template<> struct canonical<ucsl::resources::swif::v3::SRS_LAYER> { using type = ucsl::resources::swif::v3::reflections::SRS_LAYER; };
	template<> struct canonical<ucsl::resources::swif::v3::SRS_CAMERA> { using type = ucsl::resources::swif::v3::reflections::SRS_CAMERA; };
	template<> struct canonical<ucsl::resources::swif::v3::SRS_SCENE> { using type = ucsl::resources::swif::v3::reflections::SRS_SCENE; };
	template<> struct canonical<ucsl::resources::swif::v3::SRS_PROJECT> { using type = ucsl::resources::swif::v3::reflections::SRS_PROJECT; };
}