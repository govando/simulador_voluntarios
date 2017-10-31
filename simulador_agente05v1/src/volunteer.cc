#include "../include/volunteer.h"


Volunteer::Volunteer(const std::string &name,const uint32_t &id,const std::string &username,
	      const std::string &password,const uint32_t &idVol) : User (name,id,username,password) {

	this->_idVol = idVol;
	resert_variables();
	setStatusDevice( TURNED_ON );
	setEvent( LOGIN );
}

Volunteer::~Volunteer() {
    ;
}

void Volunteer::run(const uint64_t& millis) {

	
	//uint64_t millis=100000;
	std::chrono::milliseconds ms = std::chrono::milliseconds(millis);
    std::chrono::time_point<std::chrono::system_clock> end;

    end = std::chrono::system_clock::now() + ms; // this is the end point

    while(std::chrono::system_clock::now() < end){

		setSimulatorTime(Utils::currentTimestamp_to_milliseconds());

		events_handler();

	}

}


void Volunteer::events_handler()
{	
	int aux = getEvent();
	try{

	std::uniform_int_distribution<int> rn_event(TURN_ON_DEVICE,TURN_OFF_DEVICE);
	setEvent( (getEvent()  == DO_NOTHING_ONLY_MOVE) ? rn_event(rn_generator) : getEvent() );
	
	//std::cout<<" evento : "<<getEvent()<<"\n\n"<<std::endl;

	switch( getEvent() ){
		case TURN_ON_DEVICE:{
			turn_on_device();
			break;
		}
		case TURN_OFF_DEVICE:{
			turn_off_device();
			break;	
		}			
	}	


	if(isActivated()==true){  //getStatusDevice() == TURNED_ON
 
		switch( getEvent() ){
			case LOGIN:{
				login();
				break;
			}
			case LOGOUT:{
				logout();
				break;
			}
			case LOOK_INVITATION_NOTIFICATION:{
				look_invitation_notification();
				break;
			}
			case REVIEW_MISSIONS_INVITATIONS:{
				review_missions_invitations();
				break;
			}
			case SELECT_MISSION_INVITATION:{
				select_mission_invitation();
				break;
			}					
			case ACEPT_MISSION_INVITATION:{
				acept_mission_invitation();
				break;
			}	
			case REJECT_MISSION_INVITATION:{
				reject_mission_invitation();
				break;
			}

			case REVIEW_MISSIONS_ACTIVES:{
				review_missions_actives();
				break;
			}
			case SELECT_MISSION_ACTIVE:{
				select_mission_active();
				break;
			}

			case BEGIN_MISSION_ACEPTED:{
				begin_mission_acepted();
				break;
			}

			case FINALIZE_MISSION_ACEPTED:{
				finalize_mission_acepted();
				break;
			}
			case DO_NOTHING_ONLY_MOVE:{
				do_nothing_only_move();
				break;
			}		
					
		}
		//X,Y basado en el evento, ejemplo DO_NOTHING_ONLY_MOVE uede caminar o no, pero no depende de missiones 	 

	}	
	

	}catch(std::exception &e){
		std::cout<<name()<<" eventoAntes="<<aux<<" eventoDespues="<<getEvent()<<" --- error: "<<e.what()<<std::endl;
		exit(-1);
	}

}

void Volunteer::activate(void)
{
	turn_on_device();
}

void Volunteer::pasivate(void)
{
	turn_off_device();
}


void  Volunteer::setStatusDevice(const int & status){
	this->_status_device = status;
}

int  Volunteer::getStatusDevice(void){
	return this->_status_device;
}

std::string Volunteer::toString(void)
{
	char txt[300];
	std::string patern(User::toString());
	sprintf(txt,"{'Volunteer':{'idVol':%d,'patern':%s}}",this->_idVol,patern.c_str());
	return txt;
}

void Volunteer::resert_variables(void)
{
	this->_token = "";
	this->_missions_invitations=json::array();;
	this->_mission_invitation_selected={};	
	this->_missions_actives=json::array();;
	this->_mission_active_selected={};		
}

void Volunteer::turn_on_device(void)
{

	if(getStatusDevice() == TURNED_OFF){
		char buffer[2000];

		setSimulatorTime(Utils::currentTimestamp_to_milliseconds());
		sprintf(buffer,"T(%s) '%s' TURNING ON DEVICE ...... \n",simtimeToString().c_str(),name().c_str());	
		std::cout<<buffer;	
		
		hold(Utils::rand_uniform(millisEventVol["turn_on_device"]["min"],
 		    					 millisEventVol["turn_on_device"]["max"]));


		
		setStatusDevice( TURNED_ON );
		User::activate();
		resert_variables();

		setSimulatorTime(Utils::currentTimestamp_to_milliseconds());
		sprintf(buffer,"T(%s) '%s' DEVICE TURNED ON\n",simtimeToString().c_str(),name().c_str());	
		std::cout<<buffer;	

		//CALCULA EL X,Y DEPENDIENDO DEL TIEMPO QUE A PASADO HASTA ESE MOMENTO

		std::uniform_int_distribution<int> rn_event(0,100);
		if(rn_event(rn_generator)<50){
			setEvent( LOGIN );
		}
		else{
			setEvent( DO_NOTHING_ONLY_MOVE );
		}	
	}
	else{
		setEvent( DO_NOTHING_ONLY_MOVE );
		do_nothing_only_move();		
	}
}

void Volunteer::turn_off_device(void)
{
	if(getStatusDevice() == TURNED_ON){
		char buffer[2000];

		setSimulatorTime(Utils::currentTimestamp_to_milliseconds());
		sprintf(buffer,"T(%s) '%s' TURNING OFF DEVICE ...... \n",simtimeToString().c_str(),name().c_str());	
		std::cout<<buffer;	
		
		hold(Utils::rand_uniform(millisEventVol["turn_off_device"]["min"],
 		    					 millisEventVol["turn_off_device"]["max"]));

		setStatusDevice( TURNED_OFF );
		User::pasivate();
		resert_variables();	

		setSimulatorTime(Utils::currentTimestamp_to_milliseconds());
		sprintf(buffer,"T(%s) '%s' DEVICE TURNED OFF \n",simtimeToString().c_str(),name().c_str());	
		std::cout<<buffer;	

		//IMPRIME EL ULTIMO X,Y
		//setEvent( DO_NOTHING_ONLY_MOVE );		
	}
	setEvent( DO_NOTHING_ONLY_MOVE );
}

void Volunteer::login(void)
{
	if(this->_token==""){
		char buffer[2000];

		setSimulatorTime(Utils::currentTimestamp_to_milliseconds());
		sprintf(buffer,"T(%s) '%s' IS IN THE LOGIN FORM, %s COMPLETE THE INFORMATION AND LOGIN \n",simtimeToString().c_str(),name().c_str(),name().c_str());	
		std::cout<<buffer;

		hold(Utils::rand_uniform(millisEventVol["insert_data_login"]["min"],
 		    					 millisEventVol["insert_data_login"]["max"]));

		json dataLogin = {{	"username",this->_username},{"password", this->_password }};
		json dataresp = CurlHTTP::sendPostRequest(host,port, "/login", dataLogin, true );
		if(dataresp["runOk"]==true){
			this->_token = dataresp["content"]["token"];
			setSimulatorTime(Utils::currentTimestamp_to_milliseconds());
			sprintf(buffer,"T(%s) '%s' IS LOGUED WITH TOKEN %s ..................... \n",
												simtimeToString().c_str(),name().c_str(),
												this->_token.substr(1, 40).c_str());
		}
		else{
			resert_variables();
			setSimulatorTime(Utils::currentTimestamp_to_milliseconds());
			sprintf(buffer,"T(%s) '%s' ERROR WHEN TRY TO LOGIN  \n",simtimeToString().c_str(),name().c_str());	

		}
		std::cout<<buffer;
		//X,Y
		//tiempo corto de simulación
		setEvent( DO_NOTHING_ONLY_MOVE );
 	}
	else{
		setEvent( DO_NOTHING_ONLY_MOVE );
		do_nothing_only_move();
	}

}

void Volunteer::logout(void)
{
	if(this->_token==""){
		setEvent( DO_NOTHING_ONLY_MOVE );
		do_nothing_only_move();
	}
	else{
		char buffer[2000];
		resert_variables();	

		setSimulatorTime(Utils::currentTimestamp_to_milliseconds());
		sprintf(buffer,"T(%s) '%s' PUSH LOGOUT THE SYSTEM \n",simtimeToString().c_str(),name().c_str());	
		std::cout<<buffer;

		hold(Utils::rand_uniform(millisEventVol["push_logout"]["min"],
 		    					 millisEventVol["push_logout"]["max"]));


		setSimulatorTime(Utils::currentTimestamp_to_milliseconds());
		sprintf(buffer,"T(%s) '%s' NOW IS LOGOUT \n",simtimeToString().c_str(),name().c_str());
		std::cout<<buffer;
		setEvent( DO_NOTHING_ONLY_MOVE );
	}
 
	//X,Y
}


void Volunteer::review_missions_invitations(void)
{
	if(this->_token==""){
		setEvent( DO_NOTHING_ONLY_MOVE );
		do_nothing_only_move();
	}
	else{
		char buffer[2000];
		std::string param="";

		setSimulatorTime(Utils::currentTimestamp_to_milliseconds());
		sprintf(buffer,"T(%s) '%s' REVIEW MISSIONS INVITATIONS \n",simtimeToString().c_str(),name().c_str());	
		std::cout<<buffer;

		hold(Utils::rand_uniform(millisEventVol["review_missions_invitations"]["min"],
 		    					 millisEventVol["review_missions_invitations"]["max"]));

		json datarep = CurlHTTP::sendGetRequest(host,port,"/mision/invitaciones/",param, this->_token );
		
		setSimulatorTime(Utils::currentTimestamp_to_milliseconds());
		
		if(datarep["runOk"]==true){
			if(datarep["content"].empty()==true){
				sprintf(buffer,"T(%s) '%s' DON'T HAVE MISSIONS INVITATIONS \n",simtimeToString().c_str(),name().c_str());
				std::cout<<buffer;
				setEvent( DO_NOTHING_ONLY_MOVE ) ;
			}
			else{
				if(datarep["content"].size()==0){
					sprintf(buffer,"T(%s) '%s' DON'T HAVE MISSIONS INVITATIONS \n",simtimeToString().c_str(),name().c_str());
					std::cout<<buffer;
					setEvent( DO_NOTHING_ONLY_MOVE );	
				}
				else{
					this->_missions_invitations = datarep["content"];
					sprintf(buffer,"T(%s) '%s' HAVE %lu MISSIONS INVITATIONS \n",simtimeToString().c_str(),
										name().c_str(),this->_missions_invitations.size());
					std::cout<<buffer;
					std::uniform_int_distribution<int> rn_event(0,100);
					if(rn_event(rn_generator)<90){
						setEvent( SELECT_MISSION_INVITATION );
					}
					else{
						setEvent( DO_NOTHING_ONLY_MOVE );
					}
				}

			}
		}	
		else{
			sprintf(buffer,"T(%s) '%s' DON'T HAVE MISSIONS INVITATIONS \n",simtimeToString().c_str(),name().c_str());
			std::cout<<buffer;
			setEvent( DO_NOTHING_ONLY_MOVE );			
		}
	}

 
	//X,Y
}

void Volunteer::select_mission_invitation(void)
{

	int nbr_invitations = this->_missions_invitations.size(); 

	if(this->_token=="" || nbr_invitations==0){
		setEvent( DO_NOTHING_ONLY_MOVE );
		do_nothing_only_move();
	}
	else{

		char buffer[2000];

		setSimulatorTime(Utils::currentTimestamp_to_milliseconds());
		sprintf(buffer,"T(%s) '%s' IS SELECTING ONE MISSION INVITATION \n",simtimeToString().c_str(),name().c_str());	
		std::cout<<buffer;

		hold(Utils::rand_uniform(millisEventVol["select_mission_invitation"]["min"],
 		    					 millisEventVol["select_mission_invitation"]["max"]));


		std::uniform_int_distribution<int> rn_invitation(0,nbr_invitations-1);
		this->_mission_invitation_selected = this->_missions_invitations[rn_invitation(rn_generator)]["Mission"];

		setSimulatorTime(Utils::currentTimestamp_to_milliseconds());

		sprintf(buffer,"T(%s) '%s' SELECT THE MISSION INVITATION (IDMISSION:%d,IDEMERGENCY:%d) \n",simtimeToString().c_str(),
								name().c_str(),this->_mission_invitation_selected["Id"].get<int>(),
											   this->_mission_invitation_selected["emergency_id"].get<int>());
		std::cout<<buffer;

		this->_missions_invitations.clear();
		std::uniform_int_distribution<int> rn_event(0,100);
		int prob_event = rn_event(rn_generator);
		if(prob_event<45){
			setEvent( ACEPT_MISSION_INVITATION );
		}
		else if(prob_event<90){
			setEvent( REJECT_MISSION_INVITATION );
		}
		else{
			setEvent( DO_NOTHING_ONLY_MOVE );
		}

	}
 
	//X,Y
}


void Volunteer::acept_mission_invitation(void)
{
	if(this->_token=="" || _mission_invitation_selected.empty()==true){
		setEvent( DO_NOTHING_ONLY_MOVE );
		do_nothing_only_move();
	}
	else{
		char buffer[2000];

		setSimulatorTime(Utils::currentTimestamp_to_milliseconds());
		sprintf(buffer,"T(%s) '%s' IS ACCEPTING THE MISSION INVITATION SELECTED \n",simtimeToString().c_str(),name().c_str());	
		std::cout<<buffer;

		hold(Utils::rand_uniform(millisEventVol["acept_mission_invitation"]["min"],
 		    					 millisEventVol["acept_mission_invitation"]["max"]));		
		
	 	sprintf(buffer,"%d",this->_mission_invitation_selected["Id"].get<uint32_t>());
	 	std::string param = buffer;
		json data = history_mission_state_template();
		data["Mission_id"]=this->_mission_invitation_selected["Id"].get<uint32_t>();
		data["Volunteer_id"]=this->_idVol;
		data["History_mission_state_id"]=2;

		json datarep = CurlHTTP::sendPutRequest(host,port,"/historia/",param, data,true, this->_token );

		setSimulatorTime(Utils::currentTimestamp_to_milliseconds());

		sprintf(buffer,"T(%s) '%s' ACEPT THE MISSION INVITATION (IDMISSION:%d,IDEMERGENCY:%d) \n",simtimeToString().c_str(),
								name().c_str(),this->_mission_invitation_selected["Id"].get<uint32_t>(),
											   this->_mission_invitation_selected["emergency_id"].get<uint32_t>());
		std::cout<<buffer;	
		setEvent( DO_NOTHING_ONLY_MOVE );	
	}
}


void Volunteer::reject_mission_invitation(void)
{
	if(this->_token=="" || _mission_invitation_selected.empty()==true){
		setEvent( DO_NOTHING_ONLY_MOVE );
		do_nothing_only_move();
	}
	else{
		char buffer[2000];
		
		setSimulatorTime(Utils::currentTimestamp_to_milliseconds());
		sprintf(buffer,"T(%s) '%s' IS REJECTING THE MISSION INVITATION SELECTED \n",simtimeToString().c_str(),name().c_str());	
		std::cout<<buffer;

		hold(Utils::rand_uniform(millisEventVol["reject_mission_invitation"]["min"],
 		    					 millisEventVol["reject_mission_invitation"]["max"]));		
		

	 	sprintf(buffer,"%d",this->_mission_invitation_selected["Id"].get<uint32_t>());
	 	std::string param = buffer;
		json data = history_mission_state_template();
		data["Mission_id"]=this->_mission_invitation_selected["Id"].get<uint32_t>();
		data["Volunteer_id"]=this->_idVol;		
		data["History_mission_state_id"]=5;

		json datarep = CurlHTTP::sendPutRequest(host,port,"/historia/",param, data,true, this->_token );

		setSimulatorTime(Utils::currentTimestamp_to_milliseconds());

		sprintf(buffer,"T(%s) '%s' REJECT THE MISSION INVITATION (IDMISSION:%d,IDEMERGENCY:%d) \n",simtimeToString().c_str(),
								name().c_str(),this->_mission_invitation_selected["Id"].get<uint32_t>(),
											   this->_mission_invitation_selected["emergency_id"].get<uint32_t>());
		std::cout<<buffer;	
		setEvent( DO_NOTHING_ONLY_MOVE );	
	}

}


void Volunteer::review_missions_actives(void)
{

	if(this->_token==""){
		setEvent( DO_NOTHING_ONLY_MOVE );
		do_nothing_only_move();
	}
	else{
		char buffer[2000];
		std::string param="";

		setSimulatorTime(Utils::currentTimestamp_to_milliseconds());
		sprintf(buffer,"T(%s) '%s' IS REVIEWING ACTIVE MISSIONS \n",simtimeToString().c_str(),name().c_str());	
		std::cout<<buffer;

		hold(Utils::rand_uniform(millisEventVol["review_missions_actives"]["min"],
 		    					 millisEventVol["review_missions_actives"]["max"]));		

		json datarep = CurlHTTP::sendGetRequest(host,port,"/mision/activa/",param, this->_token );
		
		setSimulatorTime(Utils::currentTimestamp_to_milliseconds());

		if(datarep["runOk"]==true){
			if(datarep["content"].empty()==true){
				sprintf(buffer,"T(%s) '%s' DON'T HAVE MISSIONS ACTIVES \n",simtimeToString().c_str(),name().c_str());
				std::cout<<buffer;
				setEvent( DO_NOTHING_ONLY_MOVE );
			}
			else{
				if( datarep["content"].size()==0){
					sprintf(buffer,"T(%s) '%s' DON'T HAVE MISSIONS ACTIVES \n",simtimeToString().c_str(),name().c_str());
					std::cout<<buffer;
					setEvent( DO_NOTHING_ONLY_MOVE ) ;					
				}
				else{	
					this->_missions_actives = datarep["content"];
					sprintf(buffer,"T(%s) '%s' HAVE %lu MISSIONS ACTIVES \n",simtimeToString().c_str(),
										name().c_str(),this->_missions_actives.size());
					std::cout<<buffer;
					std::uniform_int_distribution<int> rn_event(0,100);
					if(rn_event(rn_generator)<90){
						setEvent( SELECT_MISSION_ACTIVE );
					}
					else{
						setEvent( DO_NOTHING_ONLY_MOVE );
					}
				}
			}
		}	
		else{
			sprintf(buffer,"T(%s) '%s' DON'T HAVE MISSIONS ACTIVES \n",simtimeToString().c_str(),name().c_str());
			std::cout<<buffer;
			setEvent( DO_NOTHING_ONLY_MOVE ) ;
		}

	}

 
	//X,Y
}

		
void Volunteer::select_mission_active(void)
{
	int nbr_actives = this->_missions_actives.size(); 

	if(this->_token=="" || nbr_actives==0){
		setEvent( DO_NOTHING_ONLY_MOVE );
		do_nothing_only_move();
	}
	else{
		char buffer[2000];

		setSimulatorTime(Utils::currentTimestamp_to_milliseconds());
		sprintf(buffer,"T(%s) '%s' IS SELECTING AN ACTIVE MISSION \n",simtimeToString().c_str(),name().c_str());	
		std::cout<<buffer;

		hold(Utils::rand_uniform(millisEventVol["select_mission_active"]["min"],
 		    					 millisEventVol["select_mission_active"]["max"]));


		std::uniform_int_distribution<int> rn_active(0,nbr_actives-1);
		this->_mission_active_selected = this->_missions_actives[rn_active(rn_generator)]["Mission"];

		setSimulatorTime(Utils::currentTimestamp_to_milliseconds());

		sprintf(buffer,"T(%s) '%s' SELECT THE MISSION ACTIVATED (IDMISSION:%d,IDEMERGENCY:%d) \n",simtimeToString().c_str(),
								name().c_str(),this->_mission_active_selected["Id"].get<uint32_t>(),
											   this->_mission_active_selected["emergency_id"].get<uint32_t>());
		std::cout<<buffer;

		this->_missions_actives.clear();
		std::uniform_int_distribution<int> rn_event(0,100);
		int prob_event = rn_event(rn_generator);
		if(prob_event<45){
			setEvent( BEGIN_MISSION_ACEPTED );
		}
		else if(prob_event<90){
			setEvent( FINALIZE_MISSION_ACEPTED );
		}
		else{
			setEvent( DO_NOTHING_ONLY_MOVE );
		}

	}

}

void Volunteer::begin_mission_acepted(void)
{
	if(this->_token=="" || _mission_active_selected.empty()==true){
		setEvent( DO_NOTHING_ONLY_MOVE );
		do_nothing_only_move();
	}
	else{
		char buffer[2000];

		setSimulatorTime(Utils::currentTimestamp_to_milliseconds());
		sprintf(buffer,"T(%s) '%s' IS BEGINNING THE SELECTED ACTIVE MISSION \n",simtimeToString().c_str(),name().c_str());	
		std::cout<<buffer;

		hold(Utils::rand_uniform(millisEventVol["begin_mission_acepted"]["min"],
 		    					 millisEventVol["begin_mission_acepted"]["max"]));

	 	sprintf(buffer,"%d",this->_mission_active_selected["Id"].get<uint32_t>());
	 	std::string param = buffer;
		json data = history_mission_state_template();
		data["Mission_id"]=this->_mission_active_selected["Id"].get<uint32_t>();
		data["Volunteer_id"]=this->_idVol;
		data["History_mission_state_id"]=3;

		json datarep = CurlHTTP::sendPutRequest(host,port,"/historia/",param, data,true, this->_token );

		setSimulatorTime(Utils::currentTimestamp_to_milliseconds());

		sprintf(buffer,"T(%s) '%s' BEGIN THE MISSION ACTIVATED (IDMISSION:%d,IDEMERGENCY:%d) \n",simtimeToString().c_str(),
								name().c_str(),this->_mission_active_selected["Id"].get<int>(),
											   this->_mission_active_selected["emergency_id"].get<int>());
		std::cout<<buffer;	
		setEvent( DO_NOTHING_ONLY_MOVE );	
	}
}


void Volunteer::finalize_mission_acepted(void)
{
	if(this->_token=="" || _mission_active_selected.empty()==true){
		setEvent( DO_NOTHING_ONLY_MOVE );
		do_nothing_only_move();
	}
	else{
		char buffer[2000];


		setSimulatorTime(Utils::currentTimestamp_to_milliseconds());
		sprintf(buffer,"T(%s) '%s' IS FINALIZING THE SELECTED ACTIVE MISSION \n",simtimeToString().c_str(),name().c_str());	
		std::cout<<buffer;

		hold(Utils::rand_uniform(millisEventVol["finalize_mission_acepted"]["min"],
 		    					 millisEventVol["finalize_mission_acepted"]["max"]));


	 	sprintf(buffer,"%d",this->_mission_active_selected["Id"].get<uint32_t>());
	 	std::string param = buffer;
		json data = history_mission_state_template();
		data["Mission_id"]=this->_mission_active_selected["Id"].get<uint32_t>();
		data["Volunteer_id"]=this->_idVol;
		data["History_mission_state_id"]=4;

		json datarep = CurlHTTP::sendPutRequest(host,port,"/historia/",param, data,true, this->_token );

		setSimulatorTime(Utils::currentTimestamp_to_milliseconds());

		sprintf(buffer,"T(%s) '%s' FINALIZE THE MISSION ACTIVATED (IDMISSION:%d,IDEMERGENCY:%d) \n",simtimeToString().c_str(),
								name().c_str(),this->_mission_active_selected["Id"].get<int>(),
											   this->_mission_active_selected["emergency_id"].get<int>());
		std::cout<<buffer;	
		setEvent( DO_NOTHING_ONLY_MOVE );	
	}
	//X,Y
}

void Volunteer::do_nothing_only_move(void)
{
	char buffer[2000];

	hold(Utils::rand_uniform(millisEventVol["do_nothing_only_move"]["min"],
	    					 millisEventVol["do_nothing_only_move"]["max"]));

	sprintf(buffer,"T(%s) '%s' DO NOTHING, ONLY WALK \n",simtimeToString().c_str(),name().c_str());
	std::cout<<buffer;
	//passivate();
 
	//X,Y
}


json Volunteer::history_mission_state_template(void){
	
	return {{"History_mission_state_id",0},
			{"Mission_id",0},
			{"Volunteer_id",0}};

}


void Volunteer::add_invitation_to_mission(const std::string & invitationNotification){
	
	interruptHoldNow(); //interrumpir hold cuado este haciendo otra cosa y le llegue un mensaje
	this->_invitation_notification = invitationNotification;

	if(getStatusDevice() == TURNED_OFF){
		
		setEvent( DO_NOTHING_ONLY_MOVE );
		//do_nothing_only_move();		

	}
	else{
		std::uniform_int_distribution<int> rn_event(0,100);
		int prob_event = rn_event(rn_generator);
		if(prob_event<60){
			
			setEvent( LOOK_INVITATION_NOTIFICATION );
		
		}
		else if(prob_event<90){

			setEvent( REVIEW_MISSIONS_INVITATIONS );
		
		}
		else{
			setEvent( DO_NOTHING_ONLY_MOVE );
		}		

	}

}

void Volunteer::look_invitation_notification(void)
{
	if(this->_token=="" || this->_invitation_notification.compare("") == 0){
		setEvent( DO_NOTHING_ONLY_MOVE );
		do_nothing_only_move();
	}
	else{
		char buffer[2000];


		hold(Utils::rand_uniform(millisEventVol["look_invitation_notification"]["min"],
	    					 	millisEventVol["look_invitation_notification"]["max"]));
		

		setSimulatorTime(Utils::currentTimestamp_to_milliseconds());
		sprintf(buffer,"T(%s) '%s' IS LOOKING INVITATION NOTIFICATION (%s), YOU NEED TO GO REVIEW MISSIONS INVITATIONS \n",simtimeToString().c_str(),name().c_str(),
																  this->_invitation_notification.c_str());	
		std::cout<<buffer;

		std::uniform_int_distribution<int> rn_event(0,100);
		int prob_event = rn_event(rn_generator);
		if(prob_event<90){

			setEvent( REVIEW_MISSIONS_INVITATIONS );
		
		}
		else{
			setEvent( DO_NOTHING_ONLY_MOVE );
		}		

		this->_invitation_notification = "";
	}

}