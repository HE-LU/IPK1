#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sstream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h> 
#include <string>

#include "socket.h"
#include "parser.h"

using namespace std;

int main(int argc, char** argv)
{
	string address;
	address = *(argv+1);
	address = "http://www.fit.vutbr.cz:80";
	
	class_parser parse(address);
	
	cout << address << endl << endl;
	
	cout << "protocol: " << parse.get_protocol() << endl;
	cout << "host: " << parse.get_host() << endl;
	cout << "port: " << parse.get_port() << endl;
 	cout << "path: " << parse.get_path() << endl;
	cout << "query: " << parse.get_query() << endl;
	
	class_socket socket;
	socket.set_server_address(parse.get_host(),parse.get_port());
	if(!socket.s_connect())
	cout << "OK" << endl;
	
	

        return 0;
}