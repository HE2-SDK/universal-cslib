#pragma once
#include <ucsl/resources/aism/v0.h>
#include <simple-reflection/simple-reflection.h>
#include <ucsl-reflection/operators.h>
#include <ucsl-reflection/reflections/basic-types.h>

namespace ucsl::resources::aism::v0::reflections {
	using namespace simplerfl;
	using namespace ucsl::reflection;

	namespace impl = ::ucsl::resources::aism::v0;

    using FixedBoolConditionArgumentValue = structure<impl::ConditionData::FixedBoolArgumentValue, "FixedBoolConditionArgumentValue", void,
        field<bool, "value">
    >;

    using FixedIntConditionArgumentValue = structure<impl::ConditionData::FixedIntArgumentValue, "FixedIntConditionArgumentValue", void,
        field<int, "value">
    >;

    using FixedFloatConditionArgumentValue = structure<impl::ConditionData::FixedFloatArgumentValue, "FixedFloatConditionArgumentValue", void,
        field<float, "value">
    >;

    using RandomIntConditionArgumentValue = structure<impl::ConditionData::RandomIntArgumentValue, "RandomIntConditionArgumentValue", void,
        field<int, "lowerBound">,
        field<int, "upperBound">
    >;

    using RandomFloatConditionArgumentValue = structure<impl::ConditionData::RandomFloatArgumentValue, "RandomFloatConditionArgumentValue", void,
        field<float, "lowerBound">,
        field<float, "upperBound">
    >;

    using AttributeConditionArgumentValue = structure<impl::ConditionData::AttributeArgumentValue, "AttributeConditionArgumentValue", void,
        field<const char*, "name">,
        field<const char*, "propertyName">
    >;

    union ConditionArgumentValueImpl {
        impl::ConditionData::FixedBoolArgumentValue fixedBool;
        impl::ConditionData::FixedIntArgumentValue fixedInt;
        impl::ConditionData::FixedFloatArgumentValue fixedFloat;
        impl::ConditionData::RandomIntArgumentValue randomInt;
        impl::ConditionData::RandomFloatArgumentValue randomFloat;
        impl::ConditionData::AttributeArgumentValue attribute;
    };

    inline size_t get_argument_value_type(const impl::ConditionData::ArgumentType& type) {
        switch (type) {
        case impl::ConditionData::ArgumentType::FIXED_BOOL: return 0;
        case impl::ConditionData::ArgumentType::FIXED_INT: return 1;
        case impl::ConditionData::ArgumentType::FIXED_FLOAT: return 2;
        case impl::ConditionData::ArgumentType::RANDOM_BOOL: return 0;
        case impl::ConditionData::ArgumentType::RANDOM_INT: return 3;
        case impl::ConditionData::ArgumentType::RANDOM_FLOAT: return 4;
        case impl::ConditionData::ArgumentType::ATTRIBUTE_BOOL: return 5;
        case impl::ConditionData::ArgumentType::ATTRIBUTE_INT: return 5;
        case impl::ConditionData::ArgumentType::ATTRIBUTE_FLOAT: return 5;
        default: assert(false && "invalid condition type"); return 0;
        }
    }

    template<typename Selector>
    using ConditionArgumentValue = unionof<ConditionArgumentValueImpl, "ConditionArgumentValue", Selector,
        field<FixedBoolConditionArgumentValue, "fixedBool">,
        field<FixedIntConditionArgumentValue, "fixedInt">,
        field<FixedFloatConditionArgumentValue, "fixedFloat">,
        field<RandomIntConditionArgumentValue, "randomInt">,
        field<RandomFloatConditionArgumentValue, "randomFloat">,
        field<AttributeConditionArgumentValue, "attribute">
    >;
    
    //template<typename Parent, union_resolver<Parent> resolver>
    //using ConditionArgumentValuePtr = unionof<impl::ConditionData::ArgumentValuePtr, "ConditionArgumentValuePtr", Parent, resolver,
    //    field<FixedBoolConditionArgumentValue*, "fixedBool">,
    //    field<FixedIntConditionArgumentValue*, "fixedInt">,
    //    field<FixedFloatConditionArgumentValue*, "fixedFloat">,
    //    field<RandomIntConditionArgumentValue*, "randomInt">,
    //    field<RandomFloatConditionArgumentValue*, "randomFloat">,
    //    field<AttributeConditionArgumentValue*, "attribute">
    //>;

    using ConditionArgumentType = enumeration<impl::ConditionData::ArgumentType, "ConditionArgumentType", unsigned char,
        option<"FIXED_BOOL">,
        option<"FIXED_INT">,
        option<"FIXED_FLOAT">,
        option<"RANDOM_BOOL">,
        option<"RANDOM_INT">,
        option<"RANDOM_FLOAT">,
        option<"ATTRIBUTE_BOOL">,
        option<"ATTRIBUTE_INT">,
        option<"ATTRIBUTE_FLOAT">
    >;

    using ConditionOperator = enumeration<impl::ConditionData::Operator, "ConditionOperator", unsigned char,
        option<"GREATER_THAN">,
        option<"GREATER_THAN_OR_EQUAL">,
        option<"LESS_THAN">,
        option<"LESS_THAN_OR_EQUAL">,
        option<"EQUAL">,
        option<"NOT_EQUAL">
    >;

    using ConditionData = structure<impl::ConditionData, "ConditionData", void,
        field<ConditionArgumentValue<selector_resolver<size_t, field_resolver<impl::ConditionData::ArgumentType, "lhsType">>::impl<get_argument_value_type>>*, "lhsValue">,
        field<ConditionArgumentType, "lhsType">,
        field<ConditionArgumentValue<selector_resolver<size_t, field_resolver<impl::ConditionData::ArgumentType, "rhsType">>::impl<get_argument_value_type>>*, "rhsValue">,
        field<ConditionArgumentType, "rhsType">,
        field<ConditionOperator, "op">
    >;

    using TriggerTriggerType = enumeration<impl::TriggerData::TriggerType, "TriggerTriggerType", unsigned char,
        option<"NORMAL">,
        option<"ONLY_WHEN_BOTTOM">,
        option<"ONCE">
    >;

    using TriggerConditionType = enumeration<impl::TriggerData::ConditionType, "TriggerConditionType", unsigned char,
        option<"ALL">,
        option<"ANY">
    >;

    using TriggerData = structure<impl::TriggerData, "TriggerData", void,
        field<const char*, "targetStateName">,
        field<TriggerTriggerType, "triggerType">,
        field<TriggerConditionType, "conditionType">,
        field<dynamic_carray<ConditionData, field_resolver<unsigned int, "conditionCount">>*, "conditions">,
        field<unsigned int, "conditionCount">
    >;

    using ReactionData = structure<impl::ReactionData, "ReactionData", void,
        field<const char*, "name">,
        field<const char*, "targetStateName">
    >;

	using StateData = structure<impl::StateData, "StateData", void,
        field<const char*, "name">,
        field<const char*, "type">,
        field<int, "superState">,
        field<int, "entryChildState">,
        field<dynamic_carray<TriggerData, field_resolver<unsigned int, "triggerCount">>*, "triggers">,
        field<unsigned int, "triggerCount">,
        field<dynamic_carray<ReactionData, field_resolver<unsigned int, "reactionCount">>*, "reactions">,
        field<unsigned int, "reactionCount">
	>;

	using AIStateMachineData = structure<impl::AIStateMachineData, "AIStateMachineData", void,
        field<dynamic_carray<StateData, field_resolver<unsigned int, "stateCount">>*, "states">,
		field<unsigned int, "stateCount">
	>;
}

namespace simplerfl {
    template<> struct canonical<ucsl::resources::aism::v0::ConditionData::FixedBoolArgumentValue> { using type = ucsl::resources::aism::v0::reflections::FixedBoolConditionArgumentValue; };
    template<> struct canonical<ucsl::resources::aism::v0::ConditionData::FixedIntArgumentValue> { using type = ucsl::resources::aism::v0::reflections::FixedIntConditionArgumentValue; };
    template<> struct canonical<ucsl::resources::aism::v0::ConditionData::FixedFloatArgumentValue> { using type = ucsl::resources::aism::v0::reflections::FixedFloatConditionArgumentValue; };
    template<> struct canonical<ucsl::resources::aism::v0::ConditionData::RandomIntArgumentValue> { using type = ucsl::resources::aism::v0::reflections::RandomIntConditionArgumentValue; };
    template<> struct canonical<ucsl::resources::aism::v0::ConditionData::RandomFloatArgumentValue> { using type = ucsl::resources::aism::v0::reflections::RandomFloatConditionArgumentValue; };
    template<> struct canonical<ucsl::resources::aism::v0::ConditionData::AttributeArgumentValue> { using type = ucsl::resources::aism::v0::reflections::AttributeConditionArgumentValue; };
    template<> struct canonical<ucsl::resources::aism::v0::ConditionData::ArgumentType> { using type = ucsl::resources::aism::v0::reflections::ConditionArgumentType; };
    template<> struct canonical<ucsl::resources::aism::v0::ConditionData::Operator> { using type = ucsl::resources::aism::v0::reflections::ConditionOperator; };
    template<> struct canonical<ucsl::resources::aism::v0::ConditionData> { using type = ucsl::resources::aism::v0::reflections::ConditionData; };
    template<> struct canonical<ucsl::resources::aism::v0::TriggerData::TriggerType> { using type = ucsl::resources::aism::v0::reflections::TriggerTriggerType; };
    template<> struct canonical<ucsl::resources::aism::v0::TriggerData::ConditionType> { using type = ucsl::resources::aism::v0::reflections::TriggerConditionType; };
    template<> struct canonical<ucsl::resources::aism::v0::TriggerData> { using type = ucsl::resources::aism::v0::reflections::TriggerData; };
    template<> struct canonical<ucsl::resources::aism::v0::ReactionData> { using type = ucsl::resources::aism::v0::reflections::ReactionData; };
    template<> struct canonical<ucsl::resources::aism::v0::StateData> { using type = ucsl::resources::aism::v0::reflections::StateData; };
    template<> struct canonical<ucsl::resources::aism::v0::AIStateMachineData> { using type = ucsl::resources::aism::v0::reflections::AIStateMachineData; };
}
