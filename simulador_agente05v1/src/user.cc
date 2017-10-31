#include "../include/user.h"

User::User(const std::string &name,const uint32_t &id,const std::string &username,
	      const std::string &password) : Agent(name) {

	this->_id=id;
	this->_username=username;
	this->_password=password;
}

User::User() : Agent() {
    ;
}


User::~User() {
    ;
}



std::string User::toString()
{
	char txt[200];
	std::string patern(Agent::toString());
	sprintf(txt,"{'User':{'id':%d,'username':'%s','password':'%s','parent':%s}}",
		this->_id,this->_username.c_str(),this->_password.c_str(),patern.c_str());
	return txt;
}


uint32_t User::getId(void){
	return this->_id;
}
	    
std::string User::getUsername(void){
	return this->_username;
}


std::string User::getPassword(void){
	return this->_password;
}