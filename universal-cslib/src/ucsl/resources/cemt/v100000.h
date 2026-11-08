#pragma once
#include <ucsl/bitset.h>
#include <ucsl/math.h>

namespace ucsl::resources::cemt::v100000 {
	struct EffectParam;

	struct JitteredValue {
		float value;
		float jitter;
	};

	struct JitteredVector3 {
		JitteredValue x;
		JitteredValue y;
		JitteredValue z;
	};

	struct AnimationKeyframeParam {
		enum class InterpolationType : unsigned short {
			CONSTANT,
			LINEAR,
			HERMITE,
		};

		unsigned short frame;
		InterpolationType interpolationType;
		float derivativeIn;
		float derivativeOut;
		JitteredValue value;
	};

	struct AnimationTrackParam {
		enum class RandomnessFlag : unsigned char {
			DISABLE_RANDOMNESS,
		};

		unsigned char index;
		bits::Bitset<RandomnessFlag> randomnessFlag;
		short keyframeCount;
		AnimationKeyframeParam keyframes[2];
	};

	struct AnimationParam {
		enum class LoopType : unsigned char { // completely guessed
			NORMAL,
			BOUNCE,
		};

		bool useGlobalTime;
		char unk1;
		LoopType loopType;
		bool timeScale;
		int loopCount;
		short totalFrameCount;
		unsigned short trackCount;
		AnimationTrackParam* tracks[4];
	};

	// 0x140FF4B40
	struct ChildEffect {
		enum class Flag : unsigned char {
			ENABLE_USING_ANIMATION,
			UNK1,
			INHERIT_ROTATION,
		};

		bits::Bitset<Flag> flags;
		char unkType; // 0x01 = affected by fluctuation, 0x02 = related to color?
		unsigned char unk00;
		float unk0;
		float unk1;
		float unk2;
		float unk3;
		float unk4;
		AnimationParam* enableAnimation; // single track, child effect is on if inside track frame bounds
		char name[128];
		EffectParam* param;
	};

	/*
	 * Field animation slots :
	 * GRAVITY:
	 *   0 - vec3 - rotation
	 *   1 - float - scale
	 * SPEED:
	 *   0 - float - scale
	 * MAGNET:
	 *   0 - float - scale
	 *   1 - vec3 - magnetPoint
	 * NEWTON:
	 *   0 - float - force
	 *   1 - vec3 - targetPoint
	 *   2 - float - falloffRadius
	 * VORTEX:
	 *   0 - float - max
	 *   1 - float - min
	 *   2 - float - radius
	 *   3 - vec3 - rotation
	 * SPIN:
	 *   0 - float - scale
	 *   1 - vec3 - rotation
	 * SPIN2:
	 *   0 - float - baseStrength
	 *   1 - float - spinStrength
	 *   2 - float - axisFalloff
	 *   3 - float - rotationAngle
	 *   4 - vec3 - axisVector
	 * RANDOM:
	 *   0 - float - scale
	 * TAIL:
	 *   0 - float - multiplier
	 * FLUCTUATION:
	 *   0 - float - ?
	 *   1 - float - ?
	 * UNK0:
	 *   0 - float - unk1
	 */
	struct FieldParam {
		enum class Type : unsigned char {
			GRAVITY,
			SPEED,
			MAGNET,
			NEWTON,
			VORTEX,
			SPIN,
			SPIN2,
			RANDOM,
			TAIL,
			FLUCTUATION,
			UNK0
		};

		enum class Origin : unsigned char {
			WORLD,
			EMISSION,
			INHERIT,
			UNK0 // uses option2
		};

		struct GravitySettings {
			float scale;
			math::Position rotation;
			bool usesDirection;
		};

		struct SpeedSettings {
			float scale;
		};

		struct MagnetSettings {
			math::Position magnetPoint;
			float scale;
		};

		struct NewtonSettings {
			math::Position targetPoint;
			float force;
			float falloffRadius;
		};

		struct VortexSettings {
			float max;
			float min;
			float radius;
			math::Position rotation;
			bool usesDirection;
		};

		struct SpinSettings {
			float scale;
			math::Position rotation;
			bool usesDirection;
		};

		struct Spin2Settings {
			float baseStrength;
			float spinStrength;
			float rotationAngle;
			float axisFalloff;
			math::Position axisVector;
			bool useEulerRotation;
		};

		struct RandomSettings {
			enum class Flags : unsigned short {
				RANDOMIZED_SCALE,
				X,
				Y,
				Z
			};

			float scale;
			float spreadScale;
			short updateInterval;
			bits::Bitset<Flags> flags;
			bool normalizedSpreadVector;
			bool randomPerAxis;
		};

		struct TailSettings {
			float multiplier;
		};

		struct FluctuationSettings {
			enum class WaveformType : unsigned char {
				TRIANGLE,
				INVERSE_TRIANGLE,
				SAW,
				STEP,
				SINE
			};

			WaveformType waveformType;
			bool unk2; // 0x140FF4E21
			float baseOffset;
			JitteredValue unk0;
			JitteredValue unk1;
		};

		struct UnkSettings {
			enum class AxisFlag : unsigned char {
				X,
				Y,
				Z,
			};

			float unk1;
			float unk2;
			bits::Bitset<AxisFlag> axes;
		};

		union Settings {
			GravitySettings gravity;
			SpeedSettings speed;
			MagnetSettings magnet;
			NewtonSettings newton;
			VortexSettings vortex;
			SpinSettings spin;
			Spin2Settings spin2;
			RandomSettings random;
			TailSettings tail;
			FluctuationSettings fluctuation;
			UnkSettings unk;
			unsigned int maxSize[8];
		};

		bool enabled;
		Type type;
		Origin origin;
		char option2;
		Settings settings;
	};

	struct TextureParam {
		enum class TextureFlag : unsigned int {
			TEXTURE_TYPE_0,
			TEXTURE_TYPE_1,
			TEXTURE_TYPE_2,
			TEXTURE_TYPE_3,
			TEXTURE_TYPE_4,
			TEXTURE_TYPE_5,
			TEXTURE_TYPE_6,
			TEXTURE_TYPE_7,
			TEXTURE_TYPE_8,
			TEXTURE_TYPE_9,
		};

		enum class AnimationFlag : unsigned int {
			ANIMATED, //0x01
			UNK1, //0x02
			UNK2, //0x04
			STEPWISE_PATTERN_ANIMATION, //0x08
			ENABLE_SECONDARY_SCROLL, //0x10
			UNK5, //0x20
			RANDOMIZE_STARTING_FRAMES, //0x40
			ENABLE_PATTERN_ANIMATION, //0x80
			UNK8, //0x100
			SCROLL_U, //0x200
			SCROLL_V, //0x400
			UNK11, //0x800
			USE_MATRICES_ON_TEXTURE_TYPE_0, //0x1000 - tested at 0x140FF4598 in frontiers 1.42
			USE_MATRICES_ON_TEXTURE_TYPE_1, //0x2000
			SCALE_U, //0x4000
			SCALE_V //0x8000
		};

		enum class DirectionFlag : unsigned char {
			INVERT_X,
			INVERT_Y,
		};

		enum class DirectionRandomizationFlag : unsigned char {
			RANDOMIZE_X,
			RANDOMIZE_Y,
		};

		struct ScrollColorParam {
			math::Vector2 scrollColor;
			math::Vector2 scrollStartJitter;
		};

		struct UVAnimationParam {
			math::Vector2 scale;
			float rotation;
			math::Vector2 translation;
			int gap15;
			AnimationParam* tilingAnimation;
			AnimationParam* rotationAnimation;
			AnimationParam* translationAnimation;
		};

		struct PatternAnimationParam {
			enum class Type : unsigned char {
				UNK0,
				UNK1,
				UNK2,
				UNK3,
				UNK4,
				UNK5,
				UNK6,
				UNK7,
				UNK8,
				UNK9,
				UNK10,
			};

			unsigned char startingFrame;
			unsigned char stepCountX;
			unsigned char stepCountY;
			Type type;
			unsigned char frameRangeStart;
			unsigned char frameRangeEnd;
			unsigned char frameCount;
			unsigned char repeating;
			char unk6;
			char unk6a;
			bits::Bitset<DirectionFlag> scrollDirectionFlags;
			bits::Bitset<DirectionRandomizationFlag> scrollDirectionRandomizationFlags;
			unsigned char unk9;
			AnimationParam* patternAnimation;
		};

		char name[128];
		bits::Bitset<TextureFlag> textureFlags;
		bits::Bitset<AnimationFlag> animationFlags;
		PatternAnimationParam patternAnimationParam;
		ScrollColorParam scrollColorParams[2];
		UVAnimationParam uvAnimationParams[2];
		math::Vector2 uvScale;
		int64_t unk22;
	};

	struct ColorRandomSet {
		JitteredValue color1[3][32];
		JitteredValue color2[3][32];
		JitteredValue luminance1[32];
		JitteredValue luminance2[32];
		JitteredValue alpha1[32];
		JitteredValue alpha2[32];
		unsigned char color1Count;
		unsigned char color2Count;
		unsigned char luminance1Count;
		unsigned char luminance2Count;
		unsigned char alpha1Count;
		unsigned char alpha2Count;
	};

	struct ColorParam {
		JitteredValue color1[3];
		JitteredValue alpha1;
		JitteredValue color2[3];
		JitteredValue alpha2;
		JitteredValue luminance1;
		JitteredValue luminance2;
		AnimationParam* colorAnimation1;
		AnimationParam* luminanceAnimation1;
		AnimationParam* alphaAnimation1;
		AnimationParam* colorAnimation2;
		AnimationParam* luminanceAnimation2;
		AnimationParam* alphaAnimation2;
		ColorRandomSet colorRandomSet;
		float unk7;
		AnimationParam* unk7Animation;
	};

	struct BasicLODEffectParam {
		char name[128];
		float threshold;
	};

	struct BasicLODParam {
		enum class Flag : unsigned char {
			USE_NEGATIVE_ONE_UNK1864,
		};

		unsigned char lodCount;
		bits::Bitset<Flag> flags;
		BasicLODEffectParam lods[16];
	};

	struct RaycastLODEffectParam {
		char name[128];
		unsigned int raycastFlags;
	};

	// 0x140FEBE70
	struct RaycastLODParam {
		enum class RotationMode : unsigned char {
			DISABLE,
			ROTATE_TO_NORMAL,
		};

		enum class Flag : unsigned int {
			UNK0
		};

		float unkLifetime;
		math::Position from;
		math::Position to;
		AnimationParam* fromAnimation;
		AnimationParam* toAnimation;
		RotationMode rotationMode;
		RotationMode offsetRotationMode;
		math::Position rotation;
		math::Position offset;
		AnimationParam* rotationAnimation;
		AnimationParam* offsetAnimation;
		unsigned int filterMask;
		bits::Bitset<Flag> flags;
		unsigned char lodCount;
		char gap12[7];
		RaycastLODEffectParam lods[16];
	};

	union LODParam {
		BasicLODParam basic;
		RaycastLODParam raycast;
	};

	struct UserParameter {
		enum class Type : unsigned int {
			UNK0,
			BOOL,
			INT,
			UINT,
			FLOAT,
			STRING,
			UNK1,
			VECTOR4,
		};

		union DataPtr {
			void* unk0;
			bool* boolean;
			int* integer;
			float* floating;
			math::Position* vector;
		};

		const char* name;
		Type type;
		unsigned int size;
		DataPtr data;
	};

	struct ElementParam {
		enum class LODFlag : unsigned int {
			BASIC,
			ANIMATED,
		};

		enum class ParticleType : unsigned char {
			UNK0,
			UNK1,
			UNK2,
			UNK3,
			UNK4,
			UNK5_CPU,
			HISTORICAL_STRIPE,
			POINT_LIGHT,
		};

		enum class ParticleUnkMode : unsigned char {
			UNK0,
			UNK1,
		};

		enum class DirectionType : unsigned int {
			UNK0,
			UNK1,
			UNK2,
			UNK3,
			UNK4,
			UNK5,
		};

		enum class DirectionTypeU8 : unsigned char {
			UNK0,
			UNK1,
			UNK2,
			UNK3,
			UNK4,
			UNK5,
		};

		struct Unk0ParticleParam {
			unsigned int unk0;
			float unk1;
		};

		struct Unk1ParticleParam {
			unsigned int unk0;
			float unk1;
		};

		struct Unk2ParticleParam {
			unsigned int type;
			char pad[4];
			unsigned int unk0;
			float unk1;
		};

		struct Unk3ParticleParam {
			unsigned int type;
			bool unk2;
			char pad[3];
			unsigned int unk0;
			float unk1;
		};

		struct Unk4ParticleParam {
			unsigned int type;
			DirectionType directionType;
			char pad1;
			unsigned char unk2;
			unsigned char gap3;
			unsigned int unk0;
			float unk1;
		};

		struct Unk5ParticleParam {
			unsigned int type;
			unsigned char unk3;
			char pad[3];
			unsigned int unk1;
			unsigned char unk4;
			DirectionTypeU8 directionType;
			unsigned char unk6;
			bool unk2;
			char pad2[0x8];
		};

		struct HistoricalStripeParticleParam {
			unsigned int type;
			unsigned int unkStride; // 0x141006D02
			unsigned char unk1;
			char pad1[0x3];
			DirectionTypeU8 directionType;
			unsigned char unk3;
			bool unk2;
			char pad2;
			unsigned int unkCount; // 0x141006D02
			char pad3[0x4];
		};

		struct PointLightParticleParam {
			char pad[0x18];
		};

		union ParticleParam {
			Unk0ParticleParam unk0;
			Unk1ParticleParam unk1;
			Unk2ParticleParam unk2;
			Unk3ParticleParam unk3;
			Unk4ParticleParam unk4;
			Unk5ParticleParam unk5;
			HistoricalStripeParticleParam historicalStripe;
			PointLightParticleParam pointLight;
		};

		enum class GpuParticleFlag : unsigned int {
			IS_GPU,
			UNK1,
			UNK2,
		};

		struct TransformParam {
			enum class RandomAngleFlag : unsigned char {
				CCW_90,
				CW_90,
				CW_180,
				CW_0
			};

			JitteredVector3 rotation;
			unsigned int rotationFlags;
			JitteredVector3 randomizedDirectionRotation;
			unsigned int randomizedDirectionRotationFlags;
			bits::Bitset<RandomAngleFlag> rotationRandomInitialAngleFlags[3];
			JitteredVector3 angularVelocity;
			unsigned int angularVelocityFlags;
			JitteredVector3 size;
			unsigned int sizeFlags;
			JitteredVector3 scale;
			unsigned int scaleFlags;
		};

		struct UserParameterParam {
			unsigned int userParameterCount;
			UserParameter* userParameters;
		};

		enum class UpdateFlag : unsigned int {
			HAS_CHILDREN, // 0x1
			HAS_RANDOMIZED_ROTATION_DIRECTION, // 0x2
			HAS_FIXED_ANGULAR_VELOCITY, // 0x4
			USE_GLOBAL_TIME, // 0x8
			HAS_MODEL, // 0x10
			UNK5, // 0x20
			UNK6, // 0x40
			UNK7, // 0x80
			UNK8, // 0x100
			UNK9, // 0x200
			UNK10, // 0x400
			DISABLE_ANIMATIONS, // 0x800
			UNK12, // 0x1000
			DISABLE_FIELDS, // 0x2000
			UNK14, // 0x4000
		};

		struct ModelParam {
			bool hasModel;
			unsigned char modelSetting1;
			char gap6b[128];
			char modelName[128];
			char skeletonName[128];
			char nodeAnimName[128];
		};

		struct DistanceScaleParam {
			float min;
			float max;
			float multiplier;
		};

		TransformParam transformParam;
		ColorParam colorParams[2];
		JitteredValue lifetime;
		AnimationParam* rotationAnimation;
		AnimationParam* scaleAnimation;
		AnimationParam* sizeAnimation;
		DistanceScaleParam distanceScaleParam;
		ModelParam modelParam;
		ParticleType particleType;
		ParticleUnkMode particleUnkMode; // 140FE7E43
		ParticleParam particleParam;
		float unkScale;
		TextureParam textures[4];
		unsigned int textureCount;
		ChildEffect childEffects[16];
		FieldParam fields[8];
		AnimationParam* fieldAnimations[8][5];
		bits::Bitset<UpdateFlag> updateFlags; // 0x01 = has childeffects, 0x4 = use simple unkVec2 update (only x multiplier, no anim), 0x8 = use emitter global time for unkVec2 update, 0x1000 = use -1 or fps variables
		AnimationParam* uvAnimation;
		unsigned int elementAnimationCount; // 140FF3B3D
		char gap7bb[0x34];
		bits::Bitset<GpuParticleFlag> gpuParticleFlags; // 0x1 = is gpu rendering?
		char vectorFieldName[128];
		math::Position vectorFieldSize;
		char gap7c2[0x30];
		math::Position unkVector2;
		math::Position vectorFieldParam;
		math::Position unkVector1;
		math::Position depthCollision;
		UserParameterParam userParameterParam;
		unsigned int emitterAnimationCount; // 140FE9F5B
		unsigned int unk7e;
		void* gpuElementParamTexture;
		void* gpuOtherTexture;
		bits::Bitset<LODFlag> lodFlags;
		LODParam lodParam;
	};

	struct EmitterParam {
		struct TransformParam {
			enum class TransformType : unsigned int {
				UNK0,
				UNK1,
				ROTATED,
			};

			math::Position position;
			math::Position rotation;
			math::Position scale;
			TransformType type;
			AnimationParam* positionAnimation;
			AnimationParam* rotationAnimation;
			AnimationParam* scaleAnimation;
		};

		struct EmissionParam {
			enum class Shape : unsigned int {
				POINT,
				SPHERE,
				DISC,
				CYLINDER,
				LINE,
				TORUS,
				CUBE,
				MESH,
				FIXED
			};

			Shape shape;
			math::Position positionJitter;
			float spread;
			float startAngle;
			float endAngle;
			bool useRadialDistribution;
			bool consistentAngle;
			bool useSubdivisions;
			unsigned char subdivisionCount;
			AnimationParam* positionJitterAnimation;
			AnimationParam* spreadAnimation;
			AnimationParam* startAngleAnimation;
			AnimationParam* endAngleAnimation;
		};

		struct EmitParam {
			enum class EmitMode : unsigned int {
				CONTINUOUS,
				INTERVAL,
			};

			enum class AttenuationMode : unsigned char {
				FIXED,
				POSE_POSITION,
			};

			EmitMode emissionMode;
			JitteredValue emitInterval;
			JitteredValue emissionCount;
			AttenuationMode attenuationMode;
			float minDistance;
			float maxDistance;
			float distanceAttenuation; // 0->1
			float duration;
			float startDelay;
			float fadeSpeed;
			AnimationParam* emissionCountAnimation;
		};

		struct DeathParam {
			enum class DeathMode : unsigned int {
				WAIT_FOR_ELEMENTS,
				KILL_AFTER_DELAY,
				WAIT_FOR_ELEMENTS_OR_KILL_AFTER_DELAY,
				IMMEDIATE,
			};

			DeathMode mode;
			float killDelay;
		};

		struct EmissionDynamicsParam {
			struct Settings {
				enum class SpreadDirectionType : unsigned int {
					DIRECTION,
					ANGLES,
				};

				float direction1Velocity;
				float direction2Velocity;
				float randomDirectionVelocity;
				float localSpreadVelocity;
				float localSpread;
				float worldSpreadVelocity;
				float worldSpread;
				SpreadDirectionType worldSpreadDirectionType;
				math::Position worldSpreadAnglesOrDirection;
				float initialVelocityJitter;
				float velocityJitter;
			};

			Settings settings;
			AnimationParam* direction1VelocityAnimation;
			AnimationParam* direction2VelocityAnimation;
			AnimationParam* localSpreadVelocityAnimation;
			AnimationParam* localSpreadAnimation;
			AnimationParam* worldSpreadVelocityAnimation;
			AnimationParam* worldSpreadAnimation;
			AnimationParam* worldSpreadAnglesOrDirectionAnimation;
		};

		enum class InheritFlag : unsigned int {
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
			USE_TEXTURES,
			USE_RENDER_PRIORITY,
			UNK6,
			USE_UNKFLOAT0_RANGE,
			USE_UNKFLOAT1_RANGE,
			UNK9,
			UNK10,
			UNK11, // sets emitter flag 0x40
		};

		enum class CullMode : unsigned int {
			NONE,
			FRONT,
			BACK,
		};

		enum class MaskType1 : unsigned int {
			UNK0,
			UNK1,
			UNK2,
			UNK3,
		};

		enum class MaskType2 : unsigned int {
			UNK0,
			UNK1,
			UNK2,
			UNK3,
			UNK4,
		};

		enum class MaskType3 : unsigned int {
			UNK0,
			UNK1,
			UNK2,
		};

		enum class DepthMode : unsigned int {
			BLENDING_ENABLED_WITH_DEPTH_TEST,
			BLENDING_ENABLED,
			BLENDING_DISABLED_WITH_DEPTH_TEST_AND_DEPTH_WRITE,
			BLENDING_DISABLED_WITH_DEPTH_TEST_AND_DEPTH_WRITE_2,
			BLENDING_ENABLED_WITH_DEPTH_TEST_AND_DEPTH_WRITE,
		};

		enum class BlendMode : unsigned int {
			MIX,
			ADD,
			MULTIPLY,
			SUBTRACT,
		};

		enum class BillboardType : unsigned char {
			UNK0,
			UNK1,
			UNK2,
		};

		// 0x140FE59A0
		struct Unk0Param {
			enum class Flag : unsigned char {
				UNK0, // 0x140FEB78D
				UNK1, // 0x140FE5C3B
			};

			enum class Mode : unsigned char {
				ANISOTROPIC,
				ISOTROPIC, // only uses scale.x
			};

			bits::Bitset<Flag> flags;
			Mode mode;
			math::Position position;
			math::Position scale;
		};

		TransformParam transformParam;
		EmissionParam emissionParam;
		EmitParam emitParam;
		DeathParam deathParam;
		EmissionDynamicsParam emissionDynamicsParam;
		BlendMode blendMode;
		DepthMode depthMode;
		CullMode cullMode;
		bits::Bitset<InheritFlag> inheritTransformFlags;
		float inheritTransformRatio;
		bits::Bitset<InheritFlag> inheritPoseFlags;
		float inheritPoseRatio;
		bits::Bitset<UnkFlags> unkFlags;
		float unkFloat4;
		float unkFloat5;
		float unkFloat0RangeStart;
		float unkFloat0RangeEnd;
		float unkFloat1RangeStart;
		float unkFloat1RangeEnd;
		float unkFloat2RangeStart;
		float unkFloat2RangeEnd;
		float unkFloat6RangeEnd; // 140FE4F74
		float unkFloat6RangeStart;
		unsigned int gap6b;
		float unkFloatFoo0;
		float unkFloatFoo1;
		float unkFloatFoo2;
		unsigned int randomSeed;
		char renderLayer;
		unsigned char unk6b2; // 140FE0540
		unsigned char unk6b3; // 140FE4E6C
		unsigned char unk6b4; // 140FE4F0A
		unsigned char unkType5;
		BillboardType billboardType; // handled in 0140FE9390
		bool enableUnkFloat2Range;
		char gap6b2;
		unsigned char unkAlpha;
		Unk0Param unk0Param;
		unsigned int gap6aa2;
		MaskType1 unkType0;
		unsigned int unkType0a;
		unsigned int gap6aa1a;
		MaskType2 unkType0c;
		unsigned int unkType0d;
		MaskType3 unkType1;
		MaskType1 unkType1a;
		unsigned int unkType1b;
		MaskType3 unkType2;
		MaskType2 unkType3;
		unsigned int unkType4;
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
		unsigned int emitterCount;

		EmitterParam emitterParam;

	};
}
