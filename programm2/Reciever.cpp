#include "Reciever2.h"

Server::Server() {
		server.sin_family = AF_INET;
		server.sin_addr.s_addr = INADDR_ANY;
		server.sin_port = htons( 8888 );
		socket_desc = -1;
		client_sock = -1;
		read_size = 1;
		c = sizeof(struct sockaddr_in);
		client_message.resize(3);
	}

void Server::CreateSocket() {
		if ((socket_desc = socket(AF_INET , SOCK_STREAM , 0)) == -1) {
			std::cout << "Can't create new socket" << std::endl;
			exit(1);
		};
	}

void Server::Bind() {
		if (bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) == -1) {
			std::cout << "Can't bind this socket" << std::endl;
			exit(1);
		};
		
	}

void Server::Listen(const int& ConNumber) {
		listen(socket_desc, ConNumber);
	} 

void Server::Accept() {
		if ((client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) == -1) {
			std::cout << "Acception failed" << std::endl;
			exit(1);
		};

		std::cout << "Connection accepted" << std::endl;
	}

bool Server::IfConnectionAvailable(const int& n) {
	if ( n > 0 ) {
		return true;
	} else {
		return false;
	} 
}

std::string Server::GetNewMess() {
	int len = 3;												// принимаем длину сообщения
	read_size = recv(client_sock, (char*) &len, sizeof(len), 0);
	if ( read_size > 0 )
		{
	 	client_message.resize(len);
		read_size = recv(client_sock, client_message.data(), len, 0); //принимаем само сообщение
		if ( read_size > 0 )
			{
    		std::string returnValue(client_message.begin(), client_message.end());
			return returnValue;
			} else {
				std::cout << "Client disconnected" << std::endl;
				std::string _output = Reconnect();
				return _output;
			}
		} else {
			std::cout << "Client disconnected" << std::endl;
			std::string _output = Reconnect();
			return _output;
			}
		}
		



void Server::Start() {
		CreateSocket();	
		Bind();
		Listen(3);
		Accept();
        while (true) {
			std::string message = GetNewMess();
				try {
					MessageAnalysys(message);
				}
				catch (std::exception& e) {
					std::cout << e.what() << std::endl;
				}
        }
	}

std::string Server::Reconnect() {
		shutdown(client_sock, SHUT_RDWR);
		close(client_sock);
		Accept();
		return GetNewMess();
	}

	Server::~Server() {
		shutdown(socket_desc, SHUT_RDWR);
		close(socket_desc);
		shutdown(client_sock, SHUT_RDWR);
		close(client_sock);
	}