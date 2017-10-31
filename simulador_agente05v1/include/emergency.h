#ifndef _EMERGENCY_H_
#define _EMERGENCY_H_

#include "glob.h"

class Emergency {

	public:
    	uint32_t _id;
    	uint32_t _user_id;
    	std::string _title; 

	public:
    	Emergency(const uint32_t &,const uint32_t &,const std::string &);
    	~Emergency(void);
    	std::string toString(void);

};
#endif
