#pragma once

#include "system.h"
#include <unordered_map>

//Event ID system, copied from components
typedef uint64_t EventID;

EventID generate_event_id();

template <class EventType> 
EventID get_event_id(){
	static bool initialized = false;
	static EventID id;

	if(!initialized){
		id = generate_event_id();
		initialized = true;
	}

	return id;
}

//Dummy event class to base events on
struct Event{};

//Function pointer wrapper
class EventHandlerWrapperBase{
    public:
        virtual void call(Event* event) = 0;
};

template<class T, class EventType>
class EventHandlerWrapper: public EventHandlerWrapperBase{

    public:
        typedef void(T::*HandlerFunction)(EventType*);

        void call(Event* event){
            (instance->*handler)(static_cast<EventType*>(event));
        }

        EventHandlerWrapper(T* instance, HandlerFunction handler): instance{instance}, handler{handler}{};

    private:
        T* instance;
        HandlerFunction handler;
};

//Event passer class
class EventPasser{

    public:

        template<class T, class EventType>
        void subscribe(T* system, void(T::*handler)(EventType*)){
            EventID id = get_event_id<EventType>();
            EventHandlerWrapperBase* wrapper = (EventHandlerWrapperBase*) new EventHandlerWrapper(system, handler);
            subscriber_map.insert(std::pair<EventID, EventHandlerWrapperBase*>(id, wrapper));
        }

        template<class EventType>
        void broadcast(EventType* event){
            
            //Iterate through subscribed systems and call their functions
            auto range = subscriber_map.equal_range(get_event_id<EventType>());
			for(auto it = range.first; it != range.second; ++it){
                it->second->call(event);
			}

            delete event;
        }

    private:
        std::unordered_multimap<EventID, EventHandlerWrapperBase*> subscriber_map;

};

//Needs to:
//Subscribe system to events (shouldn't need to unsubscribe)
//Broadcast events to subscribed systems