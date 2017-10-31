#ifndef _ADMINISTRATOR_H_
#define _ADMINISTRATOR_H_

#include "glob.h"
#include "user.h"
#include "curlhttp.h"
#include "emergency.h"
#include "mission.h"
#include "volunteer.h"
#include "utils.h"

extern std::string host;
extern std::string port;
extern uint32_t nbr_emergencies;
extern uint32_t nbr_missions;
extern uint32_t nbr_triesOfRequest;
extern uint32_t nbr_nsSleepByFailedTryRequest;
extern uint32_t nbr_volunteersByMission;
extern json emergencies;
extern json missions;
extern json places;
extern json abilities;
extern json emergencies_types;
extern std::mt19937 rn_generator;
extern json millisEventAdm;

extern User ** users;
extern std::map<uint32_t,uint32_t>userIDs; 

enum EVENT_ADMINISTRATOR{
	CREATE_EMERGENCY=1U,
	CREATE_MISSION
};

class Administrator : public User 
{
	private: 
		uint32_t cnt_emergencies;
		uint32_t cnt_missions;

	public:
		Administrator();
	    Administrator(const std::string &,const uint32_t &,const std::string &,const std::string &);
	    ~Administrator(void);
 		void run(const uint64_t&) ;
 		std::string toString(void);
 		json emergency_template(void);
 		json mission_template(void);
 		json history_mission_template(void);
 		Emergency * create_emergency();
 		Mission * create_mission();
 		void events_handler(void);


};

#endif