#ifndef _COORDINATOR_H_
#define _COORDINATOR_H_

#include "glob.h"
#include "agent.h"


class Coordinator : public Agent 
{

	protected:
		void inner_body(void);

	public:
	    Coordinator(const std::string &,const json&);
	    ~Coordinator(void);


};

#endif

