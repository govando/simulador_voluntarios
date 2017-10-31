
#include "../include/utils.h"

Utils::Utils(){
    ;
}


Utils::~Utils(){
    ;
}

json Utils::random_abilities(const json & _abilities){
    int abilities_size = _abilities.size(); 
    std::uniform_int_distribution<int> rn_abilities(1,abilities_size);
    json resp=json::array();
    int nbr_abilities = rn_abilities(rn_generator);
    int sw[nbr_abilities]={}; 
    int cnt_abilities=0;
    while(cnt_abilities < nbr_abilities){
        int id = rn_abilities(rn_generator)-1; 
        if(sw[id]==0){
            resp.push_back(_abilities[id]);
            sw[id]=1;
            cnt_abilities++;   
        }           
    }

    return resp;
}

json Utils::random_emergency_type(const json & emergencies_types){
    std::uniform_int_distribution<int> rn_emType(0,emergencies_types.size()-1);
    return emergencies_types[ rn_emType(rn_generator) ];
}

json Utils::abilities_to_jsonArrayWithDescrip(const json & _abilities)
{   int abilities_size = _abilities.size(); 
    json resp={{"abilities",json::array()}}; 
    for(int i=0 ; i<abilities_size ; i++)
    {
        resp["abilities"].push_back(_abilities[i]["ability"]);
    }
    return resp;
}


json Utils::random_missionByEmType(const int &emType,const json &missions)
{
    json missionsbytype = missions[emType - 1]["missionsbytype"];

    std::uniform_int_distribution<int> rn_missionsbytype(0,missionsbytype.size()-1);

    return missionsbytype[ rn_missionsbytype(rn_generator) ];
}


uint64_t Utils::currentTimestamp_to_milliseconds()
{
    auto tm = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(tm.time_since_epoch()).count();
}



std::time_t Utils::milliseconds_to_timestamp(const uint64_t & uptime)
{
    using time_point = std::chrono::system_clock::time_point;
    time_point timepoint_uptime{std::chrono::duration_cast<time_point::duration>(std::chrono::milliseconds(uptime))};
    return std::chrono::system_clock::to_time_t(timepoint_uptime);  
}

std::string Utils::milliseconds_to_timestampString(const uint64_t & uptime)
{
    using time_point = std::chrono::system_clock::time_point;
    time_point timepoint_uptime{std::chrono::duration_cast<time_point::duration>(std::chrono::milliseconds(uptime))};    
    std::time_t t = std::chrono::system_clock::to_time_t(timepoint_uptime);

    char sRep[100], resp[100];
    strftime(sRep,sizeof(sRep),"%Y-%m-%d %H:%M:%S",localtime(&t));

    auto since_epoch = timepoint_uptime.time_since_epoch();
    auto s =  std::chrono::duration_cast<std::chrono::seconds>(since_epoch);
    since_epoch = since_epoch - s;
    auto milli = std::chrono::duration_cast<std::chrono::milliseconds>(since_epoch);

    sprintf(resp,"%s.%ld",sRep,milli.count());
    return resp;
}

uint64_t Utils::rand_uniform(const uint64_t & a,const uint64_t & b){
    std::uniform_int_distribution<int> rn(a,b);
    return rn(rn_generator); 
}


