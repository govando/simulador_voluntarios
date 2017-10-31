#ifndef _AGENT_H_
#define _AGENT_H_

#include "glob.h"
#include "utils.h"

class Agent 
{
	private:
		std::string _name;
		bool _activate;
		uint64_t _simulator_time;
		//tiempos de simulacion
		int _event;
		bool _interrupt_hold;
  	
	public:
	    virtual void run(const uint64_t&) = 0;
	    virtual void events_handler(void) = 0;

	public:
		Agent();
	    Agent(const std::string &);
	    ~Agent(void);  
	    std::string name(void);
	    bool isActivated(void);
		void activate(void);
		void pasivate(void);
		std::string toString(void);
		void setSimulatorTime(const uint64_t&);
		uint64_t getSimulatorTime();
		void setEvent(const int &);
		int getEvent(void); 
		bool isIdle(void);
		void hold(const uint64_t &);
		std::string simtimeToString(void);
		void interruptHoldNow(void);


};
#endif
