#include <iostream>

/*#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
*/
#include "../include/comm.h"

int main(int argc,char** argv) {
        //boost::property_tree::ptree document;
	//read_json(argv[1], document);

	std::string url("http://localhost");
	std::string port("3000");


	std::string params;
	json pt,paramjson;
	//params="";
	//json pt = Comm::sendGetRequest( url, port, "/ability/", params );
    //std::cout << "GET\n" << "list : "<< pt << ", size :"<< pt.size() <<std::endl;
	//std::cout << "GET\n" << "(0) id: "<< pt.at(0)["Id"] << ", ability :"<< pt.at(0)["ability"]  <<std::endl;
	
	//params="1";
	//pt = Comm::sendGetRequest( url, port, "/ability/", params );
    //std::cout << "GET\n" << "obj : "<< pt <<std::endl;
	//std::cout << "GET\n" << " id: "<< pt["Id"] << ", ability :"<< pt["ability"]  <<std::endl;
	
	//params="3";
	//pt = Comm::sendGetRequest( url, port, "/ability/", params );
    //std::cout << "GET\n" << "obj : "<< pt <<std::endl;
	//std::cout << "GET\n" << " id: "<< pt["Id"] << ", ability :"<< pt["ability"]  <<std::endl;
	
	//json paramjson={{"ability","carpintero"}};
	//pt = Comm::sendPostRequest( url, port, "/ability/", paramjson, true );
	//std::cout << "post\n" << "obj : "<< pt <<std::endl;

	//params="5";
	//paramjson={{"ability","CARPINTERO"}};
	//pt = Comm::sendPutRequest( url, port, "/ability/",params, paramjson, true );
	//std::cout << "put\n" << "obj : "<< pt <<std::endl;

	//params="6";
	//pt = Comm::sendDeleteRequest( url, port, "/ability/", params, true );
    //std::cout << "delete\n" << "resp : "<< pt <<std::endl;


	//params="";
	//pt = Comm::sendGetRequest( url, port, "/ability/", params );
    //std::cout << "GET\n" << "list : "<< pt << ", size :"<< pt.size() <<std::endl;
	

}
