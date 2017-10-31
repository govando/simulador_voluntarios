#include "../include/emergency.h"


Emergency::Emergency(const uint32_t & id,const uint32_t & user_id,const std::string & title){

	this->_id = id;
	this->_user_id = user_id;
	this->_title = title;
}

Emergency::~Emergency() {
    ;
}

std::string Emergency::toString(void){
		char buffer[200];
		sprintf(buffer,"{'id':%d,'user_id':%d,'title':'%s'}",this->_id,this->_user_id,this->_title.c_str());
		return buffer; 	
}