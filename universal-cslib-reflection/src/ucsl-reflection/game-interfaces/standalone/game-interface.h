#pragma once
#include <cstdlib>
#include <iostream>
#include <ucsl/memory/iallocator.h>
#include <ucsl/containers/arrays/tarray.h>
#include <ucsl/rfl/rflclass.h>
#include "rflsystem.h"
#include "reflection-db.h"

namespace ucsl::reflection::game_interfaces::standalone {
	struct StandaloneGameInterface {
		class NativeCAllocator : public memory::IAllocator {
		public:
			void* Alloc(size_t size, size_t alignment) override { return _aligned_malloc(size, alignment); }
			void Free(void* ptr) override { _aligned_free(ptr); }
		};

		inline static NativeCAllocator nativeCAllocator{};

		struct AllocatorSystem {
			using allocator_type = memory::IAllocator;

			inline static memory::IAllocator* get_allocator() { return &nativeCAllocator; }
		};

		using RflSystem = StandaloneRflSystem;

		using GameObjectClass = GameObjectClass;

		inline static ReflectionDB<RflSystem>* reflectionDB{};

		class RflClassNameRegistry {
		public:
			inline static RflClassNameRegistry* instance{};

			inline static RflClassNameRegistry* GetInstance() { return instance; }

			inline static const typename RflSystem::RflClass* GetClassByName(const char* name) {
				return &*reflectionDB->rflClasses.at(name);
			}
		};

		class RflTypeInfoRegistry {
		public:
			inline static RflTypeInfoRegistry* instance{};

			inline static RflTypeInfoRegistry* GetInstance() { return instance; }

			inline static void* ConstructObject(memory::IAllocator* allocator, void* instance, const char* rflClassName) {
				return instance;
			}

			inline static void CleanupLoadedObject(void* instance, const char* rflClassName) {
			}
		};

		class GameObjectRegistry {
		public:
			inline static const GameObjectClass* GetGameObjectClassByName(const char* name) {
				return &reflectionDB->gameObjectClasses.at(name);
			}
		};

		class GOComponentRegistry {
		public:
			inline static const GOComponentRegistryItem* GetComponentInformationByName(const char* name) {
				return &reflectionDB->componentRegistryItems.at(name);
			}
		};

		class GameObjectSystem {
			GameObjectRegistry gameObjectRegistryInstance{};
			GOComponentRegistry goComponentRegistryInstance{};

		public:
			inline static GameObjectSystem* instance{};
			GameObjectRegistry* gameObjectRegistry{ &gameObjectRegistryInstance };
			GOComponentRegistry* goComponentRegistry{ &goComponentRegistryInstance };

			inline static GameObjectSystem* GetInstance() {
				return instance;
			}
		};

		inline static void boot() {
			reflectionDB = new ReflectionDB<RflSystem>{};
			RflClassNameRegistry::instance = new RflClassNameRegistry{};
			RflTypeInfoRegistry::instance = new RflTypeInfoRegistry{};
			GameObjectSystem::instance = new GameObjectSystem{};
		}
	};
}
