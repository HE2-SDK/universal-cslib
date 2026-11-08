#pragma once
#include <ucsl/resources/asm/v103-rangers.h>
#include <simple-reflection/simple-reflection.h>
#include <ucsl-reflection/operators.h>
#include <ucsl-reflection/reflections/basic-types.h>

namespace ucsl::resources::animation_state_machine::v103_rangers::reflections {
	using namespace simplerfl;
	using namespace ucsl::reflection;

	namespace impl = ::ucsl::resources::animation_state_machine::v103_rangers;

	using ClipDataAnimationSettings = structure<impl::ClipData::AnimationSettings, "ClipDataAnimationSettings", void,
		field<const char*, "resourceName">,
		field<float, "start">,
		field<float, "end">,
		field<float, "speed">,
		field<bitfield<unsigned char,
			component<bool, "mirror", 0, 1>,
			component<bool, "playUntilAnimationEnded", 1, 1>,
			component<bool, "noAnimationResolution", 2, 1>
		>, "flags">,
		field<bool, "loops">
	>;

	using ClipData = structure<impl::ClipData, "ClipData", void,
		field<const char*, "name">,
		field<ClipDataAnimationSettings, "animationSettings">,
		field<unsigned short, "triggerCount">,
		field<short, "triggerOffset">,
		field<short, "blendMaskIndex">,
		field<unsigned short, "childClipIndexCount">,
		field<short, "childClipIndexOffset">
	>;

	using TransitionType = enumeration<impl::TransitionType, "TransitionType", unsigned char,
		option<"IMMEDIATE">,
		option<"FROZEN">,
		option<"SMOOTH">,
		option<"SYNCHRONIZE">,
		option<"HOLD_TO">,
		option<"HOLD_BOTH">,
		option<"WAIT_FROM">,
		option<"WAIT_FROM_HOLD_TO">,
		option<"USER_0">,
		option<"USER_1">,
		option<"USER_2">
	>;

	using TransitionEasingType = enumeration<impl::TransitionEasingType, "TransitionEasingType", unsigned char,
		option<"LINEAR">,
		option<"CUBIC">
	>;

	using TransitionDataTransitionInfo = structure<impl::TransitionData::TransitionInfo, "TransitionDataTransitionInfo", void,
		field<TransitionType, "type">,
		field<TransitionEasingType, "easingType">,
		field<short, "targetStateIndex">,
		field<float, "transitionTime">
	>;

	using TransitionData = structure<impl::TransitionData, "TransitionData", void,
		field<TransitionDataTransitionInfo, "transitionInfo">,
		field<short, "transitionTimeVariableIndex">
	>;

	using StateType = enumeration<impl::StateType, "StateType", char,
		fixed_option<"NULL_STATE", -1>,
		option<"CLIP">,
		option<"BLEND_TREE">,
		option<"NONE">
	>;

	using StateData = structure<impl::StateData, "StateData", void,
		field<const char*, "name">,
		field<StateType, "type">,
		field<bool, "transitImmediately">,
		field<bitfield<unsigned char,
			component<bool, "loops", 0, 1>,
			component<bool, "unk1", 1, 1>,
			component<bool, "usePba", 2, 1>
		>, "flags">,
		field<char, "defaultLayerIndex">,
		field<short, "rootBlendNodeOrClipIndex">,
		field<short, "maxCycles">,
		field<float, "speed">,
		field<short, "speedVariableIndex">,
		field<unsigned short, "eventCount">,
		field<short, "eventOffset">,
		field<short, "transitionArrayIndex">,
		field<TransitionData, "stateEndTransition">,
		field<unsigned short, "flagIndexCount">,
		field<short, "flagIndexOffset">
	>;

	using BlendNodeType = enumeration< impl::BlendNodeType, "BlendNodeType", unsigned char,
		option<"LERP">,
		option<"ADDITIVE">,
		option<"CLIP">,
		option<"OVERRIDE">,
		option<"LAYER">,
		option<"MULTIPLY">,
		option<"BLEND_SPACE">,
		option<"TWO_POINT_LERP">
	>;

	using BlendNodeData = structure<impl::BlendNodeData, "BlendNodeData", void,
		field<BlendNodeType, "type">,
		field<short, "blendSpaceIndex">,
		field<short, "blendFactorVariableIndex">,
		field<float, "blendFactorTarget">,
		field<unsigned short, "childNodeArraySize">,
		field<short, "childNodeArrayOffset">
	>;

	using TransitionArrayData = structure<impl::TransitionArrayData, "TransitionArrayData", void,
		field<int, "size">,
		field<int, "offset">
	>;

	using EventData = structure<impl::EventData, "EventData", void,
		field<const char*, "name">,
		field<TransitionData, "transition">
	>;

	using LayerData = structure<impl::LayerData, "LayerData", void,
		field<const char*, "name">,
		field<unsigned short, "layerId">,
		field<short, "blendMaskIndex">
	>;

	using BlendMaskData = structure<impl::BlendMaskData, "BlendMaskData", void,
		field<const char*, "name">,
		field<unsigned short, "maskBoneCount">,
		field<short, "maskBoneOffset">
	>;

	using TriggerType = enumeration<impl::TriggerType, "TriggerType", unsigned char,
		option<"HIT">,
		option<"ENTER_LEAVE">
	>;

	using TriggerData = structure<impl::TriggerData, "TriggerData", void,
		field<TriggerType, "type">,
		field<float, "unknown2">,
		field<float, "unknown3">,
		field<unsigned short, "triggerTypeIndex">,
		field<short, "colliderIndex">,
		field<const char*, "name">
	>;

	using BlendSpaceDataTriangle = structure<impl::BlendSpaceData::Triangle, "BlendSpaceDataTriangle", void,
		field<short[3], "nodeIndices">,
		field<short, "unused">
	>;

	using BlendSpaceData = structure<impl::BlendSpaceData, "BlendSpaceData", void,
		field<short, "xVariableIndex">,
		field<short, "yVariableIndex">,
		field<float, "xMin">,
		field<float, "xMax">,
		field<float, "yMin">,
		field<float, "yMax">,
		field<unsigned short, "nodeCount">,
		field<unsigned short, "triangleCount">,
		field<dynamic_carray<ucsl::math::Vector2, field_resolver<unsigned short, "nodeCount">>*, "nodes">,
		field<dynamic_carray<short, field_resolver<unsigned short, "nodeCount">>*, "clipIndices">,
		field<dynamic_carray<BlendSpaceDataTriangle, field_resolver<unsigned short, "triangleCount">>*, "triangles">
	>;

	using AsmData = structure<impl::AsmData, "AsmData", void,
		field<constant<unsigned int, 0x41534D46>, "magic">,
		field<constant<int, 0x103>, "version">,
		field<int, "clipCount">,
		field<dynamic_carray<ClipData, field_resolver<int, "clipCount">>*, "clips">,
		field<int, "stateCount">,
		field<dynamic_carray<StateData, field_resolver<int, "stateCount">>*, "states">,
		field<int, "blendNodeCount">,
		field<dynamic_carray<BlendNodeData, field_resolver<int, "blendNodeCount">>*, "blendNodes">,
		field<int, "eventCount">,
		field<dynamic_carray<EventData, field_resolver<int, "eventCount">>*, "events">,
		field<int, "transitionArrayCount">,
		field<dynamic_carray<TransitionArrayData, field_resolver<int, "transitionArrayCount">>*, "transitionArrays">,
		field<int, "transitionCount">,
		field<dynamic_carray<TransitionData, field_resolver<int, "transitionCount">>*, "transitions">,
		field<TransitionData, "nullTransition">,
		field<int, "flagIndexCount">,
		field<dynamic_carray<short, field_resolver<int, "flagIndexCount">>*, "flagIndices">,
		field<int, "flagCount">,
		field<dynamic_carray<const char*, field_resolver<int, "flagCount">>*, "flags">,
		field<int, "variableCount">,
		field<dynamic_carray<const char*, field_resolver<int, "variableCount">>*, "variables">,
		field<int, "layerCount">,
		field<dynamic_carray<LayerData, field_resolver<int, "layerCount">>*, "layers">,
		field<int, "blendMaskCount">,
		field<dynamic_carray<BlendMaskData, field_resolver<int, "blendMaskCount">>*, "blendMasks">,
		field<int, "maskBoneCount">,
		field<dynamic_carray<const char*, field_resolver<int, "maskBoneCount">>*, "maskBones">,
		field<int, "triggerCount">,
		field<dynamic_carray<TriggerData, field_resolver<int, "triggerCount">>*, "triggers">,
		field<int, "triggerTypeCount">,
		field<dynamic_carray<const char*, field_resolver<int, "triggerTypeCount">>*, "triggerTypes">,
		field<int, "colliderCount">,
		field<dynamic_carray<const char*, field_resolver<int, "colliderCount">>*, "colliders">,
		field<short, "blendTreeRootNodeId">,
		field<int, "blendSpaceCount">,
		field<dynamic_carray<BlendSpaceData, field_resolver<int, "blendSpaceCount">>*, "blendSpaces">,
		field<int, "childClipIndexCount">,
		field<dynamic_carray<short, field_resolver<int, "childClipIndexCount">>*, "childClipIndices">
	>;
}

namespace simplerfl {
	template<> struct canonical<ucsl::resources::animation_state_machine::v103_rangers::ClipData::AnimationSettings> { using type = ucsl::resources::animation_state_machine::v103_rangers::reflections::ClipDataAnimationSettings; };
	template<> struct canonical<ucsl::resources::animation_state_machine::v103_rangers::ClipData> { using type = ucsl::resources::animation_state_machine::v103_rangers::reflections::ClipData; };
	template<> struct canonical<ucsl::resources::animation_state_machine::v103_rangers::TransitionType> { using type = ucsl::resources::animation_state_machine::v103_rangers::reflections::TransitionType; };
	template<> struct canonical<ucsl::resources::animation_state_machine::v103_rangers::TransitionEasingType> { using type = ucsl::resources::animation_state_machine::v103_rangers::reflections::TransitionEasingType; };
	template<> struct canonical<ucsl::resources::animation_state_machine::v103_rangers::TransitionData::TransitionInfo> { using type = ucsl::resources::animation_state_machine::v103_rangers::reflections::TransitionDataTransitionInfo; };
	template<> struct canonical<ucsl::resources::animation_state_machine::v103_rangers::TransitionData> { using type = ucsl::resources::animation_state_machine::v103_rangers::reflections::TransitionData; };
	template<> struct canonical<ucsl::resources::animation_state_machine::v103_rangers::StateType> { using type = ucsl::resources::animation_state_machine::v103_rangers::reflections::StateType; };
	template<> struct canonical<ucsl::resources::animation_state_machine::v103_rangers::StateData> { using type = ucsl::resources::animation_state_machine::v103_rangers::reflections::StateData; };
	template<> struct canonical<ucsl::resources::animation_state_machine::v103_rangers::BlendNodeType> { using type = ucsl::resources::animation_state_machine::v103_rangers::reflections::BlendNodeType; };
	template<> struct canonical<ucsl::resources::animation_state_machine::v103_rangers::BlendNodeData> { using type = ucsl::resources::animation_state_machine::v103_rangers::reflections::BlendNodeData; };
	template<> struct canonical<ucsl::resources::animation_state_machine::v103_rangers::TransitionArrayData> { using type = ucsl::resources::animation_state_machine::v103_rangers::reflections::TransitionArrayData; };
	template<> struct canonical<ucsl::resources::animation_state_machine::v103_rangers::EventData> { using type = ucsl::resources::animation_state_machine::v103_rangers::reflections::EventData; };
	template<> struct canonical<ucsl::resources::animation_state_machine::v103_rangers::LayerData> { using type = ucsl::resources::animation_state_machine::v103_rangers::reflections::LayerData; };
	template<> struct canonical<ucsl::resources::animation_state_machine::v103_rangers::BlendMaskData> { using type = ucsl::resources::animation_state_machine::v103_rangers::reflections::BlendMaskData; };
	template<> struct canonical<ucsl::resources::animation_state_machine::v103_rangers::TriggerType> { using type = ucsl::resources::animation_state_machine::v103_rangers::reflections::TriggerType; };
	template<> struct canonical<ucsl::resources::animation_state_machine::v103_rangers::TriggerData> { using type = ucsl::resources::animation_state_machine::v103_rangers::reflections::TriggerData; };
	template<> struct canonical<ucsl::resources::animation_state_machine::v103_rangers::BlendSpaceData::Triangle> { using type = ucsl::resources::animation_state_machine::v103_rangers::reflections::BlendSpaceDataTriangle; };
	template<> struct canonical<ucsl::resources::animation_state_machine::v103_rangers::BlendSpaceData> { using type = ucsl::resources::animation_state_machine::v103_rangers::reflections::BlendSpaceData; };
	template<> struct canonical<ucsl::resources::animation_state_machine::v103_rangers::AsmData> { using type = ucsl::resources::animation_state_machine::v103_rangers::reflections::AsmData; };
}
