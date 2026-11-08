#pragma once
#include <ucsl/resources/material/v3.h>
#include <simple-reflection/simple-reflection.h>
#include <ucsl-reflection/operators.h>
#include <ucsl-reflection/reflections/basic-types.h>

namespace ucsl::resources::material::v3::reflections {
	using namespace simplerfl;
	using namespace ucsl::reflection;

	namespace impl = ucsl::resources::material::v3;

    using TextureWrapMode = enumeration<impl::TextureWrapMode, "TextureWrapMode", unsigned char,
        option<"REPEAT">,
        option<"MIRROR">,
        option<"CLAMP">,
        option<"MIRROR_ONCE">,
        option<"BORDER">,
        option<"UNK5">,
        option<"UNK6">
    >;

    using BlendMode = enumeration<impl::BlendMode, "BlendMode", unsigned char,
        option<"MIX">,
        option<"ADD">,
        option<"MULTIPLY">,
        option<"SUBTRACT">
    >;

    using TextureData = structure<impl::TextureData, "TextureData", void,
        field<const char*, "name">,
        field<unsigned char, "texCoordIndex">,
        field<TextureWrapMode, "wrapModeU">,
        field<TextureWrapMode, "wrapModeV">,
        field<const char*, "type">
    >;

    template<typename T>
    using MaterialParamData = structure<impl::MaterialParamData<T>, "MaterialParamData", void,
        field<unsigned char, "flag1">,
        field<unsigned char, "flag2">,
        field<unsigned char, "valueCount">,
        field<unsigned char, "flag3">,
        field<const char*, "name">,
        field<dynamic_carray<T[4], field_resolver<unsigned char, "valueCount">>*, "values">
    >;

    using ContextsData = structure<impl::ContextsData, "ContextsData", void,
        field<const char*, "shaderName">,
        field<const char*, "subShaderName">,
        field<dynamic_carray<const char*, field_resolver<unsigned char, "textureCount">>*, "textureNames">,
        field<dynamic_carray<TextureData*, field_resolver<unsigned char, "textureCount">>*, "textures">,
        field<unsigned char, "alphaThreshold">,
        field<bool, "noBackfaceCulling">,
        field<BlendMode, "blendMode">,
        field<unsigned char, "unknownFlag1">,
        field<unsigned char, "floatParamCount">,
        field<unsigned char, "uintParamCount">,
        field<unsigned char, "boolParamCount">,
        field<unsigned char, "textureCount">,
        field<dynamic_carray<MaterialParamData<float>*, field_resolver<unsigned char, "floatParamCount">>*, "floatParams">,
        field<dynamic_carray<MaterialParamData<unsigned int>*, field_resolver<unsigned char, "uintParamCount">>*, "uintParams">,
        field<dynamic_carray<MaterialParamData<bool>*, field_resolver<unsigned char, "boolParamCount">>*, "boolParams">
    >;
}

namespace simplerfl {
	template<> struct canonical<ucsl::resources::material::v3::BlendMode> { using type = ucsl::resources::material::v3::reflections::BlendMode; };
    template<> struct canonical<ucsl::resources::material::v3::TextureData> { using type = ucsl::resources::material::v3::reflections::TextureData; };
    template<typename T> struct canonical<ucsl::resources::material::v3::MaterialParamData<T>> { using type = ucsl::resources::material::v3::reflections::MaterialParamData<T>; };
	template<> struct canonical<ucsl::resources::material::v3::ContextsData> { using type = ucsl::resources::material::v3::reflections::ContextsData; };
}
