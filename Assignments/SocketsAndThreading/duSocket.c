#include "duSocket.h"

void Node();

// Like new ServerSocket in Java
int setupServerSocket(int portno)
{
  // Get a socket of the right type
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    printf("ERROR opening socket");
    exit(1);
  }

  // server address structure
  struct sockaddr_in serv_addr;

  // Set all the values in the server address to 0
  memset(&serv_addr, '0', sizeof(serv_addr)); 

  // Setup the type of socket (internet vs filesystem)
  serv_addr.sin_family = AF_INET;

  // Basically the machine we are on...
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  // Setup the port number
  // htons - is host to network byte order
  // network byte order is most sig bype first
  //   which might be host or might not be
  serv_addr.sin_port = htons(portno);

  // Bind the socket to the given port
  if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
    printf("ERROR on binding\n");
    exit(1);
  }

  return sockfd;
}

// Like new Socket in Java
int callServer(char* host, int portno)
{
  // Socket pointer
  int sockfd;
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) {
    fprintf(stderr,"ERROR opening socket\n");
    exit(0);
  }

  // server address structure
  struct sockaddr_in serv_addr;

  // Set all the values in the server address to 0
  memset(&serv_addr, '0', sizeof(serv_addr)); 

  // Setup the type of socket (internet vs filesystem)
  serv_addr.sin_family = AF_INET;

    // Setup the port number
  // htons - is host to network byte order
  // network byte order is most sig byte first
  //   which might be host or might not be
  serv_addr.sin_port = htons(portno);


  // Setup the server host address
  struct hostent *server;
  server = gethostbyname(host);
  if (server == NULL) {
    fprintf(stderr,"ERROR, no such host\n");
    exit(0);
  }
  memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);  /// dest, src, size

  // Connect to the server
  if (connect(sockfd,(struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
    printf("ERROR connecting\n");
    exit(0);
  }

  return sockfd;
}

// Like ss.accept() in Java
int serverSocketAccept(int serverSocket)
{
  int newsockfd;
  struct sockaddr_in cli_addr;
  socklen_t clilen = sizeof(cli_addr);

    // Wait for a call
  printf("waiting for a call...\n");
  newsockfd = accept(serverSocket, (struct sockaddr *) &cli_addr, &clilen);
  printf("connected\n");
  if (newsockfd < 0) {
    printf("ERROR on accept");
    exit(1);
  }
  return newsockfd;
}

// Write an int over the given socket
void writeInt(int x, int socket)
{
    int n = write(socket, &x, sizeof(x));
    if (n < 0) {
      printf("ERROR writing to socket\n");
      exit(1);
    }
}

// Read an int from the given socket
int readInt(int socket)
{
    int buffer;
    int n = read(socket, &buffer, sizeof(int));
    if(n < 0){
      printf("Error - unable to read from socket!\n");
      exit(0);
    }
    return buffer;
}