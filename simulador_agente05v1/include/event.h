#ifndef _EVENT_H_
#define _EVENT_H_

#include "glob.h"

enum EventType{ 
          CREATE_EMERGENCY=1U,
          CREATE_MISSION,
          LOGIN=1U,
          LOGOUT,
          REVIEW_MISSIONS_INVITATIONS,
          SELECT_MISSION_INVITATION, //ACEPT - REJECT
          ACEPT_MISSION_INVITATION,
          REJECT_MISSION_INVITATION,
          REVIEW_MISSIONS_ACTIVES,
          SELECT_MISSION_ACTIVE,  //BEGIN - FINALIZE
          BEGIN_MISSION_ACEPTED,
          FINALIZE_MISSION_ACEPTED,
          DO_NOTHING,
          FINALIZE_EVENT_VOLUNTEER 
          };

class Event{
private:
  uint64_t 	_timestamp;
  EventType _type;
  uint32_t	_src;
  uint32_t 	_dst;
  json      _params;

public:
  Event();
  Event(const uint64_t&,const EventType&,const uint32_t&,const uint32_t&,const json&);
  ~Event();
				
  uint64_t timestamp(void) const;
  EventType type(void) const;
  uint32_t src(void) const;
  uint32_t dst(void) const;
  json params(void);

  void timestamp(const uint64_t&);
  void params(const json&);
};
#endif
