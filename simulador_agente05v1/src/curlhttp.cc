#include "../include/curlhttp.h"

CurlHTTP::CurlHTTP( ) {
}

CurlHTTP::~CurlHTTP( ) {
}


size_t CurlHTTP::WriteCallback(char *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

json CurlHTTP::sendGetRequest(const std::string &_host,const std::string &_port,const std::string &_resource, const std::string &_params,const std::string & token)
{
    std::string readBuffer("");
    json response = {{"content",{}},{"runOk",true}};
    char bufferSecurity[4000];

    std::string url = std::string(_host + std::string(":") + _port + _resource + _params);

    struct curl_slist *headers=NULL; // init to NULL is important

    headers = curl_slist_append( headers, "Content-Type: application/json");
    if(token.empty()==false){
        sprintf(bufferSecurity,"Authorization:Bearer %s",token.c_str());
        headers = curl_slist_append( headers,bufferSecurity);
        //std::cout<<"head:"<<bufferSecurity;
    }

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
        std::string curlerror(curl_easy_strerror(res));
        char bufferout[1000];
        sprintf(bufferout,"\nsendGetRequest - Error (%s) while performing call to (%s), waiting to try again\n",curlerror.c_str(),url.c_str());
        std::cerr <<bufferout;
        response["content"]=bufferout;
        response["runOk"]=false;
    }

    long http_code = 0;
    curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &http_code);
    if(http_code==200 || http_code==201){
        //char buffer[100000];
        //sprintf(buffer,"\n\n---------- sendGetRequest (%s) (%s) size:%d, %s ---------\n\n",url.c_str(),_params.c_str(),readBuffer.size(),readBuffer.c_str());
        //std::cout<<buffer;
        response["content"]={};
        response["runOk"]=false;
        //std::cout<<readBuffer<<" - "<<readBuffer.compare("null")<<std::endl;
        if(readBuffer.compare("null")!=0){
            response["content"]=json::parse(readBuffer);
            response["runOk"]=true;
        } 
    }
    else{
        response["content"]="error http ("+readBuffer+") ("+url+") ("+token+")";
        response["runOk"]=false;
    }
   
    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return response;
}

json CurlHTTP::sendPostRequest( const std::string &_host,const std::string &_port,const std::string &_resource,const json &_params, const bool &flgResponse,const std::string & token) {

    std::string readBuffer("");
    json response = {{"content",{}},{"runOk",true}};
    char bufferSecurity[2000];

    std::string json_str(_params.dump());

    std::string url = std::string(_host + std::string(":") + _port + _resource);
 
    //Define headers
    struct curl_slist *headers=NULL; // init to NULL is important
    headers = curl_slist_append( headers, "Content-Type: application/json");
    if(token.empty()==false){
        sprintf(bufferSecurity,"Authorization:Bearer %s",token.c_str());
        headers = curl_slist_append( headers,bufferSecurity);
    }


    CURL *curl = nullptr;
    CURLcode res;
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    //Agrega los headers definidos
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt (curl, CURLOPT_VERBOSE, 0L);

    size_t length = json_str.length();
    char *buffer = (char*)malloc(length);
    memcpy(buffer, json_str.c_str(), length);

    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, buffer);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, length);

    if(flgResponse==true) {
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    }
    else{
        response["content"]="sendPostRequest - it don't need response";
    }

    if( (res = curl_easy_perform(curl)) != CURLE_OK) {
        std::string curlerror(curl_easy_strerror(res));
        char bufferout[1000];
        sprintf(bufferout,"\nsendPostRequest - Error (%s) while performing call to (%s), waiting to try again\n",curlerror.c_str(),url.c_str());
        std::cerr << bufferout;
        response["content"]=bufferout;
        response["runOk"]=false;
    }
    else if(flgResponse==true){
        long http_code = 0;
        curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &http_code);
        if(http_code==200 || http_code==201){
             //readBuffer.replace(readBuffer.find(":null"), 5, ":\"\"");
             //std::string msg("\n\ncurl -------sendPostRequest ("+url+") ("+_params.dump()+") "+readBuffer+"+------------\n\n");
             //std::cout<<msg;

            response["content"]={};
            response["runOk"]=false;
            //std::cout<<readBuffer<<" - "<<readBuffer.compare("null")<<std::endl;
 
            if(readBuffer.compare("null")!=0){
                response["content"]=json::parse(readBuffer);
                response["runOk"]=true;
            } 
         }
         else{
            response["content"]="error http ("+readBuffer+")";
            response["runOk"]=false;
         }
    }    

    curl_easy_cleanup(curl);
    curl_global_cleanup();

    free(buffer);

    return( response );

}


json CurlHTTP::sendPutRequest( const std::string &_host,const std::string &_port,const std::string &_resource,const std::string &_params, const json &_data, const bool &flgResponse,const std::string & token) {

    std::string readBuffer("");
    json response = {{"content",{}},{"runOk",true}};
    char bufferSecurity[2000];

    std::string json_str(_data.dump());
    std::string url = std::string(_host + std::string(":") + _port + _resource + _params);

    //Define headers
    struct curl_slist *headers=NULL; // init to NULL is important
    headers = curl_slist_append( headers, "Content-Type: application/json");
    if(token.empty()==false){
        sprintf(bufferSecurity,"Authorization:Bearer %s",token.c_str());
        headers = curl_slist_append( headers,bufferSecurity);
    }


    CURL *curl = nullptr;
    CURLcode res;
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    //Agrega los headers definidos
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt (curl, CURLOPT_VERBOSE, 0L);

    size_t length = json_str.length();
    char *buffer = (char*)malloc(length);
    memcpy(buffer, json_str.c_str(), length);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, buffer);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, length);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");

    if(flgResponse==true) {
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    }
    else{
        response["content"]="sendPostRequest - it don't need response";
    }

    if( (res = curl_easy_perform(curl)) != CURLE_OK) {
        std::string curlerror(curl_easy_strerror(res));
        char bufferout[1000];
        sprintf(bufferout,"\nsendPutRequest - Error (%s) while performing call to (%s), waiting to try again\n",curlerror.c_str(),url.c_str());
        std::cerr << bufferout;
        response["content"]=bufferout;
        response["runOk"]=false;
    }
    else if(flgResponse==true){
        long http_code = 0;
        curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &http_code);
        if(http_code==200 || http_code==201){
            //readBuffer.replace(readBuffer.find(":null"), 5, ":\"\"");
             //std::string msg("\n\ncurl -------+"+readBuffer+"+------------\n\n");
             //std::cout<<msg;

            response["content"]={};
            response["runOk"]=false;
            //std::cout<<readBuffer<<" - "<<readBuffer.compare("null")<<std::endl;

            if(readBuffer.compare("null")!=0){
                response["content"]=json::parse(readBuffer);
                response["runOk"]=true;
            } 

         }
         else{
            response["content"]="error http ("+readBuffer+")";
            response["runOk"]=false;
         }
    } 

    curl_easy_cleanup(curl);
    curl_global_cleanup();

    free(buffer);

    return( response );

}


json CurlHTTP::sendDeleteRequest( const std::string &_host,const std::string &_port,const std::string &_resource,const std::string &_params, const bool &flgResponse,const std::string & token) {

    std::string readBuffer("");
    json response = {{"content",{}},{"runOk",true}};
    char bufferSecurity[2000];

    std::string url = std::string(_host + std::string(":") + _port + _resource + _params);

    //Define headers
    struct curl_slist *headers=NULL; // init to NULL is important
    headers = curl_slist_append( headers, "Content-Type: application/json");
    if(token.empty()==false){
        sprintf(bufferSecurity,"Authorization:Bearer  %s",token.c_str());
        headers = curl_slist_append( headers,bufferSecurity);
    }


    CURL *curl = nullptr;
    CURLcode res;
    curl_global_init(CURL_GLOBAL_ALL);
    curl = curl_easy_init();

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    //Agrega los headers definidos
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt (curl, CURLOPT_VERBOSE, 0L);
    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");

    if(flgResponse==true) {
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
    }
    else{
        response["content"]="sendPostRequest - it don't need response";
    }

    if( (res = curl_easy_perform(curl)) != CURLE_OK) {
        std::string curlerror(curl_easy_strerror(res));
        char bufferout[1000];
        sprintf(bufferout,"\nsendDeleteRequest - Error (%s) while performing call to (%s), waiting to try again\n",curlerror.c_str(),url.c_str());
        std::cerr << bufferout;
        response["content"]=bufferout;
        response["runOk"]=false;
    }
    else if(flgResponse==true){
        long http_code = 0;
        curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &http_code);
        if(http_code==200 || http_code==201){
            //readBuffer.replace(readBuffer.find(":null"), 5, ":\"\"");
             //std::string msg("\n\ncurl -------+"+readBuffer+"+------------\n\n");
             //std::cout<<msg;

            response["content"]={};
            response["runOk"]=false;
            //std::cout<<readBuffer<<" - "<<readBuffer.compare("null")<<std::endl;
 
            if(readBuffer.compare("null")!=0){
                response["content"]=json::parse(readBuffer);
                response["runOk"]=true;
            } 


         }
         else{
            response["content"]="error http ("+readBuffer+")";
            response["runOk"]=false;
         }
    }  

    curl_easy_cleanup(curl);
    curl_global_cleanup();

    return( response );

}

