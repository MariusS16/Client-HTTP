
# HTTP Client

This project implements an HTTP client in C that interacts with a REST API. Our client works as a command-line interface (CLI) and allows users to send commands to the server and receive responses in JSON format.

## 🌐 Features

- **User Registration (register)** - Registers a new user.
- **User Authentication (login)** - Authenticates an existing user.
- **Access Virtual Library (enter_library)** - Grants access to the virtual library.
- **Retrieve Book Details (get_book)** - Fetches details of a specified book by ID.
- **Add a New Book (add_book)** - Adds a new book to the library.
- **Delete a Book (delete_book)** - Removes a specified book.
- **User Logout (logout)** - Logs out the current user.

## 📄 Project Structure
The project is structured into multiple files to facilitate code modularization:

- **client.c** - Main logic of the CLI client, including reading user commands and managing sessions.
- **commands.c & commands.h** - Implementation of available CLI commands and sending corresponding HTTP requests.
- **helpers.c & helpers.h** - Auxiliary functions for various operations, such as sending HTTP requests to the server.
- **parson.c & parson.h** - parson library for handling JSON objects.
- **requests.c & requests.h** - Construction of HTTP requests to the server.

## 🔨 Build & Installation

Before installing, ensure that all necessary dependencies are already installed.

### Build Instructions

Follow these steps to clone, build, and run the HTTP client:

```sh
# Clone the repository
git clone https://github.com/MariusS16/Client-HTTP.git
cd Client-HTTP

# Build the project
make

# Run the HTTP client
make run
```

## 💫 For suggestions or bug reports, feel free to open an issue with the appropriate label.

