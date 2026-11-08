#pragma once
#include <ucsl/resources/pba/v1.h>
#include <simple-reflection/simple-reflection.h>
#include <ucsl-reflection/operators.h>
#include <ucsl-reflection/reflections/basic-types.h>

// Credit to AdelQue for helping with reversing

namespace ucsl::resources::pba::v1::reflections {
	using namespace simplerfl;
	using namespace ucsl::reflection;

	namespace impl = ucsl::resources::pba::v1;

    using RigidBodyType = enumeration<impl::RigidBody::Type, "Type", unsigned char,
        option<"DYNAMIC">,
        option<"STATIC">
    >;

    using RigidBodyShape = enumeration<impl::RigidBody::Shape, "Shape", unsigned char,
        option<"SPHERE">,
        option<"BOX">
    >;

    using RigidBody = structure<impl::RigidBody, "RigidBody", void,
        field<const char*, "boneName">,
        field<RigidBodyType, "type">,
        field<RigidBodyShape, "shape">,
        field<uint8_t, "unk1">,
        field<char, "group">,
        field<float, "shapeRadius">,
        field<float, "shapeHeight">,
        field<float, "shapeDepth">,
        field<float, "mass">,
        field<float, "friction">,
        field<float, "restitution">,
        field<float, "linearDamping">,
        field<float, "angularDamping">,
        field<ucsl::math::Vector3, "offsetPosition">,
        field<ucsl::math::Quaternion, "offsetRotation">
    >;

    using LimitMode = enumeration<impl::LimitMode, "LimitMode", unsigned char,
        option<"NONE">,
        option<"ENABLED">,
        option<"DISABLED">
    >;

    using Limit = structure<impl::Limit, "Limit", void,
        field<LimitMode, "mode">,
        field<bool, "springEnabled">,
        field<float, "lowLimit">,
        field<float, "highLimit">,
        field<float, "springStiffness">,
        field<float, "springDamping">
    >;

    using Constraint = structure<impl::Constraint, "Constraint", void,
        field<const char*, "boneName">,
        field<bool, "disableCollisionsBetweenLinkedBodies">,
        field<char, "unk1">,
        field<unsigned short, "overrideSolverIterationCount">,
        field<short, "parentRigidBodyIndex">,
        field<short, "childRigidBodyIndex">,
        field<short, "skeletonParentBoneIndex">,
        field<Limit[3], "angularLimits">,
        field<Limit[3], "linearLimits">,
        field<ucsl::math::Vector3, "offsetPositionA">,
        field<ucsl::math::Quaternion, "offsetRotationA">,
        field<ucsl::math::Vector3, "offsetPositionB">,
        field<ucsl::math::Quaternion, "offsetRotationB">
    >;

    using Node = structure<impl::Node, "Node", void,
        field<const char*, "boneName">,
        field<float, "mass">,
        field<short, "rigidBodyIndex">,
        field<bool, "isPinned">,
        field<short, "positiveXSiblingIndex">,
        field<short, "negativeXSiblingIndex">,
        field<short, "positiveYSiblingIndex">,
        field<short, "negativeYSiblingIndex">,
        field<short, "positiveZSiblingIndex">,
        field<short, "negativeZSiblingIndex">
    >;

    using Link = structure<impl::Link, "Link", void,
        field<short[2], "verts">,
        field<float, "restLength">,
        field<float, "stiffness">
    >;

    using SoftBody = structure<impl::SoftBody, "SoftBody", void,
        field<const char*, "name">,
        field<float, "margin">,
        field<float, "dampingCoeff">,
        field<float, "dragCoeff">,
        field<float, "liftCoeff">,
        field<float, "dynamicFrictionCoeff">,
        field<float, "poseMatchingCoeff">,
        field<float, "rigidContactsHardness">,
        field<float, "kineticContactsHardness">,
        field<float, "softContactsHardness">,
        field<float, "anchorsHardness">,
        field<char, "positionSolverIterationCount">,
        field<char, "unk0">,
        field<short, "group">,
        field<unsigned int, "nodeCount">,
        field<unsigned int, "linkCount">,
        field<dynamic_carray<Node, field_resolver<unsigned int, "nodeCount">>*, "nodes">,
        field<dynamic_carray<Link, field_resolver<unsigned int, "linkCount">>*, "links">
    >;

    using PbaData = structure<impl::PbaData, "PbaData", void,
        field<unsigned int, "magic">,
        field<unsigned int, "version">,
        field<const char*, "skeletonName">,
        field<unsigned int, "rigidBodyCount">,
        field<unsigned int, "constraintCount">,
        field<dynamic_carray<RigidBody, field_resolver<unsigned int, "rigidBodyCount">>*, "rigidBodies">,
        field<dynamic_carray<Constraint, field_resolver<unsigned int, "constraintCount">>*, "constraints">,
        field<unsigned int, "softBodyCount">,
        field<dynamic_carray<SoftBody, field_resolver<unsigned int, "softBodyCount">>*, "softBodies">
    >;
}

namespace simplerfl {
	template<> struct canonical<ucsl::resources::pba::v1::RigidBody> { using type = ucsl::resources::pba::v1::reflections::RigidBody; };
	template<> struct canonical<ucsl::resources::pba::v1::LimitMode> { using type = ucsl::resources::pba::v1::reflections::LimitMode; };
	template<> struct canonical<ucsl::resources::pba::v1::Limit> { using type = ucsl::resources::pba::v1::reflections::Limit; };
	template<> struct canonical<ucsl::resources::pba::v1::Constraint> { using type = ucsl::resources::pba::v1::reflections::Constraint; };
	template<> struct canonical<ucsl::resources::pba::v1::Node> { using type = ucsl::resources::pba::v1::reflections::Node; };
	template<> struct canonical<ucsl::resources::pba::v1::Link> { using type = ucsl::resources::pba::v1::reflections::Link; };
	template<> struct canonical<ucsl::resources::pba::v1::SoftBody> { using type = ucsl::resources::pba::v1::reflections::SoftBody; };
	template<> struct canonical<ucsl::resources::pba::v1::PbaData> { using type = ucsl::resources::pba::v1::reflections::PbaData; };
}
