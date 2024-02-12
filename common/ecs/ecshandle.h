#pragma once

#include "comp_registry.h"
#include "system_registry.h"
#include "event.h"

//Handle for systems to access shared ECS resources

class ECSHandle{

    public:
        
        ComponentRegistry* const comp_reg;
        SystemRegistry* const sys_reg;
        EventRegistry* const event_reg;
        
        ECSHandle(ComponentRegistry* comp_reg, SystemRegistry* sys_reg, EventRegistry* event_reg): comp_reg(comp_reg), sys_reg(sys_reg), event_reg(event_reg) {};
        
        template <typename CompType>
        void subscribe_system(System* sys){
            sys_reg->subscribe_system<CompType>(sys);
        }

        template<class T, class EventType>
        void subscribe_event(T* system, void(T::*handler)(EventType*)){
            event_reg->subscribe(system,handler);
        }

        template<class EventType>
        void broadcast(EventType* event){
            event_reg->broadcast(event);
        }

        //Returns a pointer to a component for a given entity
		template <typename CompType>
		CompType* get_component(Entity e) {
			return comp_reg->get_component<CompType>(e);
		}
};