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

void class_socket::set_server_address(string host,int port)
{
  server = gethostbyname(host.c_str());
  if (server == NULL) 
  {
    fprintf(stderr,"ERROR, no such host\n");
    exit(0);
  }
  bzero((char *) &server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;  
  bcopy((char *)server->h_addr, (char *)&server_addr.sin_addr.s_addr, server->h_length);
  server_addr.sin_port = htons(port);
}

int class_socket::s_connect()
{
  if (connect(sock,(struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) 
    return -1;
  else
    return 0;
}

int class_socket::s_disconnect()
{
    close(sock);
    return 0;
}

int class_socket::s_write(char* query)
{
  unsigned int sent = 0;
  int tmp = 0;
  while(sent < strlen(query))
  {
    tmp = send(sock, query+sent, strlen(query)-sent, 0);
    if(tmp == -1){
      return -1;
    }
    sent = sent + tmp;
  }
  return 0;
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

