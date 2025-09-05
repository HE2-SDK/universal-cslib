#pragma once
#include <ucsl/math.h>
#include <simple-reflection/simple-reflection.h>
#include <ucsl-reflection/operators.h>
#include <ucsl-reflection/reflections/basic-types.h>

// reversed from files

namespace ucsl::resources::converse_text::v6::reflections {
    using namespace simplerfl;
    using namespace ucsl::reflection;

    namespace impl = ::ucsl::resources::converse_text::v6;

    using AttributeEntry = structure<impl::AttributeEntry, "AttributeEntry", void,
        field<const char*, "type">,
        field<int, "unk0">,
        field<const char*, "value">
    >;

    using Attributes = structure<impl::Attributes, "Attributes", void,
        field<unsigned int, "attributeAmount">,
        field<dynamic_carray<AttributeEntry, impl::Attributes, [](const impl::Attributes& parent) -> size_t { return parent.attributeAmount; }>*, "attributes">
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
        field<dynamic_carray<wchar_t, impl::Entry, [](const impl::Entry& parent) -> size_t { return parent.textLength; }>*, "text">,
        field<unsigned int, "textLength">,
        field<Attributes*, "attributes">
    >;

    using CnvrsTextData = structure<impl::CnvrsTextData, "CnvrsTextData", void,
        field<unsigned char, "version">,
        field<unsigned char, "sheetAmount">, //unused
        field<unsigned short, "entryAmount">,
        field<dynamic_carray<Entry, impl::CnvrsTextData, [](const impl::CnvrsTextData& parent) -> size_t { return parent.entryAmount; }>*, "entries">,
        field<const char*, "sheetName">
    >;
}

namespace simplerfl {
    template<> struct canonical<ucsl::resources::converse_text::v6::AttributeEntry> { using type = ucsl::resources::converse_text::v6::reflections::AttributeEntry; };
    template<> struct canonical<ucsl::resources::converse_text::v6::Attributes> { using type = ucsl::resources::converse_text::v6::reflections::Attributes; };
    template<> struct canonical<ucsl::resources::converse_text::v6::Layout> { using type = ucsl::resources::converse_text::v6::reflections::Layout; };
    template<> struct canonical<ucsl::resources::converse_text::v6::Font> { using type = ucsl::resources::converse_text::v6::reflections::Font; };
    template<> struct canonical<ucsl::resources::converse_text::v6::FontLayout> { using type = ucsl::resources::converse_text::v6::reflections::FontLayout; };
    template<> struct canonical<ucsl::resources::converse_text::v6::Entry> { using type = ucsl::resources::converse_text::v6::reflections::Entry; };
    template<> struct canonical<ucsl::resources::converse_text::v6::CnvrsTextData> { using type = ucsl::resources::converse_text::v6::reflections::CnvrsTextData; };
}
