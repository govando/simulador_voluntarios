#include "../include/comm.h"

#include <thread>
#include <ctime>

std::random_device device;
std::mt19937 rn_generator(device());
	
std::string url("http://localhost");
std::string port("3000");
std::atomic<std::string*> _response1;

void run(int i)
{

	auto start = std::chrono::system_clock::now();

	std::string params;
	json pt,paramjson;
	params="";
	pt = Comm::sendGetRequest( url, port, "/ability/", params );

	auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    //std::time_t end_time = std::chrono::system_clock::to_time_t(end); 
    //std::cout << "finished computation at " << std::ctime(&end_time)
    //          << "elapsed time: " << elapsed_seconds.count() << "s\n";

    std::cout << "GET (" << i << ") ResponseSize:"<< pt.size() 
              <<" elapsedTime: "<< elapsed_seconds.count() << "s"<<std::endl;
}

int main(int argc,char* argv[]) {

	auto start = std::chrono::system_clock::now();

	uint32_t nbr_threads = atoi(argv[1]); 
	std::thread * threads = new std::thread[nbr_threads];
	for(uint32_t i=0; i<nbr_threads ;i++)
	{
		threads[i] = std::thread(run, i);
	}

	for(uint32_t i=0; i<nbr_threads ;i++)
    {
        threads[i].join();

    }
    delete [] threads;

	auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;

	std::cout<<"fin"<<" elapsedTime: "<< elapsed_seconds.count() << "s"<<std::endl;
	
}