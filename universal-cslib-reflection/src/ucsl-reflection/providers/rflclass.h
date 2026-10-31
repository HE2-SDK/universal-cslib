#pragma once
#include <cassert>
#include <simple-reflection/simple-reflection.h>
#include <ucsl/containers/arrays/array.h>
#include <ucsl/containers/arrays/tarray.h>
#include <ucsl/rfl/rflclass.h>
#include <ucsl/rfl/ranges.h>
#include <ucsl/math.h>
#include <ucsl/colors.h>
#include <ucsl/strings/variable-string.h>
#include <ucsl-reflection/accessors/types.h>
#include <ucsl-reflection/opaque.h>
#include <ucsl-reflection/util/memory.h>
#include <vector>
#include "types.h"

namespace ucsl::reflection::providers {
	template<typename GameInterface>
	struct rflclass {
		using MemberType = typename GameInterface::RflSystem::RflClassMember::Type;

		struct Primitive {
			const GameInterface::RflSystem::RflClassMember* member;
			MemberType type;

			constexpr static TypeKind kind = TypeKind::PRIMITIVE;
			template<typename F>
			auto visit(F f) const {
				if constexpr (GameInterface::RflSystem::TypeSet::template supports_primitive<objectids::ObjectIdV1>)
					if (type == MemberType::OBJECT_ID_V1)
						return f(PrimitiveData<objectids::ObjectIdV1>{});

				if constexpr (GameInterface::RflSystem::TypeSet::template supports_primitive<objectids::ObjectIdV2>)
					if (type == MemberType::OBJECT_ID_V2)
						return f(PrimitiveData<objectids::ObjectIdV2>{});

				if constexpr (GameInterface::RflSystem::TypeSet::template supports_primitive<colors::Color8RGBA>)
					if (type == MemberType::COLOR_BYTE_RGBA)
						return f(PrimitiveData<colors::Color8RGBA>{});

				if constexpr (GameInterface::RflSystem::TypeSet::template supports_primitive<colors::ColorfRGBA>)
					if (type == MemberType::COLOR_FLOAT_RGBA)
						return f(PrimitiveData<colors::ColorfRGBA>{});

				if constexpr (GameInterface::RflSystem::TypeSet::template supports_primitive<colors::Color8ABGR>)
					if (type == MemberType::COLOR_BYTE_ABGR)
						return f(PrimitiveData<colors::Color8ABGR>{});

				if constexpr (GameInterface::RflSystem::TypeSet::template supports_primitive<colors::ColorfABGR>)
					if (type == MemberType::COLOR_FLOAT_ABGR)
						return f(PrimitiveData<colors::ColorfABGR>{});

				switch (type) {
				case MemberType::BOOL: return f(PrimitiveData<bool>{});
				case MemberType::SINT8: return f(PrimitiveData<int8_t>{ .range = member->GetRange<ucsl::rfl::ranges::RangeSint32>() });
				case MemberType::UINT8: return f(PrimitiveData<uint8_t>{ .range = member->GetRange<ucsl::rfl::ranges::RangeUint32>() });
				case MemberType::SINT16: return f(PrimitiveData<int16_t>{ .range = member->GetRange<ucsl::rfl::ranges::RangeSint32>() });
				case MemberType::UINT16: return f(PrimitiveData<uint16_t>{ .range = member->GetRange<ucsl::rfl::ranges::RangeUint32>() });
				case MemberType::SINT32: return f(PrimitiveData<int32_t>{ .range = member->GetRange<ucsl::rfl::ranges::RangeSint32>() });
				case MemberType::UINT32: return f(PrimitiveData<uint32_t>{ .range = member->GetRange<ucsl::rfl::ranges::RangeUint32>() });
				case MemberType::SINT64: return f(PrimitiveData<int64_t>{ .range = member->GetRange<ucsl::rfl::ranges::RangeSint64>() });
				case MemberType::UINT64: return f(PrimitiveData<uint64_t>{ .range = member->GetRange<ucsl::rfl::ranges::RangeUint64>() });
				case MemberType::FLOAT: return f(PrimitiveData<float>{ .range = member->GetRange<ucsl::rfl::ranges::RangeFloat>() });
				case MemberType::VECTOR2: return f(PrimitiveData<math::Vector2>{ .range = member->GetRange<ucsl::rfl::ranges::RangeVector2>() });
				case MemberType::VECTOR3: return f(PrimitiveData<math::Vector3>{ .range = member->GetRange<ucsl::rfl::ranges::RangeVector3>() });
				case MemberType::VECTOR4: return f(PrimitiveData<math::Vector4>{ .range = member->GetRange<ucsl::rfl::ranges::RangeVector4>() });
				case MemberType::QUATERNION: return f(PrimitiveData<math::Quaternion>{});
				case MemberType::MATRIX34: return f(PrimitiveData<math::Matrix34>{});
				case MemberType::MATRIX44: return f(PrimitiveData<math::Matrix44>{});
				case MemberType::CSTRING: return f(PrimitiveData<const char*>{});
				case MemberType::STRING: return f(PrimitiveData<strings::VariableString>{});
				case MemberType::POSITION: return f(PrimitiveData<math::Position>{});
				default: assert(!"reflective operation assertion failed: unknown primitive type"); return f(PrimitiveData<bool>{});
				}
			}

			constexpr static bool is_erased() { return false; }
		};

		struct SubType;
		struct Type;

		struct Pointer {
			const GameInterface::RflSystem::RflClassMember* member;

			constexpr static TypeKind kind = TypeKind::POINTER;
			SubType get_target_type() const { return { member }; }
			constexpr static bool is_weak() { return false; }
		};

		struct Array {
			const GameInterface::RflSystem::RflClassMember* member;

			constexpr static TypeKind kind = TypeKind::ARRAY;
			SubType get_item_type() const { return { member }; }
		};

		struct TArray {
			const GameInterface::RflSystem::RflClassMember* member;

			constexpr static TypeKind kind = TypeKind::TARRAY;
			SubType get_item_type() const { return { member }; }
		};

		struct CArray {
			const GameInterface::RflSystem::RflClassMember* member;

			constexpr static TypeKind kind = TypeKind::CARRAY;
			Type get_item_type() const { return { member, false }; }
			size_t get_length() const { return member->GetArrayLength(); }
		};

		struct Enum {
			const GameInterface::RflSystem::RflClassMember* member;

			constexpr static TypeKind kind = TypeKind::ENUM;
			decltype(auto) get_options() const { return member->GetEnum()->GetValues(); }
			template<typename F>
			auto visit(F f) const {
				switch (member->GetSubType()) {
				case MemberType::SINT8: return f(PrimitiveData<int8_t>{});
				case MemberType::UINT8: return f(PrimitiveData<uint8_t>{});
				case MemberType::SINT16: return f(PrimitiveData<int16_t>{});
				case MemberType::UINT16: return f(PrimitiveData<uint16_t>{});
				case MemberType::SINT32: return f(PrimitiveData<int32_t>{});
				case MemberType::UINT32: return f(PrimitiveData<uint32_t>{});
				case MemberType::SINT64: return f(PrimitiveData<int64_t>{});
				case MemberType::UINT64: return f(PrimitiveData<uint64_t>{});
				default: assert(!"reflective operation assertion failed: unknown primitive type"); return f(PrimitiveData<int32_t>{});
				}
			}
		};

		struct AlwaysBoolPrimitive {
			constexpr static TypeKind kind = TypeKind::PRIMITIVE;

			constexpr static auto visit(auto f) { return f(PrimitiveData<bool>{}); }
			constexpr static bool is_erased() { return false; }
		};

		struct AlwaysBoolType {
			template<typename AddrType> constexpr static size_t get_size(const auto& obj) { return GameInterface::RflSystem::TypeSet::metadata[MemberType::BOOL].size; }
			template<typename AddrType> constexpr static size_t get_alignment() { return GameInterface::RflSystem::TypeSet::metadata[MemberType::BOOL].alignment; }

			constexpr static auto visit(auto f) { return f(AlwaysBoolPrimitive{}); }
		};

		struct BitfieldComponent {
			const GameInterface::RflSystem::RflClassEnumMember* enumMember;

			const char* get_name() const { return enumMember->GetEnglishName(); }
			size_t get_offset() const { return (size_t)enumMember->GetIndex(); }
			constexpr static size_t get_width() { return 1; }
			constexpr static AlwaysBoolType get_type() { return {}; }
		};

		struct Bitfield {
			const GameInterface::RflSystem::RflClassMember* member;

			constexpr static TypeKind kind = TypeKind::BITFIELD;
			auto visit_underlying(auto f) const {
				switch (member->GetSubType()) {
				case MemberType::SINT8: return f(PrimitiveData<int8_t>{});
				case MemberType::UINT8: return f(PrimitiveData<uint8_t>{});
				case MemberType::SINT16: return f(PrimitiveData<int16_t>{});
				case MemberType::UINT16: return f(PrimitiveData<uint16_t>{});
				case MemberType::SINT32: return f(PrimitiveData<int32_t>{});
				case MemberType::UINT32: return f(PrimitiveData<uint32_t>{});
				case MemberType::SINT64: return f(PrimitiveData<int64_t>{});
				case MemberType::UINT64: return f(PrimitiveData<uint64_t>{});
				default: assert(!"reflective operation assertion failed: unknown primitive type"); return f(PrimitiveData<int32_t>{});
				}
			}
			template<simplerfl::strlit name>
			auto get_component() const {
				for (const auto& value : member->GetFlagValues().value())
					if (!strcmp(value.GetName(), name))
						return BitfieldComponent{ &value };

				assert(false && "unknown field name");

				return BitfieldComponent{ &*member->GetFlagValues().values().begin() };
			}

			void visit_components(auto f) const {
				for (const auto& value : member->GetFlagValues().value())
					f(BitfieldComponent{ &value });
			}
		};

		struct Field {
			const GameInterface::RflSystem::RflClassMember* member;
			
			const char* get_name() const { return member->GetName(); }
			size_t get_offset() const { return member->GetOffset(); }
			Type get_type() const { return { member, true }; }
			Type get_type(auto new_parent) const { return get_type(); }
		};

		struct FieldWithOffset : Field {
			size_t offset{};
			size_t get_offset() const { return offset; }
		};

		struct Structure {
			const GameInterface::RflSystem::RflClass* rflClass;

			constexpr static TypeKind kind = TypeKind::STRUCTURE;
			const char* get_name() const { return rflClass->GetName(); }
			template<typename AddrType, std::enable_if_t<!std::is_same_v<AddrType, size_t>, bool> = true> size_t get_size(const auto& obj) const {
				size_t offset{};

				if (auto base = get_base())
					offset += base.value().template get_size<AddrType>(obj);

				for (const auto& member : rflClass->GetMembers()) {
					offset = util::align(offset, Field{ &member }.get_type().template get_alignment<AddrType>());
					offset += FieldWithOffset{ &member, offset }.get_type().template get_size<AddrType>(obj);
				}

				return util::align(offset, get_alignment<AddrType>());
			}
			template<typename AddrType, std::enable_if_t<std::is_same_v<AddrType, size_t>, bool> = true> size_t get_size(const auto& obj) const { return rflClass->GetSize(); }
			template<typename AddrType, std::enable_if_t<!std::is_same_v<AddrType, size_t>, bool> = true> size_t get_alignment() const {
				size_t maxAlign{ 1 };

				if (auto base = get_base())
					maxAlign = std::max(maxAlign, base.value().template get_alignment<AddrType>());

				for (const auto& member : rflClass->GetMembers()) {
					maxAlign = std::max(maxAlign, Field{ &member }.get_type().template get_alignment<AddrType>());
				}

				return maxAlign;
			}
			template<typename AddrType, std::enable_if_t<std::is_same_v<AddrType, size_t>, bool> = true> size_t get_alignment() const { return rflClass->GetAlignment(); }

			std::optional<Structure> get_base() const {
				auto* parent = rflClass->GetParent();
				
				return parent != nullptr ? std::make_optional(Structure{ parent }) : std::nullopt;
			}

			template<size_t index, typename AddrType, std::enable_if_t<!std::is_same_v<AddrType, size_t>, bool> = true>
			auto get_field_by_index(const auto& obj) const {
				size_t offset{};
				size_t i{};

				if (auto base = get_base())
					offset += base.value().template get_size<AddrType>(obj);

				for (const auto& member : rflClass->GetMembers()) {
					offset = util::align(offset, Field{ &member }.get_type().template get_alignment<AddrType>());

					if (i++ == index)
						return FieldWithOffset{ &member, offset };

					offset += FieldWithOffset{ &member, offset }.get_type().template get_size<AddrType>(obj);
				}

				assert(false && "Unknown field.");

				return FieldWithOffset{ nullptr, offset };
			}
			template<size_t index, typename AddrType, std::enable_if_t<std::is_same_v<AddrType, size_t>, bool> = true>
			auto get_field_by_index(const auto& obj) const {
				return Field{ &rflClass->GetMembers()[index] };
			}

			template<simplerfl::strlit field_name, typename AddrType, std::enable_if_t<!std::is_same_v<AddrType, size_t>, bool> = true>
			auto get_field(const auto& obj) const {
				size_t offset{};

				if (auto base = get_base())
					offset += base.value().template get_size<AddrType>(obj);

				for (const auto& member : rflClass->GetMembers()) {
					offset = util::align(offset, Field{ &member }.get_type().template get_alignment<AddrType>());

					if (!strcmp(member.GetName(), field_name))
						return FieldWithOffset{ &member, offset };

					offset += FieldWithOffset{ &member, offset }.get_type().template get_size<AddrType>(obj);
				}

				assert(false && "Unknown field.");

				return FieldWithOffset{ nullptr, offset };
			}
			template<simplerfl::strlit field_name, typename AddrType, std::enable_if_t<std::is_same_v<AddrType, size_t>, bool> = true>
			auto get_field(const auto& obj) const {
				for (const auto& member : rflClass->GetMembers())
					if (!strcmp(member.GetName(), field_name))
						return Field{ &member };

				assert(false && "Unknown field.");

				return Field{ nullptr };
			}

			template<typename AddrType, typename F, std::enable_if_t<!std::is_same_v<AddrType, size_t>, bool> = true>
			void visit_fields(const auto& obj, F f) const {
				size_t offset{};

				if (auto base = get_base())
					offset += base.value().template get_size<AddrType>(obj);

				for (const auto& member : rflClass->GetMembers()) {
					offset = util::align(offset, Field{ &member }.get_type().template get_alignment<AddrType>());

					f(FieldWithOffset{ &member, offset });

					offset += FieldWithOffset{ &member, offset }.get_type().template get_size<AddrType>(obj);
				}
			}
			template<typename AddrType, typename F, std::enable_if_t<std::is_same_v<AddrType, size_t>, bool> = true>
			void visit_fields(const auto& obj, F f) const {
				for (const auto& member : rflClass->GetMembers())
					f(Field{ &member });
			}
		};

		struct SubType {
			const GameInterface::RflSystem::RflClassMember* member;

			template<typename AddrType, std::enable_if_t<!std::is_same_v<AddrType, size_t>, bool> = true> size_t get_size(const auto& obj) const {
				switch (member->GetSubType()) {
				case MemberType::STRUCT: return Structure{ member->GetClass() }.template get_size<AddrType>(obj);
				case MemberType::CSTRING: return sizeof(AddrType);
				case MemberType::STRING: return sizeof(AddrType) * 2;
				default: return member->GetSubTypeSize();
				}
			}
			template<typename AddrType, std::enable_if_t<std::is_same_v<AddrType, size_t>, bool> = true> size_t get_size(const auto& obj) const { return member->GetSubTypeSize(); }
			template<typename AddrType, std::enable_if_t<!std::is_same_v<AddrType, size_t>, bool> = true> size_t get_alignment() const {
				switch (member->GetSubType()) {
				case MemberType::STRUCT: return Structure{ member->GetClass() }.template get_alignment<AddrType>();
				case MemberType::CSTRING: return alignof(AddrType);
				case MemberType::STRING: return alignof(AddrType);
				default: return member->GetSubTypeAlignment();
				}
			}
			template<typename AddrType, std::enable_if_t<std::is_same_v<AddrType, size_t>, bool> = true> size_t get_alignment() const { return member->GetSubTypeAlignment(); }

			template<typename F>
			auto visit(F f) const {
				switch (member->GetSubType()) {
				case MemberType::STRUCT: return f(Structure{ member->GetClass() });
				default: return f(Primitive{ member, member->GetSubType() });
				}
			}
		};

		struct Type {
			const GameInterface::RflSystem::RflClassMember* member;
			bool allowCArray;

			template<typename AddrType, std::enable_if_t<!std::is_same_v<AddrType, size_t>, bool> = true> size_t get_size(const auto& obj) const {
				auto get_single_size = [&]() -> size_t {
					if constexpr (GameInterface::RflSystem::TypeSet::supports_old_array)
						if (member->GetType() == MemberType::OLD_ARRAY)
							return sizeof(AddrType) * 3;

					switch (member->GetType()) {
					case MemberType::STRUCT: return Structure{ member->GetClass() }.template get_size<AddrType>(obj);
					case MemberType::ARRAY: return sizeof(AddrType) * 4;
					case MemberType::POINTER: return sizeof(AddrType);
					case MemberType::CSTRING: return sizeof(AddrType);
					case MemberType::STRING: return sizeof(AddrType) * 2;
					default: return member->GetSingleSize();
					}
					};

				if (allowCArray) return member->GetArrayLength() ? member->GetArrayLength() * get_single_size() : get_single_size();
				else return get_single_size();
			}
			template<typename AddrType, std::enable_if_t<std::is_same_v<AddrType, size_t>, bool> = true> size_t get_size(const auto& obj) const {
				if (allowCArray) return member->GetSize();
				else return member->GetSingleSize();
			}
			template<typename AddrType, std::enable_if_t<!std::is_same_v<AddrType, size_t>, bool> = true> size_t get_alignment() const {
				if constexpr (GameInterface::RflSystem::TypeSet::supports_old_array)
					if (member->GetType() == MemberType::OLD_ARRAY)
						return alignof(AddrType);

				switch (member->GetType()) {
				case MemberType::STRUCT: return Structure{ member->GetClass() }.template get_alignment<AddrType>();
				case MemberType::ARRAY: return alignof(AddrType);
				case MemberType::POINTER: return alignof(AddrType);
				case MemberType::CSTRING: return alignof(AddrType);
				case MemberType::STRING: return alignof(AddrType);
				default: return member->GetAlignment();
				}
			}
			template<typename AddrType, std::enable_if_t<std::is_same_v<AddrType, size_t>, bool> = true> size_t get_alignment() const { return member->GetAlignment(); }

			template<typename F>
			auto visit(F f) const {
				if (allowCArray && member->GetArrayLength() > 0)
					return f(CArray{ member });

				if constexpr (GameInterface::RflSystem::TypeSet::supports_old_array)
					if (member->GetType() == MemberType::OLD_ARRAY)
						return f(TArray{ member });

				switch (member->GetType()) {
				case MemberType::ARRAY: return f(Array{ member });
				case MemberType::POINTER: return f(Pointer{ member });
				case MemberType::ENUM: return f(Enum{ member });
				case MemberType::FLAGS: return member->GetFlagValues().has_value() ? f(Bitfield{ member }) : f(Primitive{ member, member->GetSubType() });
				case MemberType::SIMPLE_ARRAY: assert(!"This RflClass member type (SIMPLE_ARRAY) is not implemented yet because it is unused."); return f(Primitive{ member, MemberType::VOID });
				case MemberType::STRUCT: return f(Structure{ member->GetClass() });
				default: return f(Primitive{ member, member->GetType() });
				}
			}
		};

		struct RootType {
			const GameInterface::RflSystem::RflClass* rflClass;

			template<typename AddrType> size_t get_size(const auto& obj) const { return Structure{ rflClass }.template get_size<AddrType>(obj); }
			template<typename AddrType> size_t get_alignment() const { return Structure{ rflClass }.template get_alignment<AddrType>(); }

			template<typename F>
			auto visit(F f) const {
				return f(Structure{ rflClass });
			}
		};

		static constexpr RootType reflect(const GameInterface::RflSystem::RflClass* rflClass) {
			return { rflClass };
		}
	};
}