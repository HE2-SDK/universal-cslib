#pragma once

namespace ucsl::resources::light::v2 {
    enum class LightType : unsigned int {
        SUN,
        POINT,
        SPOT
    };

    struct PointLightData {
        float color[3];
    private:
        char unused0[16];
    public:
        float attenuationRadius;
        bool shadowEnabled;
    private:
        char unused1[16];
    };

    struct SpotLightData {
        csl::math::Position rotation;
        float color[3];
        float innerConeAngle;
        float outerConeAngle;
        float attenuationRadius;
        bool shadowEnabled;
    private:
        char unused1[16];
    };

	struct ResMirageLightData {
        LightType type;
        csl::math::Position position;
        union{ //temporary solution
            PointLightData pointLight;
            SpotLightData spotLight;
        };
	};
}
