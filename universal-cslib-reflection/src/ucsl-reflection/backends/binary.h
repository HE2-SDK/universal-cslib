#pragma once

namespace ucsl::reflection::backends {
	template<typename GameInterface>
	struct binary {
		struct opaque_obj {};

		template<template<typename, typename> typename A, typename Refl>
		class RflArray : public A<opaque_obj, typename GameInterface::AllocatorSystem> {
		public:
			void change_allocator(Refl refl, GameInterface::AllocatorSystem::allocator_type* new_allocator) {
				if (!new_allocator || this->allocator == new_allocator)
					return;

				if (this->is_deallocation_disabled() || !this->buffer) {
					this->allocator = new_allocator;
					return;
				}

				opaque_obj* new_buffer = static_cast<opaque_obj*>(new_allocator->Alloc(this->capacity() * refl.get_size(), 16));

				move(refl, new_buffer, this->buffer, this->length);

				if (this->allocator && !this->is_deallocation_disabled())
					this->allocator->Free(this->buffer);

				this->buffer = new_buffer;
				this->allocator = new_allocator;
			}

			void reserve(Refl refl, size_t desired_cap) {
				auto* allocator = this->get_allocator();
				auto cap = this->capacity();

				if (desired_cap <= cap)
					return;

				auto newcap = std::max(2 * cap, desired_cap);

				opaque_obj* new_buffer = static_cast<opaque_obj*>(allocator->Alloc(newcap * refl.get_size(), 16));

				if (this->buffer) {
					move(refl, new_buffer, this->buffer, this->length);

					if (!this->is_deallocation_disabled())
						allocator->Free(this->buffer);
				}

				this->buffer = new_buffer;
				this->capacity_and_flags = newcap;
			}

			opaque_obj& emplace_back(Refl refl) {
				this->reserve(refl, this->length + 1);

				opaque_obj& loc = at(this->length, refl);

				construct(refl, &loc);
				//if (member->GetSubType() == GameInterface::RflSystem::RflClassMember::Type::STRUCT)
				//	GameInterface::RflTypeInfoRegistry::GetInstance()->ConstructObject(this->get_allocator(), &at(this->length, member), member->GetClass()->GetName());
				//else
				//	memset(&at(this->length, member), 0, member->GetSubTypeSize());

				this->length++;

				return loc;
			}

			void remove(Refl refl, size_t i) {
				if (i >= this->length)
					return;

				//if (member->GetSubType() == GameInterface::RflSystem::RflClassMember::Type::STRUCT)
				//	GameInterface::RflTypeInfoRegistry::GetInstance()->CleanupLoadedObject(&at(i, member), member->GetName());

				destroy(refl, &at(refl, i));
				move(refl, &at(refl, i), &at(refl, i + 1), this->length - i - 1);

				this->length--;
			}

			void clear(Refl refl) {
				if (this->empty())
					return;

				for (size_t i = 0; i < this->length; i++)
					destroy(refl, &at(refl, i));
					//if (member->GetSubType() == GameInterface::RflSystem::RflClassMember::Type::STRUCT)
					//	GameInterface::RflTypeInfoRegistry::GetInstance()->CleanupLoadedObject(&at(i, member), member->GetName());

				this->length = 0;
			}

			opaque_obj& at(Refl refl, size_t i) {
				return *util::addptr(this->buffer, refl.get_size() * i);
			}
		};

		template<typename T = opaque_obj>
		class BackendBase {
			T& obj;
		};

		template<typename Refl>
		class Value;

		template<typename Refl>
		class Primitive : public BackendBase<typename Refl::repr> {
		public:
			using BackendBase<typename Refl::repr>::BackendBase;
			
			Primitive<Refl>& operator=(typename Refl::repr v) { this->obj = v; return *this; }
			operator typename Refl::repr() const { return this->obj; }
		};

		template<template<typename> typename A, typename Refl>
		class ArrayBase : public BackendBase<RflArray<A, Refl>> {
		public:
			using BackendBase<RflArray<A, Refl>>::BackendBase;
			
			//GameInterface::AllocatorSystem::allocator_type* get_allocator() const { return this->obj.get_allocator(); }
			//void change_allocator(GameInterface::AllocatorSystem::allocator_type* new_allocator) { this->obj.change_allocator(this->refl, new_allocator); }
			void reserve(size_t len) { this->obj.reserve(this->refl, len); }

			auto emplace_back() { return Value{ this->obj.emplace_back(this->refl), this->refl->get_item_type() }; }
			void remove(size_t i) { this->obj.remove(this->refl, i); }
			void clear() { this->obj.clear(this->refl); }

			auto operator[](size_t i) { return Value{ this->obj.at(this->refl, i), this->refl->get_item_type() }; }
			const auto operator[](size_t i) const { return Value{ this->obj.at(this->refl, i), this->refl->get_item_type() }; }

			size_t size() const { return this->obj.size(); }	
			size_t capacity() const { return this->obj.capacity(); }
		};
		
		template<typename Refl>
		class Array : public ArrayBase<ucsl::arrays::Array, Refl> {
		public:
			using ArrayBase<ucsl::arrays::Array, Refl>::ArrayBase;
		};
		
		template<typename Refl>
		class TArray : public ArrayBase<ucsl::arrays::TArray, Refl> {
		public:
			using ArrayBase<ucsl::arrays::TArray, Refl>::ArrayBase;
		};

		template<typename Refl>
		class CArray : public BackendBase<> {
		public:
			inline auto operator[](size_t idx) {
				auto item_refl = ;

				return Value{ util::addptr(&this->obj, idx * item_refl.get_size(*this)), this->refl.get_item_type() };
			}

			inline const auto operator[](size_t idx) const {
				auto item_refl = ;

				return Value{ util::addptr(&this->obj, idx * item_refl.get_size(*this)), this->refl.get_item_type() };
			}
		};
		
		template<typename Refl>
		class Value : public BackendBase<Refl> {
		public:
			using BackendBase<Refl>::BackendBase;

			auto as_array()
		};
	};
}