/* server_oob_select.c
 * receive normal or oob tcp packets with select syscall
   from client and print them */

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <unistd.h>

//server port number
#define MYPORT 4000
//socket's listening queue length
#define BACKLOG 10

int main(int argc, char *argv[])
{
	int sock_fd, new_fd;
	struct sockaddr_in my_addr;
	struct sockaddr_in their_addr;
	int sin_size, n;
	char buf[100];
	fd_set rset, xset;
	
	//create socket
	if((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		perror("socket");
		return 1;
	}
	
	bzero(&my_addr, sizeof(my_addr));
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(MYPORT);
	my_addr.sin_addr.s_addr = INADDR_ANY;
	
	//bind socket to my_addr
	if(bind(sock_fd, (struct sockaddr*)&my_addr, sizeof(struct sockaddr)) == -1)
	{
		perror("bind");
		close(sock_fd);
		return 1;
	}
	
	//listen on socket
	if(listen(sock_fd, BACKLOG) == -1)
	{
		perror("listen");
		close(sock_fd);
		return 1;
	}
	
	while(1)
	{
		sin_size = sizeof(struct sockaddr_in);
		if((new_fd = accept(sock_fd, (struct sockaddr*)&their_addr, &sin_size)) == -1)
		{
			perror("accept");
			continue;
		}
		printf("server: got connection from [%s]\n", inet_ntoa(their_addr.sin_addr));
		
		FD_ZERO(&rset);
		FD_ZERO(&xset);
			
		for (;;)
		{
			FD_SET(new_fd, &rset);
			FD_SET(new_fd, &xset);
			
			select(new_fd+1, &rset, NULL, &xset, NULL);
			
			if (FD_ISSET(new_fd, &xset))
			{
				bzero(buf, sizeof(buf));
				n = recv(new_fd, buf, sizeof(buf)-1, MSG_OOB);
				if (n < 0)
					printf("OOB recv error: errno=%d\n", errno);
				printf("recv %d OOB bytes: %s\n", n, buf);
			}

			if (FD_ISSET(new_fd, &rset))
			{
				bzero(buf, sizeof(buf));
				if((n = recv(new_fd, buf, sizeof(buf)-1, 0)) == 0)
				{
					printf("recv EOF\n");
					exit(0);
				}
				buf[n] = 0;
				printf("Recv: %d bytes: %s\n", n, buf);
			}
		}
		close(new_fd);
	}

	close(sock_fd);
	return 0;
}
