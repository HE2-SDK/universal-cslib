#pragma once
#include <ucsl/bitset.h>

namespace ucsl::resources::density_setting::v11 {
	// see 0x140A0DA41 in rangers 1.41
	enum class ModelFlag : unsigned int {
		IS_SHADOW_CASTER,
		NO_SHADOW_CAST
	};

	struct Model {
		const char* name;
		ucsl::bits::Bitset<ModelFlag> flags;
	};

	struct LodGroup {
		unsigned int footPoint;
		unsigned int flags;
		unsigned int lodGroupReferenceOffset; // offset in lod group references
		unsigned int lodGroupReferenceCount;
		unsigned int collisionDataIndex1;
		unsigned int collisionDataIndex2;
		unsigned int shadowLodGroupReferenceOffset; // offset in lod group references
		unsigned int shadowLodGroupReferenceCount;
		float angle;
		math::Position offset;
		math::Position aabb;
		unsigned int id;
	};

	struct LodGroupReference {
		unsigned int index;
		float length;
		unsigned int flags;
		unsigned int padding1;
	};

	struct Biome {
		unsigned int flags;
		unsigned int biomeReferenceOffset;
		unsigned int biomeReferenceCount;
		unsigned int minBiomeReferenceOffset;
	};

	struct BiomeReference {
		unsigned int index;
		unsigned int flags;
		unsigned int probability;
		unsigned int copyFootPoint;
		float scaleMin;
		float scaleMax;
		unsigned int hsvPh;
		unsigned int hsvSv;
		unsigned int upper;
		unsigned int padding1;
		unsigned int padding2;
		unsigned int padding3;
	};

	// grass names
	struct Unk6Data {
		const char* name;
		unsigned int unk1;
		unsigned int unk2;
		unsigned int unk3;
		unsigned int unk4;
	};

	// further to be reversed at 0x14C4D884F
	struct CollisionData {
		math::Position offset;
		math::Rotation rotation;
		float radius;
		unsigned int hitFlags;
		unsigned int attri;
		float param[4];
		unsigned char idUpperType;
		unsigned char idUpperUnk1;
		unsigned short idUpperUnk2;
		unsigned int idLower;
		int soundEffectIndex; // see 0x1543216E0 and 0x14C4D8745
	};

	// unk9, 10, 11 and 12 seem unused
	struct DensitySettingData {
		unsigned int magic;
		unsigned int version;
		float width;
		float height;
		unsigned int unk1;
		unsigned int unk2;
		unsigned int lodUnks[32];
		float lodRanges[32];
		const char* areaTexture;
		const char* layerTexture;
		const char* colorTexture;
		const char* scaleTexture;
		Model* models;
		int modelCount;
		LodGroup* lodGroups;
		int lodGroupCount;
		LodGroupReference* lodGroupReferences;
		int lodGroupReferenceCount;
		Biome* biomes;
		int biomeCount;
		BiomeReference* biomeReferences;
		int biomeReferenceCount;
		Unk6Data* unk6s;
		int unk6Count;
		CollisionData* collisionDatas;
		int collisionDataCount;
		const char** collisionResources;
		int collisionResourceCount;
		unsigned int* unk8;
		int unk8Count;
		char* unk9s;
		int unk9Count;
		char* unk10s;
		int unk10Count;
		char* unk11s;
		int unk11Count;
		char* unk12s;
		int unk12Count;
		int* lodGroupSoundEffectIndices;
		int lodGroupSoundEffectIndexCount;
		const char** soundEffects;
		int soundEffectCount;
	};
}
