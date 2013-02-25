#include "socket.h"

class_socket::class_socket()
{
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if(sock < 0) 
  {
    perror("ERROR: Cannot create socket!");
    exit(-1);
  }
}

int class_socket::s_connect(string host,string port)
{
    struct addrinfo hints, *res, *tmp;
    int status;
    
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    if ((status = getaddrinfo(host.c_str(), port.c_str(), &hints, &res)) != 0) 
      return 2;

    
    for(tmp = res;tmp != NULL; tmp = tmp->ai_next) 
    {
      if (connect(sock, tmp->ai_addr, tmp->ai_addrlen) < 0) 
	continue;
      else
      {
	freeaddrinfo(res); 
	return 0;
      }
    } 
    freeaddrinfo(res);
    return 1;
}

int class_socket::s_disconnect()
{
    close(sock);
    return 0;
}

int class_socket::s_write(string query)
{
  unsigned int sent = 0;
  int tmp = 0;
  while(sent < query.size())
  {
    tmp = send(sock, query.c_str()+sent, query.size()-sent, 0);
    if(tmp == -1)
      return -1;
    sent = sent + tmp;
  }
  return 0;
}

// string class_socket::s_read()
// {
//   char buf[BUFF_SIZE+1];
//   string body="";
//   int tmpres = 0;
//   
//   string header;
//   memset(buf, 0, sizeof(buf));
//   unsigned header_end = -1; 
//   while(header_end >= header.size())
//   {
//     //cout << header << endl;
//     tmpres = recv(sock, buf, 1, 0);
//     header.push_back(buf[0]);
//     header_end = header.find("\r\n\r\n");
//     
//     memset(buf, 0, sizeof(buf));
//   }
//   
//   unsigned chunked_start = header.find("Transfer-Encoding: chunked");
//   
//   if(chunked_start < header.size())
//   {
//     memset(buf, 0, sizeof(buf));
//     while((tmpres = recv(sock, buf, BUFF_SIZE, 0)) > 0)
//     {
//       for (int i=0;i<tmpres;i++)
//       {
// 	body.push_back(buf[i]);
//       }
//       memset(buf, 0, sizeof(buf));
//     }
//   }
//   else
//   {
//     int read_size=1;
//     while(read_size != 0)
//     {
//       unsigned size_end = 0;
//       string size="";
//       
//       memset(buf, 0, sizeof(buf));
//       cout << "RADEK" << endl;
//       
//       while(size_end >= size.size())
//       {
// 	tmpres = recv(sock, buf, 1, 0);
// 	size.push_back(buf[0]);
// 	size_end = size.find("\r\n");	
// 	memset(buf, 0, sizeof(buf));
//       }
//       
//       string ss_string = size.substr(0,size_end);
//       cout << ss_string << endl;
//       
//       //cout << size << endl;
//       istringstream ss(size);
//       int read_size;
//       ss >> read_size;
//       
//       cout << read_size << endl;
//       
//       memset(buf, 0, sizeof(buf));
//       tmpres = recv(sock, buf, read_size, 0);
//       for (int i=0;i<tmpres;i++)
// 	{
// 	  body.push_back(buf[i]);
// 	}
//       memset(buf, 0, sizeof(buf));
//     
//       break;
//     }
//   }
//   
//   string ret = header + body;
//   return ret;
// }


string class_socket::s_read()
{
  char buf[BUFF_SIZE+1];
  string body="";
  int tmpres = 0;
  char buf_small;
  string header;
  
  memset(&buf_small, 0, sizeof(char));
  unsigned header_end = -1; 
  while(header_end >= header.size())
  {
    //cout << header << endl;
    tmpres = recv(sock, &buf_small, 1, 0);
    header.push_back(buf_small);
    header_end = header.find("\r\n\r\n");
    
    memset(&buf_small, 0, sizeof(char));
  }
  cout << header << endl;
  unsigned chunked_start = header.find("Transfer-Encoding: chunked");
  cout << chunked_start << endl;
  
  
  if(chunked_start > header.size())
  {
    cout << "NORMAL MODE" << endl;
    memset(buf, 0, sizeof(buf));
    while((tmpres = recv(sock, buf, BUFF_SIZE, 0)) > 0)
    {
      for (int i=0;i<tmpres;i++)
      {
	body.push_back(buf[i]);
      }
      memset(buf, 0, sizeof(buf));
    }
  }
  else
  {
    cout << "CHUNKED MODE" << endl;
    int read_size=1;
    while(1)
    {
      unsigned size_end = 0;
      string size="";
      
      
      memset(&buf_small, 0, sizeof(char));
      while(size_end >= size.size())
      {
	tmpres = read(sock, &buf_small, 1);
	size.push_back(buf_small);
	size_end = size.find("\r\n");	
	memset(&buf_small, 0, sizeof(char));
      }      
      
      string ss_string = size.substr(0,size_end);
     
      //cout << size << endl;
      stringstream ss(size);
      int read_size;
      ss << std::hex << ss_string;
      ss >> read_size;
      
       cout << "size: " << ss_string << " num: " << read_size << endl;
      
      
      //cout << read_size << endl;
      if(read_size == 0)
	break;
      
      memset(buf, 0, sizeof(buf));
      
      unsigned int myCounter = 0;
      while(myCounter != read_size){
	tmpres = read(sock, buf, read_size - myCounter);
	
	body += string(buf);
      
	myCounter += tmpres;
      }

      //cout << "size: " << ss_string << " num: " << read_size << "READ SIZE: " << tmpres << endl;
      //for (int i=0;i<tmpres;i++)
      //{
	//body.push_back(buf[i]);
      //}
      tmpres = read(sock, buf, 2);
    }
  }
  
   string ret = header + body;
  return ret;
}
