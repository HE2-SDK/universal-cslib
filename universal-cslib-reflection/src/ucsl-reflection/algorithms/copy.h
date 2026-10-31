#pragma once
#include <ucsl/strings/variable-string.h>
#include <ucsl-reflection/accessors/types.h>
#include <ucsl-reflection/traversals/types.h>
#include <ucsl-reflection/util/memory.h>

namespace ucsl::reflection::algorithms {
	template<typename GameInterface>
	class Copy {
	public:
		static inline void process_primitive_data(auto& obj1, const auto& obj2) {
			obj1 = obj2;
		}

		static inline void process_primitive_data(ucsl::strings::VariableString& obj1, const ucsl::strings::VariableString& obj2) {
			if (obj1.GetAllocator())
				obj1.Set(obj2.c_str());
			else
				obj1.Set(obj2.c_str(), -1, GameInterface::AllocatorSystem::get_allocator());

		}

		static inline void process_primitive_data(const char*& obj1, const char* const& obj2) {
			assert(false && "copy currently doesn't handle pointers");
		}

		static inline void process_primitive(accessors::PrimitiveAccessor auto& obj1, const accessors::PrimitiveAccessor auto& obj2) {
			obj1.visit([&](auto& obj1Data) {
				obj2.visit([&](const auto& obj2Data) {
					if constexpr (std::is_same_v<std::decay_t<decltype(obj1Data.refl)>, std::decay_t<decltype(obj2Data.refl)>>)
						process_primitive_data(*obj1Data.value, *obj2Data.value);
				});
			});
		}

		static inline void process_enum(auto& obj1, const auto& obj2) {
			obj1 = obj2;
		}

		static inline void process_bitfield(auto& obj1, const auto& obj2) {
			obj1.visit([&](auto& obj1Underlying) {
				obj2.visit([&](const auto& obj2Underlying) {
					if constexpr (std::is_same_v<std::decay_t<decltype(obj1Underlying.refl)>, std::decay_t<decltype(obj2Underlying.refl)>>)
						*obj1Underlying.value = *obj2Underlying.value;
				});
			});
		}

		static inline void process_array(auto& arr1, const auto& arr2) {
			arr1.clear();

			for (auto item : arr2) {
				auto item1 = arr1.emplace_back();

				process_type(item1, item);
			}
		}

		static inline void process_tarray(auto& arr1, const auto& arr2) {
			arr1.clear();

			for (auto item : arr2) {
				auto item1 = arr1.emplace_back();

				process_type(item1, item);
			}
		}

		static inline void process_carray(accessors::CArrayAccessor auto& obj1, const accessors::CArrayAccessor auto& obj2) {
			assert(obj1.size() == obj2.size() && "carray copy currently expects equal size arrays");

			for (size_t i = 0; i < obj1.size(); i++) {
				auto item1 = obj1[i];

				process_type(item1, obj2[i]);
			}
		}

		static inline void process_pointer(accessors::PointerAccessor auto& obj1, const accessors::PointerAccessor auto& obj2) {
			assert(false && "copy currently doesn't handle pointers");
		}

		static inline void process_union(accessors::UnionAccessor auto& obj1, const accessors::UnionAccessor auto& obj2) {
			obj1.refl.visit_current_field([&](const auto& fieldRefl) {
				auto field = obj1[fieldRefl];

				process_type(field, obj2[fieldRefl]);
			});
		}

		static inline void process_fields(accessors::StructureAccessor auto& obj1, const accessors::StructureAccessor auto& obj2) {
			auto base1 = obj1.get_base();
			auto base2 = obj2.get_base();

			if (base1.has_value() && base2.has_value())
				process_fields(base1.value(), base2.value());

			obj1.refl.visit_fields<size_t>(obj1, [&](const auto& fieldRefl) {
				auto field = obj1[fieldRefl];

				process_type(field, obj2[fieldRefl]);
			});
		}

		static inline void process_struct(accessors::StructureAccessor auto& obj1, const accessors::StructureAccessor auto& obj2) {
			process_fields(obj1, obj2);
		}

		static inline void process_type(accessors::ValueAccessor auto& obj1, const accessors::ValueAccessor auto& obj2) {
			obj1.visit([&](auto& v1) {
				obj2.visit([&](const auto& v2) {
					if constexpr (!std::is_same_v<std::decay_t<decltype(v1.refl)>, std::decay_t<decltype(v2.refl)>>) return;
					else if constexpr (decltype(v1.refl)::kind == providers::TypeKind::PRIMITIVE) process_primitive(v1, v2);
					else if constexpr (decltype(v1.refl)::kind == providers::TypeKind::ENUM) process_enum(v1, v2);
					else if constexpr (decltype(v1.refl)::kind == providers::TypeKind::BITFIELD) process_bitfield(v1, v2);
					else if constexpr (decltype(v1.refl)::kind == providers::TypeKind::ARRAY) process_array(v1, v2);
					else if constexpr (decltype(v1.refl)::kind == providers::TypeKind::TARRAY) process_tarray(v1, v2);
					else if constexpr (decltype(v1.refl)::kind == providers::TypeKind::CARRAY) process_carray(v1, v2);
					else if constexpr (decltype(v1.refl)::kind == providers::TypeKind::POINTER) process_pointer(v1, v2);
					else if constexpr (decltype(v1.refl)::kind == providers::TypeKind::UNION) process_union(v1, v2);
					else if constexpr (decltype(v1.refl)::kind == providers::TypeKind::STRUCTURE) process_struct(v1, v2);
					else static_assert(false, "invalid type kind");
				});
			});
		}
	};
}
