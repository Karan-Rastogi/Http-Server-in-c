#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

int main() {

  int sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd < 0) {
    perror("socket error..");
    exit(-1);
  }

  int yes = 1;

  setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes);

  struct addrinfo hint, *res;

  hint.ai_family = AF_INET;
  hint.ai_socktype = SOCK_STREAM;
  hint.ai_flags = AI_PASSIVE;

  getaddrinfo(NULL, "8080", &hint, &res);

  int bind_result = bind(sockfd, res->ai_addr, res->ai_addrlen);
  if (bind_result < 0) {
    perror("Binding error..");
    exit(-1);
  }

  // listen for incoming connections

  int listen_result = listen(sockfd, 20);

  if (listen_result < 0) {
    perror("Listen error");
    exit(-1);
  }

  struct sockaddr_storage their_addr;
  socklen_t addr_size;
  int new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);

  int max_len = 1000;
  char received_request[max_len];
  memset(received_request, 0, max_len);
  int num_bytes = recv(new_fd, received_request, max_len, 0);

  if (strncmp(received_request, "GET", 3) == 0) {
    printf("Received HTTP GET request!\n");

    // we response to the HTTP GET request

    char *status_line = "HTTP/1.1 200 OK \r\n";
    int send_bytes = send(new_fd, status_line, strlen(status_line), 0);
    printf("send_bytes = %d\n", send_bytes);

    char *headers = "Contect-Type: 10\r\n\r\n";
    send_bytes = send(new_fd, headers, strlen(headers), 0);
    printf("send_bytes: %d\n", send_bytes);

    // FILE *index_file = fopen("index.html", "r");
    // char c;
    // while ((c = getc(index_file)) != EOF){
    // sent_bytes = send(new_fd, &c, 1, 0);
    //}

    // when we send html file we remove this till here
    char *response_body = "<h1>Karan</h1> <br> <br> <h1>This is just a HTTP "
                          "server for rendering the html file</h1>\r\n";
    send_bytes = send(new_fd, response_body, strlen(response_body), 0);
    printf("send_bytes= %d\n", send_bytes);

    // to here

  } else {
    printf("Received Non-GET request. Ignoring... \n");
    close(new_fd);
    close(sockfd);
    exit(0);
  }

  close(new_fd);
  close(sockfd);
}
