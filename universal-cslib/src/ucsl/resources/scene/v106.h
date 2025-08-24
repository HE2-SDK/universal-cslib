#pragma once
#include <ucsl/bitset.h>
#include <ucsl/math.h>

//reversed from files, by ik-01!

namespace ucsl::resources::scene::v106{
    struct TrackNode {
		struct CurveData {
			struct Keyframe {
				float frame;
				float value;
				ucsl::math::Vector2 smoothingX; //Not sure on both of these
				ucsl::math::Vector2 smoothingY; //Not sure on both of these
				char flags0;
				char flags1;
			};

			Keyframe* keyframes;
			unsigned int keyframeCount;
			int unk0;
			long long unk1;
		};

		int trackType;
		float frameStart;
		float frameEnd;
		long long unk;
		CurveData* curveData;
	};

	struct TimelineNode {
		enum class NodeType : unsigned int {
			ANIMATION,
			EFFECT,
			SCENE_CONTROL,
			SCENE_CONTROL_MODEL
		};

		TrackNode** trackNodes;
		unsigned int trackNodeCount;
		NodeType type;
		int index;
	};

	struct Timeline {
		TimelineNode** nodes;
		unsigned int count;
	};

	struct SceneGraphNode {
		const char* unkName;
		int index;
		const char* nodeName;
		int parentIndex;
		int unk0;
		int unk1;
		int unk2;
	};

	struct SceneNode {
		enum class Flag : unsigned short {
			UNK0,
			UNK1,
			UNK2,
			UNK3,
			VISIBLE_NODE,
			UNK5,
			UNK6, //Effects use it, and when a scene graph node has a parent
			UNK7,
			UNK8,
			UNK9,
			UNK10,
			UNK11,
			UNK12,
			UNK13,
			UNK14,
			UNK15,
		};

		enum class ResourceType : unsigned char {
			ResNull,
			ResModel,
			ResMirageTerrainModel,
			ResMirageTerrainInstanceInfo,
			ResEffect,
			ResSkeleton,
			ResAnimSkeleton,
			ResAnimMaterial,
			ResAnimTexPat,
			ResAnimTexSrt,
			ResAnimVis,
			ResAnimCameraContainer,
			ResAnimLightContainer,
			ResMirageList,
			ResExternal
		};

		const char* nodeName;
		const char* resourceName;
		const char* resourceDirectory;
		ucsl::math::Vector3 position;
		ucsl::math::Vector3 rotation;
		ucsl::math::Vector3 scale;
		ResourceType resourceType;
		bool isAnimation;
		ucsl::bits::Bitset<Flag> flags;
		long long unk;
	};

	struct ControlNode {
		enum class Type : unsigned int {
			NONE,
			BOOL,
			FLOAT,
			DOUBLE,
			INTEGER,
			STRING,
			VECTOR3,
			UNKNOWN2
		};

		union Value{
            bool b;
            float f;
            double d;
        	int i;
            const char* s;
        };

		const char* nodeName;
		const char* parameterName;
		const char* nodeName1;
		Type type;
		Value value;
	};

	struct SceneControl {
		ControlNode** nodes;
		long long unk;
		unsigned long long nodeCount;
		float fps;
	};

	struct SceneData {
		char signature[4];
		int version;
		SceneControl* sceneControl;
		SceneNode* sceneNodes;
		SceneGraphNode* sceneGraphNodes;
		long long unk;
		Timeline* timeline;
		unsigned int sceneNodeCount;
		unsigned int sceneGraphCount;
	};
}
