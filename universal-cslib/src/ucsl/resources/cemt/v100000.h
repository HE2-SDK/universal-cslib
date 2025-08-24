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

	struct Texture {
		char name[128];
		void* texture; // hh::needle::Texture
		char gap[0xA8];
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
		Texture textures[4];
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
		csl::math::Position position;
		csl::math::Position rotation;
		csl::math::Position scale;
		unsigned int cameraFlags; //changes how the emitter faces the camera (billboard style, using the values etc)
		Collection<Unk1Data>* unk1;
		Collection<Unk2Data>* unk2;
		Collection<Unk3Data>* unk3;
		bool positionRandom;
		csl::math::Position randomPosition;
		float spread;
		float unkE4;
		float spreadAngle;
		unsigned char unkEC;
		unsigned char unkED;
		unsigned char unkEE; //makes the emitter last longer seemingly?
		unsigned char unkEF;
		Collection<Unk4Data>* unk4;
		Collection<Unk5Data>* unk5;
		Collection<Unk6Data>* unk6;
		Collection<Unk7Data>* unk7;
		unsigned int unk7a; //when 1, turns the emitter off
		float frequency;
		float unk7c;
		float emitterCount;
		float frequencyControlUnk; //seems to make a sinewave of some sort, the higher this is, the longer it takes to respawn this emitter, it keeps going from slow to fast back to slow, like a sinewave
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
		float emitSpeed[6];
		float emitVectorUnk;
		uint32_t gap4a;
		float emitVector[3];
		float unk4b;
		float unk4c;
		char gap4[0x14];
		Collection<Unk9Data>* unk9;
		Collection<Unk10Data>* unk10;
		Collection<Unk11Data>* unk11;
		Collection<Unk12Data>* unk12;
		Collection<Unk13Data>* unk13;
		Collection<Unk14Data>* unk14;
		Collection<Unk15Data>* unk15;
		uint32_t unk16;
		uint32_t unk17;
		uint32_t unk18;
		char gap5[0x10];
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
