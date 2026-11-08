#include <catch2/catch_test_macros.hpp>
#include <ucsl-reflection/accessors/binary.h>
#include <ucsl-reflection/providers/simplerfl.h>
#include <ucsl-reflection/game-interfaces/standalone/game-interface.h>

using namespace simplerfl;
using namespace std;
using namespace ucsl::reflection;

using AlignedInt = aligned<16, int>;
using AlignedIntInner = AlignedInt::type;
using O = resolve_decl_t<int>;
//using Rep = representation_t<AlignedInt>;

struct Test1Repr {
	float foo;
	float baz;
	alignas(16) int barCount;
	int* bars;
};

using Bars = dynamic_carray<int, field_resolver<size_t, "barCount">>;

using Test1 = structure<Test1Repr, "Test1", void,
	field<float, "foo">,
	field<float, "baz">,
	field<AlignedInt, "barCount">,
	field<Bars, "bars">
>;


using GI = game_interfaces::standalone::StandaloneGameInterface;

//template<accessors::PrimitiveAccessor PA>
//void testPAConceptMock(const PA& sa);
//
//void testPAConcept() {
//	float f;
//	accessors::PrimitiveBinaryAccessor<providers::simplerfl<GI>::Primitive<primitive<float>>> pacc{ &f };
//
//	testPAConceptMock(pacc);
//}

//template<accessors::StructureAccessor SA>
//void testSAConcept(const SA& sa);


TEST_CASE( "fields can be resolved", "providers/simperfl/resolution") {
	int bars[]{ 1, 2, 3, 4, 5 };
	Test1Repr t1{ 4.0f, 5.0f, 5, bars };
	using Refl = providers::simplerfl<GI>::Structure<Test1>;
	accessors::binary<GI>::StructureBinaryAccessor<Refl, Refl, Refl, int, int> acc{ &t1 };

	REQUIRE(providers::simplerfl<GI>::resolve<field_resolver<size_t, "barCount">>(acc) == 5);
	REQUIRE(providers::simplerfl<GI>::dynamic_size_of<Bars>(acc, acc, acc[acc.refl.get_field<"bars">()]) == sizeof(int) * 5);
}