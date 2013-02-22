#include "parser.h"
#include <sstream>
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

class_parser::class_parser()
{
}

class_parser::class_parser(string url)
{
  string::const_iterator url_End = url.end();
  string::const_iterator url_Start = url.begin();
  string::const_iterator protocol_End = find(url_Start, url_End, ':');
  
  protocol = "";
  path = "";
  file = "";
  query = "";
  if(protocol_End != url_End)
  {
    string tmp = &*(protocol_End);
    if ((tmp.length() > 3) && (tmp.substr(0, 3) == "://"))
    {
      protocol = string(url_Start, protocol_End);
      protocol_End = protocol_End + 3;
    }
    else
      protocol_End = url.begin();
  }
  else
    protocol_End = url.begin();
  
  string::const_iterator port_Start = find(protocol_End, url_End, ':');
  string::const_iterator path_Start = find(protocol_End, url_End, '/');
  if(port_Start != url_End)
    host = string(protocol_End, port_Start);
  else if(path_Start != url_End)
    host = string(protocol_End, path_Start);
  else
    host = string(protocol_End, url_End);
  
  if(port_Start != url_End)
    port = string(port_Start+1, path_Start);
  else
    port = "80";
 
  string::const_iterator query_Start = find(path_Start, url_End, '?');
  string::const_iterator file_Start = find(path_Start, query_Start, '/');
  string::const_iterator iterator = path_Start;

  while(iterator != query_Start)
  {
    file_Start = iterator;
    iterator = find(iterator+1, query_Start, '/');
  }
  
  if (path_Start != url_End)
    path = string(path_Start, file_Start+1);
  if (file_Start != url_End)
    file = string(file_Start+1, query_Start);
  if (query_Start != url_End)
    query = string(query_Start, url_End);
}

string class_parser::get_path()
{
    return path;
}

string class_parser::get_host()
{
    return host;
}

int class_parser::get_port()
{
    int pom;
    stringstream buffer(port);
    buffer >> pom;
    if(pom < 0 || pom > 65535)
      pom = 80;
    return pom;
}

string class_parser::get_protocol()
{
    return protocol;
}

string class_parser::get_query()
{
    return query;
}

string class_parser::get_file()
{
    return file;
}