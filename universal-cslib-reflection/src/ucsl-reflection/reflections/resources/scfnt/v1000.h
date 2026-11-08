#pragma once
#include <ucsl/math.h>
#include <simple-reflection/simple-reflection.h>
#include <ucsl-reflection/operators.h>
#include <ucsl-reflection/reflections/basic-types.h>

// reversed from files

namespace ucsl::resources::scfnt::v1000::reflections {
    using namespace simplerfl;
    using namespace ucsl::reflection;

    namespace impl = ::ucsl::resources::scfnt::v1000::reflections;

    using SubCharacter = aligned<8, structure<impl::SubCharacter, "SubCharacter", void,
        field<wchar_t, "character">,
        field<short, "spacing">
    >>;

    using Character = structure<impl::Character, "Character", void,
        field<wchar_t, "character">,
        field<unsigned short, "subCharCount">,
        field<dynamic_carray<SubCharacter, field_resolver<unsigned short, "subCharCount">>*, "subChars">
    >;

    using ScfntData = structure<impl::ScfntData, "ScfntData", void,
        field<unsigned int, "magic">,
        field<unsigned int, "version">,
        field<const char*, "fontName">,
        field<dynamic_carray<Character, field_resolver<unsigned int, "charCount">>*, "chars">
        field<unsigned int, "charCount">,
        field<unsigned long long, "ttfDataSize">,
        field<dynamic_carray<char, field_resolver<unsigned long long, "ttfDataSize">>, "ttfData">
    >;
}

namespace simplerfl {
    template<> struct canonical<ucsl::resources::scfnt::v1000::SubCharacter> { using type = ucsl::resources::scfnt::v1000::reflections::SubCharacter; };
    template<> struct canonical<ucsl::resources::scfnt::v1000::Character> { using type = ucsl::resources::scfnt::v1000::reflections::Character; };
    template<> struct canonical<ucsl::resources::scfnt::v1000::ScfntData> { using type = ucsl::resources::scfnt::v1000::reflections::ScfntData; };
}
