#include "../include/factory.h"

Factory::Factory( ){
}
 
Factory::~Factory( ){
}

Administrator * Factory::create_administrator(const uint32_t& id){
	
	Administrator * adm=NULL;

    char buffer[2000];
    sprintf(buffer, "admin_%d",id);
    std::string name(buffer);

    json data = user_template();

    data["Id"]=id;
    data["first_name"]=name;
    data["last_name"]=name;
    data["email"]=name+"@usach.cl";
    data["password"]="12345";	
	data["user_type_id"]=ADMINISTRATOR;

    json dataresp;
    bool swSalir = false;
    uint32_t cnt_tries = 1;

    while(cnt_tries<=nbr_triesOfRequest && swSalir==false){

        dataresp = CurlHTTP::sendPostRequest( host, port, "/user/", data, true );	

        swSalir = dataresp["runOk"];
        
        if(swSalir==true){

            printf(buffer,"\n %s TRIES - [%d] \n",name.c_str(),cnt_tries);
            //std::cout<<buffer;

            adm = new Administrator(name,dataresp["content"]["Id"],dataresp["content"]["email"],
                            dataresp["content"]["password"]);
        }
        else{
            std::this_thread::sleep_for(std::chrono::milliseconds(nbr_nsSleepByFailedTryRequest));
        }

        cnt_tries++;
    }

    if(swSalir==false){

        //sprintf(buffer,"\n %s error admin - intento (%d):%s \n",name.c_str(),cnt_tries,dataresp.dump().c_str());
        //std::string aux(buffer);
        //std::cout<<aux;
        adm = new Administrator(name,id,data["email"],data["password"]);
    }

    return adm;

}


Volunteer * Factory::create_volunteer(const uint32_t& id){

    Volunteer * vol=NULL;

	char buffer[2000];
    sprintf(buffer, "volunteer_%d",id);
    std::string name(buffer);

    json data = user_template();

    data["first_name"]=name;
    data["last_name"]=name;
    data["email"]=name+"@usach.cl";
    data["password"]="12345";   
    data["user_type_id"]=VOLUNTEER;

    json dataresp;
    bool swSalir = false;
    uint32_t cnt_tries = 1;

    while(cnt_tries<=nbr_triesOfRequest && swSalir==false){

        dataresp = CurlHTTP::sendPostRequest( host, port, "/user/", data, true );   
        swSalir = dataresp["runOk"];
        
        if(swSalir==true){

            printf(buffer,"\n %s TRIES (USER) - [%d] \n",name.c_str(),cnt_tries);
            //std::string aux(buffer);
            //std::cout<<aux;
        }
        else{
            std::this_thread::sleep_for(std::chrono::milliseconds(nbr_nsSleepByFailedTryRequest));
        }

        cnt_tries++;
    }

    if(swSalir==false){

        //sprintf(buffer,"\n %s error volunteer p1 - intento (%d):%s \n",name.c_str(),cnt_tries,dataresp.dump().c_str());
        //std::string aux(buffer);
        //std::cout<<aux;
        vol = new Volunteer(name,id,data["email"],data["password"],id);
    }
    else{

        json dataVolunteer = volunteer_template();
        dataVolunteer["user_id"]=dataresp["content"]["Id"];
        dataVolunteer["Abilities"]=Utils::random_abilities(abilities);     
        bool swSalir = false;
        uint32_t cnt_tries = 1;
        
        while(cnt_tries<=nbr_triesOfRequest && swSalir==false){

            json datarespVol = CurlHTTP::sendPostRequest( host, port, "/volunteer/", dataVolunteer, true ); 
            swSalir = datarespVol["runOk"];
            
            if(swSalir==true){

                printf(buffer,"\n %s TRIES (VOLUNTEER) - [%d] \n",name.c_str(),cnt_tries); 

                vol = new Volunteer(name,dataresp["content"]["Id"],dataresp["content"]["email"],
                            dataresp["content"]["password"],datarespVol["content"]["Id"]);
            }
            else{
                std::this_thread::sleep_for(std::chrono::milliseconds(nbr_nsSleepByFailedTryRequest));
            }

            cnt_tries++;
        }

        if(swSalir==false){

            //sprintf(buffer,"\n %s error volunteer p2 - intento (%d):%s \n",name.c_str(),cnt_tries,dataresp.dump().c_str());
            //std::string aux(buffer);
            //std::cout<<aux;
            vol = new Volunteer(name,id,data["email"],data["password"],id);
        }                 

    }


	return vol;
}
 

json Factory::get_abilities(void){
	std::string params="";
	return (CurlHTTP::sendGetRequest( host, port, "/ability/", params ))["content"];

}


json Factory::get_emergencies_types(void){
	std::string params="";
	return (CurlHTTP::sendGetRequest( host, port, "/emType/", params ))["content"];

}

void Factory::delete_db(void){
	std::string params="";
	CurlHTTP::sendDeleteRequest( host, port, "/deleteAll/", params , true );
}

json Factory::user_template(void) {
	return {{"first_name","user"},
        	{"last_name","name"},
        	{"birthday","1990-04-10T08:00:00-04:00"},
        	{"password","12345"},
        	{"email","user@usach.cl"},
        	{"contact_phone_number",123456789},
        	{"emergency_phone_number",986754321},
        	{"life_insurance",true},
        	{"user_type_id",1},
        	{"enabled",true}};
}

json Factory::volunteer_template(void) {
	return {{"volunteer_status_id",1},
    		{"user_id",1},
    		{"token",""},
    		{"Abilities",json::array()}};
}

void Factory::create_ability(const uint32_t& id)
{

    char txt[100];
    sprintf(txt, "ability_%d",id);
    std::string aux(txt);
    json data={{"ability",aux}};
    CurlHTTP::sendPostRequest( host, port, "/ability/", data, true );
}