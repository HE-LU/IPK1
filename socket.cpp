#include "socket.h"



class_socket::class_socket()
{
  sock = socket(AF_INET, SOCK_STREAM, 0);
  if(sock < 0) 
  {
    perror("ERROR: Cannot create socket!");
    exit(0);
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
  int sent = 0;
  int tmp = 0;
  while(sent < query.size())
  {
    tmp = send(sock, query.c_str()+sent, query.size()-sent, 0);
    if(tmp == -1)
      return -1;
    sent = sent + tmp;
  }
  return 0;

//   int total = 0;        // how many bytes we've sent
//   int bytesleft = query.size(); // how many we have left to send
//   int n;

//   while(total < query.size()) 
//   {
//     n = send(sock, query.c_str()+total, bytesleft, 0);
//     if (n == -1) { break; }
//     total += n;
//     bytesleft -= n;
//   }
//   return 0;
  
  
}

string class_socket::s_read()
{
  char buf[BUFF_SIZE+1];
  string ret="";
  int tmpres = 0;
  
  memset(buf, 0, sizeof(buf));
  while((tmpres = recv(sock, buf, BUFF_SIZE, 0)) > 0)
  {
    for (int i=0;i<tmpres;i++)
    {
      ret.push_back(buf[i]);
    }
    memset(buf, 0, sizeof(buf));
  }
  return ret;
}

