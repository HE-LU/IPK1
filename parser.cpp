#include "parser.h"
#include <sstream>

class_parser::class_parser()
{
}

class_parser::class_parser(string url)
{
    typedef string::const_iterator iterator_t;
    iterator_t url_End = url.end();

    //start
    iterator_t query_Start = find(iterator_t(url.begin()), url_End, '?');

    //protocol
    iterator_t protocol_Start = url.begin();
    iterator_t protocol_End = find(protocol_Start, url_End, ':');

    protocol = "";
    if (protocol_End != url_End)
    {
        string tmp = &*(protocol_End);
        if ((tmp.length() > 3) && (tmp.substr(0, 3) == "://"))
        {
            protocol = string(protocol_Start, protocol_End);
            protocol_End = protocol_End + 3;
        }
        else
            protocol_End = url.begin();
    }
    else
        protocol_End = url.begin();

    //host
    iterator_t host_Start = protocol_End;
    iterator_t path_Start = find(host_Start, url_End, '/');

    iterator_t host_End;
    if(path_Start != url_End)
      host_End = find(protocol_End, path_Start, ':');
    else
      host_End = find(protocol_End, query_Start, ':');
      
    host = string(host_Start, host_End);

    // port
    if ((host_End != url_End) && ((&*(host_End))[0] == ':'))
    {
        host_End++;

	iterator_t port_End;
	if(path_Start != url_End)
	  port_End = path_Start;
	else
	  port_End = query_Start;
    
        port = string(host_End, port_End);
    }

    // path
    if (path_Start != url_End)
        path = string(path_Start, query_Start);

    // query
    if (query_Start != url_End)
        query = string(query_Start, iterator_t(url.end()));

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