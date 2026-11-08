#pragma once

namespace ucsl::resources::material::v3 {
    enum class TextureWrapMode : unsigned char {
        REPEAT,
        MIRROR,
        CLAMP,
        MIRROR_ONCE,
        BORDER,
        UNK5,
        UNK6,
    };

    enum class BlendMode : unsigned char {
        MIX,
        ADD,
        MULTIPLY,
        SUBTRACT,
    };

    struct TextureData {
        const char* name;
        unsigned char texCoordIndex;
        TextureWrapMode wrapModeU;
        TextureWrapMode wrapModeV;
        const char* type;
    };

    template<typename T>
    struct MaterialParamData {
        unsigned char flag1;
        unsigned char flag2;
        unsigned char valueCount;
        unsigned char flag3;
        const char* name;
        T (*values)[4];
    };

	struct ContextsData {
        const char* shaderName;
        const char* subShaderName;
        const char** textureNames;
        TextureData** textures;
        unsigned char alphaThreshold;
        bool noBackfaceCulling;
        BlendMode blendMode;
        unsigned char unknownFlag1;
        unsigned char floatParamCount;
        unsigned char uintParamCount;
        unsigned char boolParamCount;
        unsigned char textureCount;
        MaterialParamData<float>** floatParams;
        MaterialParamData<unsigned int>** uintParams;
        MaterialParamData<bool>** boolParams;
	};
}
