#pragma once
#include <cassert>
#include <ucsl/resources/fxcol/v1.h>
#include <simple-reflection/simple-reflection.h>
#include <ucsl-reflection/operators.h>
#include <ucsl-reflection/reflections/basic-types.h>

namespace ucsl::resources::fxcol::v1::reflections {
    using namespace simplerfl;
    using namespace ucsl::reflection;

    namespace impl = ucsl::resources::fxcol::v1;

    using Shape = enumeration<impl::ShapeData::Shape, "Shape", unsigned char,
        option<"SPHERE">,
        option<"CYLINDER">,
        option<"ANISOTROPIC_OBB">,
        option<"ISOTROPIC_OBB">
    >;

    using Type = enumeration<impl::ShapeData::Type, "Type", unsigned char,
        option<"SCENE_PARAMETER_INDEX">,
        option<"LIGHT_PARAMETER_INDEX">,
        option<"CAMERA">,
        option<"HEAT_HAZE">
    >;

    using SphereExtents = structure<impl::ShapeData::SphereExtents, "SphereExtents", void,
        field<float, "radius">,
        field<float, "borderThickness">
    >;

    using CylinderExtents = structure<impl::ShapeData::CylinderExtents, "CylinderExtents", void,
        field<float, "radius">,
        field<float, "halfHeight">,
        field<float, "borderThickness">
    >;

    using AnisotropicObbExtents = structure<impl::ShapeData::AnisotropicObbExtents, "AnisotropicObbExtents", void,
        field<float, "depth">,
        field<float, "width">,
        field<float, "height">,
        field<float, "maybeWidthAndHeightBorderThickness">,
        field<float, "positiveDepthBorderThickness">,
        field<float, "negativeDepthBorderThickness">
    >;

    using IsotropicObbExtents = structure<impl::ShapeData::IsotropicObbExtents, "IsotropicObbExtents", void,
        field<float, "depth">,
        field<float, "width">,
        field<float, "height">,
        field<float, "borderThickness">
    >;
    
    inline size_t get_shape_idx(const impl::ShapeData::Shape& shape) {
        switch (shape) {
        case impl::ShapeData::Shape::SPHERE: return 0;
        case impl::ShapeData::Shape::CYLINDER: return 1;
        case impl::ShapeData::Shape::ANISOTROPIC_OBB: return 2;
        case impl::ShapeData::Shape::ISOTROPIC_OBB: return 3;
        default: assert(false && "unknown shape"); return 0;
        }
    }

    using Extents = unionof<impl::ShapeData::Extents, "Extents", selector_resolver<size_t, field_resolver<impl::ShapeData::Shape, "shape">>::impl<get_shape_idx>,
        field<SphereExtents, "sphere">,
        field<CylinderExtents, "cylinder">,
        field<AnisotropicObbExtents, "anisotropicObb">,
        field<IsotropicObbExtents, "isotropicObb">
    >;

    using SceneParameterIndexParameters = structure<impl::ShapeData::SceneParameterIndexParameters, "SceneParameterIndexParameters", void,
        field<unsigned int, "sceneParameterIndex">,
        field<float, "interpolationTime">
    >;

    using LightParameterIndexParameters = structure<impl::ShapeData::LightParameterIndexParameters, "LightParameterIndexParameters", void,
        field<unsigned int, "lightParameterIndex">
    >;

    using HeatHazeParameters = structure<impl::ShapeData::HeatHazeParameters, "HeatHazeParameters", void,
        field<float, "heatHazeMaxHeight">,
        field<float, "interpolationTime">
    >;

    using CameraParameters = structure<impl::ShapeData::CameraParameters, "CameraParameters", void,
        field<unsigned int, "unk1">,
        field<unsigned int, "unk2">,
        field<float, "bloomScale">,
        field<float, "autoExposureMiddleGray">,
        field<unsigned int, "unk3">,
        field<float, "luminance">,
        field<float, "autoExposureAdaptedRatio">,
        field<float, "interpolationTime">
    >;

    inline size_t get_shape_type_idx(const impl::ShapeData::Type& type) {
        switch (type) {
        case impl::ShapeData::Type::SCENE_PARAMETER_INDEX: return 0;
        case impl::ShapeData::Type::LIGHT_PARAMETER_INDEX: return 1;
        case impl::ShapeData::Type::HEAT_HAZE: return 2;
        case impl::ShapeData::Type::CAMERA: return 3;
        default: assert(false && "unknown type"); return 0;
        }
    }

    using Parameters = unionof<impl::ShapeData::Parameters, "Parameters", selector_resolver<size_t, field_resolver<impl::ShapeData::Type, "type">>::impl<get_shape_type_idx>,
        field<SceneParameterIndexParameters, "sceneParameterIndex">,
        field<LightParameterIndexParameters, "lightParameterIndex">,
        field<HeatHazeParameters, "heatHaze">,
        field<CameraParameters, "camera">
    >;

    using ShapeData = structure<impl::ShapeData, "ShapeData", void,
        field<const char*, "name">,
        field<Shape, "shape">,
        field<Type, "type">,
        field<unsigned char, "unk1">,
        field<unsigned char, "priority">,
        field<Extents, "extents">,
        field<Parameters, "parameters">,
        field<const char*, "unk2">,
        field<math::Position, "position">,
        field<math::Rotation, "rotation">
    >;

    using KdTreeNodeType = enumeration<impl::KdTreeNodeType, "KdTreeNodeType", unsigned char,
        option<"SPLIT_X">,
        option<"SPLIT_Y">,
        option<"SPLIT_Z">,
        option<"LEAF">
    >;

    using KdTreeNodeData = structure<impl::KdTreeNodeData, "KdTreeNodeData", void,
        field<int, "deadZoneStartCoordOrLeafIndexAndNodeType">,
        field<float, "deadZoneEndCoord">
    >;

    using KdTreeLeafData = structure<impl::KdTreeLeafData, "KdTreeLeafData", void,
        field<unsigned int, "shapeCount">,
        field<int, "shapeOffset">,
        field<math::Position, "aabbMin">,
        field<math::Position, "aabbMax">
    >;

    using FxColData = structure<impl::FxColData, "FxColData", void,
        field<constant<unsigned int, 0x4658434F>, "magic">,
        field<constant<unsigned int, 1>, "version">,
        field<unsigned int, "shapeCount">,
        field<dynamic_carray<ShapeData, field_resolver<unsigned int, "shapeCount">>*, "shapes">,
        field<unsigned int, "kdTreeLeafCount">,
        field<dynamic_carray<KdTreeLeafData, field_resolver<unsigned int, "kdTreeLeafCount">>*, "kdTreeLeaves">,
        field<unsigned int, "kdTreeNodeCount">,
        field<dynamic_carray<KdTreeNodeData, field_resolver<unsigned int, "kdTreeNodeCount">>*, "kdTreeNodes">
    >;
}

namespace simplerfl {
    template<> struct canonical<ucsl::resources::fxcol::v1::ShapeData::Shape> { using type = ucsl::resources::fxcol::v1::reflections::Shape; };
    template<> struct canonical<ucsl::resources::fxcol::v1::ShapeData::Type> { using type = ucsl::resources::fxcol::v1::reflections::Type; };
    template<> struct canonical<ucsl::resources::fxcol::v1::ShapeData::SphereExtents> { using type = ucsl::resources::fxcol::v1::reflections::SphereExtents; };
    template<> struct canonical<ucsl::resources::fxcol::v1::ShapeData::CylinderExtents> { using type = ucsl::resources::fxcol::v1::reflections::CylinderExtents; };
    template<> struct canonical<ucsl::resources::fxcol::v1::ShapeData::AnisotropicObbExtents> { using type = ucsl::resources::fxcol::v1::reflections::AnisotropicObbExtents; };
    template<> struct canonical<ucsl::resources::fxcol::v1::ShapeData::IsotropicObbExtents> { using type = ucsl::resources::fxcol::v1::reflections::IsotropicObbExtents; };
    template<> struct canonical<ucsl::resources::fxcol::v1::ShapeData::Extents> { using type = ucsl::resources::fxcol::v1::reflections::Extents; };
    template<> struct canonical<ucsl::resources::fxcol::v1::ShapeData::SceneParameterIndexParameters> { using type = ucsl::resources::fxcol::v1::reflections::SceneParameterIndexParameters; };
    template<> struct canonical<ucsl::resources::fxcol::v1::ShapeData::LightParameterIndexParameters> { using type = ucsl::resources::fxcol::v1::reflections::LightParameterIndexParameters; };
    template<> struct canonical<ucsl::resources::fxcol::v1::ShapeData::HeatHazeParameters> { using type = ucsl::resources::fxcol::v1::reflections::HeatHazeParameters; };
    template<> struct canonical<ucsl::resources::fxcol::v1::ShapeData::CameraParameters> { using type = ucsl::resources::fxcol::v1::reflections::CameraParameters; };
    template<> struct canonical<ucsl::resources::fxcol::v1::ShapeData::Parameters> { using type = ucsl::resources::fxcol::v1::reflections::Parameters; };
    template<> struct canonical<ucsl::resources::fxcol::v1::ShapeData> { using type = ucsl::resources::fxcol::v1::reflections::ShapeData; };
    template<> struct canonical<ucsl::resources::fxcol::v1::KdTreeNodeType> { using type = ucsl::resources::fxcol::v1::reflections::KdTreeNodeType; };
    template<> struct canonical<ucsl::resources::fxcol::v1::KdTreeNodeData> { using type = ucsl::resources::fxcol::v1::reflections::KdTreeNodeData; };
    template<> struct canonical<ucsl::resources::fxcol::v1::KdTreeLeafData> { using type = ucsl::resources::fxcol::v1::reflections::KdTreeLeafData; };
    template<> struct canonical<ucsl::resources::fxcol::v1::FxColData> { using type = ucsl::resources::fxcol::v1::reflections::FxColData; };
}
