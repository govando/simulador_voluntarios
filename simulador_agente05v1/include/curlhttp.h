#ifndef _CURLHTTP_H_
#define _CURLHTTP_H_

#include "glob.h"
#include <curl/curl.h>

class CurlHTTP {

public:
    CurlHTTP( );
    ~CurlHTTP( );
   
    static json sendGetRequest(const std::string &_host,const std::string &_port,const std::string &_resource, const std::string &_params,const std::string & token="");
    static json sendPostRequest(const std::string &_host,const std::string &_port,const std::string &_resource, const json &_params, const bool &flgResponse=true, const std::string & token="");
    static json sendPutRequest(const std::string &_host,const std::string &_port,const std::string &_resource,const std::string &_params, const json &_data, const bool &flgResponse=true, const std::string & token="");
    static json sendDeleteRequest( const std::string &_host,const std::string &_port,const std::string &_resource,const std::string &_params, const bool &flgResponse=true, const std::string & token=""); 

private:
    static size_t WriteCallback(char *contents, size_t size, size_t nmemb, void *userp);

};
#endif
