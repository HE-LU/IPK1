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
#include <fstream>


#include "socket.h"
#include "parser.h"


using namespace std;

int main(int argc, char** argv)
{
	string address;
	address = *(argv+1);
// 	address = "http://www.seznam.cz/st/img/2011//logo.png";
	
	bool recursive = true;
	for(int i=0; (i<4 && recursive) ;i++)
	{
	  cout << endl << "ZACATEK!!!!"<<endl;
	  class_parser parse(address);
	  int pomport = parse.get_port();
	  
// 	  cout << address << endl << endl;
// 	  
// 	  cout << "protocol: " << parse.get_protocol() << endl;
// 	  cout << "host: " << parse.get_host() << endl;
// 	  cout << "port: " << pomport << endl;
// 	  cout << "path: " << parse.get_path() << endl;
//	  cout << "file: " << parse.get_file() << endl;
// 	  cout << "query: " << parse.get_query() << endl;
	  
	  class_socket socket;
	  socket.set_server_address(parse.get_host(),pomport);
	  if(!socket.s_connect())
	    cout << "OK" << endl;
	  else
	    cout << "FAIL" << endl;
	
	
	
	  // GET Query
	  string query_string;
	  query_string += "GET " + ((parse.get_path() == "") ? "/" : parse.get_path()) + parse.get_file() + " HTTP/1.0\r\nHost:";
	  query_string += parse.get_host() + "\r\nUser-Agent: HTMLGET 1.0\r\n\r\n"; 
	    
	  //Send query
	  char* query = (char*)query_string.c_str();
// 	  cout << "-------------------------" << endl;
//   	  cout << endl << endl << query << endl << endl; // DEBUG
// 	  cout << "-------------------------" << endl;
	  socket.s_write(query);
	
	  //Analyze response
	  string response;
	  response = socket.s_read();
	  
	  socket.s_disconnect();
	  
	  unsigned re_Start = 0;
	  unsigned re_End = response.size();
	  unsigned re_html_End = response.find("\r\n\r\n");
	  unsigned re_code = response.find(" ");
	  
	  string code = response.substr(re_code+1,3);

	  if(code == "200")
	  {
	    string re_head = response.substr(re_Start, re_html_End+4);
	    string re_body = response.substr(re_html_End+4,re_End);
	    
// 	    cout << re_Start << " " << re_End << " " << re_html_End << endl << endl; // DEBUG
// 	    cout << re_head << endl<< endl<< endl<< endl;
	    
	    ofstream file(((parse.get_file() == "") ? "index.html" : parse.get_file()).c_str());
	    file << re_body;
	    file.close();
	    recursive = false;
	  }
	  else if(code == "301" || code == "302")
	  {
	    unsigned re_loc_start = response.find("Location: ");
	    unsigned re_loc_end = response.find("\n",re_loc_start);
	    
	    string location = response.substr(re_loc_start+10,re_loc_end-re_loc_start-11);
	    address.assign(location);
	    cout << endl << address << endl;
	    
	  }
	  else if(code.at(0) == '4' || code.at(0) == '5')
	  {
	    fprintf(stderr, "4xx or 5xx error ocured!");
	  }
	  else
	  {
	    fprintf(stderr, "shit happened!");
	  }
	}
	
	
	

        return 0;
}