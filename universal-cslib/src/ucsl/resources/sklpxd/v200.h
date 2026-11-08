#pragma once
#include <ucsl/math.h>

namespace ucsl::resources::sklpxd::v200{
    template<typename AllocatorSystem>
    struct SkeletonPXDData{
        unsigned int magic;
		unsigned int version;
        ucsl::containers::arrays::Array<unsigned short, AllocatorSystem> boneParentIndices;
        ucsl::containers::arrays::Array<ucsl::strings::VariableString, AllocatorSystem> boneNames;
        ucsl::containers::arrays::Array<ucsl::math::Matrix34, AllocatorSystem> boneMatrices;
    };
};
