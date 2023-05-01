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
	int scan, i, sock, start, end, switch_flag; // declare integers
	struct sockaddr_in sa; //declare sockaddr_in (ipv4) as "sa" (man 3 socket.h)
	start = -1; // initialise so the variables don't get a random number in the beginning
	end = -1;
	switch_flag=-1; // setting flag for switch case so help lines don't display again
	sa.sin_addr.s_addr = inet_addr(hostname); // man socketaddr ; declaring ipv4 address
	sa.sin_family = AF_INET; // declaring address family as internet

	if (argc==1) // if argc sees that there is only 1 argument (name of program), then show help
	{
		help_menu();
	}
	else
	{
		while( *++argv ) //loop until (null) pointer
		{
			if ( (*argv)[0] == '-' ) //if the terminal arguments starts with "-"
			{
				switch( (*argv)[1] ) //switch case based on what comes after the "-"
				{
					default: //if what comes after the "-" is not any of the cases, print the below
						switch_flag=1;
						printf("\nUnknown option: \'-%c\'\n", (*argv)[1]);
						printf("\nTry \'./scanner -h\' for more information.\n");
						break; //stop the switch case
					case 's': // if it is "-s" (starting port option)
						*++argv; // increase pointer by one (point to the starting port number)
						if( argv[0]==NULL ) // if it is null, show help menu
						{
							//help_menu();
							break;
						}
						else if ( (*argv)[0] == '-' ) // if argument right after "-s" is another case, show help then break
						{
							//help_menu();
							break;
						}
						else //if it isn't null
						{
							start = atoi(argv[0]); //set start as the number argv is pointing to
							// atoi() converts string to integer ; man 3 atoi
							/*if ( start < -1 ) // if start is < 0, i.e. -1, -10
							{
								start = 0; // make start = 0
							}*/
						}
						break;
					case 'e': //same as the "-s" case (starting port) but with the "-e" for ending port
						*++argv; // point to argument right after "-e"
						if(argv[0]==NULL)
						{
							//help_menu();
							break;
						}
						else if ( (*argv)[0] == '-' ) // if argument right after "-e" is another case, show help then break
						{
							//help_menu();
							break;
						}
						else
						{
							end = atoi(argv[0]); // using variable "end" for the end port
							/*if ( end < -1 ) // if input is less than 0, make it 0
							{
								end = 0;
							}*/
						}
						break;
					case 'h': // if the option is "-h" then show the help menu
						switch_flag=1;
						help_menu();
						break;
				}
			}
			else
			{
				//help_menu(); // if the pointer is placed at something
				// that doesn't start with an "-", show help menu

				// works due to the way the cases break out of the switch case
				// and because of when an option is detected, the pointer is increased
				// if the pointer is not increased, it will point to a
				break;
			}
		}

		if ( (start == -1 || end == -1) && switch_flag==-1) // if start and end haven't been set deliberately and not because someone typed "./scanner -h" or an unknown option like "-a"
		{
			printf("\nPlease input all the port values.\n");
			printf("\nTry \'./scanner -h\' for more information.\n");
		}
		else if (start < -1 || end < -1)
		{
			printf("\nPort values must be larger than 0.\n");
			printf("\nTry \'./scanner -h\' for more information.\n");
		}
		else if ( start>end ) //how to do this when "./scanner -s 100" to output help and when end is smaller than start
		{
			printf("\nStarting port cannot be larger than the ending port.\n");
			printf("\nTry \'./scanner -h\' for more information.\n");
		}
		else if (start >= 0 && end >= 0 && start <= end) // only start scan if start and end ports are larger than -1 and if start is less than or equal to end
		{
			printf("\nStarting a port scan with a range from %d to %d.\n\n", start, end);
			for( i = start ; i <= end ; i++) //looping from the start port until the end port, including
			{
				sa.sin_port = htons(i); //htons() from arpa/inet.h (man htons), converts unsigned short integer (i) to network byte order; declares sin_port as the port from i
				sock = socket(AF_INET, SOCK_STREAM, 0); //(int domain, int type, int protocol), create socket of type internet (ipv4, bi-directional byte stream, 0=default protocol for AF_INET)


				//socket(SOCK_STREAM for TCP, SOCK_DGRAM for UDP)
				//impossible to know for UDP since it doesn't acknowledge the receival of the package



				if (sock < 0) // socket is OK if returns val > -1 (as file descriptor); if FAILED, returns val < 0, and errno (#include errno.h) is set to show the error
				{
					 printf("\nSocket error\n");
					 exit(1);
				}

				scan = connect(sock, (struct sockaddr*)&sa , sizeof sa); // (socket, struct pointer *address, length of struct)
				if ( scan < 0 ) // connect() returns 0 if OK and -1 if FAILED and errno is set to show error
				{
					//printf("%-5d %s\r\n" , i, strerror(errno));
					fflush(stdout); //flushing i/o buffer, useful for flushing scanf and writing to files buffer
				}
				else //if the scan is 0 then the scan connected to the socket therefore connect() returned value 0, which means OK
				{
					printf("%-5d open\n", i);
				}
				close(sock); //must close an open file descriptor so that it can be opened later again
			}
		}
	}
}
