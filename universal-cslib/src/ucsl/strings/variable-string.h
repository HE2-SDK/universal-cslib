#pragma once
#include "string-ops.h"

namespace ucsl::strings {
	class VariableString
	{
	protected:
		static constexpr size_t DEALLOCATE_FLAG = 1ull;
		static constexpr size_t ALLOCATOR_MASK = ~DEALLOCATE_FLAG;

		const char* buffer{};
		memory::IAllocator* allocatorAndFlags{};

		inline bool isFree() const
		{
			return buffer == nullptr;
		}

		bool ShouldDeallocate() const {
			return reinterpret_cast<size_t>(allocatorAndFlags) & DEALLOCATE_FLAG;
		}

		void init(const char* initstr, memory::IAllocator* allocator) {
			if (initstr) {
				buffer = StrDup(initstr, allocator);
				allocatorAndFlags = reinterpret_cast<memory::IAllocator*>(reinterpret_cast<size_t>(allocator) | DEALLOCATE_FLAG);
			}
			else {
				buffer = nullptr;
				allocatorAndFlags = allocator;
			}
		}

		void assign(memory::IAllocator* allocator, const char* str, int size) {
			if (buffer != nullptr && buffer == str)
				return;

			if (ShouldDeallocate())
				GetAllocator()->Free((void*)buffer);

			if (str) {
				if (size == -1)
					buffer = StrDup(str, allocator);
				else
					buffer = StrNdup(str, size, allocator);

				allocatorAndFlags = reinterpret_cast<memory::IAllocator*>(reinterpret_cast<size_t>(allocator) | DEALLOCATE_FLAG);
			}
			else {
				buffer = nullptr;
				allocatorAndFlags = allocator;
			}
		}

	public:
		VariableString(memory::IAllocator* allocator) : allocatorAndFlags{ allocator } {}

		VariableString(const char* initstr) : buffer{ initstr } {}

		VariableString(const char* initstr, memory::IAllocator* allocator) {
			init(initstr, allocator);
		}

		VariableString(const char* str, int size, memory::IAllocator* allocator) {
			assign(allocator, str, size);
		}

		VariableString(const VariableString& other) {
			if (!other.GetAllocator()) {
				buffer = other.buffer;
				return;
			}
			else {
				init(other.buffer, other.GetAllocator());
			}
		}

		VariableString(VariableString&& other) : buffer{ other.buffer }, allocatorAndFlags{ other.allocatorAndFlags } {
			other.buffer = nullptr;
			other.allocatorAndFlags = GetAllocator();
		}

		~VariableString() {
			if (ShouldDeallocate())
				GetAllocator()->Free((void*)buffer);

			allocatorAndFlags = nullptr;
			buffer = nullptr;
		}

		memory::IAllocator* GetAllocator() const {
			return reinterpret_cast<memory::IAllocator*>(reinterpret_cast<size_t>(allocatorAndFlags) & ALLOCATOR_MASK);
		}

		VariableString& operator=(VariableString&& other) {
			if (this == &other)
				return *this;

			this->~VariableString();

			buffer = other.buffer;
			allocatorAndFlags = other.allocatorAndFlags;
			other.allocatorAndFlags = other.GetAllocator();
			other.buffer = nullptr;

			return *this;
		}

		void SetDataUserFree(memory::IAllocator* allocator, const char* str) {
			buffer = str;
			allocatorAndFlags = allocator;
		}

		inline void Set(const char* str, int size, memory::IAllocator* allocator) {
			assign(allocator, str, size);
		}

		inline void Set(const char* str, int size) {
			assign(GetAllocator(), str, size);
		}

		inline void Set(const char* str) {
			assign(GetAllocator(), str, -1);
		}

		const char* c_str() const {
		 	if (buffer)
		 		return buffer;

		 	return "";
		}

		bool Compare(const char* str) const {
			return strcmp(c_str(), str);
		}

		operator const char*() const {
			return c_str();
		}

		VariableString& operator=(const VariableString& str) {
			return operator=(str.c_str());
		}

		VariableString& operator=(const char* str) {
			Set(str);
			return *this;
		}

		friend bool operator==(const VariableString& lhs, const VariableString& rhs) {
			return lhs.Compare(rhs.c_str());
		}

		friend bool operator==(const VariableString& lhs, const char* rhs) {
			return lhs.Compare(rhs);
		}
	};
}