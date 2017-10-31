#include <iostream>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <thread>

bool interrupt = false;

uint64_t actualTimestamp_to_milliseconds()
{
    auto tm = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(tm.time_since_epoch()).count();
}


std::string milliseconds_to_timestampString(const uint64_t & uptime)
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


void Run(std::chrono::milliseconds ms)
{
    std::chrono::time_point<std::chrono::system_clock> end,start;

    start = std::chrono::system_clock::now();
    end = start + ms; // this is the end point

    while(std::chrono::system_clock::now() < end && interrupt==false); // still less than the end?
    
    if(interrupt==true){ 
      char buffer[100];

      std::chrono::time_point<std::chrono::system_clock> end1,now1;
      now1 = std::chrono::system_clock::now();

      end1 = now1 - std::chrono::duration_cast<std::chrono::milliseconds>(start.time_since_epoch());
      
      sprintf(buffer, "(Run) FUI INTERRUMPIDO, DEBI CORRER POR %ld milliseconds, pero fui denetido a los %ld \n",
                      ms.count(), std::chrono::duration_cast<std::chrono::milliseconds>(end1.time_since_epoch()).count());
        std::cout<<buffer;
    }
}

void func_interrupt(std::chrono::milliseconds ms)
{
    std::chrono::time_point<std::chrono::system_clock> end;

    end = std::chrono::system_clock::now() + ms; // this is the end point

    while(std::chrono::system_clock::now() < end);
    interrupt=true;
}


 
int main()
{
    std::chrono::time_point<std::chrono::system_clock> p1, p2, p3;
 
    p2 = std::chrono::system_clock::now();
    p3 = p2 - std::chrono::hours(24);
 
    std::time_t epoch_time = std::chrono::system_clock::to_time_t(p1);
    std::cout << "fecha inicial : " << std::ctime(&epoch_time);

    std::time_t today_time = std::chrono::system_clock::to_time_t(p2);
    std::cout << "fecha actual: " << std::ctime(&today_time);
 
    std::cout << "hours desde fecha inicial : "
              << std::chrono::duration_cast<std::chrono::hours>(
                   p2.time_since_epoch()).count() 
              << '\n';

    std::cout <<  "milliseconds desde fecha inicial : "
              << std::chrono::duration_cast<std::chrono::milliseconds>(
                   p2.time_since_epoch()).count() 
              << '\n';


    std::cout << "yesterday, hours since epoch: "
              << std::chrono::duration_cast<std::chrono::hours>(
                   p3.time_since_epoch()).count() 
              << '\n';


    uint64_t uptime = 1508956562640; 
    using time_point = std::chrono::system_clock::time_point;
    time_point timepoint_uptime{std::chrono::duration_cast<time_point::duration>(std::chrono::milliseconds(uptime))};
    std::time_t t = std::chrono::system_clock::to_time_t(timepoint_uptime);    

     std::cout << "--->" << ctime(&t)<<std::endl;


    char sRep[100];
    strftime(sRep,sizeof(sRep),"%Y-%m-%d %H:%M:%S",localtime(&t));

    auto since_epoch = timepoint_uptime.time_since_epoch();
    auto s =  std::chrono::duration_cast<std::chrono::seconds>(since_epoch);
    since_epoch -= s;
    auto milli = std::chrono::duration_cast<std::chrono::milliseconds>(since_epoch);

    std::cout << "[" << sRep << "." << milli.count() << "]\n";


    std::cout << "------------->"<<milliseconds_to_timestampString( actualTimestamp_to_milliseconds() )<<std::endl;

    std::cout << "COMPARE "<<std::endl;
    std::string txt1("null");
    std::string txt2("null");
    std::string txt3("null1"); 

    std::cout <<txt1.compare(txt2)<<" - "
              <<txt1.compare(txt3)<<" - "
              <<txt2.compare(txt3)<<" - "
              <<txt1.compare("null")<<std::endl;


    std::thread * threads; 
    threads = new std::thread[2]; 

    today_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::cout << "fecha INI: " << std::ctime(&today_time);
    std::chrono::milliseconds ms(3000);
    std::chrono::milliseconds ms_interrupt(2000);

    threads[0] = std::thread(Run,ms);
    threads[1] = std::thread(func_interrupt,ms_interrupt);
   
    threads[0].join();
    threads[1].join();

    delete [] threads;

    today_time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::cout << "fecha FIN: " << std::ctime(&today_time);




}


