#include "../include/glob.h"
#include "../include/utils.h"
#include "../include/factory.h"
#include "../include/user.h"
#include "../include/administrator.h"
#include "../include/volunteer.h"

json setings; 
json emergencies;
json missions;
json places;
json abilities;
json emergencies_types;
json millisEventAdm;
json millisEventVol;
std::string host;
std::string port;
uint32_t nbr_emergencies;
uint32_t nbr_missions;
uint32_t nbr_triesOfRequest;
uint32_t nbr_nsSleepByFailedTryRequest;
uint32_t nbr_volunteersByMission;
uint64_t millis_simulation_time;
std::random_device device;
std::mt19937 rn_generator(device());


std::map<uint32_t,uint32_t>userIDs;
std::thread * threads; 
std::thread * threads1; 
User ** users;
//int * userIDs; 


void create_add_user(uint32_t typeUser,uint32_t pos, uint64_t simTime)
{
  char buffer[2000];
  std::string txt;
  switch(typeUser)
  {
    case Factory::ADMINISTRATOR:{
        auto start = std::chrono::system_clock::now();
        Administrator *adm = Factory::create_administrator(pos);
        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end-start; 
        users[pos]=adm;
        std::string tsim = Utils::milliseconds_to_timestampString(Utils::currentTimestamp_to_milliseconds());
        sprintf(buffer,"T(%s) '%s' WAS CREATED IN [%lfs] \n",tsim.c_str(),adm->name().c_str(),elapsed_seconds.count() );
        std::cout<<buffer;

        userIDs.insert( std::make_pair(adm->getId(),pos) );

      break;
    }
    case Factory::VOLUNTEER:{
        auto start = std::chrono::system_clock::now();
        Volunteer *vol=Factory::create_volunteer(pos);
        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end-start;
        users[pos]=vol;   
        std::string tsim = Utils::milliseconds_to_timestampString(Utils::currentTimestamp_to_milliseconds()); 
        sprintf(buffer,"T(%s) '%s' WAS CREATED IN [%lfs] \n",tsim.c_str(),vol->name().c_str(),elapsed_seconds.count());
        std::cout<<buffer;        

         userIDs.insert( std::make_pair(vol->getId(),pos) );  
      break;
    }  
  }
}

void init()
{

  host = setings["host"];
  port = setings["port"];
  nbr_emergencies = setings["nbr_emergencies"];
  nbr_missions = setings["nbr_missions"];
  nbr_triesOfRequest = setings["nbr_triesOfRequest"];
  nbr_nsSleepByFailedTryRequest = setings["nbr_nsSleepByFailedTryRequest"];
  nbr_volunteersByMission = setings["nbr_volunteersByMission"];
  millis_simulation_time = setings["millis_simulation_time"];
  millisEventAdm = setings["millisEventAdm"];
  millisEventVol = setings["millisEventVol"];

  abilities = Factory::get_abilities();
  emergencies_types = Factory::get_emergencies_types();
  Factory::delete_db();

  uint32_t nbr_administrators = setings["nbr_administrators"];
  uint32_t nbr_volunteers = setings["nbr_volunteers"];
  uint32_t nbr_users = nbr_administrators + nbr_volunteers;
  uint32_t nbr_threads = setings["nbr_threads"];
  uint32_t nbr_simcicles = setings["nbr_simcicles"];
  nbr_threads = (nbr_users < nbr_threads) ? nbr_users : nbr_threads;

  threads = new std::thread[nbr_threads]; 
  users = new User*[nbr_users]; 
  userIDs = std::map<uint32_t,uint32_t>();

  std::cout<<"threads:"<<nbr_threads<<std::endl;

  uint32_t cnt_administrators = 0;
  uint32_t cnt_volunteers = 0;
  uint32_t cnt_users = 0;
  uint32_t cnt_threads = 0;
  uint32_t cnt_simcicles = 0; 

  auto total_start = std::chrono::system_clock::now();
  //carga de datos------------------------------------------------------------------
  //carga de datos------------------------------------------------------------------
  auto start = std::chrono::system_clock::now();
  try{
  while(cnt_users<nbr_users) 
  {
    
    for(cnt_threads=0; cnt_threads<nbr_threads && cnt_users<nbr_users;cnt_threads++)
    {  
      //std::cout<<"cnt_users:"<<cnt_users<<", cnt_threads:"<<cnt_threads<<std::endl; 

      if(cnt_administrators<nbr_administrators){

        threads[cnt_threads] = std::thread(create_add_user,Factory::ADMINISTRATOR,cnt_users,cnt_simcicles); 
        cnt_administrators++;
        cnt_users++;
      }
      else if(cnt_volunteers<nbr_volunteers){
      
        threads[cnt_threads] = std::thread(create_add_user,Factory::VOLUNTEER,cnt_users,cnt_simcicles); 
        cnt_volunteers++;
        cnt_users++;
      }

    }
    for(uint32_t j=0; j<cnt_threads; j++)
    {
      threads[j].join();
    }    

  }

  }
  catch(std::exception &e){
     std::cout<<"error al crear "<<e.what()<<std::endl; 
  }
  auto end = std::chrono::system_clock::now();
  std::chrono::duration<double> elapsed_seconds = end-start;
  std::cout <<" CREATE USERS (ADMINs AND VOLs): "<< elapsed_seconds.count() << "s"<<std::endl; 
  //carga de datos------------------------------------------------------------------
  //carga de datos------------------------------------------------------------------

  
  //activa agentes------------------------------------------------------------------
  //activa agentes------------------------------------------------------------------
  cnt_administrators = 0;
  cnt_volunteers = 0;
  cnt_users = 0;
  cnt_threads = 0;
  start = std::chrono::system_clock::now();
  try{

  while(cnt_users<nbr_users) 
  {
    
    for(cnt_threads=0; cnt_threads<nbr_threads && cnt_users<nbr_users;cnt_threads++)
    {  
      threads[cnt_threads] = std::thread(&User::activate,users[cnt_users]);
      cnt_users++;
    }
    for(uint32_t j=0; j<cnt_threads; j++)
    {
      threads[j].join();
    }    

  }
  
  }
  catch(std::exception &e){
     std::cout<<"error al activar "<<e.what()<<std::endl; 
  }

  end = std::chrono::system_clock::now();
  elapsed_seconds = end-start;
  std::cout <<" ACTIVATE AGENTS: "<< elapsed_seconds.count() << "s"<<std::endl; 
  //activa agentes------------------------------------------------------------------
  //activa agentes------------------------------------------------------------------
  
  try{

  delete [] threads;
  threads1 = new std::thread[nbr_users]; 

  }
  catch(std::exception &e){
     std::cout<<"error al eliminat y crear threads "<<e.what()<<std::endl; 
  }

  //realiza simulación -------------------------------------------------------------
  //realiza simulación -------------------------------------------------------------  
  try{

  start = std::chrono::system_clock::now();
  while(cnt_simcicles<nbr_simcicles)
  {  

    cnt_administrators = 0;
    cnt_volunteers = 0;
    cnt_users = 0;
    cnt_threads = 0;
    
    //while(cnt_users<nbr_users) 
    //{
      
      for(uint32_t j=0; j<nbr_users ;j++)
      {  
        threads1[j] = std::thread(&User::run,users[j],millis_simulation_time);
        
      }
      for(uint32_t j=0; j<nbr_users; j++)
      {
        threads1[j].join();
      }    

    //}

    cnt_simcicles++;
  } 



  }
  catch(std::exception &e){
     std::cout<<"error simular :"<<e.what()<<std::endl; 
  }

  end = std::chrono::system_clock::now();
  elapsed_seconds = end-start;
  std::cout <<" SIMULATION TIME: "<< elapsed_seconds.count() << "s"<<std::endl; 
  
  auto total_end = std::chrono::system_clock::now();
  elapsed_seconds = total_end-total_start;
  std::cout <<" TOTAL TIME: "<< elapsed_seconds.count() << "s"<<std::endl; 
  
  //realiza simulación -------------------------------------------------------------
  //realiza simulación -------------------------------------------------------------  



  //delete [] userIDs;
  delete [] threads1;
  delete [] users;
  userIDs.clear();
  
  
}
 
void simulate()
{
  /*
  uint32_t nbr_simulation_cicles = 10;



  for(uint32_t i=0; i<nbr_simulation_cicles ;i++)
  {
    threads = new std::thread[nbr_threads];
  } */ 
}

void simulador()
{
  init();

}



/*#################################### main
##########################################*/

int main(int argc,char** argv){
  char c;
  extern char *optarg;
  std::ifstream ifs;

  while((c=getopt(argc,argv, "s:e:m:p:"))!=-1) {
    switch (c) {
      case 's':{
          std::ifstream ifs;
          ifs.open(optarg,std::ifstream::in);
          ifs >> setings;
          ifs.close();
          break;
      }
      case 'e':{
          std::ifstream ifs;
          json jsin;
          ifs.open(optarg,std::ifstream::in);
          ifs >> jsin;
          emergencies=jsin["emergencies"];
          ifs.close();
          break;
      }
      case 'm':{
          std::ifstream ifs;
          json jsin;
          ifs.open(optarg,std::ifstream::in);
          ifs >> jsin;
          missions=jsin["missions"];
          ifs.close();
          break;
      }         
      case 'p':{
          std::ifstream ifs;
          json jsin;
          ifs.open(optarg,std::ifstream::in);
          ifs >> jsin;
          places=jsin["places"];
          ifs.close();
          break;
      }  
      default: {
          exit(EXIT_FAILURE);
          break;
      }            
    }
  }

  if(setings.empty()){
    std::cerr << "Mandatory parameter -s <basic parameters> needed" << std::endl;
     exit(EXIT_FAILURE);
  }
  if(emergencies.empty()){
    std::cerr << "Mandatory parameter -e <some template emergency> needed" << std::endl;
     exit(EXIT_FAILURE);
  }  
  if(missions.empty()){
    std::cerr << "Mandatory parameter -m <some template of missions by emergency type> needed" << std::endl;
     exit(EXIT_FAILURE);
  }  
  if(setings.empty()){
    std::cerr << "Mandatory parameter -p <desaster places> needed" << std::endl;
     exit(EXIT_FAILURE);
  }    


  simulador();

  return 0;
}