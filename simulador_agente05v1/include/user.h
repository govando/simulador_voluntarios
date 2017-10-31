#ifndef _USER_H_
#define _USER_H_

#include "glob.h"
#include "agent.h"

class User : public Agent  
{
	public:
		uint32_t _id;
		std::string _username;
		std::string _password;

	public:
	    virtual void run(const uint64_t&) = 0;
	    virtual void events_handler(void) = 0;

	public:
		User();
	    User(const std::string &,const uint32_t &,const std::string &,const std::string &);
	    ~User(void);  
	    std::string toString();
	    uint32_t getId(void);
	    std::string getUsername(void);
	    std::string getPassword(void);

};
#endif
