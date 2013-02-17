#include <iostream>
#include <stdio.h>
#include <string.h>
#include <sstream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <string>

#include "socket.h"
#include "parser.h"

using namespace std;

int main(int argc, char** argv)
{
// 	int port;
// 	struct hostent *server_ip;
	
	string address;
	address = *(argv+1);
	address = "http://www.hovno.cz:8080/blabla.php?page=slovnik";
	
	class_parser parse(address);
	
	
	cout << address << endl << endl;
	
	cout << "protocol: " << parse.get_protocol() << endl;
	cout << "host: " << parse.get_host() << endl;
	cout << "port: " << parse.get_port() << endl;
 	cout << "path: " << parse.get_path() << endl;
	cout << "query: " << parse.get_query() << endl;
	
	
	
// 	if (server_ip == NULL) 
// 	{
// 	  cout << "No such host!" << endl;
// 	}
// 
// 	class_socket socket;
	
        return 0;
}