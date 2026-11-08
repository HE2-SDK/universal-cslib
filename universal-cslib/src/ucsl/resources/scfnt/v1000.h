#pragma once

// reversed from files

#ifdef v1000
#undef v1000
#endif

namespace ucsl::resources::scfnt::v1000 {
    struct alignas(8) SubCharacter {
        wchar_t character;
        short spacing;
    };

    struct Character {
        wchar_t character;
        short subCharCount;
        SubCharacter* subChars;
    };

    struct ScfntData {
        unsigned int magic;
        unsigned int version;
        const char* fontName;
        Character* chars;
        unsigned int charCount;
        unsigned long long ttfDataSize;
        // char ttfData[ttfDataSize];
    };
}