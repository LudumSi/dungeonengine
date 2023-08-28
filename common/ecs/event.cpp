#include "event.h"

EventID generate_event_id(){
	static EventID next_id = 0;
	next_id++;
	return next_id;
}