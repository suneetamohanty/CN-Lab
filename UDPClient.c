#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT	    8080
#define MAXLINE     1024

int main() {
    int sockfd;             // socket identifier
    char buffer[MAXLINE];   // array to hold data coming from server
    char *hello = "Hello from client";  // string to send to server
    struct sockaddr_in	 servaddr;      // server address information

    // create a datagram socket of the UDP type
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");   // if sockfd is negative then print error
        exit(EXIT_FAILURE);                 // and exit the program with a failure code.
    }

    memset(&servaddr, 0, sizeof(servaddr));     // clear out the garbage from the server
                                                // address structure by filling with 0s.

    // fill address information for connecting to the server
    servaddr.sin_family = AF_INET;          // this is an IPv4 address
    servaddr.sin_port = htons(PORT);        // convert the PORT short to network order and store
    servaddr.sin_addr.s_addr = INADDR_ANY;  // any IP address on the host is fine

    int n, len;                             // variables created to store lengths of structures

    // there is no binding required (you can bind, if you like) as the client does not
    // need to announce a fixed port to the world. when send to runs, it assigns a random
    // port to the UDP socket. only the server address information is enough for the client
    // socket to connect to the

    sendto(sockfd, hello, strlen(hello),    // send a message (in hello) to the server
           0, (struct sockaddr *) &servaddr,// the server address structure is used
           sizeof(servaddr));               // send a size anytime you send a pointer to a structure
    printf("Hello message sent.\n");

    n = recvfrom(sockfd, buffer, MAXLINE,           // receive a message from server. this is a blocking
                 0, (struct sockaddr *) &servaddr,  // call and it will wait until a message arrives.
                 &len);                             // remember to pass the pointer to len (&len) here.
                                                    // we already have address information of the server
                                                    // in the servaddr structure, but we are passing it here
                                                    // again to store the same information because the function
                                                    // expects some arguments there.

    buffer[n] = '\0';                               // buffer[n] is the position after the last byte stored in
                                                    // buffer. since buffer has a string, it needs to be null terminated
    printf("Server : %s\n", buffer);                // print the string sent by the server.

    close(sockfd);                                  // close the socket to release resources.
    return 0;                                       // return from the main function with a success value.
}
