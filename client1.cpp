#include <bits/stdc++.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main(){
	int sock = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in addr = {};
	addr.sin_family = AF_INET;
	addr.sin_port = htons(1234);
	addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // Like localhost i.e. 127.0.0.1

	int err = connect(sock, (const struct sockaddr*)&addr, sizeof(addr));
	if(err){
		printf("Couldn't connect to the server\n");
	}

	char wbuf[64] = "Hello from client";
	int n = write(sock, &wbuf, sizeof(wbuf));
	if(n < 0){
		printf("Couldn't send data to the server\n");
	}

	char rbuf[64] = {};
	n = read(sock, &rbuf, sizeof(rbuf));
	if(n < 0){
		printf("Couldn't read from the server\n");
		return 0;
	}
	printf("Server says : %s\n", rbuf);
	close(sock);
}
