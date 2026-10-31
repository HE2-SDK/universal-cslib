#pragma once
#include <simple-reflection/simple-reflection.h>
#include <ucsl/containers/arrays/array.h>
#include <ucsl/containers/arrays/tarray.h>
#include <ucsl/strings/variable-string.h>
#include <ucsl-reflection/providers/types.h>
#include <ucsl-reflection/providers/rflclass.h>
#include <ucsl-reflection/util/memory.h>

namespace ucsl::reflection::accessors {
	template<typename GameInterface>
	struct binary {
		struct opaque_value {};

		template<typename Refl>
		class ValueConstAccessor;

		template<typename Refl>
		class ValueAccessor;

		static inline void construct(const auto& refl, typename GameInterface::AllocatorSystem::allocator_type* allocator, void* ptr) {
			memset(ptr, 0, refl.template get_size<size_t>(ValueAccessor<decltype(refl)>{ ptr, refl }));
		}
		static inline void construct(const typename ucsl::reflection::providers::rflclass<GameInterface>::Structure& refl, typename GameInterface::AllocatorSystem::allocator_type* allocator, void* ptr) {
			GameInterface::RflTypeInfoRegistry::GetInstance()->ConstructObject(allocator, ptr, refl.get_name());
		}

		static inline void destroy(const auto& refl, void* ptr) {}
		static inline void destroy(const typename ucsl::reflection::providers::rflclass<GameInterface>::Structure& refl, void* ptr) {
			GameInterface::RflTypeInfoRegistry::GetInstance()->CleanupLoadedObject(ptr, refl.get_name());
		}

		template<template<typename, typename> typename A, typename Refl>
		class RflArray : public A<opaque_value, typename GameInterface::AllocatorSystem> {
		public:
			void change_allocator(const auto& refl, GameInterface::AllocatorSystem::allocator_type* new_allocator) {
				if (!new_allocator || this->allocator == new_allocator)
					return;

				if (this->is_deallocation_disabled() || !this->buffer) {
					this->allocator = new_allocator;
					return;
				}

				auto item_type = refl.get_item_type();

				opaque_value* new_buffer = static_cast<opaque_value*>(new_allocator->Alloc(this->capacity() * item_type.template get_size<size_t>(ValueAccessor<decltype(item_type)>{ this->buffer, item_type }), 16));

				move(item_type, new_buffer, this->buffer, this->length);

				if (this->allocator && !this->is_deallocation_disabled())
					this->allocator->Free(this->buffer);

				this->buffer = new_buffer;
				this->allocator = new_allocator;
			}

			void reserve(const auto& refl, size_t desired_cap) {
				auto* allocator = this->get_allocator();
				auto cap = this->capacity();

				if (desired_cap <= cap)
					return;

				auto newcap = std::max(2 * cap, desired_cap);

				auto item_type = refl.get_item_type();

				opaque_value* new_buffer = static_cast<opaque_value*>(allocator->Alloc(newcap * item_type.template get_size<size_t>(ValueAccessor<decltype(item_type)>{ this->buffer, item_type }), 16));

				if (this->buffer) {
					move(item_type, new_buffer, this->buffer, this->length);

					if (!this->is_deallocation_disabled())
						allocator->Free(this->buffer);
				}

				this->buffer = new_buffer;
				this->capacity_and_flags = newcap;
			}

			opaque_value& emplace_back(const auto& refl) {
				this->reserve(refl, this->length + 1);

				opaque_value& loc = at(refl, this->length);

				auto item_type = refl.get_item_type();

				construct(item_type, this->allocator, &loc);

				this->length++;

				return loc;
			}

			void remove(const auto& refl, size_t i) {
				if (i >= this->length)
					return;

				auto item_type = refl.get_item_type();

				destroy(item_type, &at(refl, i));
				move(item_type, &at(refl, i), &at(refl, i + 1), this->length - i - 1);

				this->length--;
			}

			void clear(const auto& refl) {
				if (this->empty())
					return;

				auto item_type = refl.get_item_type();

				for (size_t i = 0; i < this->length; i++)
					destroy(item_type, &at(refl, i));

				this->length = 0;
			}

			opaque_value& at(const auto& refl, size_t i) {
				auto item_type = refl.get_item_type();

				return *util::addptr(this->buffer, i * item_type.template get_size<size_t>(ValueAccessor<decltype(item_type)>{ this->buffer, item_type }));
			}

			const opaque_value& at(const auto& refl, size_t i) const {
				auto item_type = refl.get_item_type();

				return *util::addptr(this->buffer, i * item_type.template get_size<size_t>(ValueConstAccessor<decltype(item_type)>{ this->buffer, item_type }));
			}
		private:
			void move(const auto& item_type, opaque_value* dst, opaque_value* src, size_t count) {
				memmove(dst, src, count * item_type.template get_size<size_t>(ValueAccessor<decltype(item_type)>{ src, item_type }));
			}
		};

		template<typename Refl>
		class AddressConstAccessor;

		template<typename Refl>
		class AddressAccessor;

		template<typename Refl, typename T = opaque_value>
		class ConstAccessorBase {
		public:
			const T* const value;
			const Refl refl;

			template<typename U>
			constexpr ConstAccessorBase(const U* value, const Refl& refl) : value{ (const T*)value }, refl{ refl } {}

			constexpr AddressConstAccessor<Refl> operator&() const {
				return { value, refl };
			}
		};

		template<typename Refl, typename T = opaque_value>
		class AccessorBase {
		public:
			T* const value;
			const Refl refl;

			template<typename U>
			constexpr AccessorBase(U* value, const Refl& refl) : value{ (T*)value }, refl{ refl } {}

			constexpr AddressAccessor<Refl> operator&() const {
				return { value, refl };
			}
		};

		template<typename Refl>
		class PrimitiveDataConstAccessor : public ConstAccessorBase<Refl, typename Refl::repr> {
		public:
			using ConstAccessorBase<Refl, typename Refl::repr>::ConstAccessorBase;

			constexpr operator std::conditional_t<std::is_same_v<typename Refl::repr, const char*> || std::is_same_v<typename Refl::repr, ucsl::strings::VariableString> || (std::is_array_v<typename Refl::repr> && std::is_same_v<std::remove_extent_t<typename Refl::repr>, char>),std::string,typename Refl::repr>() const {
				if constexpr (std::is_same_v<typename Refl::repr, ucsl::strings::VariableString>) {
					return std::string{ this->value->c_str() };
				}
				else if constexpr (std::is_same_v<typename Refl::repr, const char*>) {
					return *this->value == nullptr ? std::string{} : std::string{ *this->value };
				}
				else if constexpr (std::is_array_v<typename Refl::repr> && std::is_same_v<std::remove_extent_t<typename Refl::repr>, char>) {
					std::string res{ this->value };
					res.resize(std::extent_v<typename Refl::repr> - 1);
					return res;
				}
				else {
					return *this->value;
				}
			}

			//constexpr operator typename Refl::repr() const {
			//	if constexpr (std::is_fundamental_v<typename Refl::repr>)
			//		return this->refl.is_erased ? typename Refl::repr{} : this->refl.constant_value.has_value() ? this->refl.constant_value.value() : *this->value;
			//	else
			//		return this->refl.constant_value.has_value() ? this->refl.constant_value.value() : *this->value;
			//}

			constexpr const typename Refl::repr& ref() const & {
				return *this->value;
			}
		};

		template<typename Refl>
		class PrimitiveDataAccessor : public AccessorBase<Refl, typename Refl::repr> {
		public:
			using AccessorBase<Refl, typename Refl::repr>::AccessorBase;

			constexpr operator std::conditional_t<std::is_same_v<typename Refl::repr, const char*> || std::is_same_v<typename Refl::repr, ucsl::strings::VariableString> || (std::is_array_v<typename Refl::repr> && std::is_same_v<std::remove_extent_t<typename Refl::repr>, char>), std::string, typename Refl::repr>() const {
				if constexpr (std::is_same_v<typename Refl::repr, ucsl::strings::VariableString>) {
					return std::string{ this->value->c_str() };
				}
				else if constexpr (std::is_same_v<typename Refl::repr, const char*>) {
					return *this->value == nullptr ? std::string{} : std::string{ *this->value };
				}
				else if constexpr (std::is_array_v<typename Refl::repr> && std::is_same_v<std::remove_extent_t<typename Refl::repr>, char>) {
					std::string res{ this->value };
					res.resize(std::extent_v<typename Refl::repr> -1);
					return res;
				}
				else {
					return *this->value;
				}
			}

			constexpr PrimitiveDataAccessor<Refl>& operator=(typename Refl::repr v) {
				if constexpr (std::is_fundamental_v<typename Refl::repr>)
					*this->value = this->refl.is_erased ? typename Refl::repr{} : this->refl.constant_value.has_value() ? this->refl.constant_value.value() : v;
				else
					*this->value = this->refl.constant_value.has_value() ? this->refl.constant_value.value() : v;
				return *this;
			}

			//constexpr operator typename Refl::repr() const {
			//	if constexpr (std::is_fundamental_v<typename Refl::repr>)
			//		return this->refl.is_erased ? typename Refl::repr{} : this->refl.constant_value.has_value() ? this->refl.constant_value.value() : *this->value;
			//	else
			//		return this->refl.constant_value.has_value() ? this->refl.constant_value.value() : *this->value;
			//}

			constexpr typename Refl::repr& ref() & {
				return *this->value;
			}

			constexpr const typename Refl::repr& ref() const & {
				return *this->value;
			}
		};

		template<typename Refl>
		class PrimitiveConstAccessor : public ConstAccessorBase<Refl> {
		public:
			using ConstAccessorBase<Refl>::ConstAccessorBase;

			constexpr auto visit(auto f) const {
				return this->refl.visit([&](const auto& r) { return f(PrimitiveDataConstAccessor<std::decay_t<decltype(r)>>{ this->value, r }); });
			}

			template<typename T>
			constexpr const auto as() const {
				return this->refl.visit([&](const auto& r) { if constexpr (std::is_same_v<typename decltype(r)::repr, T>) return PrimitiveDataConstAccessor<decltype(r)>{ this->reference, r }; else static_assert(false, "not the correct primitive type"); });
			}
		};

		template<typename Refl>
		class PrimitiveAccessor : public AccessorBase<Refl> {
		public:
			using AccessorBase<Refl>::AccessorBase;

			constexpr auto visit(auto f) {
				return this->refl.visit([&](const auto& r) { PrimitiveDataAccessor<std::decay_t<decltype(r)>> acc{ this->value, r }; return f(acc); });
			}

			constexpr auto visit(auto f) const {
				return this->refl.visit([&](const auto& r){ return f(PrimitiveDataConstAccessor<std::decay_t<decltype(r)>>{ this->value, r }); });
			}

			template<typename T>
			constexpr auto as() {
				return this->refl.visit([&](const auto& r) { if constexpr (std::is_same_v<typename decltype(r)::repr, T>) return PrimitiveDataAccessor<decltype(r)>{ this->reference, r }; else static_assert(false, "not the correct primitive type"); });
			}

			template<typename T>
			constexpr const auto as() const {
				return this->refl.visit([&](const auto& r) { if constexpr (std::is_same_v<typename decltype(r)::repr, T>) return PrimitiveDataConstAccessor<decltype(r)>{ this->reference, r }; else static_assert(false, "not the correct primitive type"); });
			}
		};

		template<typename Refl>
		class EnumConstAccessor : public ConstAccessorBase<Refl> {
		public:
			using ConstAccessorBase<Refl>::ConstAccessorBase;

			constexpr operator long long() const {
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
			
			constexpr EnumAccessor<Refl>& operator=(long long v) {
				this->refl.visit([&](const auto& r) {
					PrimitiveDataAccessor<std::decay_t<decltype(r)>> pd{ this->value, r };

					pd = static_cast<typename std::decay_t<decltype(r)>::repr>(v);
				});
				return *this;
			}

			constexpr operator long long() const {
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
				return this->refl.visit_underlying([&](auto r) { PrimitiveDataAccessor<decltype(r)> acc{ this->value, r }; return f(acc); });
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

				return ValueConstAccessor<decltype(type)>{ util::addptr(this->value, field_refl.get_offset()), type };
			}

			template<simplerfl::strlit FieldName>
			constexpr auto get_field() const {
				return (*this)[this->refl.template get_field<FieldName, size_t>(*this)];
			}

			constexpr void visit_fields(auto f) const {
				return this->refl.template visit_fields<size_t>(*this, [&](const auto& field) { f((*this)[field], field); });
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

				return ValueConstAccessor<decltype(type)>{ util::addptr(this->value, field_refl.get_offset()), type };
			}

			constexpr auto operator[](const auto& field_refl) {
				const auto type = field_refl.get_type(*this);

				return ValueAccessor<decltype(type)>{ util::addptr(this->value, field_refl.get_offset()), type };
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
				return this->refl.template visit_fields<size_t>(*this, [&](const auto& fieldRefl) { f((*this)[fieldRefl], fieldRefl); });
			}

			constexpr void visit_fields(auto f) {
				return this->refl.template visit_fields<size_t>(*this, [&](const auto& fieldRefl) { auto field = (*this)[fieldRefl]; f(field, fieldRefl); });
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
				constexpr const_iterator(const CArrayConstAccessor& accessor, size_t idx) : accessor{ accessor }, idx{ idx } {}
				constexpr const_iterator(const const_iterator& other) : accessor{ other.accessor }, idx{ other.idx } {}

				constexpr const_iterator& operator++() {
					idx++;
					return *this;
				}

				constexpr const_iterator operator++(int) {
					const_iterator result{ *this };
					idx++;
					return result;
				}

				constexpr const_iterator& operator--() {
					idx--;
					return *this;
				}

				constexpr const_iterator operator--(int) {
					const_iterator result{ *this };
					idx--;
					return result;
				}

				constexpr bool operator==(const const_iterator& other) const { return idx == other.idx; }
				constexpr bool operator!=(const const_iterator& other) const { return idx != other.idx; }
				constexpr bool operator<(const const_iterator& other) const { return idx < other.idx; }
				constexpr bool operator>(const const_iterator& other) const { return idx > other.idx; }
				constexpr bool operator<=(const const_iterator& other) const { return idx <= other.idx; }
				constexpr bool operator>=(const const_iterator& other) const { return idx >= other.idx; }
				constexpr const auto operator*() const { return accessor[idx]; }
			};

			constexpr size_t size() const {
				return this->refl.get_length();
			}

			constexpr const auto operator[](size_t idx) const {
				auto item_refl = this->refl.get_item_type();

				return ValueConstAccessor<decltype(item_refl)>{ util::addptr(this->value, idx* item_refl.template get_size<size_t>(ValueAccessor<decltype(item_refl)>{ this->value, item_refl })), item_refl };
			}

			constexpr const_iterator begin() const { return { *this, 0 }; }
			constexpr const_iterator cbegin() const { return { *this, 0 }; }
			constexpr const_iterator end() const { return { *this, size() }; }
			constexpr const_iterator cend() const { return { *this, size() }; }
		};

		template<typename Refl>
		class CArrayAccessor : public AccessorBase<Refl> {
		public:
			using AccessorBase<Refl>::AccessorBase;

			class iterator {
				const CArrayAccessor& accessor;
				size_t idx{};

			public:
				constexpr iterator(const CArrayAccessor& accessor, size_t idx) : accessor{ accessor }, idx{ idx } {}
				constexpr iterator(const iterator& other) : accessor{ other.accessor }, idx{ other.idx } {}

				constexpr iterator& operator++() {
					idx++;
					return *this;
				}

				constexpr iterator operator++(int) {
					iterator result{ *this };
					idx++;
					return result;
				}

				constexpr iterator& operator--() {
					idx--;
					return *this;
				}

				constexpr iterator operator--(int) {
					iterator result{ *this };
					idx--;
					return result;
				}

				constexpr bool operator==(const iterator& other) const { return idx == other.idx; }
				constexpr bool operator!=(const iterator& other) const { return idx != other.idx; }
				constexpr bool operator<(const iterator& other) const { return idx < other.idx; }
				constexpr bool operator>(const iterator& other) const { return idx > other.idx; }
				constexpr bool operator<=(const iterator& other) const { return idx <= other.idx; }
				constexpr bool operator>=(const iterator& other) const { return idx >= other.idx; }
				constexpr const auto operator*() const { return accessor[idx]; }
			};

			class const_iterator {
				const CArrayAccessor& accessor;
				size_t idx{};

			public:
				constexpr const_iterator(const CArrayAccessor& accessor, size_t idx) : accessor{ accessor }, idx{ idx } {}
				constexpr const_iterator(const const_iterator& other) : accessor{ other.accessor }, idx{ other.idx } {}

				constexpr const_iterator& operator++() {
					idx++;
					return *this;
				}

				constexpr const_iterator operator++(int) {
					const_iterator result{ *this };
					idx++;
					return result;
				}

				constexpr const_iterator& operator--() {
					idx--;
					return *this;
				}

				constexpr const_iterator operator--(int) {
					const_iterator result{ *this };
					idx--;
					return result;
				}

				constexpr bool operator==(const const_iterator& other) const { return idx == other.idx; }
				constexpr bool operator!=(const const_iterator& other) const { return idx != other.idx; }
				constexpr bool operator<(const const_iterator& other) const { return idx < other.idx; }
				constexpr bool operator>(const const_iterator& other) const { return idx > other.idx; }
				constexpr bool operator<=(const const_iterator& other) const { return idx <= other.idx; }
				constexpr bool operator>=(const const_iterator& other) const { return idx >= other.idx; }
				constexpr const auto operator*() const { return accessor[idx]; }
			};

			constexpr size_t size() const {
				return this->refl.get_length();
			}

			constexpr auto operator[](size_t idx) {
				auto item_refl = this->refl.get_item_type();

				return ValueAccessor<decltype(item_refl)>{ util::addptr(this->value, idx * item_refl.template get_size<size_t>(ValueAccessor<decltype(item_refl)>{ this->value, item_refl })), item_refl };
			}

			constexpr const auto operator[](size_t idx) const {
				auto item_refl = this->refl.get_item_type();

				return ValueConstAccessor<decltype(item_refl)>{ util::addptr(this->value, idx * item_refl.template get_size<size_t>(ValueAccessor<decltype(item_refl)>{ this->value, item_refl })), item_refl };
			}

			constexpr const_iterator begin() const { return { *this, 0 }; }
			constexpr iterator begin() { return { *this, 0 }; }
			constexpr const_iterator cbegin() const { return { *this, 0 }; }
			constexpr const_iterator end() const { return { *this, size() }; }
			constexpr iterator end() { return { *this, size() }; }
			constexpr const_iterator cend() const { return { *this, size() }; }
		};

		template<typename Refl>
		class AddressConstAccessor : public ConstAccessorBase<Refl> {
		public:
			using ConstAccessorBase<Refl>::ConstAccessorBase;

			constexpr operator bool() const {
				return this->value != nullptr;
			}

			constexpr operator size_t() const {
				return reinterpret_cast<size_t>(this->value);
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
				return reinterpret_cast<size_t>(this->value);
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

				if (*this->value == nullptr)
					return AddressConstAccessor<decltype(target_type)>{ (opaque_value*)nullptr, target_type };

				AddressConstAccessor<decltype(target_type)> addr{ *this->value, target_type };

				return addr;// target_type.template get_size<typename Stream::size_t>(*addr) > 0 ? addr : AddressConstAccessor<decltype(target_type)>{ {this->reference.stream, 0ull}, target_type };
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

				if (*this->value == nullptr)
					return AddressAccessor<decltype(target_type)>{ (opaque_value*)nullptr, target_type };

				AddressAccessor<decltype(target_type)> addr{ *this->value, target_type };

				return addr;// target_type.template get_size<typename Stream::size_t>(*addr) > 0 ? addr : AddressAccessor<decltype(target_type)>{ {this->reference.stream, 0ull}, target_type };
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
				constexpr const_iterator(const ArrayConstAccessor& accessor, size_t idx) : accessor{ accessor }, idx{ idx } {}
				constexpr const_iterator(const const_iterator& other) : accessor{ other.accessor }, idx{ other.idx } {}

				constexpr const_iterator& operator++() {
					idx++;
					return *this;
				}

				constexpr const_iterator operator++(int) {
					const_iterator result{ *this };
					idx++;
					return result;
				}

				constexpr const_iterator& operator--() {
					idx--;
					return *this;
				}

				constexpr const_iterator operator--(int) {
					const_iterator result{ *this };
					idx--;
					return result;
				}

				constexpr bool operator==(const const_iterator& other) const { return idx == other.idx; }
				constexpr bool operator!=(const const_iterator& other) const { return idx != other.idx; }
				constexpr bool operator<(const const_iterator& other) const { return idx < other.idx; }
				constexpr bool operator>(const const_iterator& other) const { return idx > other.idx; }
				constexpr bool operator<=(const const_iterator& other) const { return idx <= other.idx; }
				constexpr bool operator>=(const const_iterator& other) const { return idx >= other.idx; }
				constexpr const auto operator*() const { return accessor[idx]; }
			};

			//template<typename T> OpaqueReflArray(A<T, typename GameInterface::AllocatorSystem>& underlying, Refl refl) : underlying{ static_cast<RflArray<A>&>(underlying) }, refl{ refl } {}
			//template<typename T> OpaqueReflArray(const OpaqueReflArray<A, GameInterface, Refl>& other, Refl refl) : underlying{ other.underlying }, refl{ other.refl } {}

			using ConstAccessorBase<Refl, RflArray<A, Refl>>::ConstAccessorBase;

			constexpr GameInterface::AllocatorSystem::allocator_type* get_allocator() const { return this->value->get_allocator(); }

			constexpr const_iterator begin() const { return { *this, 0 }; }
			constexpr const_iterator cbegin() const { return { *this, 0 }; }
			constexpr const_iterator end() const { return { *this, size() }; }
			constexpr const_iterator cend() const { return { *this, size() }; }

			constexpr const auto operator[](size_t i) const {
				auto item_type = this->refl.get_item_type();

				return ValueConstAccessor<decltype(item_type)>{ &this->value->at(this->refl, i), item_type };
			}

			constexpr size_t size() const { return this->value->size(); }
			constexpr size_t capacity() const { return this->value->capacity(); }
		};

		template<template<typename, typename> typename A, typename Refl>
		class ArrayAccessor : public AccessorBase<Refl, RflArray<A, Refl>> {
		public:
			class iterator {
				const ArrayAccessor& accessor;
				size_t idx{};

			public:
				constexpr iterator(const ArrayAccessor& accessor, size_t idx) : accessor{ accessor }, idx{ idx } {}
				constexpr iterator(const iterator& other) : accessor{ other.accessor }, idx{ other.idx } {}

				constexpr iterator& operator++() {
					idx++;
					return *this;
				}

				constexpr iterator operator++(int) {
					iterator result{ *this };
					idx++;
					return result;
				}

				constexpr iterator& operator--() {
					idx--;
					return *this;
				}

				constexpr iterator operator--(int) {
					iterator result{ *this };
					idx--;
					return result;
				}

				constexpr bool operator==(const iterator& other) const { return idx == other.idx; }
				constexpr bool operator!=(const iterator& other) const { return idx != other.idx; }
				constexpr bool operator<(const iterator& other) const { return idx < other.idx; }
				constexpr bool operator>(const iterator& other) const { return idx > other.idx; }
				constexpr bool operator<=(const iterator& other) const { return idx <= other.idx; }
				constexpr bool operator>=(const iterator& other) const { return idx >= other.idx; }
				constexpr auto operator*() { return accessor[idx]; }
			};

			class const_iterator {
				const ArrayAccessor& accessor;
				size_t idx{};

			public:
				constexpr const_iterator(const ArrayAccessor& accessor, size_t idx) : accessor{ accessor }, idx{ idx } {}
				constexpr const_iterator(const const_iterator& other) : accessor{ other.accessor }, idx{ other.idx } {}

				constexpr const_iterator& operator++() {
					idx++;
					return *this;
				}

				constexpr const_iterator operator++(int) {
					const_iterator result{ *this };
					idx++;
					return result;
				}

				constexpr const_iterator& operator--() {
					idx--;
					return *this;
				}

				constexpr const_iterator operator--(int) {
					const_iterator result{ *this };
					idx--;
					return result;
				}

				constexpr bool operator==(const const_iterator& other) const { return idx == other.idx; }
				constexpr bool operator!=(const const_iterator& other) const { return idx != other.idx; }
				constexpr bool operator<(const const_iterator& other) const { return idx < other.idx; }
				constexpr bool operator>(const const_iterator& other) const { return idx > other.idx; }
				constexpr bool operator<=(const const_iterator& other) const { return idx <= other.idx; }
				constexpr bool operator>=(const const_iterator& other) const { return idx >= other.idx; }
				constexpr const auto operator*() const { return accessor[idx]; }
			};

			//template<typename T> OpaqueReflArray(A<T, typename GameInterface::AllocatorSystem>& underlying, Refl refl) : underlying{ static_cast<RflArray<A>&>(underlying) }, refl{ refl } {}
			//template<typename T> OpaqueReflArray(const OpaqueReflArray<A, GameInterface, Refl>& other, Refl refl) : underlying{ other.underlying }, refl{ other.refl } {}
			
			using AccessorBase<Refl, RflArray<A, Refl>>::AccessorBase;

			constexpr GameInterface::AllocatorSystem::allocator_type* get_allocator() const { return this->value->get_allocator(); }
			constexpr void change_allocator(GameInterface::AllocatorSystem::allocator_type* new_allocator) { this->value->change_allocator(this->refl, new_allocator); }
			constexpr void reserve(size_t len) { this->value->reserve(this->refl, len); }

			constexpr auto emplace_back() {
				auto item_type = this->refl.get_item_type();

				return ValueAccessor<decltype(item_type)>{ &this->value->emplace_back(this->refl), item_type };
			}
			constexpr void remove(size_t i) { this->value->remove(this->refl, i); }
			constexpr void clear() { this->value->clear(this->refl); }

			constexpr iterator begin() { return { *this, 0 }; }
			constexpr const_iterator begin() const { return { *this, 0 }; }
			constexpr const_iterator cbegin() const { return { *this, 0 }; }
			constexpr iterator end() { return { *this, size() }; }
			constexpr const_iterator end() const { return { *this, size() }; }
			constexpr const_iterator cend() const { return { *this, size() }; }

			constexpr auto operator[](size_t i) {
				auto item_type = this->refl.get_item_type();

				return ValueAccessor<decltype(item_type)>{ &this->value->at(this->refl, i), item_type };
			}
			constexpr const auto operator[](size_t i) const {
				auto item_type = this->refl.get_item_type();

				return ValueConstAccessor<decltype(item_type)>{ &this->value->at(this->refl, i), item_type };
			}

			constexpr size_t size() const { return this->value->size(); }
			constexpr size_t capacity() const { return this->value->capacity(); }
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
					if constexpr (decltype(r)::kind == ucsl::reflection::providers::TypeKind::PRIMITIVE) { PrimitiveAccessor<decltype(r)> acc{ this->value, r }; return f(acc); }
					else if constexpr (decltype(r)::kind == ucsl::reflection::providers::TypeKind::ENUM) { EnumAccessor<decltype(r)> acc{ this->value, r }; return f(acc); }
					else if constexpr (decltype(r)::kind == ucsl::reflection::providers::TypeKind::BITFIELD) { BitfieldAccessor<decltype(r)> acc{ this->value, r }; return f(acc); }
					else if constexpr (decltype(r)::kind == ucsl::reflection::providers::TypeKind::ARRAY) { ArrayAccessor<ucsl::containers::arrays::Array, decltype(r)> acc{ this->value, r }; return f(acc); }
					else if constexpr (decltype(r)::kind == ucsl::reflection::providers::TypeKind::TARRAY) { ArrayAccessor<ucsl::containers::arrays::TArray, decltype(r)> acc{ this->value, r }; return f(acc); }
					else if constexpr (decltype(r)::kind == ucsl::reflection::providers::TypeKind::POINTER) { PointerAccessor<decltype(r)> acc{ this->value, r }; return f(acc); }
					else if constexpr (decltype(r)::kind == ucsl::reflection::providers::TypeKind::CARRAY) { CArrayAccessor<decltype(r)> acc{ this->value, r }; return f(acc); }
					else if constexpr (decltype(r)::kind == ucsl::reflection::providers::TypeKind::UNION) { UnionAccessor<decltype(r)> acc{ this->value, r }; return f(acc); }
					else if constexpr (decltype(r)::kind == ucsl::reflection::providers::TypeKind::STRUCTURE) { StructureAccessor<decltype(r)> acc{ this->value, r }; return f(acc); }
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
