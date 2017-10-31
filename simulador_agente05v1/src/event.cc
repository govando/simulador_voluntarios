#include "../include/event.h"

Event::Event() {
    ;
}

Event::Event(const uint64_t &_timestamp,const EventType &_type,const uint32_t &_src,const uint32_t &_dst,const json &_params) {
    this->_timestamp=_timestamp;
    this->_type=_type;
    this->_src=_src;
    this->_dst=_dst;
    this->_params=_params;
}

Event::~Event() {
    ;
}
uint64_t Event::timestamp(void) const {
    return(this->_timestamp);
}

EventType Event::type(void) const {
    return(this->_type);
}
uint32_t Event::src(void) const {
    return(this->_src);
}
uint32_t Event::dst(void) const {
    return(this->_dst);
}
void Event::timestamp(const uint64_t &_timestamp) {
    this->_timestamp=_timestamp;
}
json Event::params(void) {
    return(this->_params);
}
void Event::params(const json &_params){
	this->_params=_params;
}