#include <curl/curl.h>
#include <fstream>
#include <sstream>
#include <iostream>
#include "../include/glob.h"
#include <thread>
#include <ctime>

// callback function writes data to a std::ostream
static size_t data_write(void* buf, size_t size, size_t nmemb, void* userp)
{
  if(userp)
  {
    std::ostream& os = *static_cast<std::ostream*>(userp);
    std::streamsize len = size * nmemb;
    if(os.write(static_cast<char*>(buf), len))
      return len;
  }

  return 0;
}

/**
 * timeout is in seconds
 **/
CURLcode curl_read(const std::string& url, std::ostream& os, long timeout = 30)
{
  CURLcode code(CURLE_FAILED_INIT);
  CURL* curl = curl_easy_init();

  if(curl)
  {
    if(CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &data_write))
    && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L))
    && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L))
    && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_FILE, &os))
    && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout))
    && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_URL, url.c_str())))
    {
      code = curl_easy_perform(curl);
    }
    curl_easy_cleanup(curl);
  }
  return code;
}

CURLcode curl_read1(const std::string& url, std::ostringstream& oss)
{
  curl_global_init(CURL_GLOBAL_ALL);
  long timeout = 30;
  CURLcode code(CURLE_FAILED_INIT);
  CURL* curl = curl_easy_init();
  std::string aaa;

  //param="123";

  if(curl)
  {
    if(CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &data_write))
    && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L))
    && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L))
    && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_FILE, &aaa))
    //&& CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout))
    && CURLE_OK == (code = curl_easy_setopt(curl, CURLOPT_URL, url.c_str())))
    {
      code = curl_easy_perform(curl);

    }
    curl_easy_cleanup(curl);
  }
  curl_global_cleanup();

 
  
  return code;
}


size_t WriteCallback(char *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string httpget(std::string url)
{
    std::string readBuffer("");
    //std::string url("http://localhost:3000/ability/");
    struct curl_slist *headers=NULL; // init to NULL is important
    headers = curl_slist_append( headers, "Content-Type: application/json");

    CURL *curl = nullptr;
    CURLcode res;
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    //Agrega los headers definidos
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt (curl, CURLOPT_VERBOSE, 0L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

    if( (res = curl_easy_perform(curl)) != CURLE_OK) {
        std::cerr << "curl_easy_perform() failed" << std::endl;
        std::cout<<"com::send - Error \""<<curl_easy_strerror(res)<<"\" while performing call to \""<<url<<"\", waiting to try again.\n";
    }

    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return readBuffer;
}


void run(int hilo, std::string ruta)
{ 
    auto start = std::chrono::system_clock::now();
    
    std::string resp = httpget(ruta);

    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;

    std::cout << "(" << hilo << ") resp:"<< resp.size()
              <<" elapsedTime: "<< elapsed_seconds.count() << "s"<<std::endl;
}  


int main(int argc,char* argv[])
{
  //curl_global_init(CURL_GLOBAL_ALL);
/*
  std::ofstream ofs("output.html");
  if(CURLE_OK == curl_read("http://google.com", ofs))
  {
    // Web page successfully written to file
  }*/
  /*
  std::ostringstream oss;
  if(CURLE_OK == curl_read("http://localhost:3000/ability", oss))
  {
    // Web page successfully written to string
    json resp = json::parse(oss.str());
    std::cout << resp[0] << std::endl;
  }*/
/*
  if(CURLE_OK == curl_read("http://google.com", std::cout))
  {
    // Web page successfully written to standard output (console?)
  }
*/

  //auto start = std::chrono::system_clock::now();

 /* uint32_t nbr_threads = (argv[1]==NULL) ? 10 :atoi(argv[1]); 
  std::thread * threads = new std::thread[nbr_threads];
  std::ostringstream oss;

  for(uint32_t i=0; i<nbr_threads ;i++)
  { 
    threads[i] = std::thread(curl_read1, "http://localhost:3000/ability",std::ref(oss));
  }*/

/*
  for(uint32_t i=0; i<nbr_threads ;i++)
  {
      threads[i].join();
  }*/

  //delete [] threads;


  //std::cout<<"salir:"<<oss.str()<<std::endl;

 // curl_global_cleanup();

  /*  curl_global_init(CURL_GLOBAL_ALL);

    CURL* easyhandle = curl_easy_init();
    

    struct curl_slist *headers=NULL; // init to NULL is important
    headers = curl_slist_append( headers, "Content-Type: application/json");


    curl_easy_setopt(easyhandle, CURLOPT_URL, "http://localhost:3000/ability");
    curl_easy_setopt(easyhandle, CURLOPT_VERBOSE, 1L);
    //curl_easy_setopt(easyhandle, CURLOPT_PROXY, "http://my.proxy.net");   // replace with your actual proxy
    //curl_easy_setopt(easyhandle, CURLOPT_PROXYPORT, 8080L);
    curl_easy_setopt(easyhandle, CURLOPT_WRITEFUNCTION, &WriteCallback);
    curl_easy_setopt(easyhandle, CURLOPT_WRITEDATA, &readBuffer);
    curl_easy_setopt(easyhandle, CURLOPT_HTTPHEADER, headers);
*/
  /* std::string readBuffer;
    std::string url("http://localhost:3000/ability/");
    struct curl_slist *headers=NULL; // init to NULL is important
    headers = curl_slist_append( headers, "Content-Type: application/json");

    CURL *curl = nullptr;
    CURLcode res;
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    //Agrega los headers definidos
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt (curl, CURLOPT_VERBOSE, 0L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

    if( (res = curl_easy_perform(curl)) != CURLE_OK) {
        std::cerr << "curl_easy_perform() failed" << std::endl;
        std::cout<<"com::send - Error \""<<curl_easy_strerror(res)<<"\" while performing call to \""<<url<<"\", waiting to try again.\n";
    }

    curl_easy_cleanup(curl);
    curl_global_cleanup();

    std::cout << readBuffer << std::endl;*/


    auto start = std::chrono::system_clock::now();

    uint32_t nbr_threads = (argv[1]==NULL) ? 10 :atoi(argv[1]); 
    std::thread * threads = new std::thread[nbr_threads]; 
    for(uint32_t i=0; i<nbr_threads ;i++)
    {  
      threads[i] = std::thread(run,i,"http://localhost:3000/ability/");
    }
  
    for(uint32_t i=0; i<nbr_threads ;i++)
    {
      threads[i].join();
    }

    delete [] threads;

    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::cout<<"fin"<<" elapsedTime: "<< elapsed_seconds.count() << "s"<<std::endl;

    return 0;
}