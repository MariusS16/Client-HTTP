#include <stdio.h>      /* printf, sprintf */
#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"
#include "commands.h"

int main(int argc, char *argv[]) {
	int sockfd = open_connection(IP, PORT, AF_INET, SOCK_STREAM, 0);
	char *cookie = (char *) calloc(LINELEN, sizeof(char));
	char *token = (char *) calloc(LINELEN, sizeof(char));
	char command[LINELEN];

	while (1) {
		fgets(command, LINELEN, stdin);
		command[strlen(command) - 1] = '\0';

		if (strcmp(command, "exit") == 0) {
			close_connection(sockfd);
			break;
		}

		if (strcmp(command, "register") == 0)
			register_command(sockfd, cookie);

		else if (strcmp(command, "login") == 0)
			login_command(sockfd, cookie);

		else if (strcmp(command, "enter_library") == 0)
			enter_library_command(sockfd, cookie, token);

		else if (strcmp(command, "get_books") == 0)
			get_books_command(sockfd, cookie, token);

		else if (strcmp(command, "get_book") == 0)
			get_book_command(sockfd, cookie, token);

		else if (strcmp(command, "add_book") == 0)
			add_book_command(sockfd, cookie, token);

		else if (strcmp(command, "delete_book") == 0)
			delete_book_command(sockfd, cookie, token);

		else if (strcmp(command, "logout") == 0)
			logout_command(sockfd, cookie, token);

		else
			printf("EROARE - Comanda nu exista\n");
	}

	return 0;
}
