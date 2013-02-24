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
  {
    iterator = file_Start;
    iterator = find(iterator+1, query_Start, ' ');
    if(iterator == query_Start)
      file = string(file_Start+1, query_Start); 
    else
    {  
      file = string(file_Start+1, iterator);
      while(iterator != query_Start)
      {
	file_Start = iterator;
	iterator = find(iterator+1, query_Start, ' ');
	file += "%20";
	file += string(file_Start+1, iterator);
      }
    }
  }
  if (query_Start != url_End)
  {
    iterator = query_Start;
    iterator = find(iterator+1, url_End, ' ');
    if(iterator == url_End)
      query = string(query_Start+1, url_End); 
    else
    {  
      query = string(query_Start+1, iterator);
      while(iterator != url_End)
      {
	query_Start = iterator;
	iterator = find(iterator+1, url_End, ' ');
	query += "+";
	query += string(query_Start+1, iterator);
      }
    }
  }
}
/*
class_parser::class_parser(string url)
{
  unsigned url_End = url.size();
  unsigned url_Start = 0;
  unsigned protocol_End = url.find(":");
  
  protocol = "";
  path = "";
  file = "";
  query = "";
  if(protocol_End != url_End)
  {
    string tmp = url.substr(protocol_End,url_End);
    if ((tmp.length() > 3) && (tmp.substr(0, 3) == "://"))
    {
      protocol = url.substr(url_Start, protocol_End);
      protocol_End += 3;
    }
    else
      protocol_End = url_Start;
  }
  else
    protocol_End = url_Start;
   
  unsigned port_Start = url.find(":",protocol_End);
  unsigned path_Start = url.find("/",protocol_End);
  if(port_Start < url_End)
    host = url.substr(protocol_End, port_Start - protocol_End);
  else if(path_Start < url_End)
    host = url.substr(protocol_End, path_Start - protocol_End);
  else
    host = url.substr(protocol_End, url_End);
  
  if(port_Start < url_End)
    port = url.substr(port_Start+1, path_Start - port_Start);
  else
    port = "80";
 
  unsigned query_Start = url.find("?",path_Start);
  if(query_Start > url_End)
    query_Start = url_End;
  unsigned file_Start = url.find("/",path_Start,query_Start-path_Start);
  
  file_Start = url.find_last_of("/",query_Start);

  if (path_Start != url_End)
    path = url.substr(path_Start, file_Start-path_Start+1);
  if (file_Start != url_End)
    file = url.substr(file_Start+1, query_Start-file_Start-1);
  if (query_Start != url_End)
    query = url.substr(query_Start, url_End-query_Start);
}*/

string class_parser::get_path()
{
    return path;
}

string class_parser::get_host()
{
    return host;
}

string class_parser::get_port()
{
    return port;
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

string class_parser::get_file_name()
{
    string tmp = file;
    
    size_t pos = 0;
    while((pos = file.find("%20",pos+1)) < file.size())
      tmp.replace(pos,3," ");
    return tmp;
}
