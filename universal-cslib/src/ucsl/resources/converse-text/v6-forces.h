#pragma once

// reversed from files

namespace ucsl::resources::converse_text::v6_forces {
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
        float* defaultSize;
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
        int* unk11;
    };

    struct FontLayout {
        const char* entryKey;
        Font* font;
        Layout* layout;
    };

    struct Entry {
        int hash;
        const char* key;
        FontLayout* fontLayout;
        wchar_t* text;
    };

    struct Sheet {
        const char* sheetName;
        unsigned short entryAmount;
        Entry* entries;
    };

    struct CnvrsTextData {
        unsigned char version;
        unsigned char sheetAmount;
        Sheet* sheets;
    };
}
