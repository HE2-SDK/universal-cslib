#pragma once
#include <ucsl-reflection/opaque.h>
#include <ucsl-reflection/providers/types.h>
#include "types.h"

namespace ucsl::reflection::traversals {
	template<typename Algorithm, typename = std::make_index_sequence<Algorithm::arity>>
	class traversal;
	template<typename Algorithm, std::size_t... S>
	class traversal<Algorithm, std::index_sequence<S...>> {
		template<std::size_t, typename T> using Spread = T;

		//using Objs = Spread<S, opaque_obj&>;

		bool has_root{};
		std::tuple<Spread<S, opaque_obj*>...> roots{};

		Algorithm algorithm;
			
		template<typename Primitive>
		typename Algorithm::result_type process_primitive(Spread<S, opaque_obj&>... objs, bool erased, Primitive refl) {
			return refl.visit([&](auto repr) {
				using Repr = typename decltype(repr)::repr;

				return algorithm.visit_primitive(reinterpret_cast<Repr&>(objs)..., PrimitiveInfo<Repr>{ erased, repr.constant_value });
			});
		}

		template<typename Enum>
		typename Algorithm::result_type process_enum(Spread<S, opaque_obj&>... objs, bool erased, Enum refl) {
			return refl.visit([&](auto repr) {
				using Repr = typename decltype(repr)::repr;

				return algorithm.visit_enum(reinterpret_cast<Repr&>(objs)..., EnumInfo{ refl.get_options() });
			});
		}

		template<typename Flags>
		typename Algorithm::result_type process_flags(Spread<S, opaque_obj&>... objs, bool erased, Flags refl) {
			return refl.visit([&](auto repr) {
				using Repr = typename decltype(repr)::repr;

				return algorithm.visit_flags(reinterpret_cast<Repr&>(objs)..., FlagsInfo{ refl.get_values() });
			});
		}

		template<typename Array>
		typename Algorithm::result_type process_array(Spread<S, opaque_obj&>... objs, Spread<S, opaque_obj&>... parents, Array refl) {
			auto item = refl.get_item_type();
			auto accessors = std::tuple{ refl.get_accessor(objs)... };

			return algorithm.visit_array(
				std::get<S>(accessors)...,
				ArrayInfo{ .itemAlignment = item.get_alignment(parents, *std::get<S>(roots)), .itemSize = item.get_size(parents, *std::get<S>(roots), objs) }...,
				[]() { return nullptr; },
				[](void* obj) {},
				[&, item](Spread<S, opaque_obj&>... items) { return process_type(items..., parents..., item); }
			);
		}

		template<typename TArray>
		typename Algorithm::result_type process_tarray(Spread<S, opaque_obj&>... objs, Spread<S, opaque_obj&>... parents, TArray refl) {
			auto item = refl.get_item_type();
			auto accessors = std::tuple{ refl.get_accessor(objs)... };

			return algorithm.visit_tarray(
				std::get<S>(accessors)...,
				ArrayInfo{ .itemAlignment = item.get_alignment(parents, *std::get<S>(roots)), .itemSize = item.get_size(parents, *std::get<S>(roots), objs) }...,
				[]() { return nullptr; },
				[](void* obj) {},
				[&, item](Spread<S, opaque_obj&>... items) { return process_type(items..., parents..., item); }
			);
		}

		template<typename Pointer>
		typename Algorithm::result_type process_pointer(Spread<S, opaque_obj&>... objs, Spread<S, opaque_obj&>... parents, Pointer refl, bool isWeak) {
			auto target = refl.get_target_type();

			return algorithm.visit_pointer(
				(Spread<S, opaque_obj*&>)objs...,
				PointerInfo{
					.getTargetAlignment = [&parents, lroots = std::get<S>(roots), target]() { return target.get_alignment(parents, *lroots); },
					.getTargetSize = [&parents, lroots = std::get<S>(roots), &objs, target]() { return target.get_size(parents, *lroots, *(Spread<S, opaque_obj*&>)objs); },
					.isWeak = isWeak,
				}...,
				[&, target](Spread<S, opaque_obj&>...targets) { return process_type(targets..., parents..., target); }
			);
		}

		//template<typename Offset>
		//typename Algorithm::result_type process_offset(Spread<S, opaque_obj&>... objs, Spread<S, opaque_obj&>... parents, Offset refl) {
		//	auto target = refl.get_target_type();

		//	return refl.visit([&](auto repr) {
		//		using Repr = typename decltype(repr)::repr;

		//		algorithm.visit_offset(
		//			reinterpret_cast<Repr&>(objs)...,
		//			OffsetInfo{
		//				.getTargetAlignment = [&parents, lroots = std::get<S>(roots), target]() { return target.get_alignment(parents, *lroots); },
		//				.getTargetSize = [&parents, lroots = std::get<S>(roots), &objs, target]() { return target.get_size(parents, *lroots, *(util::addptr(&objs, reinterpret_cast<Repr&>(objs)))); },
		//			}...,
		//			[&, target](Spread<S, opaque_obj&>...targets) { return process_type(targets..., parents..., target); }
		//		);
		//	});
		//}

		template<typename CArray>
		typename Algorithm::result_type process_carray(Spread<S, opaque_obj&>... objs, Spread<S, opaque_obj&>... parents, CArray refl) {
			auto item = refl.get_item_type();

			return algorithm.visit_carray(&objs..., CArrayInfo{ refl.get_length(parents), item.get_size(parents, *std::get<S>(roots), objs) }..., [&, item](Spread<S, opaque_obj&>... items) { return process_type(items..., parents..., item); });
		}

		template<typename Union>
		typename Algorithm::result_type process_union(Spread<S, opaque_obj&>... objs, Spread<S, opaque_obj&>... parents, Union refl) {
			return algorithm.visit_union(objs..., UnionInfo{}, [&, refl](Spread<S, opaque_obj&>... objs) {
				typename Algorithm::result_type result{};

				refl.visit_current_field(*std::get<0>(std::tuple{ &parents... }), [&](auto chosen) { result = process_type(objs..., parents..., chosen.get_type()); });

				return result;
			});
		}

		template<typename Field>
		typename Algorithm::result_type process_field(Spread<S, opaque_obj&>... objs, Field refl) {
			return algorithm.visit_field(*util::addptr(&objs, refl.get_offset())..., FieldInfo{ .name = refl.get_name(), .erased = false }, [&, refl](Spread<S, opaque_obj&>... fields) { return process_type(fields..., objs..., refl.get_type()); });
		}

		template<typename Structure>
		typename Algorithm::result_type process_fields(Spread<S, opaque_obj&>... objs, Spread<S, opaque_obj&>... parents, Structure refl) {
			typename Algorithm::result_type result{};

			auto base = refl.get_base();

			if (base.has_value())
				result |= process_base_struct(objs..., parents..., base.value());

			refl.visit_fields(*std::get<0>(std::tuple{ &objs... }), *std::get<0>(roots), [&](auto field) { result |= process_field(objs..., field); });

			return result;
		}

		template<typename Structure>
		typename Algorithm::result_type process_base_struct(Spread<S, opaque_obj&>... objs, Spread<S, opaque_obj&>... parents, Structure refl) {
			return algorithm.visit_type(objs..., TypeInfo{ .alignment = refl.get_alignment(parents, *std::get<S>(roots)), .size = refl.get_size(parents, *std::get<S>(roots), objs) }..., [&, refl](Spread<S, opaque_obj&>... objs) {
				return algorithm.visit_base_struct(objs..., StructureInfo{ refl.get_name() }, [&, refl](Spread<S, opaque_obj&>... objs) { return process_fields(objs..., parents..., refl); });
			});
		}

		template<typename Structure>
		typename Algorithm::result_type process_struct(Spread<S, opaque_obj&>... objs, Spread<S, opaque_obj&>... parents, Structure refl) {
			return algorithm.visit_struct(objs..., StructureInfo{ refl.get_name(), refl.rflClass }, [&, refl](Spread<S, opaque_obj&>... objs) { return process_fields(objs..., parents..., refl); });
		}

		template<typename Type>
		typename Algorithm::result_type process_type(Spread<S, opaque_obj&>... objs, Spread<S, opaque_obj&>... parents, Type refl) {
			if (!has_root) {
				roots = { &objs... };
				has_root = true;
			}
			return algorithm.visit_type(objs..., TypeInfo{ .alignment = refl.get_alignment(parents, *std::get<S>(roots)), .size = refl.get_size(parents, *std::get<S>(roots), objs) }..., [&, refl](Spread<S, opaque_obj&>... objs) {
				return refl.visit(*std::get<0>(std::tuple{ &parents... }), *std::get<0>(roots), [&, refl](auto r) {
					if constexpr (decltype(r)::kind == providers::TypeKind::PRIMITIVE) return process_primitive(objs..., refl.is_erased(), r);
					else if constexpr (decltype(r)::kind == providers::TypeKind::ENUM) return process_enum(objs..., refl.is_erased(), r);
					else if constexpr (decltype(r)::kind == providers::TypeKind::FLAGS) return process_flags(objs..., refl.is_erased(), r);
					else if constexpr (decltype(r)::kind == providers::TypeKind::ARRAY) return process_array(objs..., parents..., r);
					else if constexpr (decltype(r)::kind == providers::TypeKind::TARRAY) return process_tarray(objs..., parents..., r);
					else if constexpr (decltype(r)::kind == providers::TypeKind::POINTER) return process_pointer(objs..., parents..., r, refl.is_weak());
					//else if constexpr (decltype(r)::kind == providers::TypeKind::OFFSET) return process_offset(objs..., parents..., r);
					else if constexpr (decltype(r)::kind == providers::TypeKind::CARRAY) return process_carray(objs..., parents..., r);
					else if constexpr (decltype(r)::kind == providers::TypeKind::UNION) return process_union(objs..., parents..., r);
					else if constexpr (decltype(r)::kind == providers::TypeKind::STRUCTURE) return process_struct(objs..., parents..., r);
					else static_assert(false, "invalid type kind");
				});
			});
		}

		template<typename Type>
		typename Algorithm::result_type process_root(Spread<S, opaque_obj&>... objs, Type refl) {
			return algorithm.visit_root(objs..., RootInfo{ .alignment = refl.get_alignment(objs, objs), .size = refl.get_size(objs, objs, objs) }..., [&, refl](Spread<S, opaque_obj&>...objs) { return process_type(objs..., objs..., refl); });
		}

	public:
		template<typename ...Args>
		traversal(Args&& ...objs) : algorithm{ std::forward<Args>(objs)... } {}

		//template<typename T>
		//typename Algorithm::result_type operator()(Spread<S, opaque_obj&>... objs) {
		//	return process_root<T>(objs...);
		//}

		template<typename T, typename Refl>
		typename Algorithm::result_type operator()(Spread<S, T&>... objs, Refl refl) {
			return process_root(((Spread<S, opaque_obj&>)objs)..., refl);
		}

		template<typename Refl>
		typename Algorithm::result_type operator()(Spread<S, void*>... objs, Refl refl) {
			return process_root((*(Spread<S, opaque_obj*>)objs)..., refl);
		}

		template<typename T, typename ...Args>
		static typename Algorithm::result_type apply(Spread<S, opaque_obj&>... objs, Args&& ...args) {
			traversal<Algorithm> t{ std::forward<Args>(args)... };

			return t.template operator()<T>(objs...);
		}
	};
}
