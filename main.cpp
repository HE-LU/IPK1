#include <fstream>
#include <iostream>
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

	  class_socket socket;
	  int tmp = socket.s_connect(parse.get_host(),parse.get_port());
	  if ( tmp == 0)
	  {}
	  else if ( tmp == 2)
	  {
	    fprintf(stderr, "FAIL IN GETADDRINFO\n");
	    exit(-1);
	  }
	  else
	  {
	    fprintf(stderr, "SOCKET FAIL\n");
	    exit(-1);
	  }
	  
	  // GET Query
	  string query;
	  query += "GET " + ((parse.get_path() == "") ? "/" : parse.get_path()) + parse.get_file() + " HTTP/1.0\r\nHost:";
	  query += parse.get_host() + "\r\nUser-Agent: HTMLGET 1.0\r\n\r\n"; 
	   
	  //Send query
	  socket.s_write(query);
	   
	  //Analyze response
	  string response;
	  response = socket.s_read();
	  
	  cout << "ALALALALAALALAL" << endl;
	  
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
	    
	    if(location[0] == '/')
	    {
	      address = parse.get_protocol() + parse.get_host() + ":" + parse.get_port() + location;
	    }
	    else
	      address.assign(location);
	  }
	  else if(code.at(0) == '4' || code.at(0) == '5')
	  {
	    fprintf(stderr, "4xx or 5xx error ocured!\n");
	    recursive = false;
	  }
	  else
	  {
	    fprintf(stderr, "we got xxx error here!\n");
	    recursive = false;
	  }
	}
	

        return 0;
}

