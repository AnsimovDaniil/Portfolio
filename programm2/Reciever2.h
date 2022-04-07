#pragma once
#include <iostream>
#include <algorithm>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>	
#include <future>
#include <vector>
#include <sstream>

void MessageAnalysys(std::string& m);

void THR1();

class Server {
	public:
	Server();

	void CreateSocket();

	void Bind();

	void Listen(const int& ConNumber);

	void Accept();

	bool IfConnectionAvailable(const int&);

	std::string GetNewMess();

	void Start();

	std::string Reconnect();

	~Server();

	private:
	int socket_desc, client_sock, c, read_size;
	struct sockaddr_in server, client;
	std::vector<char> client_message;
};
