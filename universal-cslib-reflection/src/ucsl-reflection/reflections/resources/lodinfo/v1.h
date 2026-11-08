#pragma once
#include <ucsl/math.h>
#include <ucsl/resources/lodinfo/v1.h>

namespace ucsl::resources::lodinfo::v1::reflections {
	using namespace simplerfl;
	using namespace ucsl::reflection;

	namespace impl = ucsl::resources::lodinfo::v1;

	using LODInfo = structure<impl::LODInfo, "LODInfo", void,
		field<unsigned char, "unk0">,
		field<unsigned char, "lodCount">,
		field<unsigned int[32], "flags">,
		field<float[32], "unk1s">,
		field<unsigned char[32], "cascadeLevels">
	>;
}

namespace simplerfl {
	template<> struct canonical<ucsl::resources::lodinfo::v1::LODInfo> { using type = ucsl::resources::lodinfo::v1::reflections::LODInfo; };
}
