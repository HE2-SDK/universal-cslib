#pragma once
#include <ucsl/math.h>
#include <simple-reflection/simple-reflection.h>
#include <ucsl-reflection/operators.h>
#include <ucsl-reflection/reflections/basic-types.h>

// reversed from files

namespace ucsl::resources::converse_project::v4::reflections {
    using namespace simplerfl;
    using namespace ucsl::reflection;

    namespace impl = ::ucsl::resources::converse_project::v4;

    using Color8 = structure<impl::Color8, "Color8", void,
        field<unsigned char, "r">,
        field<unsigned char, "g">,
        field<unsigned char, "b">,
        field<unsigned char, "a">,
    >;

    using Language = structure<impl::Language, "Language", void,
        field<const char*, "name">,
        field<const char*, "shortName">,
        field<long long, "id">,
        field<long long, "unk0">
    >;

    using LanguageInfo = structure<impl::LanguageInfo, "LanguageInfo", void,
        field<int, "languageCount">,
        field<dynamic_carray<Language*, field_resolver<"languageCount">>*, "languages">
    >;

    using Color = structure<impl::Color, "Color", void,
        field<const char*, "name">,
        field<Color8, "color">,
        field<long long, "unk0">
    >;

    using ColorInfo = structure<impl::ColorInfo, "ColorInfo", void,
        field<unsigned int, "colorCount">,
        field<dynamic_carray<Color*, field_resolver<"colorCount">>*, "colors">
    >;

    using ProjectSettings = structure<impl::ProjectSettings, "ProjectSettings", void,
        field<ColorInfo*, "colorInfo">,
        field<LanguageInfo*, "langInfo">
    >;

    using Layout = structure<impl::Layout, "Layout", void,
        field<const char*, "idName">,
        field<int*, "unk0">,
        field<float*, "unk1">,
        field<float*, "unk2">,
        field<int*, "unk3">,
        field<int*, "unk4">,
        field<int*, "unk5">,
        field<int*, "unk6">,
        field<int*, "unk7">,
        field<int*, "unk8">,
        field<int*, "unk9">
    >;

    using Font = structure<impl::Font, "Font", void,
        field<const char*, "idName">,
        field<const char*, "fontName">,
        field<float*, "unk0">,
        field<float*, "unk1">,
        field<float*, "unk2">,
        field<int*, "unk3">,
        field<int*, "unk4">,
        field<int*, "unk5">,
        field<int*, "unk6">,
        field<int*, "unk7">,
        field<int*, "unk8">,
        field<int*, "unk9">,
        field<int*, "unk10">,
        field<int*, "unk11">
    >;

    template<typename T>
    using LanguageItem = structure<impl::LanguageItem<T>, "LanguageItem", void,
        field<const char*, "langName">,
        field<unsigned int, "itemCount">,
        field<dynamic_carray<T*, field_resolver<unsigned int, "itemCount">>*, "items">
    >;

    using LanguageSettings = structure<impl::LanguageSettings, "LanguageSettings", void,
        field<unsigned int, "count">,
        field<dynamic_carray<LanguageItem<Font>*, field_resolver<unsigned int, "count">>*, "fonts">,
        field<dynamic_carray<LanguageItem<Layout>*, field_resolver<unsigned int, "count">>*, "layouts">
    >;

    using CnvrsProjData = structure<impl::CnvrsProjData, "CnvrsProjData", void,
        field<unsigned int, "version">,
        field<ProjectSettings*, "projSettings">,
        field<LanguageSettings*, "langSettings">
    >;
}

namespace simplerfl {
    template<> struct canonical<ucsl::resources::converse_project::v4::Color8> { using type = ucsl::resources::converse_project::v4::reflections::Color8; };
    template<> struct canonical<ucsl::resources::converse_project::v4::Language> { using type = ucsl::resources::converse_project::v4::reflections::Language; };
    template<> struct canonical<ucsl::resources::converse_project::v4::LanguageInfo> { using type = ucsl::resources::converse_project::v4::reflections::LanguageInfo; };
    template<> struct canonical<ucsl::resources::converse_project::v4::Color> { using type = ucsl::resources::converse_project::v4::reflections::Color; };
    template<> struct canonical<ucsl::resources::converse_project::v4::ColorInfo> { using type = ucsl::resources::converse_project::v4::reflections::ColorInfo; };
    template<> struct canonical<ucsl::resources::converse_project::v4::ProjectSettings> { using type = ucsl::resources::converse_project::v4::reflections::ProjectSettings; };
    template<> struct canonical<ucsl::resources::converse_project::v4::Layout> { using type = ucsl::resources::converse_project::v4::reflections::Layout; };
    template<> struct canonical<ucsl::resources::converse_project::v4::Font> { using type = ucsl::resources::converse_project::v4::reflections::Font; };
    template<> struct canonical<ucsl::resources::converse_project::v4::LanguageItem<Layout>> { using type = ucsl::resources::converse_project::v4::reflections::LanguageItem<Layout>; };
    template<> struct canonical<ucsl::resources::converse_project::v4::LanguageItem<Font>> { using type = ucsl::resources::converse_project::v4::reflections::LanguageItem<Font>; };
    template<> struct canonical<ucsl::resources::converse_project::v4::LanguageSettings> { using type = ucsl::resources::converse_project::v4::reflections::LanguageSettings; };
    template<> struct canonical<ucsl::resources::converse_project::v4::CnvrsProjData> { using type = ucsl::resources::converse_project::v4::reflections::CnvrsProjData; };
}
