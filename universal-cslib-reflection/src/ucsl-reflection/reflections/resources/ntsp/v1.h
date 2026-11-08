#pragma once
#include <ucsl/resources/ntsp/v1.h>
#include <simple-reflection/simple-reflection.h>
#include <ucsl-reflection/operators.h>
#include <ucsl-reflection/reflections/basic-types.h>

namespace ucsl::resources::ntsp::v1::reflections {
	using namespace simplerfl;
	using namespace ucsl::reflection;

	namespace impl = ucsl::resources::ntsp::v1;

    using Texture = structure<impl::Texture, "Texture", void,
        field<unsigned int, "nameHash">,
        field<unsigned int, "mipmapOffset">,
        field<unsigned int, "mipmapCount">,
        field<unsigned short, "width">,
        field<unsigned short, "height">,
        field<const char*, "name">
    >;

    using Mipmap = structure<impl::Mipmap, "Mipmap", void,
        field<dynamic_carray<uint8_t, field_resolver<size_t, "size">>*, "data">,
        field<size_t, "size">
    >;

    using NeedleTextureStreamingPackage = structure < impl::NeedleTextureStreamingPackage, "NeedleTextureStreamingPackage", void,
        field<unsigned int, "magic">,
        field<unsigned int, "version">,
        field<unsigned int, "textureCount">,
        field<unsigned int, "mipmapCount">,
        field<size_t, "tocSize">,
        field<dynamic_carray<Texture, field_resolver<unsigned int, "textureCount">>, "textures">,
        field<dynamic_carray<Mipmap, field_resolver<unsigned int, "mipmapCount">>, "mipmaps">
    >;
}

namespace simplerfl {
	template<> struct canonical<ucsl::resources::ntsp::v1::NeedleTextureStreamingPackage> { using type = ucsl::resources::ntsp::v1::reflections::NeedleTextureStreamingPackage; };
}
