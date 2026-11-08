#pragma once
#include <ucsl/colors.h>

// reversed from files

namespace ucsl::resources::converse_project::v4 {
    struct Color8 {
        unsigned char r, g, b, a;
    };

    struct Language {
        const char* name;
        const char* shortName;
        long long id;
        long long unk0;
    };

    struct LanguageInfo {
        unsigned int languageCount;
        Language** languages;
    };

    struct Color {
        const char* name;
        Color8 color;
        long long unk0;
    };

    struct ColorInfo {
        unsigned int colorCount;
        Color** colors;
    };

    struct ProjectSettings {
        ColorInfo* colorInfo;
        LanguageInfo* langInfo;
    };

    struct Layout {
        const char* idName;
        int* unk0;
        float* unk1;
        float* unk2;
        int* unk3;
        int* unk4;
        int* unk5;
        int* unk6;
        int* unk7;
        int* unk8;
        int* unk9;
    };

    struct Font {
        const char* idName;
        const char* fontName;
        float* unk0;
        float* unk1;
        float* unk2;
        int* unk3;
        int* unk4;
        int* unk5;
        int* unk6;
        int* unk7;
        int* unk8;
        int* unk9;
        int* unk10;
        int* decoTechParamIndex;
    };

    template<typename T>
    struct LanguageItem {
        const char* langName;
        unsigned int itemCount;
        T** items;
    };

    struct LanguageSettings {
        int count;
        LanguageItem<Font>** fonts;
        LanguageItem<Layout>** layouts;
    };

    struct CnvrsProjData {
        unsigned int version;
        ProjectSettings* projSettings;
        LanguageSettings* langSettings;
    };
}