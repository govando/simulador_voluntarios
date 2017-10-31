#ifndef _MISSION_H_
#define _MISSION_H_

#include "glob.h"

class Mission {

	public:
    	uint32_t _id;
    	uint32_t _emergency_id;
    	uint32_t _user_id;
    	std::string _title; 

	public:
    	Mission(const uint32_t &,const uint32_t &,const uint32_t &,const std::string &);
    	~Mission(void);
    	std::string toString(void);
};
#endif
