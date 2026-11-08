#pragma once
#include <ucsl/math.h>
#include <simple-reflection/simple-reflection.h>
#include <ucsl-reflection/operators.h>
#include <ucsl-reflection/reflections/basic-types.h>

// reversed from files

namespace ucsl::resources::terrain_model::v1::reflections {
    using namespace simplerfl;
    using namespace ucsl::reflection;

    namespace impl = ::ucsl::resources::terrain_model::v1::reflections;

    using Material = structure<impl::Material, "Material", void,
        field<const char*, "materialType">,
        field<int, "id">,
        field<int, "unk0">,
        field<int, "unk1">,
        field<const char*, "detailAbdTextureName">,
        field<const char*, "detailNrmTextureName">,
        field<const char*, "detailHgtTextureName">,
        field<const char*, "abdTextureName">,
        field<const char*, "nrmTextureName">,
        field<const char*, "prmTextureName">
    >;

    using TerrainMaterialData = structure<impl::TerrainMaterialData, "TerrainMaterialData", void,
        field<unsigned int, "magic">,
        field<unsigned int, "version">,
        field<dynamic_carray<Material, field_resolver<int, "materialCount">>*, "materials">
        field<int, "materialCount">
    >;
}

namespace simplerfl {
    template<> struct canonical<ucsl::resources::terrain_model::v1::Material> { using type = ucsl::resources::terrain_model::v1::reflections::Material; };
    template<> struct canonical<ucsl::resources::terrain_model::v1::TerrainMaterialData> { using type = ucsl::resources::terrain_model::v1::reflections::TerrainMaterialData; };
}
