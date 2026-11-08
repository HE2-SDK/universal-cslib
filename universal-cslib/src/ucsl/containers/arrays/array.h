#pragma once
#include "array-base.h"

namespace ucsl::containers::arrays {
	template<typename T, typename AllocatorSystem>
	class Array;

	template<typename T, typename AllocatorSystem>
	struct ArrayArrayAllocatorSystem {
		using allocator_type = typename AllocatorSystem::allocator_type;

		inline static allocator_type* get_allocator(const ArrayBase<T, size_t, ArrayArrayAllocatorSystem<T, AllocatorSystem>>& arr) { return static_cast<const Array<T, AllocatorSystem>&>(arr).allocator; }
	};

	template<typename T, typename AllocatorSystem>
	class Array : public ArrayBase<T, size_t, ArrayArrayAllocatorSystem<T, AllocatorSystem>> {
		friend struct ArrayArrayAllocatorSystem<T, AllocatorSystem>;

	protected:
		AllocatorSystem::allocator_type* allocator{};

	public:
		Array() : ArrayBase<T, size_t, ArrayArrayAllocatorSystem<T, AllocatorSystem>>{} {}
		Array(AllocatorSystem::allocator_type* allocator) : allocator{ allocator }, ArrayBase<T, size_t, ArrayArrayAllocatorSystem<T, AllocatorSystem>>{} {}
		Array(AllocatorSystem::allocator_type* allocator, size_t capacity) : allocator{ allocator }, ArrayBase<T, size_t, ArrayArrayAllocatorSystem<T, AllocatorSystem>>{ capacity } {}
		Array(const Array<T, AllocatorSystem>& other) : allocator{ other.allocator }, ArrayBase<T, size_t, ArrayArrayAllocatorSystem<T, AllocatorSystem>>{ other } {}
		Array(Array<T, AllocatorSystem>&& other) : allocator{ other.allocator }, ArrayBase<T, size_t, ArrayArrayAllocatorSystem<T, AllocatorSystem>>{ std::move(other) } {}

		void change_allocator(AllocatorSystem::allocator_type* new_allocator) {
			if (!new_allocator || allocator == new_allocator)
				return;

			if (this->is_deallocation_disabled() || !this->buffer) {
				allocator = new_allocator;
				return;
			}

			T* new_buffer = static_cast<T*>(new_allocator->Alloc(this->capacity() * sizeof(T), 16));

			std::uninitialized_move(this->begin(), this->end(), new_buffer);
			std::destroy(this->begin(), this->end());

			if (allocator && !this->is_deallocation_disabled())
				allocator->Free(this->buffer);

			this->buffer = new_buffer;
			allocator = new_allocator;
		}

		void swap(Array<T, AllocatorSystem>& other) noexcept {
			ArrayBase<T, size_t, ArrayArrayAllocatorSystem<T, AllocatorSystem>>::swap(*this, other);
			std::swap(allocator, other.allocator);
		}
	};

	template<typename T, size_t Len, typename AllocatorSystem>
	class InplaceArray : public Array<T, AllocatorSystem> {
		T reserved[Len];

	public:
		InplaceArray(AllocatorSystem::allocator_type* allocator) : Array<T, AllocatorSystem>{ allocator } {
			this->capacity_and_flags = Len | this->DONT_DEALLOCATE_FLAG;
			this->buffer = reserved;
		}
	};
}
