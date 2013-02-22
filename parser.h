#ifndef CLASS_PARSER_H
#define CLASS_PARSER_H
 
#include <string>
#include <algorithm>
 
using namespace std;
 
class class_parser
{
      private:
	string protocol;
	string host;
	string port;
	string path;
	string file;
	string query;
	
      public:
	class_parser();
	class_parser(string url);
	string get_protocol();
	string get_host();
	int get_port();
	string get_path();
	string get_file();
	string get_query();
};
 
#endif // CLASS_PARSER_H