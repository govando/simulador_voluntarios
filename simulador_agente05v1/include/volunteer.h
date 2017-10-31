#ifndef _VOLUNTEER_H_
#define _VOLUNTEER_H_

#include "glob.h"
#include "user.h"
#include "curlhttp.h"

extern std::string host;
extern std::string port;
extern std::mt19937 rn_generator;
extern json millisEventVol;

enum EVENT_VOLUNTEER{
	TURN_ON_DEVICE=1U,
	LOGIN,
	LOGOUT,
	LOOK_INVITATION_NOTIFICATION,
	REVIEW_MISSIONS_INVITATIONS,
	SELECT_MISSION_INVITATION, //ACEPT - REJECT
	ACEPT_MISSION_INVITATION,
	REJECT_MISSION_INVITATION,
	REVIEW_MISSIONS_ACTIVES,
	SELECT_MISSION_ACTIVE,  //BEGIN - FINALIZE
	BEGIN_MISSION_ACEPTED,
	FINALIZE_MISSION_ACEPTED,
	DO_NOTHING_ONLY_MOVE,
	TURN_OFF_DEVICE
};

enum STATUS_DEVICE{
	TURNED_ON=1U,
	TURNED_OFF
};

class Volunteer : public User 
{
	public:
		uint32_t _idVol;
		std::string _token;
		std::string _invitation_notification;
		json _missions_invitations;
		json _mission_invitation_selected;	
		json _missions_actives;
		json _mission_active_selected;
		int _status_device;


	public:
	    Volunteer(const std::string &,const uint32_t &,const std::string &,const std::string &,const uint32_t &);
	    ~Volunteer(void);
 		void run(const uint64_t&) ;
 		std::string toString(void);
 		void login(void);
		void logout(void);
		void review_missions_invitations(void);
		void select_mission_invitation(void);
		void acept_mission_invitation(void);
		void reject_mission_invitation(void);
		void review_missions_actives(void);
		void select_mission_active(void);	
		void begin_mission_acepted(void);
		void finalize_mission_acepted(void);
		void do_nothing_only_move(void);	
		json history_mission_state_template(void);	
		void add_invitation_to_mission(const std::string &);
		void turn_on_device(void);
		void turn_off_device(void);
		void setStatusDevice(const int &);
		int getStatusDevice(void);
		void activate(void);
		void pasivate(void);
		void resert_variables(void);
		void events_handler(void);
		void look_invitation_notification(void);



};

#endif

