#include <bits/stdc++.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

void dummy_interact(int conn){
	//read
	char rbuf[64] = {};
	int n = read(conn, &rbuf, sizeof(rbuf)-1);
	if(n < 0){
		printf("Couldn't read\n");
		return;
	}
	else{
		printf("Client says : %s\n", rbuf);
	}

	//write
	char wbuf[] = "Hello from server";
	write(conn, wbuf, strlen(wbuf));
}

int main(){
	int ls = socket(AF_INET, SOCK_STREAM, 0); //creates a bare socket

	int reuse = 1;
	setsockopt(ls, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

	struct sockaddr_in addr = {}; //create struct to store address info
	addr.sin_family = AF_INET; //set ip version to IPv4
	addr.sin_port = htons(1234); //set the port to 1234. htons converts it to required 'big endian' format
	addr.sin_addr.s_addr = htonl(0); //set the ip address
	int err = bind(ls, (const struct sockaddr*)&addr, sizeof(addr)); //result is 0 if binding successful, -1 if failure
	if(err){
		printf("Error binding the socket");
	}
	
	// This actually creates the socket. 
	//2nd argument i.e. SOMAXCONN is the size of the queue. It doesn't matter because accept isn't a bottleneck
	err = listen(ls,SOMAXCONN); 
	if(err){
		printf("Error listening on the socket");
	}

	//Create the connection by actively listening for connections
	while(true){
		struct sockaddr_in client_addr = {}; //set up a struct which'll store client address's info
		socklen_t addr_len = sizeof(client_addr);
		int conn = accept(ls, (struct sockaddr*)&client_addr, &addr_len); //get a client connection from the queue associated with ls socket
		//It will return a connection socket with which we can interact with the client
		if(conn < 0){
			printf("Couldn't process a connections\n");
			continue;
		}
		dummy_interact(conn);
	}
}