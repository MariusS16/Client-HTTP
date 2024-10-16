#ifndef _COMMANDS_
#define _COMMANDS_

void register_command(int sockfd, char *cookie);

void login_command(int sockfd, char *cookie);

void enter_library_command(int sockfd, char *cookie, char *token);

void get_books_command(int sockfd, char *cookie, char *token);

void get_book_command(int sockfd, char *cookie, char *token);

void add_book_command(int sockfd, char *cookie, char *token);

void delete_book_command(int sockfd, char *cookie, char *token);

void logout_command(int sockfd, char *cookie, char *token);

#endif
