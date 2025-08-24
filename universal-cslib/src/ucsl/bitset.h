#pragma once

namespace ucsl::bits {
#ifdef EXPORTING_TYPES
	template <typename T, typename U = T>
#else
	template <typename T, bool isenum> struct bitset_type {};
	template <typename T> struct bitset_type<T, true> { using type = std::underlying_type_t<T>; };
	template <typename T> struct bitset_type<T, false> { using type = T; };

	template <typename T, typename U = typename bitset_type<T, std::is_enum<T>::value>::type>
#endif
	class Bitset
	{
	public:
		typedef U underlying_type;

		U bits{};
		
		Bitset() {}
		Bitset(U value) : bits(value){}
		template<typename... TArgs> constexpr Bitset(TArgs... bits) { (set(bits), ...); }
		
		constexpr U value() const { return bits; }
		constexpr operator U() const { return bits; }
		constexpr void reset() { bits = U{}; }
		constexpr bool test(T bit) const { return bits & (static_cast<U>(1) << static_cast<U>(bit)); }
		constexpr void flip(T bit) { set(bit, !test(bit)); }
		constexpr void reset(T bit) { bits = bits & ~(static_cast<U>(1) << static_cast<U>(bit)); }
		constexpr void set(T bit) { bits = bits | (static_cast<U>(1) << static_cast<U>(bit)); }
		constexpr void set(T bit, bool flag) {
			if (flag)
				set(bit);
			else
				reset(bit);
		}
	};
}
