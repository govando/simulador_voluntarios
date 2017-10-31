#include "../include/mission.h"

Mission::Mission(const uint32_t & id,const uint32_t & emergency_id,const uint32_t & user_id,
				const std::string & title){

	this->_id = id;
	this->_emergency_id = emergency_id;
	this->_user_id = user_id;
	this->_title = title;


}

Mission::~Mission() {
    ;
}


std::string Mission::toString(void){
		char buffer[200];
		sprintf(buffer,"{'id':%d,'id_emergency':%d,'user_id':%d,'title':'%s'}",
			      this->_id,this->_emergency_id,this->_user_id,this->_title.c_str());
		return buffer; 	
}