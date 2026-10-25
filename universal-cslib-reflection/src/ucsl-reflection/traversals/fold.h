#pragma once
#include <ucsl-reflection/opaque.h>
#include <ucsl-reflection/providers/types.h>
#include <ucsl-reflection/accessors/types.h>
#include "types.h"

namespace ucsl::reflection::traversals {
	//template<typename T, std::size_t arity>
	//concept Algorithm = requires (T t) {
	//	
	//}

	template<typename Algorithm, typename AddrType = size_t, typename = std::make_index_sequence<Algorithm::arity>>
	class traversal;
	template<typename Algorithm, typename AddrType, std::size_t... S>
	class traversal<Algorithm, AddrType, std::index_sequence<S...>> {
		template<std::size_t, typename T> using Spread = T;

		//using Objs = Spread<S, opaque_obj&>;

		bool has_root{};
		std::tuple<Spread<S, opaque_obj*>...> roots{};

		Algorithm algorithm;

		template<typename Refl, typename... Obj>
		typename Algorithm::result_type process_primitive(Refl refl, Obj... objs) {
			return algorithm.visit_primitive(objs...);
		}

		template<typename Refl, typename... Obj>
		typename Algorithm::result_type process_enum(Refl refl, Obj... objs) {
			return algorithm.visit_enum(objs...);
		}

		//template<typename Refl, typename... Obj>
		//typename Algorithm::result_type process_flags(Obj&... objs, bool erased, Refl refl) {
		//	return refl.visit([&](auto repr) {
		//		using Repr = typename decltype(repr)::repr;

		//		return algorithm.visit_flags(objs.as<Repr>(objs)..., FlagsInfo{ refl.get_values() });
		//	});
		//}

		template<typename Refl, typename... Obj>
		typename Algorithm::result_type process_array(Refl refl, Obj... objs) {
			auto item = refl.get_item_type();

			return algorithm.visit_array(objs..., [&, item](decltype(objs[0])&... items) { return process_type(item, items...); });
		}

		template<typename Refl, typename... Obj>
		typename Algorithm::result_type process_tarray(Refl refl, Obj... objs) {
			auto item = refl.get_item_type();

			return algorithm.visit_tarray(objs..., [&, item](decltype(objs[0])&... items) { return process_type(item, items...); });
		}

		template<typename Refl, typename... Obj>
		typename Algorithm::result_type process_pointer(Refl refl, Obj... objs) {
			auto target = refl.get_target_type();

			return algorithm.visit_pointer(objs..., [&, target](decltype(objs.get().value())&... targets) { return process_type(target, targets...); });
		}

		template<typename Refl, typename... Obj>
		typename Algorithm::result_type process_carray(Refl refl, Obj... objs) {
			auto item = refl.get_item_type();

			return algorithm.visit_carray(objs..., [&, item](decltype(objs[0])&... items) { return process_type(item, items...); });
		}

		template<typename Refl, typename... Obj>
		typename Algorithm::result_type process_union(Refl refl, Obj... objs) {
			typename Algorithm::result_type result{};

			refl.visit_current_field([&](auto r) {
				result = algorithm.visit_union(objs..., [&, r](decltype(objs)&... fields) { return process_type(r.get_type(std::get<0>(std::tuple{ objs... })), fields[r]...); });
			});

			return result;
		}

		template<typename Refl, typename... Obj>
		typename Algorithm::result_type process_field(Refl refl, Obj... objs) {
			return algorithm.visit_field(objs[refl]..., FieldInfo{ .name = refl.get_name(), .erased = false }, [&, refl](decltype(objs[refl])&... fields) { return process_type(refl.get_type(std::get<0>(std::tuple{ objs... })), fields...); });
		}

		template<typename Refl, typename... Obj>
		typename Algorithm::result_type process_fields(Refl refl, Obj... objs) {
			typename Algorithm::result_type result{};

			auto base = refl.get_base();

			if (base.has_value())
				result |= process_base_struct(base.value(), objs...);

			refl.template visit_fields<AddrType>(std::get<0>(std::tuple{ objs... }), [&](auto field) { result |= process_field(field, objs...); });

			return result;
		}

		template<typename Refl, typename... Obj>
		typename Algorithm::result_type process_base_struct(Refl refl, Obj... objs) {
			return algorithm.visit_type(objs.get_base().value()..., [&, refl](decltype(objs.get_base().value())&... objs) {
				return algorithm.visit_base_struct(objs..., [&, refl](decltype(objs)&... objs) { return process_fields(refl, objs...); });
			});
		}

		template<typename Refl, typename... Obj>
		typename Algorithm::result_type process_struct(Refl refl, Obj... objs) {
			return algorithm.visit_struct(objs..., [&, refl](decltype(objs)&... objs) { return process_fields(refl, objs...); });
		}

		template<typename Refl, typename... Obj>
		typename Algorithm::result_type process_type(Refl refl, Obj... objs) {
			return algorithm.visit_type(objs..., [&, refl](Obj... objs) {
				return refl.visit([&, refl](auto r) {
					if constexpr (decltype(r)::kind == providers::TypeKind::PRIMITIVE) return process_primitive(r, objs.as_primitive()...);
					else if constexpr (decltype(r)::kind == providers::TypeKind::ENUM) return process_enum(r, objs.as_enum()...);
					//else if constexpr (decltype(r)::kind == providers::TypeKind::FLAGS) return process_flags(r, objs.as_flags()...);
					else if constexpr (decltype(r)::kind == providers::TypeKind::ARRAY) return process_array(r, objs.as_array()...);
					else if constexpr (decltype(r)::kind == providers::TypeKind::TARRAY) return process_tarray(r, objs.as_tarray()...);
					else if constexpr (decltype(r)::kind == providers::TypeKind::POINTER) return process_pointer(r, objs.as_pointer()...);
					else if constexpr (decltype(r)::kind == providers::TypeKind::CARRAY) return process_carray(r, objs.as_carray()...);
					else if constexpr (decltype(r)::kind == providers::TypeKind::UNION) return process_union(r, objs.as_union()...);
					else if constexpr (decltype(r)::kind == providers::TypeKind::STRUCTURE) return process_struct(r, objs.as_structure()...);
					else static_assert(false, "invalid type kind");
				});
			});
		}

		template<typename Refl, typename... Obj>
		typename Algorithm::result_type process_root(Refl refl, Obj&... objs) {
			return algorithm.visit_root(objs..., [&, refl](Obj&...objs) { return process_type(refl, objs...); });
		}

	public:
		template<typename ...Args>
		traversal(Args&& ...objs) : algorithm{ std::forward<Args>(objs)... } {}

		//template<typename T>
		//typename Algorithm::result_type operator()(Spread<S, opaque_obj&>... objs) {
		//	return process_root<T>(objs...);
		//}

		template<typename Refl, typename... Obj>
		typename Algorithm::result_type call(Refl refl, Obj&... objs) {
			return process_root(refl, objs...);
		}

		//template<typename Refl>
		//typename Algorithm::result_type operator()(Spread<S, void*>... objs, Refl refl) {
		//	return process_root((*(Spread<S, opaque_obj*>)objs)..., refl);
		//}

		//template<typename T, typename ...Args>
		//static typename Algorithm::result_type apply(Spread<S, opaque_obj&>... objs, Args&& ...args) {
		//	traversal<Algorithm> t{ std::forward<Args>(args)... };

		//	return t.operator()<T>(objs...);
		//}
	};
}
