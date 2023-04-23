#include "component.h"

CompID generate_comp_id(){
	static CompID next_id = 0;

	next_id++;

	return next_id;
}