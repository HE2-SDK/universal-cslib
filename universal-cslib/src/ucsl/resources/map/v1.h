#pragma once
#include <ucsl/containers/arrays/tarray.h>
#include <ucsl/math.h>
#include <ucsl/object-id.h>

namespace ucsl::resources::map::v1 {
	struct SectorData {
		const char* name;
		unsigned int id;
		unsigned int unk1;
		ucsl::math::Position position;
	};

	struct MapData {
		SectorData* sectors;
		unsigned int sectorCount;
		unsigned int unk1;
	};
}
