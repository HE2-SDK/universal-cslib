#pragma once

namespace ucsl::resources::cemt::v100000 {
	template<typename T>
	struct Collection {
		uint64_t gap1;
		uint16_t gap2;
		unsigned short count;
		// followed by array of T*
	};

	struct Unk1Data {};

	struct Unk2Data {};

	struct Unk3Data {};

	struct Unk4Data {};

	struct Unk5Data {};

	struct Unk6Data {};

	struct Unk7Data {};

	struct Unk8Data {};

	struct Unk9Data {};

	struct Unk10Data {};

	struct Unk11Data {};

	struct Unk12Data {};

	struct Unk13Data {};

	struct Unk14Data {};

	struct Unk15Data {};

	struct Unk16Data {};

	struct Unk17Data {};

	struct ChildEffect {
		uint8_t flags;
		char gap[0x1F];
		char name[128];
		void* param; // EffectParam
	};

	struct ModifierParam {
		enum class Type : unsigned char {
			GRAVITY_VECTOR,
			GRAVITY_OTHER,
			UNK0,
			UNK1,
			VORTEX,
			UNK2,
			UNK3,
			UNK4,
			UNK5,
			FLUCTUATION,
		};

		struct GravityVectorSettings {
			float scale;
			float value[3];
		};

		struct GravityOtherSettings {
			float value;
		};

		struct VortexSettings {
			float unk1;
			float unk2;
			float unk3;
		};

		struct FluctuationSettings {
			bool unk0;
			bool unk1;
			unsigned char unk2;
			float unk3;
			float unk4;
			float unk5;
			float unk6;
			float unk7;
		};

		union Settings {
			GravityVectorSettings gravityVector;
			GravityOtherSettings gravityOther;
			VortexSettings vortex;
			FluctuationSettings fluctuation;
			unsigned int maxSize[8];
		};

		bool enabled;
		Type type;
		bool option1;
		bool option2;
		Settings settings;
	};

	struct TextureParam {
		enum class UVFlags {
			UNK0, //0x01
			UNK1, //0x02
			UNK2, //0x04
			UNK3, //0x08
			UNK4, //0x10
			UNK5, //0x20
			UNK6, //0x40
			UNK7, //0x80
			UNK8, //0x100
			SCROLL_U, //0x200
			SCROLL_V, //0x400
			UNK11, //0x800
			UNK12, //0x1000
			UNK13, //0x2000
			SCALE_U, //0x4000
			SCALE_V //0x8000
		};

		char name[128];
		int flags0;
		csl::ut::Bitset<UVFlags> flags1;
		char unk0;
		char unk1;
		char unk2;
		char unk3;
		short unk4;
		short unk5;
		short unk6;
		short unk7;
		int unk8;
		int unk9;
		int unk10;
		float scrollColorX;
		float scrollColorY;
		float scrollColorRandomX;
		float scrollColorRandomY;
		float unk11;
		float unk12;
		float unk13;
		float unk14;
		float uvTilingX;
		float uvTilingY;
		float uvRotation;
		float uvOffsetX;
		float uvOffsetY;
		int unk15;
		Collection<Unk1Data>* unk16;
		int64_t unk17;
		Collection<Unk1Data>* unk18;
		float unk19;
		float unk20;
		char unk21[32];
		void* texture;
		int64_t unk22;
	};

	struct Table {
		struct Settings {
		};

		struct Unk1 {
			float value;
			float unk1;
		};

		Unk1 red1;
		Unk1 green1;
		Unk1 blue1;
		Unk1 alpha1;
		Unk1 red2;
		Unk1 green2;
		Unk1 blue2;
		Unk1 alpha2;
		float unk6;
		float unk8;
		char gap3[0x8];
		void* unk3;
		char gap4[0x8];
		void* unk4;
		void* unk7;
		void* unk5;
		char gap6[0x8];
		Unk1 reds1[32];
		Unk1 greens1[32];
		Unk1 blues1[32];
		Unk1 reds2[32];
		Unk1 greens2[32];
		Unk1 blues2[32];
		Unk1 unks1[32];
		Unk1 unks2[32];
		Unk1 alphas1[32];
		Unk1 alphas2[32];
		bool count1;
		bool count2;
		bool unk1;
		bool unk2;
		unsigned char alphaCount1;
		unsigned char alphaCount2;
		char gap7[0x12];
	};

	struct ElementParam {
		float initialRotation[6];
		unsigned int unk18a0;
		char gap6aa2[0x20];
		float angularVelocity[6];
		unsigned int unk18a01;
		float emitSpeed2[4];
		uint64_t unk18a02;
		unsigned int unkCount2;
		char gap6a0[0x18];
		unsigned int unkCount3;
		Table tables[2];
		float fps;
		float unk19a23;
		unsigned int unk18a6;
		unsigned int unk18a8;
		unsigned int unk18a1;
		float unk18a2;
		char gap6aa[0x14];
		bool unk18a;
		char gap6b[0x81];
		char modelName[128];
		char gap7a0[0x100];
		unsigned short particleTypeOrSomething; // if 2 or 3 it loads particleInfo1, if 4 it loads particleInfo1, particleInfo2, particleInfo3, see 0x140FDC9B8 in rangers 1.42
		unsigned int particleInfo1;
		unsigned int particleInfo2;
		unsigned char unk18a3;
		unsigned char particleInfo3;
		char gap7[0x16];
		TextureParam textures[4];
		unsigned int textureCount;
		ChildEffect childEffects[16];
		ModifierParam modifiers[8];
		char gap7a[0x140];
		unsigned int flags3;
		char gap7b[0x44];
		unsigned int flags4; // 0x1 = is gpu rendering?
		char vectorFieldName[128];
		math::Position vectorFieldSize;
		char gap7c2[0x3C];
		math::Position vectorFieldParam;
		char gap7c1[0xC];
		math::Position depthCollision;
		char gap7c[0x18];
		void* gpuElementParamTexture;
		void* gpuOtherTexture;
		unsigned int flags2;
		uint32_t gap8;
		uint8_t unkCount;
		uint8_t unk19;
		char gap9[2];

		char gap10[0x8AC];
	};

	struct EmitterParam {
		enum class Shape : unsigned int {
			POINT,
			SPHERE,
			DISC,
			CYLINDER,
			LINE,
			TORUS,
			CUBE,
			FIXED = 8
		};

		enum class InheritTransformFlags : unsigned int {
			TRANSLATION_RAW, // ignore world scale
			ROTATION, // parent rotation
			TRANSLATION, // add parent translation scaled by inheritRatio
			TRANSLATION_RELATIVE, // apply relative translation
			INHERIT_DIRECT // sets the matrix to parents
		};

		enum class UnkFlags : unsigned int {
			UNK0, // sets Emitter::flags to |= 0x20
			UNK1,
			UNK2,
			UNK3,
			USE_TEXTURES
		};

		enum class CullMode : unsigned int {
			UNK0,
			UNK1,
			UNK2
		};

		csl::math::Position position;
		csl::math::Position rotation;
		csl::math::Position scale;
		unsigned int cameraFlags; //changes how the emitter faces the camera (billboard style, using the values etc)
		Collection<Unk1Data>* unk1;
		Collection<Unk2Data>* unk2;
		Collection<Unk3Data>* unk3;
		Shape shape;
		csl::math::Position randomTransform;
		float spread;
		float unkE4;
		float spreadAngle;
		bool useRadialDistribution;
		bool consistentSpreadAngle;
		bool useAngularSubdivisions;
		unsigned char numSubDivisions;
		Collection<Unk4Data>* unk4;
		Collection<Unk5Data>* unk5;
		Collection<Unk6Data>* unk6;
		Collection<Unk7Data>* unk7;
		bool disabled;
		float frequency;
		float frequencyRandomness;
		float emitterCount;
		float emitterCountRandomness;
		int unk124;
		float unk128;
		float unk12C;
		float unk130; //acts like transparency on some particles
		float duration;
		float startDelay;
		float fadeSpeed;
		Collection<Unk8Data>* unk8;
		uint32_t unk7f1;
		float lifeEndTime;
		float emitSpeed[3];
		float unkShapeRelated0;
		float shapeRadius;
		float unkShapeRelated1;
		float emitSize;
		uint32_t gap4a;
		float emitVector[3];
		float unk4b;
		float unk4c;
		float unk4d;
		Collection<Unk9Data>* unk9;
		Collection<Unk10Data>* unk10;
		Collection<Unk11Data>* unk11;
		Collection<Unk12Data>* unk12;
		Collection<Unk13Data>* unk13;
		Collection<Unk14Data>* unk14;
		Collection<Unk15Data>* unk15;
		int unk16;
		unsigned int unkType;
		CullMode cullMode;
		ucsl::bits::Bitset<InheritTransformFlags> inheritTransformFlags;
		float inheritRatio;
		uint32_t unk17;
		uint32_t unk18;
		ucsl::bits::Bitset<UnkFlags> unkFlags;
		char gap5[0x09];
		unsigned char flags1;
		char gap6[0x3B];
		uint32_t randomSeed;
		char gap6aa1[0x4C];
		ElementParam elementParam;
	};

	struct EffectParam {
		char magic[3];
		bool resolved;
		unsigned char majorVersion;
		unsigned char minorVersion;
		unsigned char patchVersion;
		unsigned char buildVersion;
		char name[0x80];
		unsigned int animationBufferSize;
		unsigned int numEffects; // 8C



	};
}
