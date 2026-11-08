#pragma once
#include <ucsl/resources/rfl/v1.h>
#include <simple-reflection/simple-reflection.h>
#include <ucsl-reflection/operators.h>
#include <ucsl-reflection/reflections/basic-types.h>

namespace ucsl::resources::rfl::v1::reflections {
    using namespace simplerfl;
    using namespace ucsl::reflection;

    namespace impl = ::ucsl::resources::rfl::v1;

    template<typename RflData, typename resolver>
    using Ref1Data = structure<impl::Ref1Data<RflData>, "Ref1Data", void,
        field<aligned<16, rflclass<resolver>>, "data">
    >;
}
