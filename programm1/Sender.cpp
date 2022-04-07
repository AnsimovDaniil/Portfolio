#include "Sender.h"

Client::Client() {
		server.sin_addr.s_addr = inet_addr("127.0.0.1");
		server.sin_family = AF_INET;
		server.sin_port = htons( 8888 );
	}

void Client::CreateSocket() {
		sock = socket(AF_INET , SOCK_STREAM , 0);
	    if (sock == -1)
		{
			std::cout << "Could not create socket" << std::endl;
		}
		std::cout << "Socket created" << std::endl;
	}
	
bool Client::Connect() {
			if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
		{
			perror("connect failed. Error");
			return false;
		} else {
			std::cout << "Connected" << std::endl;
			return true;
		}
	}

int Client::SendMessage(std::string& newMess) {
		int len = newMess.length();
		int n = send(sock, (const char *)&len, sizeof(len), 0);
		if (n < 0) return n;
		return send(sock, newMess.c_str(), newMess.size(), 0);
	}

void Client::Reconnect() {
		shutdown(sock, SHUT_RDWR);
		close(sock);
		sleep(5);
		CreateSocket();
		if (!Connect()) {
			Reconnect();
		};
	}

Client::~Client() {
		shutdown(sock, SHUT_RDWR);
		close(sock);
	}