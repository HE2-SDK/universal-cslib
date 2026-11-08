#pragma once
#include <ucsl/resources/sobj/v1.h>
#include <simple-reflection/simple-reflection.h>
#include <ucsl-reflection/operators.h>
#include <ucsl-reflection/reflections/basic-types.h>

namespace ucsl::resources::sobj::v1::reflections {
	using namespace simplerfl;
	using namespace ucsl::reflection;

	namespace impl = ucsl::resources::sobj::v1;

	using ObjectTransformData = structure<impl::ObjectTransformData, "ObjectTransformData", void,
		field<math::Position, "position">,
		field<math::Position, "rotation">
	>;

	struct get_object_type {
		inline std::string operator()(const auto& parent, const auto& root) {
			auto objectAddr = &parent;
			auto objectId = static_cast<ucsl::objectids::ObjectIdV1>(parent.template get_field<"id">().as_primitive().template as<ucsl::objectids::ObjectIdV1>());
			auto objects = (*root.template get_field<"objects">().as_pointer()).as_carray();

			unsigned short objIndex{};
			for (const auto& object : objects) {
				if (static_cast<size_t>(object.as_pointer().get()) == static_cast<size_t>(objectAddr))
					break;

				objIndex++;
			}

			auto objectTypes = (*root.template get_field<"objectTypes">().as_pointer()).as_carray();

			for (const auto& type_ : objectTypes) {
				auto type = type_.as_structure();
				auto objectIndices = (*type.template get_field<"objectIndices">().as_pointer()).as_carray();

				for (const auto& index : objectIndices)
					if (static_cast<unsigned short>(index.as_primitive().template as<unsigned short>()) == objIndex)
						return static_cast<std::string>(type.template get_field<"name">().as_primitive().template as<const char*>());
			}

			assert(false && "object type not found");

			return "";
		}
	};

	template<typename AllocatorSystem>
	using ObjectData = structure<impl::ObjectData<AllocatorSystem>, "ObjectData", void,
		field<ucsl::objectids::ObjectIdV1, "id">,
		field<unsigned int, "objectClassId">,
		field<unsigned int, "bvhNode">,
		field<float, "replicationInterval">,
		field<float, "m_distance">,
		field<float, "m_range">,
		field<containers::arrays::TArray<ObjectTransformData, AllocatorSystem>, "instances">,
		field<aligned<1, spawner_data_rflclass<custom_resolver<std::string, get_object_type>>>, "spawnerData">
	>;

	using ObjectTypeData = structure<impl::ObjectTypeData, "ObjectTypeData", void,
		field<const char*, "name">,
		field<unsigned int, "objectIndexCount">,
		field<dynamic_carray<unsigned short, field_resolver<unsigned int, "objectIndexCount">>*, "objectIndices">
	>;

	template<typename AllocatorSystem>
	using SetObjectData = structure<impl::SetObjectData<AllocatorSystem>, "SetObjectData", void,
		field<constant<unsigned int, 0x534F424A>, "magic">,
		field<constant<unsigned int, 1>, "version">,
		field<unsigned int, "objectTypeCount">,
		field<dynamic_carray<ObjectTypeData, field_resolver<unsigned int, "objectTypeCount">>*, "objectTypes">,
		field<int, "bvh">,
		field<dynamic_carray<ObjectData<AllocatorSystem>*, field_resolver<unsigned int, "objectCount">>*, "objects">,
		field<unsigned int, "objectCount">,
		field<unsigned int, "bvhNodeCount">,
		field<unsigned int, "objectInstanceCount">
	>;
}

namespace simplerfl {
	template<> struct canonical<ucsl::resources::sobj::v1::ObjectTransformData> { using type = ucsl::resources::sobj::v1::reflections::ObjectTransformData; };
	template<typename AllocatorSystem> struct canonical<ucsl::resources::sobj::v1::ObjectData<AllocatorSystem>> { using type = ucsl::resources::sobj::v1::reflections::ObjectData<AllocatorSystem>; };
	template<> struct canonical<ucsl::resources::sobj::v1::ObjectTypeData> { using type = ucsl::resources::sobj::v1::reflections::ObjectTypeData; };
	template<typename AllocatorSystem> struct canonical<ucsl::resources::sobj::v1::SetObjectData<AllocatorSystem>> { using type = ucsl::resources::sobj::v1::reflections::SetObjectData<AllocatorSystem>; };
}
