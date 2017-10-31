#include "../include/administrator.h"

Administrator::Administrator(const std::string &name,const uint32_t &id,const std::string &username,
	      const std::string &password): User (name,id,username,password)  {

	cnt_emergencies=0;
	cnt_missions=0;
	setEvent(CREATE_EMERGENCY);
}

Administrator::Administrator() : User() {
	;
}

Administrator::~Administrator() {
	;
}

void Administrator::run(const uint64_t& millis) {

	//uint64_t millis=100000;
	std::chrono::milliseconds ms = std::chrono::milliseconds(millis);
    std::chrono::time_point<std::chrono::system_clock> end;

    end = std::chrono::system_clock::now() + ms; // this is the end point

    while(std::chrono::system_clock::now() < end){

		setSimulatorTime(Utils::currentTimestamp_to_milliseconds());

		events_handler();

	}


}

void Administrator::events_handler(void)
{

	int aux = getEvent();
	try{



	if(isActivated()==true){

		if(cnt_emergencies < nbr_emergencies){
			setEvent(CREATE_EMERGENCY);
			
		}		
		else if(cnt_missions < nbr_missions){
			setEvent(CREATE_MISSION);	
			
		}

		if(cnt_emergencies < nbr_emergencies || cnt_missions < nbr_missions ){
			
 			switch(getEvent()){
 				case CREATE_EMERGENCY:{
 					create_emergency();	
 					cnt_emergencies++;	
 					hold(Utils::rand_uniform(millisEventAdm["create_emergency"]["min"],
 											 millisEventAdm["create_emergency"]["max"]));	
 				break;
 				}
 				case CREATE_MISSION:{
					create_mission();
					cnt_missions++;
					hold(Utils::rand_uniform(millisEventAdm["create_mission"]["min"],
 											 millisEventAdm["create_mission"]["max"]));
 				break;
 				}
 			}
 		
 		}
 		else{
			pasivate();	
		} 		
	}


	}catch(std::exception &e){
		std::cout<<name()<<" eventoAntes="<<aux<<" eventoDespues="<<getEvent()<<" --- error: "<<e.what()<<std::endl;
		exit(-1);
	}
}

std::string Administrator::toString()
{
	char txt[300];
	std::string patern(User::toString());
	sprintf(txt,"{'Administrator':{'patern':%s}}",patern.c_str());
	return txt;
}


json Administrator::emergency_template()
{
	return	{{"title", "Explosión Santiago"},
	    	{"place_latitude", 1},
	    	{"place_longitude", 1},
	    	{"place_radius", 1},
	    	{"description", "Emergency test descripcion"},
	    	{"commune", "Santiago Centro"},
	    	{"city", "Santiago"},
	    	{"region", "Region Metropolitana"},
	    	{"emergency_type_id", 1},
	    	{"emergency_status_id", 1},
	    	{"user_id", 1},
	    	{"CreatedAt","2017-04-10T08:00:00-04:00"}};
}

json Administrator::mission_template()
{
	return 	{{"meeting_point_latitude", 1},
	    	{"meeting_point_longitude", 1},
	    	{"title", "Mission 85"},
	    	{"description", "Descripcion de mission 85"},
	    	{"meeting_point_address", "Calle 1 "},
	    	{"start_date", "2016-12-05T00:00:00Z"},
	    	{"finish_date", "2017-03-03T00:00:00Z"},
	    	{"scheduled_start_date", "2016-12-05T00:00:00Z"},
	    	{"scheduled_finish_date", "2017-03-03T00:00:00Z"},
	    	{"createdAt","2017-04-10T08:00:00-04:00"},
	    	{"emergency_id", 1},
	    	{"mission_status_id", 1},
	    	{"user_id", 2},
	    	{"Abilities",json::array()}};
}

json Administrator::history_mission_template()
{
	return {{"Mission_id",1},
			{"Volunteer_id",1},
			{"History_mission_state_id",1}};
}

Emergency * Administrator::create_emergency()
{
	Emergency * emergencyCreated=NULL;
	char buffer[2000];

	json emergency = emergency_template();

	std::uniform_int_distribution<int> rn_emergencies(0,emergencies.size()-1);
	json rn_emergency = emergencies[rn_emergencies(rn_generator)];

	std::uniform_int_distribution<int> rn_places(0,places.size()-1);
	json rn_place = places[rn_places(rn_generator)];


	hold(Utils::rand_uniform(millisEventAdm["load_form_emergency"]["min"],
							 millisEventAdm["load_form_emergency"]["max"]));

	emergency["user_id"] = this->_id;
	emergency["title"] = rn_emergency["title"];
	emergency["description"] = rn_emergency["description"];	
	emergency["type"] = rn_emergency["type"];
	emergency["commune"] = rn_place["commune"];
	emergency["city"] = rn_place["city"];
	emergency["region"] = rn_place["region"];
	emergency["place_latitude"] = rn_place["place_latitude"];
	emergency["place_longitude"] = rn_place["place_longitude"];
	emergency["place_radius"] = rn_place["place_radius"];

	
	hold(Utils::rand_uniform(millisEventAdm["insert_data_form_emergency"]["min"],
							 millisEventAdm["insert_data_form_emergency"]["max"]));

	json dataresp = CurlHTTP::sendPostRequest(host,port, "/emergency/", emergency, true );

	//std::cout<<name()<<" create_emergency:"<<emergency<<" --- resp:"<<dataresp<<"\n\n";

	emergencyCreated = new Emergency(dataresp["content"]["Id"],dataresp["content"]["user_id"],
									dataresp["content"]["title"]);

	setSimulatorTime(Utils::currentTimestamp_to_milliseconds());

	sprintf(buffer,"T(%s) '%s' CREATE EMERGENCY %s \n",simtimeToString().c_str(),name().c_str(),
													emergencyCreated->toString().c_str() );
 	std::cout<<buffer;

	return emergencyCreated;
} 


Mission * Administrator::create_mission()
{
	Mission * missionCreated=NULL;
	char buffer[2000];

	std::string param = "";
	json emergenciesdb = CurlHTTP::sendGetRequest(host,port, "/emergencia/activas/", param )["content"];


	hold(Utils::rand_uniform(millisEventAdm["load_form_mission"]["min"],
							 millisEventAdm["load_form_mission"]["max"]));	

	//selecciona al azar una emergencia
	std::uniform_int_distribution<int> rn_emergenciesdb(0, emergenciesdb.size()-1);
	json emergencySelected = emergenciesdb[rn_emergenciesdb(rn_generator)];
	
	hold(Utils::rand_uniform(millisEventAdm["select_emergency_to_create_mission"]["min"],
							 millisEventAdm["select_emergency_to_create_mission"]["max"]));	

	sprintf(buffer,"T(%s) '%s' EMERGENCY (ID:%d, TITLE:%s) WAS SELECTED TO CREATE A NEW MISSION\n",
							simtimeToString().c_str(),name().c_str(),emergencySelected["Id"].get<int>(),
							emergencySelected["title"].dump().c_str());
	std::cout<<buffer;
	

	//carga las abilidades y seccionas las necesarias para la mission
	//selecciona las abilidades a azar para la misión
	json abilitiesSelected = Utils::random_abilities(abilities);
	json abilitiesArray = Utils::abilities_to_jsonArrayWithDescrip(abilitiesSelected);

	//busco el listado de voluntarios idoneos para la misión
	//NOTA, HAY QUE MEJORAR ESTE SERVICIO, NO PUEDE SER QUE CUANDO TENGAMOS 1 MILLON DE USUARIOS, LOS TRAIGA TODOS
	json volunteersRanking = CurlHTTP::sendPostRequest(host,port,"/voluntarios/ranking/",abilitiesArray,true)["content"];
	
	//selecciono una misión a alzar respecto del tipo de emergencia seleccionada
	json rn_mission = Utils::random_missionByEmType(emergencySelected["Emergency_type"]["Id"], missions);
	json mission = mission_template(); 

	mission["title"]=rn_mission["title"];
	mission["description"]=rn_mission["description"];
	mission["emergency_id"]=emergencySelected["Id"];
	mission["user_id"]=this->_id;
	mission["Abilities"]=abilitiesSelected;

	hold(Utils::rand_uniform(millisEventAdm["insert_data_form_mission"]["min"],
							 millisEventAdm["insert_data_form_mission"]["max"]));	
	
	json missionresp = CurlHTTP::sendPostRequest(host,port,"/mission/",mission,true)["content"];

	//std::cout<<name()<<" create_mission:"<<mission<<" --- resp:"<<missionresp<<"\n\n";

	missionCreated = new Mission(missionresp["Id"],missionresp["emergency_id"],
						missionresp["user_id"],missionresp["title"]);
	
	
	setSimulatorTime(Utils::currentTimestamp_to_milliseconds());

	sprintf(buffer,"T(%s) '%s' CREATE MISSION %s \n",simtimeToString().c_str(),name().c_str(),
													missionCreated->toString().c_str() );
 	std::cout<<buffer;

	//invita a voluntarios
	uint32_t nbr_volunteersRanking = volunteersRanking.size();

	uint32_t nbr_volunteersThisMission = (nbr_volunteersByMission<nbr_volunteersRanking) ? nbr_volunteersByMission : 
	                                                                              			nbr_volunteersRanking;

	json history_mission  =history_mission_template(); 
	history_mission["Mission_id"]=missionresp["Id"];

	json invitation={ { "Mission", { {"Id",missionCreated->_id},{"emergency_id",missionCreated->_emergency_id} } }	};	
	
	for(uint32_t cnt_vol=0; cnt_vol<nbr_volunteersThisMission; cnt_vol++)
	{	
		uint32_t idUser = volunteersRanking[cnt_vol]["user"]["Id"];
		Volunteer * vol = (Volunteer *)users[userIDs[idUser]];
		//std::cout<<"\n\n---------->"<<vol->toString().c_str()<<"("<<userIDs[idUser]<<","<<idUser<<")\n\n";

		uint32_t idVolunteer = volunteersRanking[cnt_vol]["user"]["Volunteer"]["Id"];
		history_mission["Volunteer_id"]=idVolunteer;


		//sprintf(buffer,"%s - (%d) history_mission : %s\n",name().c_str(),cnt_vol,history_mission.dump().c_str());
		//std::cout<<buffer;
		json history_missionCreated = CurlHTTP::sendPostRequest(host,port,"/historyMissions/", history_mission, true )["content"];
		
		setSimulatorTime(Utils::currentTimestamp_to_milliseconds());

		sprintf(buffer,"T(%s) '%s' VOLUNTEER (ID=%d,NAME=%s) WAS INVITED TO THE MISSION %s\n",simtimeToString().c_str(),
																name().c_str(),idVolunteer,vol->name().c_str(),
			                                                    missionCreated->toString().c_str());
		std::cout<<buffer;

		sprintf(buffer,"YOU HAVE BEEN INVITED TO THE MISSION %s",missionCreated->toString().c_str());
		vol->add_invitation_to_mission(buffer);

	}
	

	return missionCreated;
	
}
