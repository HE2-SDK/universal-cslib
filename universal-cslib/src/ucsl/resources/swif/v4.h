#pragma once
#include <ucsl/math.h>

namespace ucsl::resources::swif::v4 {
    // Basic types
    using Vector2 = math::Vector2;
    using Vector3 = math::Position;
    using Matrix34 = math::Matrix34;
    using Matrix44 = math::Matrix44;
    struct Rotation3 { int x; int y; int z; };

    struct Color {
        unsigned char r{};
        unsigned char g{};
        unsigned char b{};
        unsigned char a{};
    };

    struct Colorf {
        float a{};
        float r{};
        float g{};
        float b{};
    };

    enum class EPivotType {
        TOP_LEFT,
        TOP_CENTER,
        TOP_RIGHT,
        CENTER_LEFT,
        CENTER_CENTER,
        CENTER_RIGHT,
        BOTTOM_LEFT,
        BOTTOM_CENTER,
        BOTTOM_RIGHT,
        CUSTOM,
    };

    enum class EOrientation {
        UP,
        LEFT,
        DOWN,
        RIGHT,
    };

    // Animations
    enum class ECurveType : unsigned short {
        TranslationX,
        TranslationY,
        TranslationZ,
        RotationX,
        RotationY,
        RotationZ,
        ScaleX,
        ScaleY,
        ScaleZ,
        MaterialColor,
        Display,
        Width,
        Height,
        VertexColorTopLeft,
        VertexColorTopRight,
        VertexColorBottomLeft,
        VertexColorBottomRight,
        CropIndex0,
        CropIndex1,
        Unknown,
        IlluminationColor,
        MaterialColorR,
        MaterialColorG,
        MaterialColorB,
        MaterialColorA,
        VertexColorTopLeftR,
        VertexColorTopLeftG,
        VertexColorTopLeftB,
        VertexColorTopLeftA,
        VertexColorTopRightR,
        VertexColorTopRightG,
        VertexColorTopRightB,
        VertexColorTopRightA,
        VertexColorBottomLeftR,
        VertexColorBottomLeftG,
        VertexColorBottomLeftB,
        VertexColorBottomLeftA,
        VertexColorBottomRightR,
        VertexColorBottomRightG,
        VertexColorBottomRightB,
        VertexColorBottomRightA,
        IlluminationColorR,
        IlluminationColorG,
        IlluminationColorB,
        IlluminationColorA,
    };

    enum class EInterpolationType : unsigned int {
        CONSTANT,
        LINEAR,
        HERMITE,
        INDIVIDUAL,
    };

    enum class ETrackDataType : unsigned int {
        UNKNOWN,
        FLOAT,
        INDEX,
        BOOL,
        INT,
        COLOR,

        // These are uncertain
        UNK_FLOAT,
        UNK_DOUBLE, // Types say double, but used for what? Won't interpolate linearly.
        UNK_CHAR, // Char? doubt
    };

    enum class ETrackRepeatType : unsigned int {
        ONCE,
        REPEAT,
    };

    // User data
    union SRS_DATA_VALUE_PTR {
        bool* b;
        int* i32;
        unsigned int* u32;
        float* f32;
        const char* str;
    };

    struct SRS_DATA {
        enum class Type {
            BOOL,
            INT,
            UINT,
            FLOAT,
            UNK,
            STRING,
        };

        const char* name{};
        Type type{};
        SRS_DATA_VALUE_PTR value{};
    };

    struct SRS_USERDATA {
        unsigned int count{};
        SRS_DATA* items{};
    };

    struct SRS_KEYFRAME {
        unsigned int frame{};
    };

    template<typename T>
    struct Key : public SRS_KEYFRAME {
        using ValueType = T;

        T value{};
    };

    template<typename T>
    struct KeyLinear : public Key<T> {
    };

    template<typename T>
    struct KeyHermite : public KeyLinear<T> {
        float derivativeIn{};
        float derivativeOut{};
    };

    template<typename T>
    struct KeyIndividual : public KeyHermite<T> {
        EInterpolationType interpolationType{};
    };

    union SRS_KEYFRAME_PTR {
        Key<float>* constantFloat;
        Key<int>* constantIndex;
        Key<int>* constantInt;
        Key<bool>* constantBool;
        Key<Color>* constantColor;

        KeyLinear<float>* linearFloat;
        KeyLinear<int>* linearIndex;
        KeyLinear<int>* linearInt;
        KeyLinear<bool>* linearBool;
        KeyLinear<Color>* linearColor;

        KeyHermite<float>* hermiteFloat;
        KeyHermite<int>* hermiteIndex;
        KeyHermite<int>* hermiteInt;
        KeyHermite<bool>* hermiteBool;
        KeyHermite<Color>* hermiteColor;

        KeyIndividual<float>* individualFloat;
        KeyIndividual<int>* individualIndex;
        KeyIndividual<int>* individualInt;
        KeyIndividual<bool>* individualBool;
        KeyIndividual<Color>* individualColor;
    };

    struct SRS_TRACK {
        ECurveType trackType{};
        unsigned short keyCount{};
        unsigned int flags{};
        unsigned int firstFrame{};
        unsigned int lastFrame{};
        SRS_KEYFRAME_PTR keyFrames{};

        inline EInterpolationType GetInterpolationType() const {
            return static_cast<EInterpolationType>(flags & 0x3);
        }

        inline void SetInterpolationType(EInterpolationType type) {
            flags = (flags & ~0x3) | (static_cast<unsigned int>(type) & 0x3);
        }

        inline ETrackDataType GetDataType() const {
            return static_cast<ETrackDataType>((flags >> 4) & 0xF);
        }

        inline void SetDataType(ETrackDataType type) {
            flags = (flags & ~(0xF << 4)) | ((static_cast<unsigned int>(type) & 0xF) << 4);
        }

        inline ETrackRepeatType GetRepeatType() const {
            return static_cast<ETrackRepeatType>((flags >> 8) & 0x3);
        }

        inline void SetRepeatType(ETrackRepeatType type) {
            flags = (flags & ~(0x3 << 8)) | ((static_cast<unsigned int>(type) & 0x3) << 8);
        }
    };

    struct SRS_MOTION {
        unsigned short castId{};
        unsigned short trackCount{};
        SRS_TRACK* tracks{};
    };

    struct SRS_ANIMATION {
        const char* name{};
        unsigned int id{};
        unsigned int motionCount{};
        unsigned int frameCount{};
        SRS_MOTION* motions{};
        SRS_USERDATA* userData{};
        bool repeat{};
    };

    // Crops
    struct SRS_CROP {
        float left{};
        float top{};
        float right{};
        float bottom{};
    };

    struct SRS_CROPREF {
        short textureListIndex{};
        short textureIndex{};
        short cropIndex{};
    };

    // Textures
    struct SRS_TEXTURE {
        const char* filename{};
        unsigned int id{};
        unsigned short width{};
        unsigned short height{};
        unsigned int flags{};
        unsigned int cropCount{};
        SRS_CROP* crops{};
        SRS_USERDATA* userData{};
    };

    struct SRS_TEXTURELIST {
        const char* name{};
        unsigned int textureCount{};
        SRS_TEXTURE* textures{};
        SRS_USERDATA* userData{};
    };

    // Fonts
    struct SRS_CHARACTER_MAPPING {
        unsigned short character{};
        SRS_CROPREF cropRef{};
    };

    struct SRS_FONT {
        const char* name{};
        unsigned int id{};
        unsigned int unk1{};
        unsigned short characterCount{};
        unsigned short unk3{};
        SRS_CHARACTER_MAPPING* characters{};
        unsigned long long unk4{};
        SRS_USERDATA* userData{};
    };

    // Text
    struct SRS_TEXTDATA {
        enum class VerticalAlignment {
            TOP,
            CENTER,
            BOTTOM,
        };

        unsigned int flags{};
        unsigned int fontIndex{};
        const char* text{}; // UTF-8 encoded
        Vector2 scale{};
        short paddingLeft{};
        short paddingRight{};
        short paddingTop{};
        short paddingBottom{};
        short tracking{};
        short lineHeight{};
        SRS_FONT* font{};

        inline bool GetWrapFlag() const {
            return flags & 0x1;
        }

        inline void SetWrapFlag(bool state) {
            unsigned int mask = 0x1;
            if (state)
                flags |= mask;
            else
                flags &= ~mask;
        }

        inline bool GetDisableLineHeightFlag() const {
            return flags & 0x2;
        }

        inline void SetDisableLineHeightFlag(bool state) {
            unsigned int mask = 0x2;
            if (state)
                flags |= mask;
            else
                flags &= ~mask;
        }

        inline VerticalAlignment GetVerticalAlignment() const {
            return static_cast<VerticalAlignment>((flags >> 4) & 0xF);
        }

        inline void SetVerticalAlignment(VerticalAlignment alignment) {
            flags = (flags & ~0xF0) | ((static_cast<unsigned int>(alignment) & 0xF) << 4);
        }
    };

    // Effects
    enum class EEffectType : unsigned int {
        NONE,
        BLUR,
        REFLECT,
    };

    enum class ERenderMode : unsigned int {
        UNK0,
        OVERRIDE,
        UNK1,
        UNK2,
    };

    enum class ECropBlendMode : unsigned int {
        CROP0,
        CROP1,
        MODULATE,
        ALPHA_BLEND,
    };

    enum class EBlendMode : unsigned int {
        DEFAULT,
        ADD,
        SUBTRACT,
        MULTIPLY,
        DEFAULT_NO_ALPHA,
        OVERRIDE,
    };

    struct SRS_EFFECT {};

    struct SRS_BLUR : public SRS_EFFECT {
        unsigned int unk1{};
        unsigned int unk2{};
        unsigned int cropCount{};
        unsigned int steps{};
        int duration{};
        unsigned int flags{};
        Color color{};

        inline EBlendMode GetBlendMode() const {
            return static_cast<EBlendMode>(flags & 0xF);
        }

        inline void SetBlendMode(EBlendMode mode) {
            flags = (flags & ~0xF) | (static_cast<unsigned int>(mode) & 0xF);
        }

        inline bool GetHideFlag() const {
            return flags & 0x1000;
        }

        inline void SetHideFlag(bool type) {
            if (type)
                flags |= 0x1000;
            else
                flags &= ~0x1000;
        }
    };

    struct SRS_BLUR3D : public SRS_BLUR {};

    struct SRS_REFLECT : public SRS_EFFECT {
        unsigned int unk1{};
        unsigned int unk2{};
        unsigned int unk3{};
        float unk4{};
        float unk5{};
        float unk6{};
        float unk7{};
        unsigned int flags{};
        Color color{};

        inline EBlendMode GetBlendMode() const {
            return static_cast<EBlendMode>(flags & 0xF);
        }

        inline void SetBlendMode(EBlendMode mode) {
            flags = (flags & ~0xF) | (static_cast<unsigned int>(mode) & 0xF);
        }

        inline bool GetHideFlag() const {
            return flags & 0x1000;
        }

        inline void SetHideFlag(bool type) {
            if (type)
                flags |= 0x1000;
            else
                flags &= ~0x1000;
        }
    };

    struct SRS_REFLECT3D : public SRS_REFLECT {};

    // Transforms
    struct SRS_TRS_BASE {
        Color materialColor{};
        Color illuminationColor{};
        bool display{};
    };

    struct SRS_TRS2D : public SRS_TRS_BASE {
        Vector2 position{};
        int rotation{};
        Vector2 scale{ 1.0f, 1.0f };
        unsigned int unkX{};
        unsigned int unkY{};
    };

    struct SRS_TRS3D : public SRS_TRS_BASE {
        Vector3 position{};
        Rotation3 rotation{};
        Vector3 scale{ 1.0f, 1.0f, 1.0f };
    };

    // Casts
    union SRS_EFFECT_PTR {
        SRS_BLUR3D* blur;
        SRS_REFLECT3D* reflect;
    };

    struct SRS_IMAGECAST {
        unsigned int flags{};
        Vector2 size{};
        Vector2 pivot{};
        Color vertexColorTopLeft{};
        Color vertexColorBottomLeft{};
        Color vertexColorTopRight{};
        Color vertexColorBottomRight{};
        short cropIndex0{};
        short cropIndex1{};
        short cropRef0Count{};
        short cropRef1Count{};
        SRS_CROPREF* cropRefs0{};
        SRS_CROPREF* cropRefs1{};
        SRS_TEXTDATA* textData{};
        unsigned int effectType{};
        SRS_EFFECT_PTR effectData{};

        inline EEffectType GetEffectType() const {
            return static_cast<EEffectType>(effectType & 0xF);
        }

        inline void SetEffectType(EEffectType type) {
            effectType = (effectType & ~0xF) | (static_cast<unsigned int>(type) & 0xF);
        }

        // 0xF
        inline EBlendMode GetBlendMode() const {
            return static_cast<EBlendMode>(flags & 0xF);
        }

        inline void SetBlendMode(EBlendMode mode) {
            flags = (flags & ~0xF) | (static_cast<unsigned int>(mode) & 0xF);
        }

        // 0x10
        inline bool GetMirrorHorizontallyFlag() const {
            return flags & 0x10;
        }

        inline void SetMirrorHorizontallyFlag(bool enabled) {
            if (enabled)
                flags |= 0x10;
            else
                flags &= ~0x10;
        }

        // 0x20
        inline bool GetMirrorVerticallyFlag() const {
            return flags & 0x20;
        }

        inline void SetMirrorVerticallyFlag(bool enabled) {
            if (enabled)
                flags |= 0x20;
            else
                flags &= ~0x20;
        }

        // 0xC0
        inline EOrientation GetOrientation() const {
            return static_cast<EOrientation>((flags >> 6) & 0x3);
        }

        inline void SetOrientation(EOrientation orientation) {
            flags = (flags & ~(0x3 << 6)) | ((static_cast<unsigned int>(orientation) & 0x3) << 6);
        }

        // 0x100
        inline bool GetTextFlag() const {
            return flags & 0x100;
        }

        inline void SetTextFlag(bool enabled) {
            if (enabled)
                flags |= 0x100;
            else
                flags &= ~0x100;
        }

        // 0x600
        inline ERenderMode GetRenderMode() const {
            return static_cast<ERenderMode>((flags >> 9) & 0x3);
        }

        inline void SetRenderMode(ERenderMode mode) {
            flags = (flags & ~(0x3 << 9)) | ((static_cast<unsigned int>(mode) & 0x3) << 9);
        }

        // 0x1800
        inline ECropBlendMode GetCropBlendMode() const {
            return static_cast<ECropBlendMode>((flags >> 11) & 0x3);
        }

        inline void SetCropBlendMode(ECropBlendMode mode) {
            flags = (flags & ~(0x3 << 11)) | ((static_cast<unsigned int>(mode) & 0x3) << 11);
        }

        // 0x2000
        inline bool GetUnkFlag1() const {
            return flags & 0x2000;
        }

        inline void SetUnkFlag1(bool enabled) {
            if (enabled)
                flags |= 0x2000;
            else
                flags &= ~0x2000;
        }

        // 0x780000
        inline EPivotType GetPivotType() const {
            return static_cast<EPivotType>((flags >> 19) & 0xF);
        }

        inline void SetPivotType(EPivotType pivotType) {
            flags = (flags & ~(0xF << 19)) | ((static_cast<unsigned int>(pivotType) & 0xF) << 19);
        }

        // 0x20000000
        inline bool GetUseCrop0Transparency() const {
            return flags & 0x20000000;
        }

        inline void SetUseCrop0Transparency(bool enabled) {
            if (enabled)
                flags |= 0x20000000;
            else
                flags &= ~0x20000000;
        }
    };

    struct SRS_SLICE {
        unsigned int flags{};
        Vector2 fixedSize{};
        Color materialColor{};
        Color illuminationColor{};
        Color vertexColorTopLeft{};
        Color vertexColorBottomLeft{};
        Color vertexColorTopRight{};
        Color vertexColorBottomRight{};
        short cropIndex0{};
    };

    struct SRS_SLICECAST {
        unsigned int flags{};
        Vector2 size{};
        Vector2 pivot{};
        Color vertexColorTopLeft{};
        Color vertexColorBottomLeft{};
        Color vertexColorTopRight{};
        Color vertexColorBottomRight{};
        Vector2 fixedSize{};
        short sliceHorizontalCount{};
        short sliceVerticalCount{};
        short horizontalFixedCount{};
        short verticalFixedCount{};
        short cropRef0Count{};
        SRS_CROPREF* cropRefs0{};
        unsigned int effectType{};
        SRS_EFFECT_PTR effectData{};

        inline EEffectType GetEffectType() const {
            return static_cast<EEffectType>(effectType & 0xF);
        }

        inline void SetEffectType(EEffectType type) {
            effectType = (effectType & ~0xF) | (static_cast<unsigned int>(type) & 0xF);
        }

        // 0xF
        inline EBlendMode GetBlendMode() const {
            return static_cast<EBlendMode>(flags & 0xF);
        }

        inline void SetBlendMode(EBlendMode mode) {
            flags = (flags & ~0xF) | (static_cast<unsigned int>(mode) & 0xF);
        }

        // 0x10
        inline bool GetMirrorHorizontallyFlag() const {
            return flags & 0x10;
        }

        inline void SetMirrorHorizontallyFlag(bool enabled) {
            if (enabled)
                flags |= 0x10;
            else
                flags &= ~0x10;
        }

        // 0x20
        inline bool GetMirrorVerticallyFlag() const {
            return flags & 0x20;
        }

        inline void SetMirrorVerticallyFlag(bool enabled) {
            if (enabled)
                flags |= 0x20;
            else
                flags &= ~0x20;
        }

        // 0xC0
        inline EOrientation GetOrientation() const {
            return static_cast<EOrientation>((flags >> 6) & 0x3);
        }

        inline void SetOrientation(EOrientation orientation) {
            flags = (flags & ~(0x3 << 6)) | ((static_cast<unsigned int>(orientation) & 0x3) << 6);
        }

        // 0x100
        inline bool GetTextFlag() const {
            return flags & 0x100;
        }

        inline void SetTextFlag(bool enabled) {
            if (enabled)
                flags |= 0x100;
            else
                flags &= ~0x100;
        }

        // 0x600
        inline ERenderMode GetRenderMode() const {
            return static_cast<ERenderMode>((flags >> 9) & 0x3);
        }

        inline void SetRenderMode(ERenderMode mode) {
            flags = (flags & ~(0x3 << 9)) | ((static_cast<unsigned int>(mode) & 0x3) << 9);
        }

        // 0x1800
        inline ECropBlendMode GetCropBlendMode() const {
            return static_cast<ECropBlendMode>((flags >> 11) & 0x3);
        }

        inline void SetCropBlendMode(ECropBlendMode mode) {
            flags = (flags & ~(0x3 << 11)) | ((static_cast<unsigned int>(mode) & 0x3) << 11);
        }

        // 0x2000
        inline bool GetUnkFlag1() const {
            return flags & 0x2000;
        }

        inline void SetUnkFlag1(bool enabled) {
            if (enabled)
                flags |= 0x2000;
            else
                flags &= ~0x2000;
        }

        // 0x780000
        inline EPivotType GetPivotType() const {
            return static_cast<EPivotType>((flags >> 19) & 0xF);
        }

        inline void SetPivotType(EPivotType pivotType) {
            flags = (flags & ~(0xF << 19)) | ((static_cast<unsigned int>(pivotType) & 0xF) << 19);
        }

        // 0x20000000
        inline bool GetUseCrop0Transparency() const {
            return flags & 0x20000000;
        }

        inline void SetUseCrop0Transparency(bool enabled) {
            if (enabled)
                flags |= 0x20000000;
            else
                flags &= ~0x20000000;
        }
    };

    struct SRS_LAYER;
    struct SRS_REFERENCECAST {
        SRS_LAYER* layer{};
        unsigned int flags{};
        unsigned int animationId{};
        unsigned int animationFrame{};
        int unk2{};

        // 0x2
        inline bool GetBlendEnabledFlag() const {
            return flags & 0x2;
        }

        inline void SetBlendEnabledFlag(bool enabled) {
            if (enabled)
                flags |= 0x2;
            else
                flags &= ~0x2;
        }

        // 0x3C
        inline EBlendMode GetBlendMode() const {
            return static_cast<EBlendMode>((flags >> 2) & 0xF);
        }

        inline void SetBlendMode(EBlendMode mode) {
            flags = (flags & ~(0xF << 2)) | ((static_cast<unsigned int>(mode) & 0xF) << 2);
        }
    };

    union SRS_CAST_PTR {
        void* none;
        SRS_IMAGECAST* image;
        SRS_SLICECAST* slice;
        SRS_REFERENCECAST* reference;
    };

    enum class ESliceConstraint {
        CENTER,
        UNK1,
        UNK2,
        UNK3,
        UNK4,
        UNK5,
        UNK6,
        UNK7,
        UNK8,
        UNK9,
    };

    struct SRS_CASTNODE {
        enum class Type {
            NORMAL,
            IMAGE,
            SLICE,
            REFERENCE,
        };

        const char* name{};
        unsigned int id{};
        unsigned int flags{};
        SRS_CAST_PTR data{};
        short childIndex{ -1 };
        short siblingIndex{ -1 };
        SRS_USERDATA* userData{};

        // 0xF
        inline Type GetType() const {
            return static_cast<Type>(flags & 0xF);
        }

        inline void SetType(Type type) {
            flags = (flags & ~0xF) | (static_cast<unsigned int>(type) & 0xF);
        }

        // 0x20
        inline bool GetTransformMaterialColorFlag() const {
            return flags & 0x20;
        }

        inline void SetTransformMaterialColorFlag(bool enabled) {
            if (enabled)
                flags |= 0x20;
            else
                flags &= ~0x20;
        }

        // 0x40
        inline bool GetTransformDisplayFlag() const {
            return flags & 0x40;
        }

        inline void SetTransformDisplayFlag(bool enabled) {
            if (enabled)
                flags |= 0x40;
            else
                flags &= ~0x40;
        }

        // 0x80
        inline bool GetTransformIlluminationColorFlag() const {
            return flags & 0x80;
        }

        inline void SetTransformIlluminationColorFlag(bool enabled) {
            if (enabled)
                flags |= 0x80;
            else
                flags &= ~0x80;
        }

        // 0x100
        inline bool GetDisablePositionFlag() const {
            return flags & 0x100;
        }

        inline void SetDisablePositionFlag(bool enabled) {
            if (enabled)
                flags |= 0x100;
            else
                flags &= ~0x100;
        }

        // 0x200
        inline bool GetDisableRotationFlag() const {
            return flags & 0x200;
        }

        inline void SetDisableRotationFlag(bool enabled) {
            if (enabled)
                flags |= 0x200;
            else
                flags &= ~0x200;
        }

        // 0x400
        inline bool GetDisableScaleFlag() const {
            return flags & 0x400;
        }

        inline void SetDisableScaleFlag(bool enabled) {
            if (enabled)
                flags |= 0x400;
            else
                flags &= ~0x400;
        }

        // 0x1000
        inline bool GetHideFlag() const {
            return flags & 0x1000;
        }

        inline void SetHideFlag(bool enabled) {
            if (enabled)
                flags |= 0x1000;
            else
                flags &= ~0x1000;
        }

        // 0x4000
        inline bool GetTransformCropIndex0Flag() const {
            return flags & 0x4000;
        }

        inline void SetTransformCropIndex0Flag(bool enabled) {
            if (enabled)
                flags |= 0x4000;
            else
                flags &= ~0x4000;
        }

        // 0x8000
        inline bool GetTransformCropIndex1Flag() const {
            return flags & 0x8000;
        }

        inline void SetTransformCropIndex1Flag(bool enabled) {
            if (enabled)
                flags |= 0x8000;
            else
                flags &= ~0x8000;
        }

        // 0x10000
        inline bool GetTransformTranslationFlag() const {
            return flags & 0x10000;
        }

        inline void SetTransformTranslationFlag(bool enabled) {
            if (enabled)
                flags |= 0x10000;
            else
                flags &= ~0x10000;
        }

        // 0x20000
        inline bool GetTransformRotationFlag() const {
            return flags & 0x20000;
        }

        inline void SetTransformRotationFlag(bool enabled) {
            if (enabled)
                flags |= 0x20000;
            else
                flags &= ~0x20000;
        }

        // 0x40000
        inline bool GetTransformScaleFlag() const {
            return flags & 0x40000;
        }

        inline void SetTransformScaleFlag(bool enabled) {
            if (enabled)
                flags |= 0x40000;
            else
                flags &= ~0x40000;
        }

        // 0x780000
        inline ESliceConstraint GetSliceConstraint() const {
            return static_cast<ESliceConstraint>((flags >> 19) & 0xF);
        }

        inline void SetSliceConstraint(ESliceConstraint pivotType) {
            flags = (flags & ~(0xF << 19)) | ((static_cast<unsigned int>(pivotType) & 0xF) << 19);
        }
    };

    // Layers
    union SRS_TRS_PTR {
        SRS_TRS2D* transforms2d;
        SRS_TRS3D* transforms3d;
    };

    enum class EResolutionMode {
        DIRECT,
        RESOLUTION_INDEPENDENT,
    };

    struct SRS_LAYER {
        const char* name{};
        unsigned int id{};
        unsigned int flags{};
        int castCount{};
        SRS_CASTNODE* casts{};
        SRS_TRS_PTR transforms{};
        int animationCount{};
        SRS_ANIMATION* animations{};
        int currentAnimationIndex{};
        SRS_USERDATA* userData{};

        inline bool Is3D() const {
            return flags & 0x1;
        }

        inline void SetIs3D(bool enabled) {
            if (enabled)
                flags |= 0x1;
            else
                flags &= ~0x1;
        }

        inline EResolutionMode GetResolutionMode() const {
            return static_cast<EResolutionMode>((flags >> 4) & 0xF);
        }

        inline void SetResolutionMode(EResolutionMode pivotType) {
            flags = (flags & ~(0xF << 4)) | ((static_cast<unsigned int>(pivotType) & 0xF) << 4);
        }

        inline bool GetHideFlag() const {
            return flags & 0x100;
        }

        inline void SetHideFlag(bool enabled) {
            if (enabled)
                flags |= 0x100;
            else
                flags &= ~0x100;
        }
    };

    // Cameras
    struct SRS_CAMERA {
        const char* name{};
        unsigned int id{};
        Vector3 position{};
        Vector3 target{};
        int fov{};
        float nearPlane{};
        float farPlane{};
        float unk1{};
        SRS_USERDATA* userData{};
    };

    // Scenes
    struct SRS_SCENE {
        const char* name{};
        unsigned int id{};
        unsigned int flags{};
        int layerCount{};
        SRS_LAYER* layers{};
        unsigned short cameraCount;
        unsigned short currentCameraIndex{};
        SRS_CAMERA* cameras{};
        Color backgroundColor{};
        Vector2 resolution{};
        SRS_USERDATA* userData{};

        inline bool GetHideFlag() const {
            return flags & 0x1;
        }

        inline void SetHideFlag(bool enabled) {
            if (enabled)
                flags |= 0x1;
            else
                flags &= ~0x1;
        }
    };

    // Projects
    struct SRS_PROJECT {
        const char* name{};
        unsigned short sceneCount{};
        unsigned short unk1{};
        unsigned short textureListCount{};
        unsigned short fontCount{};
        SRS_SCENE* scenes{};
        SRS_TEXTURELIST* textureLists{};
        SRS_FONT* fonts{};
        SRS_CAMERA camera{};
        int startFrame{};
        int endFrame{};
        float frameRate{};
        SRS_USERDATA* userData{};
    };
}
