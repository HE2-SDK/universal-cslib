#pragma once
#include <ucsl/math.h>
#include <simple-reflection/simple-reflection.h>
#include <ucsl-reflection/operators.h>
#include <ucsl-reflection/reflections/basic-types.h>

// reversed from files

namespace ucsl::resources::converse_text::v6_forces::reflections {
    using namespace simplerfl;
    using namespace ucsl::reflection;

    namespace impl = ::ucsl::resources::converse_text::v6_forces;

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
        field<float*, "defaultSize">,
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

    using FontLayout = structure<impl::FontLayout, "FontLayout", void,
        field<const char*, "entryKey">,
        field<Font*, "font">,
        field<Layout*, "layout">
    >;

    using Entry = structure<impl::Entry, "Entry", void,
        field<int, "hash">,
        field<const char*, "key">,
        field<FontLayout*, "fontLayout">,
        field<wchar_t*, "text">
    >;

    using Sheet = structure<impl::Sheet, "Sheet", void,
        field<const char*, "sheetName">,
        field<unsigned short, "entryAmount">,
        field<dynamic_carray<Entry, impl::Sheet, [](const impl::Sheet& parent) -> size_t { return parent.entryAmount; }>*, "entries">
    >;

    using CnvrsTextData = structure<impl::CnvrsTextData, "CnvrsTextData", void,
        field<unsigned char, "version">,
        field<unsigned char, "sheetAmount">,
        field<dynamic_carray<Sheet, impl::CnvrsTextData, [](const impl::CnvrsTextData& parent) -> size_t { return parent.sheetAmount; }>*, "sheets">
    >;
}

namespace simplerfl {
    template<> struct canonical<ucsl::resources::converse_text::v6_forces::Layout> { using type = ucsl::resources::converse_text::v6_forces::reflections::Layout; };
    template<> struct canonical<ucsl::resources::converse_text::v6_forces::Font> { using type = ucsl::resources::converse_text::v6_forces::reflections::Font; };
    template<> struct canonical<ucsl::resources::converse_text::v6_forces::FontLayout> { using type = ucsl::resources::converse_text::v6_forces::reflections::FontLayout; };
    template<> struct canonical<ucsl::resources::converse_text::v6_forces::Entry> { using type = ucsl::resources::converse_text::v6_forces::reflections::Entry; };
    template<> struct canonical<ucsl::resources::converse_text::v6_forces::Sheet> { using type = ucsl::resources::converse_text::v6_forces::reflections::Sheet; };
    template<> struct canonical<ucsl::resources::converse_text::v6_forces::CnvrsTextData> { using type = ucsl::resources::converse_text::v6_forces::reflections::CnvrsTextData; };
}
