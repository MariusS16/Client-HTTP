#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <ctype.h>

#include "commands.h"
#include "helpers.h"
#include "requests.h"
#include "parson.h"

void register_command(int sockfd, char *cookie) {
	if (strlen(cookie) != 0) {
		printf("EROARE - Esti deja autentificat\n");
		return;
	}

	char username[LINELEN], password[LINELEN];
	printf("username=");
	fgets(username, LINELEN, stdin);
	username[strlen(username) - 1] = '\0';
	printf("password=");
	fgets(password, LINELEN, stdin);
	password[strlen(password) - 1] = '\0';

	if (strchr(username, ' ') != NULL || strchr(password, ' ') != NULL) {
		printf("EROARE - Username sau parola nu pot contine spatii\n");
		return;
	}

	JSON_Value *root_user = json_value_init_object();
	JSON_Object *user = json_value_get_object(root_user);
	json_object_set_string(user, "username", username);
	json_object_set_string(user, "password", password);

	char *body_data[1];
	body_data[0] = json_serialize_to_string_pretty(root_user);

	char *request = compute_post_request(IP, "/api/v1/tema/auth/register", "application/json",
	                                     body_data, 1, NULL, 0, NULL);
	send_to_server(sockfd, request);
	char *response = receive_from_server(sockfd);

	if (strlen(response) == 0) {
		sockfd = open_connection(IP, PORT, AF_INET, SOCK_STREAM, 0);
		send_to_server(sockfd, request);
		response = receive_from_server(sockfd);
	}

	int status_code = strtol(response + 9, NULL, 10);

	if (status_code >= 200 && status_code < 300) {
		printf("SUCCES - Utilizator inregistrat\n");
	} else {
		printf("EROARE - Acest username exista deja\n");
	}

	free(body_data[0]);
	free(request);
	free(response);
}

void login_command(int sockfd, char *cookie) {
	if (strlen(cookie) != 0) {
		printf("EROARE - Esti deja autentificat\n");
		return;
	}

	char username[LINELEN], password[LINELEN];
	printf("username=");
	fgets(username, LINELEN, stdin);
	username[strlen(username) - 1] = '\0';
	printf("password=");
	fgets(password, LINELEN, stdin);
	password[strlen(password) - 1] = '\0';

	if (strchr(username, ' ') != NULL || strchr(password, ' ') != NULL) {
		printf("EROARE - Username sau parola nu pot contine spatii\n");
		return;
	}

	JSON_Value *root_user = json_value_init_object();
	JSON_Object *user = json_value_get_object(root_user);
	json_object_set_string(user, "username", username);
	json_object_set_string(user, "password", password);

	char *body_data[1];
	body_data[0] = json_serialize_to_string_pretty(root_user);

	char *request = compute_post_request(IP, "/api/v1/tema/auth/login", "application/json",
	                                     body_data, 1, NULL, 0, NULL);

	send_to_server(sockfd, request);
	char *response = receive_from_server(sockfd);

	if (strlen(response) == 0) {
		sockfd = open_connection(IP, PORT, AF_INET, SOCK_STREAM, 0);
		send_to_server(sockfd, request);
		response = receive_from_server(sockfd);
	}

	int status_code = strtol(response + 9, NULL, 10);

	if (status_code >= 200 && status_code < 300) {
		char *cookie_start = strstr(response, "connect.sid");
		char *cookie_end = strstr(cookie_start, "\r\n");
		strncpy(cookie, cookie_start, cookie_end - cookie_start);
		cookie[cookie_end - cookie_start] = '\0';
		printf("SUCCES - Utilizator autentificat\n");
	} else {
		printf("EROARE - Username sau parola sunt gresite\n");
		return;
	}

	free(body_data[0]);
	free(request);
	free(response);
}

void enter_library_command(int sockfd, char *cookie, char *token) {
	if (strlen(token) != 0) {
		printf("EROARE - Ai deja acces la biblioteca\n");
		return;
	}

	char *cookies[1];
	cookies[0] = (char *) calloc(LINELEN, sizeof(char));
	strcpy(cookies[0], cookie);

	char *request = compute_get_request(IP, "/api/v1/tema/library/access", NULL,
	                                    cookies, 1, NULL);
	send_to_server(sockfd, request);
	char *response = receive_from_server(sockfd);

	if (strlen(response) == 0) {
		sockfd = open_connection(IP, PORT, AF_INET, SOCK_STREAM, 0);
		send_to_server(sockfd, request);
		response = receive_from_server(sockfd);
	}

	int status_code = strtol(response + 9, NULL, 10);

	if (status_code >= 200 && status_code < 300) {
		char *token_start = strstr(response, "token") + 8;
		char *token_end = strstr(token_start, "\"");
		strncpy(token, token_start, token_end - token_start);
		token[token_end - token_start] = '\0';
		printf("SUCCES - Acces permis\n");
	} else {
		printf("EROARE - Nu esti autentificat\n");
	}

	free(cookies[0]);
	free(request);
	free(response);
}

void get_books_command(int sockfd, char *cookie, char *token) {
	if (strlen(token) == 0) {
		printf("EROARE -Nu ai acces la biblioteca\n");
		return;
	}

	char *cookies[1];
	cookies[0] = (char *) calloc(LINELEN, sizeof(char));
	strcpy(cookies[0], cookie);

	char *request = compute_get_request(IP, "/api/v1/tema/library/books", NULL,
	                                    cookies, 1, token);
	send_to_server(sockfd, request);
	char *response = receive_from_server(sockfd);

	if (strlen(response) == 0) {
		sockfd = open_connection(IP, PORT, AF_INET, SOCK_STREAM, 0);
		send_to_server(sockfd, request);
		response = receive_from_server(sockfd);
	}

	int status_code = strtol(response + 9, NULL, 10);

	if (status_code >= 200 && status_code < 300) {
		char *books = strstr(response, "[");
		printf("%s\n", json_serialize_to_string_pretty(json_parse_string(books)));
	} else {
		printf("EROARE - Nu au fost gasite carti\n");
	}

	free(cookies[0]);
	free(request);
	free(response);
}

void get_book_command(int sockfd, char *cookie, char *token) {
	if (strlen(token) == 0) {
		printf("EROARE - Nu ai acces la biblioteca\n");
		return;
	}

	char id[LINELEN];
	printf("id=");
	fgets(id, LINELEN, stdin);
	id[strlen(id) - 1] = '\0';

	if (strtol(id, NULL, 10) == 0) {
		printf("EROARE - Id-ul trebuie sa fie un numar pozitiv\n");
		return;
	}

	char *cookies[1];
	cookies[0] = (char *) calloc(LINELEN, sizeof(char));
	strcpy(cookies[0], cookie);

	char *request = compute_get_request(IP, "/api/v1/tema/library/books", id,
	                                    cookies, 1, token);
	send_to_server(sockfd, request);
	char *response = receive_from_server(sockfd);

	if (strlen(response) == 0) {
		sockfd = open_connection(IP, PORT, AF_INET, SOCK_STREAM, 0);
		send_to_server(sockfd, request);
		response = receive_from_server(sockfd);
	}

	int status_code = strtol(response + 9, NULL, 10);

	if (status_code >= 200 && status_code < 300) {
		char *book = strstr(response, "{");
		printf("%s\n", json_serialize_to_string_pretty(json_parse_string(book)));
	} else {
		printf("EROARE - Cartea nu a fost gasita\n");\

	}

	free(cookies[0]);
	free(request);
	free(response);
}

void add_book_command(int sockfd, char *cookie, char *token) {
	if (strlen(token) == 0) {
		printf("EROARE - Nu ai acces la biblioteca\n");
		return;
	}

	char title[LINELEN], author[LINELEN], genre[LINELEN], publisher[LINELEN], page_count[LINELEN];
	printf("title=");
	fgets(title, LINELEN, stdin);
	title[strlen(title) - 1] = '\0';
	format(title);
	printf("author=");
	fgets(author, LINELEN, stdin);
	author[strlen(author) - 1] = '\0';
	format(author);
	printf("genre=");
	fgets(genre, LINELEN, stdin);
	genre[strlen(genre) - 1] = '\0';
	format(genre);
	printf("publisher=");
	fgets(publisher, LINELEN, stdin);
	publisher[strlen(publisher) - 1] = '\0';
	format(publisher);
	printf("page_count=");
	fgets(page_count, LINELEN, stdin);
	page_count[strlen(page_count) - 1] = '\0';
	format(page_count);

	if (strtol(page_count, NULL, 10) == 0) {
		printf("EROARE - Numarul de pagini trebuie sa fie un numar intreg pozitiv\n");
		return;
	}

	JSON_Value *root_book = json_value_init_object();
	JSON_Object *book = json_value_get_object(root_book);
	json_object_set_string(book, "title", title);
	json_object_set_string(book, "author", author);
	json_object_set_string(book, "genre", genre);
	json_object_set_string(book, "publisher", publisher);
	json_object_set_number(book, "page_count", strtol(page_count, NULL, 10));

	char *body_data[1];
	body_data[0] = json_serialize_to_string_pretty(root_book);
	json_value_free(root_book);

	char *cookies[1];
	cookies[0] = (char *) calloc(LINELEN, sizeof(char));
	strcpy(cookies[0], cookie);

	char *request = compute_post_request(IP, "/api/v1/tema/library/books", "application/json",
	                                     body_data, 1, cookies, 1, token);
	send_to_server(sockfd, request);
	char *response = receive_from_server(sockfd);

	if (strlen(response) == 0) {
		sockfd = open_connection(IP, PORT, AF_INET, SOCK_STREAM, 0);
		send_to_server(sockfd, request);
		response = receive_from_server(sockfd);
	}

	int status_code = strtol(response + 9, NULL, 10);

	if (status_code >= 200 && status_code < 300) {
		printf("SUCCES - Cartea a fost adaugata\n");
	} else {
		printf("EROARE - Cartea nu a fost adaugata\n");
	}

	free(body_data[0]);
	free(cookies[0]);
	free(request);
	free(response);
}

void delete_book_command(int sockfd, char *cookie, char *token) {
	if (strlen(token) == 0) {
		printf("EROARE - Nu ai acces la biblioteca\n");
		return;
	}

	char id[LINELEN];
	printf("id=");
	fgets(id, LINELEN, stdin);
	id[strlen(id) - 1] = '\0';

	if (strtol(id, NULL, 10) == 0) {
		printf("EROARE - Id trebuie sa fie un numar intreg pozitiv\n");
		return;
	}

	char *cookies[1];
	cookies[0] = (char *) calloc(LINELEN, sizeof(char));
	strcpy(cookies[0], cookie);

	char *request = compute_delete_request(IP, "/api/v1/tema/library/books", id,
	                                       cookies, 1, token);
	send_to_server(sockfd, request);
	char *response = receive_from_server(sockfd);

	if (strlen(response) == 0) {
		sockfd = open_connection(IP, PORT, AF_INET, SOCK_STREAM, 0);
		send_to_server(sockfd, request);
		response = receive_from_server(sockfd);
	}

	int status_code = strtol(response + 9, NULL, 10);

	if (status_code >= 200 && status_code < 300) {
		printf("SUCCES - Cartea a fost stearsa\n");
	} else {
		printf("EROARE - Cartea nu a fost stearsa\n");
	}

	free(cookies[0]);
	free(request);
	free(response);
}

void logout_command(int sockfd, char *cookie, char *token) {
	if (strlen(cookie) == 0) {
		printf("EROARE - Nu esti autentificat\n");
		return;
	}

	char *cookies[1];
	cookies[0] = (char *) calloc(LINELEN, sizeof(char));
	strcpy(cookies[0], cookie);

	char *request = compute_get_request(IP, "/api/v1/tema/auth/logout", NULL,
	                                    cookies, 1, token);
	send_to_server(sockfd, request);
	char *response = receive_from_server(sockfd);

	if (strlen(response) == 0) {
		sockfd = open_connection(IP, PORT, AF_INET, SOCK_STREAM, 0);
		send_to_server(sockfd, request);
		response = receive_from_server(sockfd);
	}

	int status_code = strtol(response + 9, NULL, 10);

	if (status_code >= 200 && status_code < 300) {
		printf("SUCCES - Ai fost delogat\n");
	} else {
		printf("EROARE - Nu esti autentificat\n");
	}

	memset(cookie, 0, LINELEN);
	memset(token, 0, LINELEN);
	free(cookies[0]);
	free(request);
	free(response);
}
