#pragma once

namespace ucsl::resources::lodinfo::v1 {
	struct LODInfo {
		unsigned char unk0;
		unsigned char lodCount;
		unsigned int flags[32];
		float unk1s[32];
		unsigned char cascadeLevels[32];
	};
}
