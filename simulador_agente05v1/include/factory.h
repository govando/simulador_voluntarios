#ifndef _FACTORY_H_
#define _FACTORY_H_

#include "glob.h"
#include "administrator.h"
//#include "coordinator.h"
#include "volunteer.h"
#include "curlhttp.h"
#include "utils.h"


extern std::string host;
extern std::string port;
extern json abilities;
extern uint32_t nbr_triesOfRequest;
extern uint32_t nbr_nsSleepByFailedTryRequest;

class Factory {

	public: 

		static const uint32_t ADMINISTRATOR=1;
		static const uint32_t COORDINATOR=2;
		static const uint32_t VOLUNTEER=3;

		CurlHTTP curlhttp;	
		Utils utils;

	public:
	    Factory( );
		~Factory( );

		static Administrator *  create_administrator(const uint32_t& );
		//Coordinator * create_coordinator(const uint32_t& , json &);
		static Volunteer * create_volunteer(const uint32_t&);
	    static json get_abilities(void);
	    static json get_emergencies_types(void);
	    static void delete_db(void);
	    static json user_template(void);
	    static json volunteer_template(void);
	    static void create_ability(const uint32_t&);
};
#endif
