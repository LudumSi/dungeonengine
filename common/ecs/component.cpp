#include "component.h"

CompID Component::generate_comp_id(){
	static CompID comp_id_idx;
	static bool idx_initialized = false;

	if(!idx_initialized){
		comp_id_idx = 0;
		idx_initialized = true;
	}

	comp_id_idx++;

	return comp_id_idx;
}

Component::Component(){
	this->comp_id = this->generate_comp_id();
}