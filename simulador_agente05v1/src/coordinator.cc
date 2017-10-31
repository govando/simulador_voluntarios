#include "../include/coordinator.h"


Coordinator::Coordinator(const std::string &_name, const json &_coord): Agent (_name,_coord)  {
	std::cout<<"t("<<this->time()<<") "<<name()<<" was created "<<std::endl;
}

Coordinator::~Coordinator() {
    ;
}

void Coordinator::inner_body(void) {
 int i=0;
 while(i<4){
 	std::cout << "i=" << i << " - "<< name() <<" - "<< this->_id << std::endl;
 	i=i+1;
 }
}