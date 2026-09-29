//multiple request over a single connexion
//one request at a time ie not concurrent
//simple binary protocol + parser

#include <bits/stdc++.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdio>
#include <unistd.h>
#include <cstdlib>
#include "format.cpp"
#include <cassert>

static int on = 1, off = 0;
static const int MAX_BUF = 4096;

int one_request(int c_fd){
	int len = parse_len(c_fd);
	if(len < 0){
		return -1;
	}
	assert(len < MAX_BUF);

	char rbuf[MAX_BUF] = {};
	int n = read_all(c_fd, rbuf, len);
	if(n < 0){
		return -1;
	}

	printf("Client says : %s\n",rbuf); 

	char wbuf[4 + MAX_BUF] = {};
	char msg[] = "Hello from the server";
	int sz = strlen(msg);
	memcpy(wbuf+4, msg, sz);
	write_len(sz, wbuf);

	n = write_all(c_fd, wbuf, sz+4);
	if(n < 0){
		return -1;
	}

	return 0;
}

int main(){
	int fd = socket(AF_INET, SOCK_STREAM, 0);
	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

	struct sockaddr_in addr = {};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(1234);
	addr.sin_addr.s_addr = htonl(0);
	int n = bind(fd, (const struct sockaddr*)&addr, sizeof(addr));
	if(n){
		perror("Couldn't bind the socket\n");
	}

	n = listen(fd, 10); // tells the OS that if there's some incoming request on this socket i.e. the i/p and port, 
	//then make a TCP connection with that client and put that connection on the queue. 
	//the queue cannot be greater than 10 (passed parameter).
	if(n){
		perror("Couldn't listen on the socket\n");
	}
	printf("Server is listening\n");
	while(true){
		struct sockaddr_in c_addr = {};
		socklen_t len = sizeof(c_addr);
		int c_fd = accept(fd, (struct sockaddr*)&c_addr, &len);
		if(c_fd < 0){
			perror("Couldn't connect to the client\n");
			continue;
		}

		while(true){
			int err = one_request(c_fd);
			if(err){
				break;
			}
		}
		printf("Connection closed\n");
		close(c_fd);
	}
}