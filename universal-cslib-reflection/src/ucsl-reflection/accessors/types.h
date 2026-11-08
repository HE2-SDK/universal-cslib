#pragma once
#include <ucsl-reflection/providers/types.h>
//#include <ucsl-reflection/bound-reflection.h>
#include <ucsl-reflection/game-interfaces/standalone/game-interface.h>
#include <more_concepts/sequence_containers.hpp>

namespace ucsl::reflection::accessors {
	namespace mocks {
		struct Primitive {
			static constexpr providers::TypeKind kind = providers::TypeKind::PRIMITIVE;
		};

		struct Type {
			constexpr const auto visit(auto f) const { return f(Primitive{}); }
			constexpr auto visit(auto f) { return f(Primitive{}); }
		};
		
		struct Field {
			constexpr const char* get_name() const { return "foo"; }
			constexpr size_t get_offset() const { return 15; }
			constexpr auto get_type() const { return Type{}; }
			constexpr auto get_type(auto new_parent) const { return Type{}; }
		};
	}

	template<typename T, typename R>
	concept Accessor = requires (T t) {
		{ t.refl } -> std::convertible_to<R>;
	};

	template<typename T>
	concept ReadOnlyValueAccessor = requires (const T t) {
		{ t.visit([](auto v) -> size_t { return 0; }) } -> std::convertible_to<size_t>;
	};

	template<typename T>
	concept ValueAccessor = ReadOnlyValueAccessor<T> && requires (T t) {
		{ t.visit([](auto v) -> size_t { return 0; }) } -> std::convertible_to<size_t>;
	};

	template<typename T, typename U>
	concept ReadOnlyPrimitiveDataAccessor = std::convertible_to<T, U>;

	template<typename T, typename U>
	concept PrimitiveDataAccessor = ReadOnlyPrimitiveDataAccessor<T, U> && std::assignable_from<T, U>;

	template<typename T>
	concept ReadOnlyPrimitiveAccessor = requires (const T t) {
		//{ t.template as<int>() } -> ReadOnlyPrimitiveDataAccessor<int>;
		{ t.visit([](auto pdacc) -> size_t { return 0; }) } -> std::convertible_to<size_t>;
	};

	template<typename T>
	concept PrimitiveAccessor = ReadOnlyPrimitiveAccessor<T> && requires (T t) {
		//{ t.template as<int>() } -> PrimitiveDataAccessor<int>;
		{ t.visit([](auto pdacc) -> size_t { return 0; }) } -> std::convertible_to<size_t>;
	};

	template<typename T>
	concept ReadOnlyStructureAccessor = requires (const T t) {
		{ t[mocks::Field{}] } -> ReadOnlyValueAccessor;
	};

	template<typename T>
	concept StructureAccessor = ReadOnlyStructureAccessor<T> && requires (T t) {
		{ t[mocks::Field{}] } -> ValueAccessor;
	};

	template<typename T>
	concept ReadOnlyUnionAccessor = requires (const T t) {
		{ t[mocks::Field{}] } -> ReadOnlyValueAccessor;
		//{ t.visit([](auto curAcc) -> size_t { return 0; }) } -> std::convertible_to<size_t>;
	};

	template<typename T>
	concept UnionAccessor = ReadOnlyUnionAccessor<T> && requires (T t) {
		{ t[mocks::Field{}] } -> ValueAccessor;
		//{ t.visit([](auto curAcc) -> size_t { return 0; }) } -> std::convertible_to<size_t>;
	};

	template<typename T>
	concept ArrayAccessor = more_concepts::sequence_container<T>;
	
	template<typename T>
	concept ReadOnlyCArrayAccessor = requires(const T t) {
		{ t[0] } -> ReadOnlyValueAccessor;
	};

	template<typename T>
	concept CArrayAccessor = ReadOnlyCArrayAccessor<T> && requires(T t) {
		{ t[0] } -> ValueAccessor;
	};

	template<typename T>
	concept ReadOnlyPointerAccessor = requires (const T t) {
		{ *t } -> ReadOnlyValueAccessor;
	};

	template<typename T>
	concept PointerAccessor = ReadOnlyPointerAccessor<T> && requires (T t) {
		{ *t } -> ValueAccessor;
	};

	//&& requires (const T t) {
	//	GameInterface::AllocatorSystem::allocator_type* get_allocator() const { return obj.get_allocator(); }
	//	void change_allocator(GameInterface::AllocatorSystem::allocator_type* new_allocator) { obj.change_allocator(refl, new_allocator); }
	//}
}
