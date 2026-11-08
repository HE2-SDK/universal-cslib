#pragma once
#include <ucsl/resources/effdb/v100.h>
#include <simple-reflection/simple-reflection.h>
#include <ucsl-reflection/operators.h>
#include <ucsl-reflection/reflections/basic-types.h>

// reversed from files

namespace ucsl::resources::effdb::v100::reflections {
    using namespace simplerfl;
    using namespace ucsl::reflection;

    namespace impl = ::ucsl::resources::effdb::v100;

    using EmitterTransType = enumeration<impl::Emitter::TransType, "Emitter::TransType", unsigned int,
        option<"NODE">,
        option<"NODE_AND_FRAME">,
        option<"NODE_POSITION">
    >;

    using Emitter = structure<impl::Emitter, "Emitter", void,
        field<bitfield<unsigned int,
            component<bool, "isAttached", 0, 1>,
            component<bool, "disableBoneScale", 1, 1>,
            component<EmitterTransType, "transType", 7, 2>,
            component<bool, "usePosition", 9, 1>,
            component<bool, "useRotation", 10, 1>,
            component<bool, "useScale", 11, 1>
        >, "flags">,
        field<unsigned int, "attachTimeInFrames">,
        field<const char*, "emitterName">,
        field<const char*, "boneName">,
        field<math::Vector3, "position">,
        field<math::Quaternion, "rotation">,
        field<math::Vector3, "scale">
    >;

    using EmitterSet = structure<impl::EmitterSet, "EmitterSet", void,
        field<const char*, "clipName">,
        field<unsigned int, "emitterCount">,
        field<dynamic_carray<Emitter, field_resolver<unsigned int, "emitterCount">>*, "emitters">,
        field<unsigned int, "soundNameCount">,
        field<dynamic_carray<const char*, field_resolver<unsigned int, "soundNameCount">>*, "soundNames">
    >;

    using EffdbData = structure<impl::EffdbData, "EffdbData", void,
        field<unsigned int, "magic">,
        field<unsigned int, "version">,
        field<unsigned int, "emitterSetCount">,
        field<dynamic_carray<EmitterSet, field_resolver<unsigned int, "emitterSetCount">>*, "emitterSets">
    >;
}

namespace simplerfl {
    template<> struct canonical<ucsl::resources::effdb::v100::EffdbData> { using type = ucsl::resources::effdb::v100::reflections::EffdbData; };
}
