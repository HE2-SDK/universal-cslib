#pragma once

namespace ucsl::resources::ntsp::v1{
    struct Texture {
        unsigned int nameHash;
        unsigned int mipmapOffset;
        unsigned int mipmapCount;
        unsigned short width;
        unsigned short height;
        const char* name;
    };

    struct Mipmap {
        uint8_t* data;
        size_t size;
    };

    struct NeedleTextureStreamingPackage {
        unsigned int magic;
        unsigned int version;
        unsigned int textureCount;
        unsigned int mipmapCount;
        size_t tocSize;
    };
}
