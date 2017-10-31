#include "../include/agent.h"

Agent::Agent(const std::string &name)  {

	this->_name=name;
	this->_activate=false;
}

Agent::Agent() {
    ;
}

Agent::~Agent() {
    ;
}

std::string Agent::name(void){
	return this->_name;
}

bool Agent::isActivated(void){
	return this->_activate;
}

void Agent::activate(void){
	this->_activate=true;
}

void Agent::pasivate(void){
	this->_activate=false;
}


bool Agent::isIdle(void){
	return 	!this->_activate;
}


std::string Agent::toString()
{
	char txt[200];
	sprintf(txt,"{'Agent':'%s'}",this->_name.c_str());
	return txt;
}

void Agent::setSimulatorTime(const uint64_t& time)
{
	this->_simulator_time = time;
}

uint64_t Agent::getSimulatorTime()
{
	return this->_simulator_time;	
}

void Agent::setEvent(const int & event){
	this->_event=event;
}

int Agent::getEvent(){
	return this->_event;
} 

void Agent::hold(const uint64_t & millis){

	std::chrono::milliseconds ms = std::chrono::milliseconds(millis);
    std::chrono::time_point<std::chrono::system_clock> end;

    end = std::chrono::system_clock::now() + ms; // this is the end point

    _interrupt_hold=false;
    while(std::chrono::system_clock::now() < end && _interrupt_hold==false); 
    _interrupt_hold=false;
}


std::string Agent::simtimeToString(void){
	return Utils::milliseconds_to_timestampString(this->_simulator_time);
}


void Agent::interruptHoldNow()
{
	_interrupt_hold=true;
}