#pragma once
#include <ucsl/bitset.h>
#include <ucsl/math.h>

// Based on research by ik-01!

namespace ucsl::resources::scene::v106 {
    struct TrackNode {
		struct CurveData {
			enum class InterpolationType : unsigned int {
				HERMITE = 1,
				LINEAR
			};

			enum class PrePostBehavior : unsigned int {
				CLAMP,
				LOOP,
				LOOP1, // acts identically as LOOP...
				BOUNCE,
				EXTRAPOLATION
			};

			struct Keyframe {
				float frame;
				float value;
				ucsl::math::Vector2 smoothingX;
				ucsl::math::Vector2 smoothingY;
				char flags0;
				char flags1;
				int unk0;
			};

			Keyframe* keyframes;
			unsigned int keyframeCount;
			InterpolationType interpolationType;
			PrePostBehavior preBehavior;
			PrePostBehavior postBehavior;
		};

		int trackType; // if not zero, frame end is endless, and in file it's int
		float frameStart;
		float frameEnd;
		int unk0;
		int unk1; // if track type 0, and this is 2, frame end is endless, and in file it's int
		int unk2;
		CurveData* curveData;
	};

	struct TimelineNode {
		enum class NodeType : unsigned int {
			ANIMATION,
			EFFECT,
			SCENE_CONTROL,
			SCENE_CONTROL_MODEL
		};

		struct Animation {
			const char* name;
			unsigned int sceneNodeIndex;
		};

		union Value {
			unsigned int sceneControl; // Control Node Index
			Animation animation;
		};

		TrackNode** trackNodes;
		unsigned int trackNodeCount;
		NodeType type;
		Value value;
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
			ResTerrainModel,
			ResMirageTerrainInstanceInfo,
			ResEffect,
			ResSkeleton,
			ResAnimation,
			ResAnimMaterial,
			ResAnimTexPat,
			ResAnimTexSrt,
			ResAnimVis,
			ResAnimCameraContainer,
			ResAnimLightContainer,
			ResMirageLight,
			ResExternal
		};

		const char* nodeName;
		const char* resourceName;
		const char* resourceDirectory;
		ucsl::math::Position position;
		ucsl::math::Position rotation;
		ucsl::math::Position scale;
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
			UNKNOWN2 // if this is set, the node gets treated as string?? @ ResScene Load func
		};

		union Value {
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
		char unk0;
		char unk1;
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
