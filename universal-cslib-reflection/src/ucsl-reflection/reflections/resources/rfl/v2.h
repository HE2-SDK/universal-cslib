#pragma once
#include <ucsl/resources/rfl/v2.h>
#include <simple-reflection/simple-reflection.h>
#include <ucsl-reflection/operators.h>
#include <ucsl-reflection/reflections/basic-types.h>

namespace ucsl::resources::rfl::v2::reflections {
    using namespace simplerfl;
    using namespace ucsl::reflection;

    namespace impl = ::ucsl::resources::rfl::v2;

    template<typename RflData, typename resolver>
    using Ref2Data = structure<impl::Ref2Data<RflData>, "Ref2Data", void,
        field<constant<unsigned int, 0x32464552>, "magic">,
        field<constant<unsigned int, 0x30302E31>, "version">,
        field<unsigned int, "hash">,
        field<aligned<16, rflclass<resolver>>, "data">
    >;
}

//namespace simplerfl {
//    template<> struct canonical<ucsl::resources::rfl::v2::Ref2Data> { using type = ucsl::resources::rfl::v2::reflections::Ref2Data; };
//}
