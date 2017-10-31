#include "../include/glob.h"
#include "../include/curlhttp.h"
#include "../include/user.h"
#include "../include/administrator.h"
//#include "../include/coordinator.h"
#include "../include/volunteer.h"

json setings; 
json emergencies;
json missions;
json places;
json abilities;
json emergencies_types;
std::random_device device;
std::mt19937 rn_generator(device());

/*
void run(int hilo)
{ 
    auto start = std::chrono::system_clock::now();
    
    std::string params="";
    json resp = CurlHTTP::sendGetRequest( setings["host"], setings["port"] , "/ability/", params );

    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;

    std::cout << "(" << hilo << ") resp:"<< resp.size()
              <<" elapsedTime: "<< elapsed_seconds.count() << "s"<<std::endl;
}  */

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

  /*
  uint32_t nbr_threads = 10;
  std::thread * threads = new std::thread[nbr_threads]; 
  for(uint32_t i=0; i<nbr_threads ;i++)
  {  
    threads[i] = std::thread(run,i);
  }

  for(uint32_t i=0; i<nbr_threads ;i++)
  {
    threads[i].join();
  }
  delete [] threads;
  */

  std::vector<User *> users;
  uint32_t nbr_threads = 8;
  std::thread * threads = new std::thread[nbr_threads]; 
   
  users.push_back(new Administrator("adm1",1,"adm1@usach.cl","2345"));
  users.push_back(new Administrator("adm2",2,"adm2@usach.cl","2345"));
  users.push_back(new Administrator("adm3",3,"adm3@usach.cl","2345"));
  users.push_back(new Administrator("adm4",4,"adm4@usach.cl","2345"));
  users.push_back(new Volunteer("vol1",5,"vol1@usach.cl","2345",1));
  users.push_back(new Volunteer("vol2",6,"vol2@usach.cl","2345",2));
  users.push_back(new Volunteer("vol3",7,"vol3@usach.cl","2345",3));
  users.push_back(new Volunteer("vol4",8,"vol4@usach.cl","2345",4));


  for(uint32_t i=0; i<nbr_threads ;i++)
  {  
    threads[i] = std::thread(&User::run,users[i]);
  }

  for(uint32_t i=0; i<nbr_threads ;i++)
  {
    threads[i].join();
  }
  delete [] threads;

  return 0;
}