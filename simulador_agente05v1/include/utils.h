#ifndef _UTILS_H_
#define _UTILS_H_

#include "glob.h"

extern std::mt19937 rn_generator;

class Utils{

public:
	Utils();
	~Utils();
	static json random_abilities(const json &);
    static json random_emergency_type(const json &);
    static json abilities_to_jsonArrayWithDescrip(const json &);
    static json random_missionByEmType(const int &,const json &);
    static uint64_t currentTimestamp_to_milliseconds();
    static time_t milliseconds_to_timestamp(const uint64_t &);
    static std::string milliseconds_to_timestampString(const uint64_t &);
    static uint64_t rand_uniform(const uint64_t &,const uint64_t &);


};

#endif