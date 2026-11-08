#pragma once
#include <ucsl/resources/nxs/v1.h>
#include <simple-reflection/simple-reflection.h>
#include <ucsl-reflection/operators.h>
#include <ucsl-reflection/reflections/basic-types.h>

namespace ucsl::resources::nxs::v1::reflections {
	using namespace simplerfl;
	using namespace ucsl::reflection;

	namespace impl = ucsl::resources::nxs::v1;

	using MaterialData = structure<impl::MaterialData, "MaterialData", void,
		field<unsigned int, "unk1">,
		field<unsigned int, "flags">
	>;

	using NXSData = structure<impl::NXSData, "NXSData", void,
		field<unsigned int, "magic">,
		field<unsigned int, "version">,
		field<unsigned int, "mesh1VertexCount">,
		field<unsigned int, "mesh1TriangleCount">,
		field<size_t, "materialCount">,
		field<size_t, "mesh1Size">,
		field<size_t, "mesh2Size">,
		field<size_t, "mesh3Size">,
		field<dynamic_carray<MaterialData, field_resolver<size_t, "materialCount">>*, "materials">,
		field<dynamic_carray<uint8_t, field_resolver<size_t, "mesh1Size">>*, "mesh1">,
		field<dynamic_carray<uint8_t, field_resolver<size_t, "mesh2Size">>*, "mesh2">,
		field<dynamic_carray<uint8_t, field_resolver<size_t, "mesh3Size">>*, "mesh3">
	>;
}

namespace simplerfl {
	template<> struct canonical<ucsl::resources::nxs::v1::NXSData> { using type = ucsl::resources::nxs::v1::reflections::NXSData; };
}
