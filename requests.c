#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <stdio.h>
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"

char *compute_get_request(char *host, char *url, char *query_params,
                          char **cookies, int cookies_count, char *token) {
	char *message = calloc(BUFLEN, sizeof(char));
	char *line = calloc(LINELEN, sizeof(char));

	// Step 1: write the method name, URL, request params (if any) and protocol type
	if (query_params != NULL) {
		sprintf(line, "GET %s/%s HTTP/1.1", url, query_params);
	} else {
		sprintf(line, "GET %s HTTP/1.1", url);
	}

	compute_message(message, line);
	free(line);

	if (token != NULL) {
		char *token_line = calloc(LINELEN, sizeof(char));
		strcat(token_line, "Authorization: Bearer ");
		strcat(token_line, token);
		compute_message(message, token_line);
		free(token_line);
	}

	// Step 2: add the host
	char *host_line = calloc(LINELEN, sizeof(char));
	strcat(host_line, "Host: ");
	strcat(host_line, host);
	compute_message(message, host_line);
	free(host_line);

	// Step 3 (optional): add headers and/or cookies, according to the protocol format
	if (cookies != NULL) {
		char *cookie_line = calloc(LINELEN, sizeof(char));
		strcat(cookie_line, "Cookie: ");

		for (int index = 0; index < cookies_count; ++index) {
			strcat(cookie_line, cookies[index]);
			if (index < cookies_count - 1)
				strcat(cookie_line, ";");
		}

		compute_message(message, cookie_line);
		free(cookie_line);
	}

	// Step 4: add final new line
	compute_message(message, "");
	return message;
}

char *compute_post_request(char *host, char *url, char *content_type, char **body_data,
                           int body_data_fields_count, char **cookies, int cookies_count,
                           char *token) {
	char *message = calloc(BUFLEN, sizeof(char));
	char *line = calloc(LINELEN, sizeof(char));
	char *body_data_buffer = calloc(LINELEN, sizeof(char));

	// Step 1: write the method name, URL and protocol type
	sprintf(line, "POST %s HTTP/1.1", url);
	compute_message(message, line);
	free(line);

	// Step 2: add the host
	char *host_line = calloc(LINELEN, sizeof(char));
	strcat(host_line, "Host: ");
	strcat(host_line, host);
	compute_message(message, host_line);
	free(host_line);

	if (token != NULL) {
		char *token_line = calloc(LINELEN, sizeof(char));
		strcat(token_line, "Authorization: Bearer ");
		strcat(token_line, token);
		compute_message(message, token_line);
		free(token_line);
	}

	/* Step 3: add necessary headers (Content-Type and Content-Length are mandatory)
					in order to write Content-Length you must first compute the message size
	*/
	for (int index = 0; index < body_data_fields_count; ++index) {
		strncat(body_data_buffer, body_data[index], strlen(body_data[index]));
		if (index < body_data_fields_count - 1)
			strcat(body_data_buffer, "&");
	}

	char *content_type_line = calloc(LINELEN, sizeof(char));
	strcat(content_type_line, "Content-Type: ");
	strcat(content_type_line, content_type);
	compute_message(message, content_type_line);
	free(content_type_line);

	char *content_length_line = calloc(LINELEN, sizeof(char));
	strcat(content_length_line, "Content-Length: ");
	char *content_length = calloc(LINELEN, sizeof(char));
	sprintf(content_length, "%lu", strlen(body_data_buffer));
	strcat(content_length_line, content_length);
	compute_message(message, content_length_line);
	free(content_length_line);

	// Step 4 (optional): add cookies
	if (cookies != NULL) {
		char *cookie_line = calloc(LINELEN, sizeof(char));
		strcat(cookie_line, "Cookie: ");

		for (int index = 0; index < cookies_count; ++index) {
			strcat(cookie_line, cookies[index]);
			if (index < cookies_count - 1)
				strcat(cookie_line, ";");
		}

		compute_message(message, cookie_line);
		free(cookie_line);
	}

	// Step 5: add new line at end of header
	compute_message(message, "");

	// Step 6: add the actual payload data
	strcat(message, body_data_buffer);
	free(body_data_buffer);
	return message;
}

char *compute_delete_request(char *host, char *url, char *query_params,
                             char **cookies, int cookies_count, char *token) {
	char *message = calloc(BUFLEN, sizeof(char));
	char *line = calloc(LINELEN, sizeof(char));

	// Step 1: write the method name, URL, request params (if any) and protocol type
	if (query_params != NULL) {
		sprintf(line, "DELETE %s/%s HTTP/1.1", url, query_params);
	} else {
		sprintf(line, "DELETE %s HTTP/1.1", url);
	}

	compute_message(message, line);
	free(line);

	// Step 2: add the host
	char *host_line = calloc(LINELEN, sizeof(char));
	strcat(host_line, "Host: ");
	strcat(host_line, host);
	compute_message(message, host_line);
	free(host_line);

	if (token != NULL) {
		char *token_line = calloc(LINELEN, sizeof(char));
		strcat(token_line, "Authorization: Bearer ");
		strcat(token_line, token);
		compute_message(message, token_line);
		free(token_line);
	}

	// Step 3 (optional): add headers and/or cookies, according to the protocol format
	if (cookies != NULL) {
		char *cookie_line = calloc(LINELEN, sizeof(char));
		strcat(cookie_line, "Cookie: ");

		for (int index = 0; index < cookies_count; ++index) {
			strcat(cookie_line, cookies[index]);
			if (index < cookies_count - 1)
				strcat(cookie_line, ";");
		}

		compute_message(message, cookie_line);
		free(cookie_line);
	}

	// Step 4: add final new line
	compute_message(message, "");
	return message;
}
