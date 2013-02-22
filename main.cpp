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
// 	address = "http://www.seznam.cz/st/img/2011//logo.png";
	
	class_parser parse(address);
	int pomport = parse.get_port();
	
	cout << address << endl << endl;
	
	cout << "protocol: " << parse.get_protocol() << endl;
	cout << "host: " << parse.get_host() << endl;
	cout << "port: " << pomport << endl;
 	cout << "path: " << parse.get_path() << endl;
	cout << "file: " << parse.get_file() << endl;
	cout << "query: " << parse.get_query() << endl;
	
	class_socket socket;
	socket.set_server_address(parse.get_host(),pomport);
	if(!socket.s_connect())
	cout << "OK" << endl;
	
	// GET Query
	char *query;
	char *getpage = (char*) parse.get_path().c_str();
	char *tpl = "GET /%s HTTP/1.0\r\nHost: %s\r\nUser-Agent: %s\r\n\r\n";
	if(getpage[0] == '/'){
	  getpage = getpage + 1;
	  fprintf(stderr,"Removing leading \"/\", converting %s to %s\n", parse.get_path().c_str(), getpage);
	}
	// -5 is to consider the %s %s %s in tpl and the ending \0
	query = (char *)malloc(parse.get_host().size()+strlen(getpage)+strlen("HTMLGET 1.0")+strlen(tpl)-5);
	sprintf(query, tpl, getpage, parse.get_host().c_str(), "HTMLGET 1.0");
	
	cout << "Query: "<<endl << query << endl;
	
	 //Send the query to the server
	int sent = 0;
	int tmpres = 0;
	while(sent < strlen(query))
	{ 
	  tmpres = send(socket.sock, query+sent, strlen(query)-sent, 0);
	  if(tmpres == -1){
	    perror("Can't send query");
	    exit(1);
	  }
	  sent += tmpres;
	}
	
	//now it is time to receive the page
	char buf[10000+1];
	memset(buf, 0, sizeof(buf));
	int htmlstart = 0;
	char * htmlcontent;
	FILE* soubor;
	soubor = fopen("soubor.txt", "w");
	
	
	while((tmpres = recv(socket.sock, buf, 10000, 0)) > 0){
	  if(htmlstart == 0)
	  {
	    /* Under certain conditions this will not work.
	    * If the \r\n\r\n part is splitted into two messages
	    * it will fail to detect the beginning of HTML content
	    */
	    htmlcontent = strstr(buf, "\r\n\r\n");
	    if(htmlcontent != NULL){
	      htmlstart = 1;
	      htmlcontent += 4;
	    }
	  }else{
	    htmlcontent = buf;
	  }
	  if(htmlstart){
	    fprintf(soubor, htmlcontent);
	  }
	  memset(buf, 0, tmpres);
	}
	if(tmpres < 0)
	{
	  perror("Error receiving data");
	}
	
	fclose(soubor);
	
	
	

        return 0;
}