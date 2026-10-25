#pragma once
#include <simple-reflection/simple-reflection.h>
#include <ucsl/containers/arrays/array.h>
#include <ucsl/containers/arrays/tarray.h>
#include <ucsl-reflection/providers/types.h>
#include <ucsl-reflection/util/memory.h>

namespace ucsl::reflection::accessors {
	template<typename GameInterface>
	struct binary {
		struct opaque_value {};

		//template<typename Refl>
		//void construct(Refl refl, void* ptr, size_t count = 1);
		//{
		//	for (size_t i = 0; i < count; i++) {
		//		auto item_size = refl.get_size()
		//		auto* item_ptr = 

		//		return refl.visit([&](auto r) {
		//			if constexpr (decltype(r)::kind == providers::TypeKind::PRIMITIVE) return f(PrimitiveAccessor<decltype(r)>{ this->value, r });
		//			else if constexpr (decltype(r)::kind == providers::TypeKind::ENUM) return f(EnumAccessor<decltype(r)>{ this->value, r });
		//			//else if constexpr (decltype(r)::kind == providers::TypeKind::FLAGS) return f(flags(this->value, refl.is_erased(), r));
		//			else if constexpr (decltype(r)::kind == providers::TypeKind::ARRAY) return f(ArrayAccessor<ucsl::containers::arrays::Array, decltype(r)>{ this->value, r });
		//			else if constexpr (decltype(r)::kind == providers::TypeKind::TARRAY) return f(ArrayAccessor<ucsl::containers::arrays::TArray, decltype(r)>{ this->value, r });
		//			else if constexpr (decltype(r)::kind == providers::TypeKind::POINTER) return f(PointerAccessor<decltype(r)>{ this->value, r });
		//			else if constexpr (decltype(r)::kind == providers::TypeKind::CARRAY) return f(CArrayAccessor<decltype(r)>{ this->value, r });
		//			//else if constexpr (decltype(r)::kind == providers::TypeKind::UNION) return f(union(this->value, parent, r));
		//			else if constexpr (decltype(r)::kind == providers::TypeKind::STRUCTURE) {
		//				r.visit_fields([&](auto f) {
		//					construct(f.get_type(), util::addptr(ptr, field_refl.get_offset()));
		//				});
		//			}
		//			else static_assert(false, "invalid type kind");
		//		});
		//	}
		//}

		template<typename Refl>
		void destroy(Refl refl, void* ptr, size_t count = 1);

		template<typename Refl>
		void copy(Refl refl, void* to, void* from, size_t count = 1);

		template<typename Refl>
		void move(Refl refl, void* to, void* from, size_t count = 1);

		template<template<typename, typename> typename A, typename Refl>
		class RflArray : public A<opaque_value, typename GameInterface::AllocatorSystem> {
		public:
			void change_allocator(auto accessor, GameInterface::AllocatorSystem::allocator_type* new_allocator) {
				if (!new_allocator || this->allocator == new_allocator)
					return;

				if (this->is_deallocation_disabled() || !this->buffer) {
					this->allocator = new_allocator;
					return;
				}

				opaque_value* new_buffer = static_cast<opaque_value*>(new_allocator->Alloc(this->capacity() * accessor.refl.get_item_type().get_size(accessor), 16));

				move(accessor.refl, new_buffer, this->buffer, this->length);

				if (this->allocator && !this->is_deallocation_disabled())
					this->allocator->Free(this->buffer);

				this->buffer = new_buffer;
				this->allocator = new_allocator;
			}

			void reserve(auto accessor, size_t desired_cap) {
				auto* allocator = this->get_allocator();
				auto cap = this->capacity();

				if (desired_cap <= cap)
					return;

				auto newcap = std::max(2 * cap, desired_cap);

				opaque_value* new_buffer = static_cast<opaque_value*>(allocator->Alloc(newcap * accessor.refl.get_item_type().get_size(accessor), 16));

				if (this->buffer) {
					move(accessor.refl, new_buffer, this->buffer, this->length);

					if (!this->is_deallocation_disabled())
						allocator->Free(this->buffer);
				}

				this->buffer = new_buffer;
				this->capacity_and_flags = newcap;
			}

			opaque_value& emplace_back(auto accessor) {
				this->reserve(accessor.refl, this->length + 1);

				opaque_value& loc = at(accessor, this->length);

				construct(accessor.refl, &loc);
				//if (member->GetSubType() == GameInterface::RflSystem::RflClassMember::Type::STRUCT)
				//	GameInterface::RflTypeInfoRegistry::GetInstance()->ConstructObject(this->get_allocator(), &at(this->length, member), member->GetClass()->GetName());
				//else
				//	memset(&at(this->length, member), 0, member->GetSubTypeSize());

				this->length++;

				return loc;
			}

			void remove(auto accessor, size_t i) {
				if (i >= this->length)
					return;

				//if (member->GetSubType() == GameInterface::RflSystem::RflClassMember::Type::STRUCT)
				//	GameInterface::RflTypeInfoRegistry::GetInstance()->CleanupLoadedObject(&at(i, member), member->GetName());

				destroy(accessor.refl, &at(accessor, i));
				move(accessor.refl, &at(accessor, i), &at(accessor, i + 1), this->length - i - 1);

				this->length--;
			}

			void clear(auto accessor) {
				if (this->empty())
					return;

				for (size_t i = 0; i < this->length; i++)
					destroy(accessor.refl, &at(accessor, i));
					//if (member->GetSubType() == GameInterface::RflSystem::RflClassMember::Type::STRUCT)
					//	GameInterface::RflTypeInfoRegistry::GetInstance()->CleanupLoadedObject(&at(i, member), member->GetName());

				this->length = 0;
			}

			opaque_value& at(auto accessor, size_t i) {
				return *util::addptr(this->buffer, accessor.refl.get_item_type().get_size(accessor) * i);
			}
		};

		template<typename Refl>
		class AddressConstAccessor;

		template<typename Refl>
		class AddressAccessor;

		template<typename Refl, typename T = opaque_value>
		class ConstAccessorBase {
		public:
			const T& value;
			const Refl refl;

			template<typename U>
			constexpr ConstAccessorBase(const U& value, const Refl& refl) : value{ (const T&)value }, refl{ refl } {}

			constexpr AddressConstAccessor<Refl> operator&() const {
				return { value, refl };
			}
		};

		template<typename Refl, typename T = opaque_value>
		class AccessorBase {
		public:
			T& value;
			const Refl refl;

			template<typename U>
			constexpr AccessorBase(U& value, const Refl& refl) : value{ (T&)value }, refl{ refl } {}

			constexpr AddressAccessor<Refl> operator&() const {
				return { value, refl };
			}
		};

		template<typename Refl>
		class ValueConstAccessor;

		template<typename Refl>
		class ValueAccessor;

		template<typename Refl>
		class PrimitiveDataConstAccessor : public ConstAccessorBase<Refl, typename Refl::repr> {
		public:
			using ConstAccessorBase<Refl, typename Refl::repr>::ConstAccessorBase;

			operator typename Refl::repr() const {
				if constexpr (std::is_fundamental_v<typename Refl::repr>)
					return this->refl.is_erased ? typename Refl::repr{} : this->refl.constant_value.has_value() ? this->refl.constant_value.value() : this->value;
				else
					return this->refl.constant_value.has_value() ? this->refl.constant_value.value() : this->value;
			}
		};

		template<typename Refl>
		class PrimitiveDataAccessor : public AccessorBase<Refl, typename Refl::repr> {
		public:
			using AccessorBase<Refl, typename Refl::repr>::AccessorBase;
			
			PrimitiveDataAccessor<Refl>& operator=(typename Refl::repr v) {
				if constexpr (std::is_fundamental_v<typename Refl::repr>)
					this->value = this->refl.is_erased ? typename Refl::repr{} : this->refl.constant_value.has_value() ? this->refl.constant_value.value() : v;
				else
					this->value = this->refl.constant_value.has_value() ? this->refl.constant_value.value() : v;
				return *this;
			}

			operator typename Refl::repr() const {
				if constexpr (std::is_fundamental_v<typename Refl::repr>)
					return this->refl.is_erased ? typename Refl::repr{} : this->refl.constant_value.has_value() ? this->refl.constant_value.value() : this->value;
				else
					return this->refl.constant_value.has_value() ? this->refl.constant_value.value() : this->value;
			}
		};

		template<typename Refl>
		class PrimitiveConstAccessor : public ConstAccessorBase<Refl> {
		public:
			using ConstAccessorBase<Refl>::ConstAccessorBase;

			constexpr auto visit(auto f) const {
				return this->refl.visit([&](const auto& r) { return f(PrimitiveDataConstAccessor<std::decay_t<decltype(r)>>{ this->value, r }); });
			}
		};

		template<typename Refl>
		class PrimitiveAccessor : public AccessorBase<Refl> {
		public:
			using AccessorBase<Refl>::AccessorBase;

			constexpr auto visit(auto f) {
				return this->refl.visit([&](const auto& r){ return f(PrimitiveDataAccessor<std::decay_t<decltype(r)>>{ this->value, r }); });
			}

			constexpr auto visit(auto f) const {
				return this->refl.visit([&](const auto& r){ return f(PrimitiveDataConstAccessor<std::decay_t<decltype(r)>>{ this->value, r }); });
			}
		};

		template<typename Refl>
		class EnumConstAccessor : public ConstAccessorBase<Refl> {
		public:
			using ConstAccessorBase<Refl>::ConstAccessorBase;

			operator long long() const {
				return this->refl.visit([&](const auto& r) {
					PrimitiveDataConstAccessor<std::decay_t<decltype(r)>> pd{ this->value, r };

					return static_cast<long long>(pd);
				});
			}
		};

		template<typename Refl>
		class EnumAccessor : public AccessorBase<Refl> {
		public:
			using AccessorBase<Refl>::AccessorBase;
			
			EnumAccessor<Refl>& operator=(long long v) {
				this->refl.visit([&](const auto& r) {
					PrimitiveDataAccessor<std::decay_t<decltype(r)>> pd{ this->value, r };

					pd = static_cast<typename std::decay_t<decltype(r)>::repr>(v);
				});
				return *this;
			}

			operator long long() const {
				return this->refl.visit([&](const auto& r){
					PrimitiveDataConstAccessor<std::decay_t<decltype(r)>> pd{ this->value, r };

					return static_cast<long long>(pd);
				});
			}
		};


		template<typename Refl>
		class BitfieldConstAccessor : public ConstAccessorBase<Refl> {
		public:
			using ConstAccessorBase<Refl>::ConstAccessorBase;

			template<typename F>
			constexpr const auto visit(F f) const {
				return this->refl.visit_underlying([&](auto r) { return f(PrimitiveDataConstAccessor<decltype(r)>{ this->value, r }); });
			}

			template<typename T>
			constexpr const auto as() const {
				return this->refl.visit_underlying([&](auto r) { if constexpr (std::is_same_v<typename decltype(r)::repr, T>) return PrimitiveDataConstAccessor<decltype(r)>{ this->value, r }; else static_assert(false, "not the correct primitive type"); });
			}
		};


		template<typename Refl>
		class BitfieldAccessor : public AccessorBase<Refl> {
		public:
			using AccessorBase<Refl>::AccessorBase;

			template<typename F>
			constexpr const auto visit(F f) const {
				return this->refl.visit_underlying([&](auto r) { return f(PrimitiveDataConstAccessor<decltype(r)>{ this->value, r }); });
			}

			template<typename F>
			constexpr const auto visit(F f) {
				return this->refl.visit_underlying([&](auto r) { return f(PrimitiveDataAccessor<decltype(r)>{ this->value, r }); });
			}

			template<typename T>
			constexpr const auto as() const {
				return this->refl.visit_underlying([&](auto r) { if constexpr (std::is_same_v<typename decltype(r)::repr, T>) return PrimitiveDataConstAccessor<decltype(r)>{ this->value, r }; else static_assert(false, "not the correct primitive type"); });
			}

			template<typename T>
			constexpr const auto as() {
				return this->refl.visit_underlying([&](auto r) { if constexpr (std::is_same_v<typename decltype(r)::repr, T>) return PrimitiveDataAccessor<decltype(r)>{ this->value, r }; else static_assert(false, "not the correct primitive type"); });
			}
		};

		template<typename Refl>
		class StructureConstAccessor : public ConstAccessorBase<Refl> {
		public:
			using ConstAccessorBase<Refl>::ConstAccessorBase;

			constexpr auto operator[](const auto& field_refl) const {
				const auto type = field_refl.get_type(*this);

				return ValueConstAccessor<decltype(type)>{ *util::addptr(&this->value, field_refl.get_offset()), type };
			}

			constexpr auto get_base() const {
				const auto base = this->refl.get_base();

				return base.has_value() ? std::make_optional(StructureConstAccessor<std::decay_t<decltype(base.value())>>{ this->value, base.value() }) : std::nullopt;
			}
		};

		template<typename Refl>
		class StructureAccessor : public AccessorBase<Refl> {
		public:
			using AccessorBase<Refl>::AccessorBase;

			constexpr auto operator[](const auto& field_refl) const {
				const auto type = field_refl.get_type(*this);

				return ValueConstAccessor<decltype(type)>{ *util::addptr(&this->value, field_refl.get_offset()), type };
			}

			constexpr auto operator[](const auto& field_refl) {
				const auto type = field_refl.get_type(*this);

				return ValueAccessor<decltype(type)>{ *util::addptr(&this->value, field_refl.get_offset()), type };
			}

			template<simplerfl::strlit FieldName>
			constexpr auto get_field() const {
				return (*this)[this->refl.template get_field<FieldName, size_t>(*this)];
			}

			template<simplerfl::strlit FieldName>
			constexpr auto get_field() {
				return (*this)[this->refl.template get_field<FieldName, size_t>(*this)];
			}

			constexpr void visit_fields(auto f) const {
				return this->refl.template visit_fields<size_t>(*this, [&](const auto& field) { f((*this)[field], field); });
			}

			constexpr void visit_fields(auto f) {
				return this->refl.template visit_fields<size_t>(*this, [&](const auto& field) { f((*this)[field], field); });
			}

			constexpr auto get_base() const {
				const auto base = this->refl.get_base();

				return base.has_value() ? std::make_optional(StructureConstAccessor<std::decay_t<decltype(base.value())>>{ this->value, base.value() }) : std::nullopt;
			}
			constexpr auto get_base() {
				const auto base = this->refl.get_base();

				return base.has_value() ? std::make_optional(StructureAccessor<std::decay_t<decltype(base.value())>>{ this->value, base.value() }) : std::nullopt;
			}
		};

		template<typename Refl>
		class CArrayConstAccessor : public ConstAccessorBase<Refl> {
		public:
			using ConstAccessorBase<Refl>::ConstAccessorBase;

			class const_iterator {
				const CArrayConstAccessor& accessor;
				size_t idx{};

			public:
				inline const_iterator(const CArrayConstAccessor& accessor, size_t idx) : accessor{ accessor }, idx{ idx } {}
				inline const_iterator(const const_iterator& other) : accessor{ other.accessor }, idx{ other.idx } {}

				inline const_iterator& operator++() {
					idx++;
					return *this;
				}

				inline const_iterator operator++(int) {
					const_iterator result{ *this };
					idx++;
					return result;
				}

				inline const_iterator& operator--() {
					idx--;
					return *this;
				}

				inline const_iterator operator--(int) {
					const_iterator result{ *this };
					idx--;
					return result;
				}

				inline bool operator==(const const_iterator& other) const { return idx == other.idx; }
				inline bool operator!=(const const_iterator& other) const { return idx != other.idx; }
				inline bool operator<(const const_iterator& other) const { return idx < other.idx; }
				inline bool operator>(const const_iterator& other) const { return idx > other.idx; }
				inline bool operator<=(const const_iterator& other) const { return idx <= other.idx; }
				inline bool operator>=(const const_iterator& other) const { return idx >= other.idx; }
				inline const auto operator*() const { return accessor[idx]; }
			};

			constexpr size_t size() const {
				return this->refl.get_length();
			}

			constexpr const auto operator[](size_t idx) const {
				auto item_refl = this->refl.get_item_type();

				return ValueConstAccessor<decltype(item_refl)>{ *util::addptr(&this->value, idx* item_refl.get_size(*this)), item_refl };
			}
		};

		template<typename Refl>
		class CArrayAccessor : public AccessorBase<Refl> {
		public:
			using AccessorBase<Refl>::AccessorBase;

			class const_iterator {
				const CArrayAccessor& accessor;
				size_t idx{};

			public:
				inline const_iterator(const CArrayAccessor& accessor, size_t idx) : accessor{ accessor }, idx{ idx } {}
				inline const_iterator(const const_iterator& other) : accessor{ other.accessor }, idx{ other.idx } {}

				inline const_iterator& operator++() {
					idx++;
					return *this;
				}

				inline const_iterator operator++(int) {
					const_iterator result{ *this };
					idx++;
					return result;
				}

				inline const_iterator& operator--() {
					idx--;
					return *this;
				}

				inline const_iterator operator--(int) {
					const_iterator result{ *this };
					idx--;
					return result;
				}

				inline bool operator==(const const_iterator& other) const { return idx == other.idx; }
				inline bool operator!=(const const_iterator& other) const { return idx != other.idx; }
				inline bool operator<(const const_iterator& other) const { return idx < other.idx; }
				inline bool operator>(const const_iterator& other) const { return idx > other.idx; }
				inline bool operator<=(const const_iterator& other) const { return idx <= other.idx; }
				inline bool operator>=(const const_iterator& other) const { return idx >= other.idx; }
				inline const auto operator*() const { return accessor[idx]; }
			};

			constexpr size_t size() const {
				return this->refl.get_length();
			}

			constexpr auto operator[](size_t idx) {
				auto item_refl = this->refl.get_item_type();

				return ValueAccessor<decltype(item_refl)>{ *util::addptr(&this->value, idx * item_refl.get_size(*this)), item_refl };
			}

			constexpr const auto operator[](size_t idx) const {
				auto item_refl = this->refl.get_item_type();

				return ValueConstAccessor<decltype(item_refl)>{ *util::addptr(&this->value, idx * item_refl.get_size(*this)), item_refl };
			}
		};

		template<typename Refl>
		class AddressConstAccessor : public ConstAccessorBase<Refl> {
		public:
			using ConstAccessorBase<Refl>::ConstAccessorBase;

			constexpr operator bool() const {
				return this->value != nullptr;
			}

			constexpr operator size_t() const {
				return static_cast<size_t>(this->value);
			}

			constexpr ValueConstAccessor<Refl> operator*() const noexcept {
				return { this->value, this->refl };
			}

			//constexpr auto operator->() const noexcept {
			//	return ValueConstAccessor<decltype(target_type)>{ this->value, this->refl };
			//}

			constexpr bool operator==(std::nullptr_t ptr) const {
				return this->value == nullptr;
			}

			constexpr bool operator==(const AddressConstAccessor<Refl>& other) const {
				return this->value == other->value;
			}

			constexpr bool operator!=(std::nullptr_t ptr) const {
				return this->value != nullptr;
			}

			constexpr bool operator!=(const AddressConstAccessor<Refl>& other) const {
				return this->value != other->value;
			}
		};

		template<typename Refl>
		class AddressAccessor : public AccessorBase<Refl> {
		public:
			using AccessorBase<Refl>::AccessorBase;

			constexpr operator bool() const {
				return this->value != nullptr;
			}

			constexpr operator size_t() const {
				return static_cast<size_t>(this->value);
			}

			constexpr ValueAccessor<Refl> operator*() const noexcept {
				return { this->value, this->refl };
			}

			//constexpr auto operator->() const noexcept {
			//	return ValueAccessor<decltype(target_type)>{ this->value, this->refl };
			//}

			constexpr bool operator==(std::nullptr_t ptr) const {
				return this->value == nullptr;
			}

			constexpr bool operator==(const AddressAccessor<Refl>& other) const {
				return this->value == other->value;
			}

			constexpr bool operator!=(std::nullptr_t ptr) const {
				return this->value != nullptr;
			}

			constexpr bool operator!=(const AddressAccessor<Refl>& other) const {
				return this->value != other->value;
			}
		};

		template<typename Refl>
		class PointerConstAccessor : public ConstAccessorBase<Refl, opaque_value*> {
		public:
			using ConstAccessorBase<Refl, opaque_value*>::ConstAccessorBase;

			//inline auto get() const {
			//	auto target_type = this->refl.get_target_type();

			//	return this->reference.withStream([&](auto& stream) {
			//		rip::binary::offset_t<void> offset{};
			//		stream.read(offset);

			//		return !offset.has_value() ? std::nullopt : std::make_optional<const ValueConstAccessor<decltype(target_type)>>({ { this->reference.stream, offset.value() }, target_type });
			//	});
			//}

			constexpr auto get() const {
				auto target_type = this->refl.get_target_type();

				if (this->value == nullptr)
					return AddressConstAccessor<decltype(target_type)>{ nullptr, target_type };

				AddressConstAccessor<decltype(target_type)> addr{ *this->value, target_type };

				return addr;// target_type.template get_size<typename Stream::AddrType>(*addr) > 0 ? addr : AddressConstAccessor<decltype(target_type)>{ {this->reference.stream, 0ull}, target_type };
			}

			constexpr operator auto() const {
				return get();
			}

			constexpr operator size_t() const {
				return get();
			}

			constexpr operator bool() const {
				return get();
			}

			constexpr auto operator*() const noexcept {
				return *get();
			}

			//constexpr auto operator->() const noexcept {
			//	return *get();
			//}

			constexpr bool operator==(std::nullptr_t ptr) const {
				return get() == ptr;
			}

			constexpr bool operator==(const PointerConstAccessor<Refl>& other) const {
				return get() == other.get();
			}

			constexpr bool operator!=(std::nullptr_t ptr) const {
				return get() != ptr;
			}

			constexpr bool operator!=(const PointerConstAccessor<Refl>& other) const {
				return get() != other.get();
			}
		};

		template<typename Refl>
		class PointerAccessor : public AccessorBase<Refl, opaque_value*> {
		public:
			using AccessorBase<Refl, opaque_value*>::AccessorBase;

			//inline auto get() const {
			//	auto target_type = this->refl.get_target_type();

			//	return this->reference.withStream([&](auto& stream) {
			//		rip::binary::offset_t<void> offset{};
			//		stream.read(offset);

			//		return !offset.has_value() ? std::nullopt : std::make_optional<const ValueAccessor<decltype(target_type)>>({ { this->reference.stream, offset.value() }, target_type });
			//	});
			//}

			constexpr auto get() const {
				auto target_type = this->refl.get_target_type();

				if (this->value == nullptr)
					return AddressAccessor<decltype(target_type)>{ nullptr, target_type };

				AddressAccessor<decltype(target_type)> addr{ *this->value, target_type };

				return addr;// target_type.template get_size<typename Stream::AddrType>(*addr) > 0 ? addr : AddressAccessor<decltype(target_type)>{ {this->reference.stream, 0ull}, target_type };
			}

			constexpr operator auto() const {
				return get();
			}

			constexpr operator size_t() const {
				return get();
			}

			constexpr operator bool() const {
				return get();
			}

			constexpr auto operator*() const noexcept {
				return *get();
			}

			//constexpr auto operator->() const noexcept {
			//	return *get();
			//}

			constexpr PointerAccessor<Refl>& operator=(std::nullptr_t other) {
				*this->value = nullptr;
				return *this;
			}

			constexpr PointerAccessor<Refl>& operator=(const AddressAccessor<Refl>& other) {
				*this->value = other.value;
				return *this;
			}

			constexpr bool operator==(std::nullptr_t ptr) const {
				return get() == ptr;
			}

			constexpr bool operator==(const PointerConstAccessor<Refl>& other) const {
				return get() == other.get();
			}

			constexpr bool operator==(const PointerAccessor<Refl>& other) const {
				return get() == other.get();
			}

			constexpr bool operator!=(std::nullptr_t ptr) const {
				return get() != ptr;
			}

			constexpr bool operator!=(const PointerConstAccessor<Refl>& other) const {
				return get() != other.get();
			}

			constexpr bool operator!=(const PointerAccessor<Refl>& other) const {
				return get() != other.get();
			}
		};

		//template<typename Refl>
		//class PointerConstAccessor : public ConstAccessorBase<Refl, opaque_value*> {
		//public:
		//	using ConstAccessorBase<Refl, opaque_value*>::ConstAccessorBase;

		//	constexpr void clear() {
		//		this->value = nullptr;
		//	}

		//	constexpr void set(const ConstAccessorBase<Refl>& other) {
		//		this->value = &other.value;
		//	}

		//	constexpr auto get() {
		//		auto target_type = this->refl.get_target_type();

		//		return this->value == nullptr ? std::nullopt : std::make_optional<ValueConstAccessor<decltype(target_type)>>({ *this->value, target_type });
		//	}

		//	constexpr auto get() const {
		//		auto target_type = this->refl.get_target_type();

		//		return this->value == nullptr ? std::nullopt : std::make_optional<const ValueConstAccessor<decltype(target_type)>>({ *this->value, target_type });
		//	}
		//};

		//template<typename Refl>
		//class PointerAccessor : public AccessorBase<Refl, opaque_value*> {
		//public:
		//	using AccessorBase<Refl, opaque_value*>::AccessorBase;

		//	constexpr void clear() {
		//		this->value = nullptr;
		//	}

		//	constexpr void set(const AccessorBase<Refl>& other) {
		//		this->value = &other.value;
		//	}

		//	constexpr auto get() {
		//		auto target_type = this->refl.get_target_type();

		//		return this->value == nullptr ? std::nullopt : std::make_optional<ValueAccessor<decltype(target_type)>>({ *this->value, target_type });
		//	}

		//	constexpr auto get() const {
		//		auto target_type = this->refl.get_target_type();

		//		return this->value == nullptr ? std::nullopt : std::make_optional<const ValueAccessor<decltype(target_type)>>({ *this->value, target_type });
		//	}
		//};

		template<typename Refl>
		class UnionConstAccessor : public ConstAccessorBase<Refl> {
		public:
			using ConstAccessorBase<Refl>::ConstAccessorBase;

			template<typename FieldRefl>
			constexpr auto operator[](const FieldRefl& field_refl) const {
				auto type = field_refl.get_type();

				return ValueConstAccessor<decltype(type)>{ this->value, type };
			}

			template<simplerfl::strlit FieldName>
			constexpr auto get_field() const {
				return (*this)[this->refl.template get_field<FieldName>(*this)];
			}

			template<typename F>
			constexpr void visit(F f) const {
				this->refl.visit_current_field([&](const auto& field_refl) {
					f((*this)[field_refl]);
				});
			}
		};

		template<typename Refl>
		class UnionAccessor : public AccessorBase<Refl> {
		public:
			using AccessorBase<Refl>::AccessorBase;

			template<typename FieldRefl>
			constexpr auto operator[](const FieldRefl& field_refl) const {
				auto type = field_refl.get_type();

				return ValueConstAccessor<decltype(type)>{ this->value, type };
			}

			template<typename FieldRefl>
			constexpr auto operator[](const FieldRefl& field_refl) {
				auto type = field_refl.get_type();

				return ValueAccessor<decltype(type)>{ this->value, type };
			}

			template<simplerfl::strlit FieldName>
			constexpr auto get_field() const {
				return (*this)[this->refl.template get_field<FieldName>(*this)];
			}

			template<typename F>
			constexpr void visit(F f) const {
				this->refl.visit_current_field([&](const auto& field_refl) {
					f((*this)[field_refl]);
				});
			}
		};

		template<template<typename, typename> typename A, typename Refl>
		class ArrayConstAccessor : public ConstAccessorBase<Refl, RflArray<A, Refl>> {
		public:
			class const_iterator {
				const ArrayConstAccessor& accessor;
				size_t idx{};

			public:
				inline const_iterator(const ArrayConstAccessor& accessor, size_t idx) : accessor{ accessor }, idx{ idx } {}
				inline const_iterator(const const_iterator& other) : accessor{ other.accessor }, idx{ other.idx } {}

				inline const_iterator& operator++() {
					idx++;
					return *this;
				}

				inline const_iterator operator++(int) {
					const_iterator result{ *this };
					idx++;
					return result;
				}

				inline const_iterator& operator--() {
					idx--;
					return *this;
				}

				inline const_iterator operator--(int) {
					const_iterator result{ *this };
					idx--;
					return result;
				}

				inline bool operator==(const const_iterator& other) const { return idx == other.idx; }
				inline bool operator!=(const const_iterator& other) const { return idx != other.idx; }
				inline bool operator<(const const_iterator& other) const { return idx < other.idx; }
				inline bool operator>(const const_iterator& other) const { return idx > other.idx; }
				inline bool operator<=(const const_iterator& other) const { return idx <= other.idx; }
				inline bool operator>=(const const_iterator& other) const { return idx >= other.idx; }
				inline const auto operator*() const { return accessor[idx]; }
			};

			//template<typename T> OpaqueReflArray(A<T, typename GameInterface::AllocatorSystem>& underlying, Refl refl) : underlying{ static_cast<RflArray<A>&>(underlying) }, refl{ refl } {}
			//template<typename T> OpaqueReflArray(const OpaqueReflArray<A, GameInterface, Refl>& other, Refl refl) : underlying{ other.underlying }, refl{ other.refl } {}
			
			using ConstAccessorBase<Refl, RflArray<A, Refl>>::ConstAccessorBase;

			inline GameInterface::AllocatorSystem::allocator_type* get_allocator() const { return this->value.get_allocator(); }

			inline const_iterator begin() const { return { *this, 0 }; }
			inline const_iterator cbegin() const { return { *this, 0 }; }
			inline const_iterator end() const { return { *this, size() }; }
			inline const_iterator cend() const { return { *this, size() }; }

			inline const auto operator[](size_t i) const {
				auto item_type = this->refl.get_item_type();

				return ValueConstAccessor<decltype(item_type)>{ this->value.at(*this, i), item_type };
			}

			inline size_t size() const { return this->value.size(); }
			inline size_t capacity() const { return this->value.capacity(); }
		};

		template<template<typename, typename> typename A, typename Refl>
		class ArrayAccessor : public AccessorBase<Refl, RflArray<A, Refl>> {
		public:
			class iterator {
				const ArrayAccessor& accessor;
				size_t idx{};

			public:
				inline iterator(const ArrayAccessor& accessor, size_t idx) : accessor{ accessor }, idx{ idx } {}
				inline iterator(const iterator& other) : accessor{ other.accessor }, idx{ other.idx } {}

				inline iterator& operator++() {
					idx++;
					return *this;
				}

				inline iterator operator++(int) {
					iterator result{ *this };
					idx++;
					return result;
				}

				inline iterator& operator--() {
					idx--;
					return *this;
				}

				inline iterator operator--(int) {
					iterator result{ *this };
					idx--;
					return result;
				}

				inline bool operator==(const iterator& other) const { return idx == other.idx; }
				inline bool operator!=(const iterator& other) const { return idx != other.idx; }
				inline bool operator<(const iterator& other) const { return idx < other.idx; }
				inline bool operator>(const iterator& other) const { return idx > other.idx; }
				inline bool operator<=(const iterator& other) const { return idx <= other.idx; }
				inline bool operator>=(const iterator& other) const { return idx >= other.idx; }
				inline auto operator*() { return accessor[idx]; }
			};

			class const_iterator {
				const ArrayAccessor& accessor;
				size_t idx{};

			public:
				inline const_iterator(const ArrayAccessor& accessor, size_t idx) : accessor{ accessor }, idx{ idx } {}
				inline const_iterator(const const_iterator& other) : accessor{ other.accessor }, idx{ other.idx } {}

				inline const_iterator& operator++() {
					idx++;
					return *this;
				}

				inline const_iterator operator++(int) {
					const_iterator result{ *this };
					idx++;
					return result;
				}

				inline const_iterator& operator--() {
					idx--;
					return *this;
				}

				inline const_iterator operator--(int) {
					const_iterator result{ *this };
					idx--;
					return result;
				}

				inline bool operator==(const const_iterator& other) const { return idx == other.idx; }
				inline bool operator!=(const const_iterator& other) const { return idx != other.idx; }
				inline bool operator<(const const_iterator& other) const { return idx < other.idx; }
				inline bool operator>(const const_iterator& other) const { return idx > other.idx; }
				inline bool operator<=(const const_iterator& other) const { return idx <= other.idx; }
				inline bool operator>=(const const_iterator& other) const { return idx >= other.idx; }
				inline const auto operator*() const { return accessor[idx]; }
			};

			//template<typename T> OpaqueReflArray(A<T, typename GameInterface::AllocatorSystem>& underlying, Refl refl) : underlying{ static_cast<RflArray<A>&>(underlying) }, refl{ refl } {}
			//template<typename T> OpaqueReflArray(const OpaqueReflArray<A, GameInterface, Refl>& other, Refl refl) : underlying{ other.underlying }, refl{ other.refl } {}
			
			using AccessorBase<Refl, RflArray<A, Refl>>::AccessorBase;

			inline GameInterface::AllocatorSystem::allocator_type* get_allocator() const { return this->value.get_allocator(); }
			inline void change_allocator(GameInterface::AllocatorSystem::allocator_type* new_allocator) { this->value.change_allocator(*this, new_allocator); }
			inline void reserve(size_t len) { this->value.reserve(*this, len); }

			inline auto emplace_back() {
				auto item_type = this->refl.get_item_type();

				return ValueAccessor<decltype(item_type)>{ this->value.emplace_back(*this), item_type };
			}
			inline void remove(size_t i) { this->value.remove(*this, i); }
			inline void clear() { this->value.clear(*this); }

			inline iterator begin() { return { *this, 0 }; }
			inline const_iterator begin() const { return { *this, 0 }; }
			inline const_iterator cbegin() const { return { *this, 0 }; }
			inline iterator end() { return { *this, size() }; }
			inline const_iterator end() const { return { *this, size() }; }
			inline const_iterator cend() const { return { *this, size() }; }

			inline auto operator[](size_t i) {
				auto item_type = this->refl.get_item_type();

				return ValueAccessor<decltype(item_type)>{ this->value.at(*this, i), item_type };
			}
			inline const auto operator[](size_t i) const {
				auto item_type = this->refl.get_item_type();

				return ValueConstAccessor<decltype(item_type)>{ this->value.at(*this, i), item_type };
			}

			inline size_t size() const { return this->value.size(); }
			inline size_t capacity() const { return this->value.capacity(); }
		};

		template<typename Refl>
		class ValueConstAccessor : public ConstAccessorBase<Refl> {
		public:
			using ConstAccessorBase<Refl>::ConstAccessorBase;

			constexpr const auto visit(auto f) const {
				return this->refl.visit([&](auto r) {
					if constexpr (decltype(r)::kind == ucsl::reflection::providers::TypeKind::PRIMITIVE) return f(PrimitiveConstAccessor<decltype(r)>{ this->value, r });
					else if constexpr (decltype(r)::kind == ucsl::reflection::providers::TypeKind::ENUM) return f(EnumConstAccessor<decltype(r)>{ this->value, r });
					else if constexpr (decltype(r)::kind == ucsl::reflection::providers::TypeKind::BITFIELD) return f(BitfieldConstAccessor<decltype(r)>{ this->value, r });
					else if constexpr (decltype(r)::kind == ucsl::reflection::providers::TypeKind::ARRAY) return f(ArrayConstAccessor<ucsl::containers::arrays::Array, decltype(r)>{ this->value, r });
					else if constexpr (decltype(r)::kind == ucsl::reflection::providers::TypeKind::TARRAY) return f(ArrayConstAccessor<ucsl::containers::arrays::TArray, decltype(r)>{ this->value, r });
					else if constexpr (decltype(r)::kind == ucsl::reflection::providers::TypeKind::POINTER) return f(PointerConstAccessor<decltype(r)>{ this->value, r });
					else if constexpr (decltype(r)::kind == ucsl::reflection::providers::TypeKind::CARRAY) return f(CArrayConstAccessor<decltype(r)>{ this->value, r });
					else if constexpr (decltype(r)::kind == ucsl::reflection::providers::TypeKind::UNION) return f(UnionConstAccessor<decltype(r)>{ this->value, r });
					else if constexpr (decltype(r)::kind == ucsl::reflection::providers::TypeKind::STRUCTURE) return f(StructureConstAccessor<decltype(r)>{ this->value, r });
					else static_assert(false, "invalid type kind");
				});
			}

			constexpr auto as_primitive() const { return this->refl.visit([&](auto r) { if constexpr (decltype(r)::kind == ucsl::reflection::providers::TypeKind::PRIMITIVE) return PrimitiveConstAccessor<decltype(r)>{ this->value, r }; else static_assert(false, "not a primitive"); }); }
			constexpr auto as_enum() const { return this->refl.visit([&](auto r) { if constexpr (decltype(r)::kind == ucsl::reflection::providers::TypeKind::ENUM) return EnumConstAccessor<decltype(r)>{ this->value, r }; else static_assert(false, "not an enum"); }); }
			constexpr auto as_bitfield() const { return this->refl.visit([&](auto r) { if constexpr (decltype(r)::kind == ucsl::reflection::providers::TypeKind::BITFIELD) return BitfieldConstAccessor<decltype(r)>{ this->value, r }; else static_assert(false, "not a bitfield"); }); }
			constexpr auto as_array() const { return this->refl.visit([&](auto r) { if constexpr (decltype(r)::kind == ucsl::reflection::providers::TypeKind::ARRAY) return ArrayConstAccessor<ucsl::containers::arrays::Array, decltype(r)>{ this->value, r }; else static_assert(false, "not a array"); }); }
			constexpr auto as_tarray() const { return this->refl.visit([&](auto r) { if constexpr (decltype(r)::kind == ucsl::reflection::providers::TypeKind::TARRAY) return ArrayConstAccessor<ucsl::containers::arrays::TArray, decltype(r)>{ this->value, r }; else static_assert(false, "not a tarray"); }); }
			constexpr auto as_carray() const { return this->refl.visit([&](auto r) { if constexpr (decltype(r)::kind == ucsl::reflection::providers::TypeKind::CARRAY) return CArrayConstAccessor<decltype(r)>{ this->value, r }; else static_assert(false, "not a carray"); }); }
			constexpr auto as_pointer() const { return this->refl.visit([&](auto r) { if constexpr (decltype(r)::kind == ucsl::reflection::providers::TypeKind::POINTER) return PointerConstAccessor<decltype(r)>{ this->value, r }; else static_assert(false, "not a pointer"); }); }
			constexpr auto as_union() const { return this->refl.visit([&](auto r) { if constexpr (decltype(r)::kind == ucsl::reflection::providers::TypeKind::UNION) return UnionConstAccessor<decltype(r)>{ this->value, r }; else static_assert(false, "not a union"); }); }
			constexpr auto as_structure() const { return this->refl.visit([&](auto r) { if constexpr (decltype(r)::kind == ucsl::reflection::providers::TypeKind::STRUCTURE) return StructureConstAccessor<decltype(r)>{ this->value, r }; else static_assert(false, "not a structure"); }); }
		};

		template<typename Refl>
		class ValueAccessor : public AccessorBase<Refl> {
		public:
			using AccessorBase<Refl>::AccessorBase;

			constexpr const auto visit(auto f) const {
				return this->refl.visit([&](auto r) {
					if constexpr (decltype(r)::kind == ucsl::reflection::providers::TypeKind::PRIMITIVE) return f(PrimitiveConstAccessor<decltype(r)>{ this->value, r });
					else if constexpr (decltype(r)::kind == ucsl::reflection::providers::TypeKind::ENUM) return f(EnumConstAccessor<decltype(r)>{ this->value, r });
					else if constexpr (decltype(r)::kind == ucsl::reflection::providers::TypeKind::BITFIELD) return f(BitfieldConstAccessor<decltype(r)>{ this->value, r });
					else if constexpr (decltype(r)::kind == ucsl::reflection::providers::TypeKind::ARRAY) return f(ArrayConstAccessor<ucsl::containers::arrays::Array, decltype(r)>{ this->value, r });
					else if constexpr (decltype(r)::kind == ucsl::reflection::providers::TypeKind::TARRAY) return f(ArrayConstAccessor<ucsl::containers::arrays::TArray, decltype(r)>{ this->value, r });
					else if constexpr (decltype(r)::kind == ucsl::reflection::providers::TypeKind::POINTER) return f(PointerConstAccessor<decltype(r)>{ this->value, r });
					else if constexpr (decltype(r)::kind == ucsl::reflection::providers::TypeKind::CARRAY) return f(CArrayConstAccessor<decltype(r)>{ this->value, r });
					else if constexpr (decltype(r)::kind == ucsl::reflection::providers::TypeKind::UNION) return f(UnionConstAccessor<decltype(r)>{ this->value, r });
					else if constexpr (decltype(r)::kind == ucsl::reflection::providers::TypeKind::STRUCTURE) return f(StructureConstAccessor<decltype(r)>{ this->value, r });
					else static_assert(false, "invalid type kind");
				});
			}

			constexpr auto visit(auto f) {
				return this->refl.visit([&](auto r) {
					if constexpr (decltype(r)::kind == ucsl::reflection::providers::TypeKind::PRIMITIVE) return f(PrimitiveAccessor<decltype(r)>{ this->value, r });
					else if constexpr (decltype(r)::kind == ucsl::reflection::providers::TypeKind::ENUM) return f(EnumAccessor<decltype(r)>{ this->value, r });
					else if constexpr (decltype(r)::kind == ucsl::reflection::providers::TypeKind::BITFIELD) return f(BitfieldAccessor<decltype(r)>{ this->value, r });
					else if constexpr (decltype(r)::kind == ucsl::reflection::providers::TypeKind::ARRAY) return f(ArrayAccessor<ucsl::containers::arrays::Array, decltype(r)>{ this->value, r });
					else if constexpr (decltype(r)::kind == ucsl::reflection::providers::TypeKind::TARRAY) return f(ArrayAccessor<ucsl::containers::arrays::TArray, decltype(r)>{ this->value, r });
					else if constexpr (decltype(r)::kind == ucsl::reflection::providers::TypeKind::POINTER) return f(PointerAccessor<decltype(r)>{ this->value, r });
					else if constexpr (decltype(r)::kind == ucsl::reflection::providers::TypeKind::CARRAY) return f(CArrayAccessor<decltype(r)>{ this->value, r });
					else if constexpr (decltype(r)::kind == ucsl::reflection::providers::TypeKind::UNION) return f(UnionAccessor<decltype(r)>{ this->value, r });
					else if constexpr (decltype(r)::kind == ucsl::reflection::providers::TypeKind::STRUCTURE) return f(StructureAccessor<decltype(r)>{ this->value, r });
					else static_assert(false, "invalid type kind");
				});
			}

			constexpr auto as_primitive() const { return this->refl.visit([&](auto r) { if constexpr (decltype(r)::kind == ucsl::reflection::providers::TypeKind::PRIMITIVE) return PrimitiveConstAccessor<decltype(r)>{ this->value, r }; else static_assert(false, "not a primitive"); }); }
			constexpr auto as_enum() const { return this->refl.visit([&](auto r) { if constexpr (decltype(r)::kind == ucsl::reflection::providers::TypeKind::ENUM) return EnumConstAccessor<decltype(r)>{ this->value, r }; else static_assert(false, "not an enum"); }); }
			constexpr auto as_bitfield() const { return this->refl.visit([&](auto r) { if constexpr (decltype(r)::kind == ucsl::reflection::providers::TypeKind::BITFIELD) return BitfieldConstAccessor<decltype(r)>{ this->value, r }; else static_assert(false, "not a bitfield"); }); }
			constexpr auto as_array() const { return this->refl.visit([&](auto r) { if constexpr (decltype(r)::kind == ucsl::reflection::providers::TypeKind::ARRAY) return ArrayConstAccessor<ucsl::containers::arrays::Array, decltype(r)>{ this->value, r }; else static_assert(false, "not a array"); }); }
			constexpr auto as_tarray() const { return this->refl.visit([&](auto r) { if constexpr (decltype(r)::kind == ucsl::reflection::providers::TypeKind::TARRAY) return ArrayConstAccessor<ucsl::containers::arrays::TArray, decltype(r)>{ this->value, r }; else static_assert(false, "not a tarray"); }); }
			constexpr auto as_carray() const { return this->refl.visit([&](auto r) { if constexpr (decltype(r)::kind == ucsl::reflection::providers::TypeKind::CARRAY) return CArrayConstAccessor<decltype(r)>{ this->value, r }; else static_assert(false, "not a carray"); }); }
			constexpr auto as_pointer() const { return this->refl.visit([&](auto r) { if constexpr (decltype(r)::kind == ucsl::reflection::providers::TypeKind::POINTER) return PointerConstAccessor<decltype(r)>{ this->value, r }; else static_assert(false, "not a pointer"); }); }
			constexpr auto as_union() const { return this->refl.visit([&](auto r) { if constexpr (decltype(r)::kind == ucsl::reflection::providers::TypeKind::UNION) return UnionConstAccessor<decltype(r)>{ this->value, r }; else static_assert(false, "not a union"); }); }
			constexpr auto as_structure() const { return this->refl.visit([&](auto r) { if constexpr (decltype(r)::kind == ucsl::reflection::providers::TypeKind::STRUCTURE) return StructureConstAccessor<decltype(r)>{ this->value, r }; else static_assert(false, "not a structure"); }); }

			constexpr auto as_primitive() { return this->refl.visit([&](auto r) { if constexpr (decltype(r)::kind == ucsl::reflection::providers::TypeKind::PRIMITIVE) return PrimitiveAccessor<decltype(r)>{ this->value, r }; else static_assert(false, "not a primitive"); }); }
			constexpr auto as_enum() { return this->refl.visit([&](auto r) { if constexpr (decltype(r)::kind == ucsl::reflection::providers::TypeKind::ENUM) return EnumAccessor<decltype(r)>{ this->value, r }; else static_assert(false, "not an enum"); }); }
			constexpr auto as_bitfield() { return this->refl.visit([&](auto r) { if constexpr (decltype(r)::kind == ucsl::reflection::providers::TypeKind::BITFIELD) return BitfieldAccessor<decltype(r)>{ this->value, r }; else static_assert(false, "not a bitfield"); }); }
			constexpr auto as_array() { return this->refl.visit([&](auto r) { if constexpr (decltype(r)::kind == ucsl::reflection::providers::TypeKind::ARRAY) return ArrayAccessor<ucsl::containers::arrays::Array, decltype(r)>{ this->value, r }; else static_assert(false, "not a array"); }); }
			constexpr auto as_tarray() { return this->refl.visit([&](auto r) { if constexpr (decltype(r)::kind == ucsl::reflection::providers::TypeKind::TARRAY) return ArrayAccessor<ucsl::containers::arrays::TArray, decltype(r)>{ this->value, r }; else static_assert(false, "not a tarray"); }); }
			constexpr auto as_carray() { return this->refl.visit([&](auto r) { if constexpr (decltype(r)::kind == ucsl::reflection::providers::TypeKind::CARRAY) return CArrayAccessor<decltype(r)>{ this->value, r }; else static_assert(false, "not a carray"); }); }
			constexpr auto as_pointer() { return this->refl.visit([&](auto r) { if constexpr (decltype(r)::kind == ucsl::reflection::providers::TypeKind::POINTER) return PointerAccessor<decltype(r)>{ this->value, r }; else static_assert(false, "not a pointer"); }); }
			constexpr auto as_union() { return this->refl.visit([&](auto r) { if constexpr (decltype(r)::kind == ucsl::reflection::providers::TypeKind::UNION) return UnionAccessor<decltype(r)>{ this->value, r }; else static_assert(false, "not a union"); }); }
			constexpr auto as_structure() { return this->refl.visit([&](auto r) { if constexpr (decltype(r)::kind == ucsl::reflection::providers::TypeKind::STRUCTURE) return StructureAccessor<decltype(r)>{ this->value, r }; else static_assert(false, "not a structure"); }); }
		};
	};
}
