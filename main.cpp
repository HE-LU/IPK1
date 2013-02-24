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

	bool recursive = true;
	for(int i=0; (i<4 && recursive) ;i++)
	{
	  class_parser parse(address);
	  
	  
	         cout << address << endl << endl;
       
       cout << "protocol: " << parse.get_protocol() << endl;
       cout << "host: " << parse.get_host() << endl;
       cout << "port: " << parse.get_port() << endl;
       cout << "path: " << parse.get_path() << endl;
       cout << "file: " << parse.get_file() << endl;
       cout << "query: " << parse.get_query() << endl;
	  
	  class_socket socket;
	  int tmp = socket.s_connect(parse.get_host(),parse.get_port());
	  if( tmp == 0)
	    cout << "OK" << endl;
	  else if ( tmp == 2)
	    cout << "FAIL IN GETADDRINFO" << endl;
	  else
	    cout << "FAIL" << endl;
	
	  // GET Query
	  string query;
	  query += "GET " + ((parse.get_path() == "") ? "/" : parse.get_path()) + parse.get_file() + " HTTP/1.0\r\nHost:";
	  query += parse.get_host() + "\r\nUser-Agent: HTMLGET 1.0\r\n\r\n"; 
	   
	  cout << endl << query << endl;
	  //Send query
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

	  cout << code << endl;
	  if(code == "200")
	  {
	    string re_head = response.substr(re_Start, re_html_End+4);
	    string re_body = response.substr(re_html_End+4,re_End);
	    
	    ofstream file(((parse.get_file_name() == "") ? "index.html" : parse.get_file_name()).c_str());
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