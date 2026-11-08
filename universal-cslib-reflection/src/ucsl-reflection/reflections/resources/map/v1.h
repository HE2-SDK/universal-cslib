#pragma once
#include <ucsl/resources/map/v1.h>
#include <simple-reflection/simple-reflection.h>
#include <ucsl-reflection/operators.h>
#include <ucsl-reflection/reflections/basic-types.h>

namespace ucsl::resources::map::v1::reflections {
	using namespace simplerfl;
	using namespace ucsl::reflection;

	namespace impl = ucsl::resources::map::v1;

	using SectorData = structure<impl::SectorData, "SectorData", void,
		field<const char*, "name">,
		field<unsigned int, "id">,
		field<unsigned int, "unk1">,
		field<ucsl::math::Position, "position">
	>;

	using MapData = structure<impl::MapData, "MapData", void,
		field<dynamic_carray<SectorData, field_resolver<unsigned int, "sectorCount">>*, "sectors">,
		field<unsigned int, "sectorCount">,
		field<unsigned int, "unk1">
	>;
}

namespace simplerfl {
	template<> struct canonical<ucsl::resources::map::v1::SectorData> { using type = ucsl::resources::map::v1::reflections::SectorData; };
	template<> struct canonical<ucsl::resources::map::v1::MapData> { using type = ucsl::resources::map::v1::reflections::MapData; };
}
