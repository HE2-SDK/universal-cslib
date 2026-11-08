#pragma once
#include <ucsl/containers/arrays/array.h>
#include <ucsl/containers/arrays/tarray.h>

namespace ucsl::reflection {
    static constexpr unsigned long long MOD_ERASED = 0x4552415345443030;
    template<typename Type> struct erased : simplerfl::mod<MOD_ERASED> {
        using type = simplerfl::resolve_decl_t<Type>;
    };

    static constexpr unsigned long long MOD_WEAK = 0x5745414b30303030;
    template<typename Type> struct weak : simplerfl::mod<MOD_WEAK> {
        using type = simplerfl::resolve_decl_t<Type>;
    };

    //static constexpr unsigned long long MOD_PRIORITY = 0x5052494f52495459;
    //template<char value, typename Type> struct priority : simplerfl::mod<MOD_PRIORITY> {
    //    using type = simplerfl::resolve_decl_t<Type>;
    //    static constexpr char value = value;
    //};

    static constexpr unsigned long long DESCTYPE_CONSTANT = 0x434f4e5354414e54;
    template<typename Repr, Repr value_> struct constant : simplerfl::decl<DESCTYPE_CONSTANT> {
        using repr = Repr;
        static constexpr Repr value = value_;
    };

    static constexpr unsigned long long DESCTYPE_RFLCLASS = 0x52464c434c415353;
    template<typename Resolver> struct rflclass : simplerfl::decl<DESCTYPE_RFLCLASS> {
        using resolver = Resolver;
    };

    static constexpr unsigned long long DESCTYPE_SPAWNER_DATA_RFLCLASS = 0x5350574e44415441;
    template<typename Resolver> struct spawner_data_rflclass : simplerfl::decl<DESCTYPE_SPAWNER_DATA_RFLCLASS> {
        using resolver = Resolver;
    };

    static constexpr unsigned long long DESCTYPE_COMPONENT_DATA = 0x434f4d5044415441;
    template<typename Resolver> struct component_data_rflclass : simplerfl::decl<DESCTYPE_COMPONENT_DATA> {
        using resolver = Resolver;
    };

    static constexpr unsigned long long DESCTYPE_ARRAY = 0x4152524159303030;
    template<typename Type, typename AllocatorSystem> struct array : simplerfl::decl<DESCTYPE_ARRAY> {
        using type = simplerfl::resolve_decl_t<Type>;
        using allocator_system = AllocatorSystem;
    };

    static constexpr unsigned long long DESCTYPE_TARRAY = 0x5441525241593030;
    template<typename Type, typename AllocatorSystem> struct tarray : simplerfl::decl<DESCTYPE_TARRAY> {
        using type = simplerfl::resolve_decl_t<Type>;
        using allocator_system = AllocatorSystem;
    };

    //static constexpr unsigned long long DESCTYPE_OFFSET = 0x4f46465345543030;
    //template<typename AddrRepr, typename Type> struct offset : simplerfl::decl<DESCTYPE_OFFSET> {
    //    using type = simplerfl::resolve_decl_t<Type>;
    //    using repr = AddrRepr;
    //};

    //static constexpr unsigned long long DESCTYPE_STRING_OFFSET = 0x4f46465345545354;
    //template<typename AddrRepr> struct offset : simplerfl::decl<DESCTYPE_STRING_OFFSET> {
    //    using repr = AddrRepr;
    //};

    static constexpr unsigned long long RESOLVER_TYPE_CUSTOM = 0x4143434553534F52;
    template<typename Result, typename F> struct custom_resolver : simplerfl::resolver<RESOLVER_TYPE_CUSTOM, Result> {
        using f = F;
    };

    //template<typename Type> struct is_erased { static constexpr bool value = is_erased<Type::type>::value; };
    //template<typename T> struct is_erased<simplerfl::primitive<T>> { static constexpr bool value = false; };
    //template<typename Repr, Repr value> struct is_erased<constant<Repr, value>> { static constexpr bool value = false; };
    //template<typename Repr, simplerfl::strlit name, typename Underlying, typename... Options> struct is_erased<simplerfl::enumeration<Repr, name, Underlying, Options...>> { static constexpr bool value = false; };
    //template<typename T> struct is_erased<simplerfl::pointer<T>> { static constexpr bool value = false; };
    //template<typename Repr, simplerfl::strlit name, typename Base, typename... Fields> struct is_erased<simplerfl::structure<Repr, name, Base, Fields...>> { static constexpr bool value = false; };
    //template<typename Repr, simplerfl::strlit name, typename Parent, simplerfl::union_resolver<Parent> resolver, typename... Fields> struct is_erased<simplerfl::unionof<Repr, name, Parent, resolver, Fields...>> { static constexpr bool value = false; };
    //template<typename Parent, rflclass_resolver<Parent> resolver> struct is_erased<rflclass<Parent, resolver>> { static constexpr bool value = false; };
    //template<typename Type> struct is_erased<erased<Type>> { static constexpr bool value = true; };
    template<typename Type> static constexpr bool is_erased_v = simplerfl::has_modifier_v<MOD_ERASED, Type>;
    template<typename Type> static constexpr bool is_weak_v = simplerfl::has_modifier_v<MOD_WEAK, Type>;
    //template<typename Type> struct get_priority { static constexpr char value = 0; };
    //template<char value, typename Type> struct get_priority<priority<value, Type>> { static constexpr char value = value; };
    //template<typename Type> static constexpr char get_priority_v = get_priority<Type>::value;

    template<size_t size> using strbuf = simplerfl::primitive<char[size]>;
}

namespace simplerfl {
    template<typename Repr, Repr value> struct is_realigned<ucsl::reflection::constant<Repr, value>> { static constexpr bool value = false; };
    template<typename Resolver> struct is_realigned<ucsl::reflection::rflclass<Resolver>> { static constexpr bool value = false; };
    template<typename Resolver> struct is_realigned<ucsl::reflection::spawner_data_rflclass<Resolver>> { static constexpr bool value = false; };
    template<typename Resolver> struct is_realigned<ucsl::reflection::component_data_rflclass<Resolver>> { static constexpr bool value = false; };
    template<typename Type, typename AllocatorSystem> struct is_realigned<ucsl::reflection::array<Type, AllocatorSystem>> { static constexpr bool value = false; };
    template<typename Type, typename AllocatorSystem> struct is_realigned<ucsl::reflection::tarray<Type, AllocatorSystem>> { static constexpr bool value = false; };
    //template<typename AddrRepr, typename Type> struct is_realigned<ucsl::reflection::offset<AddrRepr, Type>> { static constexpr bool value = false; };

    template<typename Repr, long long value> struct representation<ucsl::reflection::constant<Repr, value>> { using type = Repr; };
    template<typename Resolver> struct representation<ucsl::reflection::rflclass<Resolver>> { static_assert("Cannot get representation of dynamic type."); using type = void; };
    template<typename Resolver> struct representation<ucsl::reflection::spawner_data_rflclass<Resolver>> { static_assert("Cannot get representation of dynamic type."); using type = void; };
    template<typename Resolver> struct representation<ucsl::reflection::component_data_rflclass<Resolver>> { static_assert("Cannot get representation of dynamic type."); using type = void; };
    template<typename Type, typename AllocatorSystem> struct representation<ucsl::reflection::array<Type, AllocatorSystem>> { using type = ucsl::containers::arrays::Array<typename representation<typename ucsl::reflection::array<Type, AllocatorSystem>::type>::type, AllocatorSystem>; };
    template<typename Type, typename AllocatorSystem> struct representation<ucsl::reflection::tarray<Type, AllocatorSystem>> { using type = ucsl::containers::arrays::TArray<typename representation<typename ucsl::reflection::tarray<Type, AllocatorSystem>::type>::type, AllocatorSystem>; };
    //template<typename AddrRepr, typename Type> struct representation<ucsl::reflection::offset<AddrRepr, Type>> { using type = AddrRepr; };
}
