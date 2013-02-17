#ifndef CLASS_PARSER_H
#define CLASS_PARSER_H
 
#include <string>
#include <algorithm>
 
using namespace std;
 
class class_parser
{
      private:
	string query;
	string host;
	string path;
	string protocol;
	string port;
	
      public:
	class_parser();
	class_parser(string url);
	string get_query();
	string get_path();
	string get_host();
	int get_port();
	string get_protocol();
};
 
#endif // CLASS_PARSER_H