#pragma once

#ifndef EXPORTING_TYPES
#ifndef NO_EIGEN_MATH
#include <Eigen/Eigen>
// #include <unsupported/Eigen/AlignedVector3>

#include "newtype.h"

namespace ucsl::math {
	namespace internal {
		using UnalignedQuaternion = Eigen::Quaternion<float, Eigen::DontAlign>;
	}
	UCSL_NEWTYPE(Vector2, Eigen::Vector2f, Matrix);
	// UCSL_NEWTYPE(Vector3, Eigen::AlignedVector3<float>, AlignedVector3);
	UCSL_NEWTYPE_ALIGNED(Vector3, Eigen::Vector3f, Matrix, 16);
	UCSL_NEWTYPE(Vector4, Eigen::Vector4f, Matrix);
	UCSL_NEWTYPE(Quaternion, Eigen::Quaternionf, Quaternion);
	UCSL_NEWTYPE(Matrix34, Eigen::Affine3f, Transform);
	UCSL_NEWTYPE(Matrix44, Eigen::Projective3f, Transform);
	UCSL_NEWTYPE(Position, Eigen::Vector3f, Matrix);
	UCSL_NEWTYPE(Rotation, internal::UnalignedQuaternion, Quaternion);
}

inline bool operator==(const ucsl::math::Matrix44& one, const ucsl::math::Matrix44& other) {
	return one.matrix() == other.matrix();
}

inline bool operator!=(const ucsl::math::Matrix44& one, const ucsl::math::Matrix44& other) {
	return !(one == other);
}

inline bool operator==(const ucsl::math::Matrix34& one, const ucsl::math::Matrix34& other) {
	return one.matrix() == other.matrix();
}

inline bool operator!=(const ucsl::math::Matrix34& one, const ucsl::math::Matrix34& other) {
	return !(one == other);
}
#endif
#else
namespace ucsl::math {
class Vector2 { public: float x; float y; };
class alignas(16) Vector3 { public: float x; float y; float z; };
class alignas(16) Vector4 { public: float x; float y; float z; float w; };
class alignas(16) Quaternion { public: float x; float y; float z; float w; };
class alignas(16) Matrix44 { public: Vector4 t; Vector4 u; Vector4 v; Vector4 w; };
class alignas(16) Matrix34 { public: Vector4 t; Vector4 u; Vector4 v; Vector4 w; };
class Position { public: float x; float y; float z; };
class Rotation { public: float x; float y; float z; float w; };
}
#endif

static_assert(alignof(ucsl::math::Vector2) == 4);
static_assert(alignof(ucsl::math::Vector3) == 16);
static_assert(alignof(ucsl::math::Vector4) == 16);
static_assert(alignof(ucsl::math::Quaternion) == 16);
static_assert(alignof(ucsl::math::Matrix34) == 16);
static_assert(alignof(ucsl::math::Matrix44) == 16);
static_assert(alignof(ucsl::math::Position) == 4);
static_assert(alignof(ucsl::math::Rotation) == 4);
