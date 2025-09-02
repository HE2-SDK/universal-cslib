#pragma once

namespace ucsl::resources::ntsp::v1{
    struct Texture {
        unsigned int nameHash;
        unsigned int blockIndex;
        unsigned int mipLevels;
        unsigned short width;
        unsigned short height;
        const char* name;
    };

    struct Block {
        void* data;
        int64_t dataSize;
    };

    struct NeedleTextureStreamingPackage {
        unsigned int magic;
        unsigned int version;
        unsigned int textureCount;
        unsigned int blockCount;
        void* textureData;
        
        //Texture textures[textureCount];
        //Block blocks[blockCount];
    };
}
