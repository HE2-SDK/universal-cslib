#pragma once

namespace ucsl::reflection::accessors {
	template<typename Backend>
	struct basic_backend {
		template<typename Refl>
		class AccessorBase {
		public:
			typename Backend::Value value;
			Refl refl;

			inline AccessorBase(Backend::Value value, const Refl& refl) : value{ value }, refl{ refl } {}
		};

		template<typename Refl>
		class ValueAccessor;

		template<typename Refl>
		class PrimitiveDataAccessor : public AccessorBase<Refl> {
		public:
			using AccessorBase<Refl>::AccessorBase;
			
			PrimitiveDataAccessor<Refl>& operator=(typename Refl::repr v) {
				this->value = this->refl.is_erased ? typename Refl::repr{} : this->refl.constant_value.has_value() ? this->refl.constant_value.value() : v;
				return *this;
			}

			operator typename Refl::repr() const {
				return this->refl.is_erased ? typename Refl::repr{} : this->refl.constant_value.has_value() ? this->refl.constant_value.value() : this->value;
			}
		};

		template<typename Refl>
		class PrimitiveAccessor : public AccessorBase<Refl> {
		public:
			using AccessorBase<Refl>::AccessorBase;

			template<typename F>
			inline auto visit(F f) {
				return this->refl.visit([&](auto r){ return f(PrimitiveDataAccessor<decltype(r)>{ this->value, r }); });
			}

			template<typename F>
			inline const auto visit(F f) const {
				return this->refl.visit([&](auto r){ return f(PrimitiveDataAccessor<decltype(r)>{ this->value, r }); });
			}
		};

		template<typename Refl>
		class EnumAccessor : public PrimitiveAccessor<Refl> {
		public:
			using PrimitiveAccessor<Refl>::PrimitiveAccessor;
		};

		template<typename Refl>
		class StructureAccessor : public AccessorBase<Refl> {
		public:
			using AccessorBase<Refl>::AccessorBase;

			template<typename FieldRefl>
			inline auto operator[](const FieldRefl& field_refl) {
				auto type = field_refl.get_type(*this);

				return ValueAccessor<decltype(r)>{ util::addptr(this->value, field_refl.get_offset()), type };
			}

			template<typename FieldRefl>
			inline const auto operator[](const FieldRefl& field_refl) const {
				auto type = field_refl.get_type(*this);

				return ValueAccessor<decltype(r)>{ util::addptr(this->value, field_refl.get_offset()), type };
			}

			inline auto get_base() const {
				auto base = this->refl.get_base();

				return base.has_value() ? std::make_optional(StructureAccessor<std::remove_reference_t<decltype(base.value())>>{ this->value, base.value() }) : std::nullopt;
			}
		};

		template<typename Refl>
		class CArrayAccessor : public AccessorBase<Refl> {
		public:
			using AccessorBase<Refl>::AccessorBase;

			inline size_t get_length() const {
				return this->refl.get_length();
			}

			inline auto operator[](size_t idx) {
				auto item_refl = this->refl.get_item_type();

				assert(idx < this->refl.get_length());

				return ValueAccessor<decltype(item_refl)>{ util::addptr(this->value, idx * item_refl.get_size(*this)), item_refl };
			}

			inline const auto operator[](size_t idx) const {
				auto item_refl = this->refl.get_item_type();

				assert(idx < this->refl.get_length());

				return ValueAccessor<decltype(item_refl)>{ util::addptr(this->value, idx * item_refl.get_size(*this)), item_refl };
			}
		};

		template<typename Refl>
		class PointerAccessor : public AccessorBase<Refl> {
		public:
			using AccessorBase<Refl>::AccessorBase;

			inline void clear() {
				this->obj = nullptr;
			}

			inline void set(const Accessor<Refl>& other) {
				this->obj = &other.obj;
			}

			inline auto get() {
				auto target_type = this->refl.get_target_type();

				return this->obj == nullptr ? std::nullopt : std::make_optional<ValueAccessor<decltype(target_type)>>({ this->obj, target_type });
			}

			inline auto get() const {
				auto target_type = this->refl.get_target_type();

				return this->obj == nullptr ? std::nullopt : std::make_optional<const ValueAccessor<decltype(target_type)>>({ this->obj, target_type });
			}
		};

		template<template<typename, typename> typename A, typename typename Refl>
		class ArrayAccessor : public AccessorBase<Refl> {
		public:
			class iterator {
				ArrayAccessor& accessor;
				size_t idx{};

			public:
				iterator(ArrayAccessor& accessor, size_t idx) : accessor{ accessor }, idx{ idx } {}
				iterator(const iterator& other) : accessor{ other.accessor }, idx{ other.idx } {}

				iterator& operator++() {
					idx++;
					return *this;
				}

				iterator operator++(int) {
					iterator result{ *this };
					idx++;
					return result;
				}

				iterator& operator--() {
					idx--;
					return *this;
				}

				iterator operator--(int) {
					iterator result{ *this };
					idx--;
					return result;
				}

				bool operator==(const iterator& other) const { return idx == other.idx; }
				bool operator!=(const iterator& other) const { return idx != other.idx; }
				bool operator<(const iterator& other) const { return idx < other.idx; }
				bool operator>(const iterator& other) const { return idx > other.idx; }
				bool operator<=(const iterator& other) const { return idx <= other.idx; }
				bool operator>=(const iterator& other) const { return idx >= other.idx; }
				auto operator*() { return accessor[idx]; }
			};

			class const_iterator {
				ArrayAccessor& accessor;
				size_t idx{};

			public:
				const_iterator(ArrayAccessor& accessor, size_t idx) : accessor{ accessor }, idx{ idx } {}
				const_iterator(const const_iterator& other) : accessor{ other.accessor }, idx{ other.idx } {}

				const_iterator& operator++() {
					idx++;
					return *this;
				}

				const_iterator operator++(int) {
					const_iterator result{ *this };
					idx++;
					return result;
				}

				const_iterator& operator--() {
					idx--;
					return *this;
				}

				const_iterator operator--(int) {
					const_iterator result{ *this };
					idx--;
					return result;
				}

				bool operator==(const const_iterator& other) const { return idx == other.idx; }
				bool operator!=(const const_iterator& other) const { return idx != other.idx; }
				bool operator<(const const_iterator& other) const { return idx < other.idx; }
				bool operator>(const const_iterator& other) const { return idx > other.idx; }
				bool operator<=(const const_iterator& other) const { return idx <= other.idx; }
				bool operator>=(const const_iterator& other) const { return idx >= other.idx; }
				const auto operator*() const { return accessor[idx]; }
			};

			//template<typename T> OpaqueReflArray(A<T, typename GameInterface::AllocatorSystem>& underlying, Refl refl) : underlying{ static_cast<RflArray<A>&>(underlying) }, refl{ refl } {}
			//template<typename T> OpaqueReflArray(const OpaqueReflArray<A, GameInterface, Refl>& other, Refl refl) : underlying{ other.underlying }, refl{ other.refl } {}
			
			using AccessorBase<Refl>::AccessorBase;

			GameInterface::AllocatorSystem::allocator_type* get_allocator() const { return this->obj.get_allocator(); }
			void change_allocator(GameInterface::AllocatorSystem::allocator_type* new_allocator) { this->obj.change_allocator(this->refl, new_allocator); }
			void reserve(size_t len) { this->obj.reserve(this->refl, len); }

			auto emplace_back() {
				auto item_type = this->refl->get_item_type();

				return ValueAccessor<decltype(item_type)>{ this->obj.emplace_back(this->refl), item_type };
			}
			void remove(size_t i) { this->obj.remove(this->refl, i); }
			void clear() { this->obj.clear(this->refl); }

			iterator begin() { return { *this, 0 }; }
			const_iterator begin() const { return { *this, 0 }; }
			const_iterator cbegin() const { return { *this, 0 }; }
			iterator end() { return { *this, size() }; }
			const_iterator end() const { return { *this, size() }; }
			const_iterator cend() const { return { *this, size() }; }

			auto operator[](size_t i) {
				auto item_type = this->refl->get_item_type();

				return ValueAccessor<decltype(item_type)>{ this->obj.at(this->refl, i), item_type };
			}
			const auto operator[](size_t i) const {
				auto item_type = this->refl->get_item_type();

				return ValueAccessor<decltype(item_type)>{ this->obj.at(this->refl, i), item_type };
			}

			size_t size() const { return this->obj.size(); }
			size_t capacity() const { return this->obj.capacity(); }
		};

		template<typename Refl>
		class ValueAccessor : public AccessorBase<Refl> {
		public:
			using AccessorBase<Refl>::AccessorBase;

			const auto visit(auto f) const {
				return this->refl.visit(this->parent, this->root, [&](auto r) {
					if constexpr (decltype(r)::kind == providers::TypeKind::PRIMITIVE) return f(PrimitiveAccessor<decltype(r)>{ this->value, r });
					else if constexpr (decltype(r)::kind == providers::TypeKind::ENUM) return f(EnumAccessor<decltype(r)>{ this->value, r });
					//else if constexpr (decltype(r)::kind == providers::TypeKind::FLAGS) return f(flags(this->obj, refl.is_erased(), r));
					else if constexpr (decltype(r)::kind == providers::TypeKind::ARRAY) return f(ArrayAccessor<ucsl::containers::arrays::Array, decltype(r)>{ this->value, r });
					else if constexpr (decltype(r)::kind == providers::TypeKind::TARRAY) return f(ArrayAccessor<ucsl::containers::arrays::TArray, decltype(r)>{ this->value, r });
					else if constexpr (decltype(r)::kind == providers::TypeKind::POINTER) return f(PointerAccessor<decltype(r)>{ this->value, r });
					else if constexpr (decltype(r)::kind == providers::TypeKind::CARRAY) return f(CArrayAccessor<decltype(r)>{ this->value, r });
					//else if constexpr (decltype(r)::kind == providers::TypeKind::UNION) return f(union(this->obj, parent, r));
					else if constexpr (decltype(r)::kind == providers::TypeKind::STRUCTURE) return f(StructureAccessor<decltype(r)>{ this->value, r });
					else static_assert(false, "invalid type kind");
				});
			}

			auto visit(auto f) {
				return this->refl.visit(this->parent, this->root, [&](auto r) {
					if constexpr (decltype(r)::kind == providers::TypeKind::PRIMITIVE) return f(PrimitiveAccessor<decltype(r)>{ this->value, r });
					else if constexpr (decltype(r)::kind == providers::TypeKind::ENUM) return f(EnumAccessor<decltype(r)>{ this->value, r });
					//else if constexpr (decltype(r)::kind == providers::TypeKind::FLAGS) return f(flags(this->obj, refl.is_erased(), r));
					else if constexpr (decltype(r)::kind == providers::TypeKind::ARRAY) return f(ArrayAccessor<ucsl::containers::arrays::Array, decltype(r)>{ this->value, r });
					else if constexpr (decltype(r)::kind == providers::TypeKind::TARRAY) return f(ArrayAccessor<ucsl::containers::arrays::TArray, decltype(r)>{ this->value, r });
					else if constexpr (decltype(r)::kind == providers::TypeKind::POINTER) return f(PointerAccessor<decltype(r)>{ this->value, r });
					else if constexpr (decltype(r)::kind == providers::TypeKind::CARRAY) return f(CArrayAccessor<decltype(r)>{ this->value, r });
					//else if constexpr (decltype(r)::kind == providers::TypeKind::UNION) return f(union(this->obj, parent, r));
					else if constexpr (decltype(r)::kind == providers::TypeKind::STRUCTURE) return f(StructureAccessor<decltype(r)>{ this->value, r });
					else static_assert(false, "invalid type kind");
				});
			}

			auto as_primitive() {
				return this->refl.visit(this->parent, this->root, [&](auto r) {
					if constexpr (decltype(r)::kind == providers::TypeKind::PRIMITIVE) { return PrimitiveAccessor<decltype(r)>{ this->value, r }; } else static_assert(false, "not a primitive");
				});
			}

			auto as_enum() {
				return this->refl.visit(this->parent, this->root, [&](auto r) {
					if constexpr (decltype(r)::kind == providers::TypeKind::ENUM) { return EnumAccessor<decltype(r)>{ this->value, r }; } else static_assert(false, "not an enum");
				});
			}

			auto as_array() {
				return this->refl.visit(this->parent, this->root, [&](auto r) {
					if constexpr (decltype(r)::kind == providers::TypeKind::ARRAY) { return PrimitiveAccessor<decltype(r)>{ this->value, r }; } else static_assert(false, "not a array");
				});
			}

			auto as_tarray() {
				return this->refl.visit(this->parent, this->root, [&](auto r) {
					if constexpr (decltype(r)::kind == providers::TypeKind::TARRAY) { return PrimitiveAccessor<decltype(r)>{ this->value, r }; } else static_assert(false, "not a tarray");
				});
			}

			auto as_carray() {
				return this->refl.visit(this->parent, this->root, [&](auto r) {
					if constexpr (decltype(r)::kind == providers::TypeKind::CARRAY) { return CArrayAccessor<decltype(r)>{ this->value, r }; } else static_assert(false, "not a carray");
				});
			}

			auto as_pointer() {
				return this->refl.visit(this->parent, this->root, [&](auto r) {
					if constexpr (decltype(r)::kind == providers::TypeKind::POINTER) { return PointerAccessor<decltype(r)>{ this->value, r }; } else static_assert(false, "not a pointer");
				});
			}

			auto as_structure() {
				return this->refl.visit(this->parent, this->root, [&](auto r) {
					if constexpr (decltype(r)::kind == providers::TypeKind::STRUCTURE) { return StructureAccessor<decltype(r)>{ this->value, r }; } else static_assert(false, "not a structure");
				});
			}
		};
	};
}
