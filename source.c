#include <stdio.h> //stdin and stdout
#include <string.h> //string manipulation macros i.e. strncpy(), etc...
#include <errno.h> //for socket errors
#include <stdlib.h> //used for exit(), free(), malloc(), etc...
#include <arpa/inet.h> //to connect to ip address
#include <ctype.h> //checking types of data isint(), isdigit()
#include <sys/socket.h> //to initialise internet sockets

void help_menu()
{
	printf("\nUsage:");
	printf("\n ./scanner -s <starting port> -e <ending port>\n");
	printf("\nScan a range of ports against your localhost.\n");
	printf("\nOptions:");
	printf("\n -s <starting port>     range starting port");
	printf("\n -e <ending port>       range ending port");
	printf("\n -h                     display this help\n");
}

int main (int argc, char **argv)
{
	char hostname[100]="127.0.0.1"; //only works for local machine
	int scan, i, sock, start, end; // declare integers
	struct sockaddr_in sa; //declare sockaddr_in (ipv4) as "sa" (man 3 socket.h)
	start = -1;
	end = -1;
	sa.sin_addr.s_addr = inet_addr(hostname); // man socketaddr ; declaring ipv4 address
	sa.sin_family = AF_INET; // declaring address family as internet

	if (argc==1)
	{
		help_menu();
	}
	else
	{
		while( *++argv ) //loop until (null) pointer
		{
			if ((*argv)[0] == '-')
			{
				switch((*argv)[1])
				{
					default:
						printf("\nUnknown option: \'-%c\' \nTry \'./scanner -h\' for more information.\n", (*argv)[1]);
						break;
					case 's':
						*++argv;
						if(argv[0]==NULL)
						{
							help_menu();
							break;
						}
						else
						{
							start = atoi(argv[0]);
							if ( start < 0 )
							{
								start = 0;
							}
						}
						break;
					case 'e':
						*++argv;
						if(argv[0]==NULL)
						{
							help_menu();
							break;
						}
						else
						{
							end = atoi(argv[0]);
							if ( end < 0 )
							{
								end = 0;
							}
						}
						break;
					case 'h':
						help_menu();
						break;
				}
			}
			else
			{
				break;
			}
		}
		if (start > -1 && end > -1 && start <= end)
		{
			printf("\nStarting a port scan with a range from %d to %d.\n\n", start, end);
			for( i = start ; i <= end ; i++)
			{
				sa.sin_port = htons(i); //htons() from arpa/inet.h (man htons), converts unsigned short integer (i) to network byte order; declares sin_port as the port from i
				sock = socket(AF_INET, SOCK_STREAM, 0); //(int domain, int type, int protocol), create socket of type internet (ipv4, bi-directional byte stream, 0=default protocol for AF_INET)

				if (sock < 0) // socket is OK if returns val > -1 (as file descriptor); if FAILED, returns val < 0, and errno (#include errno.h) is set to show the error
				{
					 printf("\nSocket error");
					 exit(1);
				}

				scan = connect(sock, (struct sockaddr*)&sa , sizeof sa); // (socket, struct pointer *address, length of struct)
				if ( scan < 0 ) // connect() returns 0 if OK and -1 if FAILED and errno is set to show error
				{
					//printf("%-5d %s\r\n" , i, strerror(errno));
					fflush(stdout); //flushing i/o buffer, useful for flushing scanf and writing to files buffer
				}
				else
				{
					printf("%-5d open\n", i);
				}
				close(sock); //must close an open file descriptor so that it can be opened later again
			}
		}
	}
}
