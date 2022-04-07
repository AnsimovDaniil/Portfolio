#pragma once
#include <iostream>	
#include <sys/socket.h>	
#include <arpa/inet.h>
#include <cstring>
#include <unistd.h>
#include <chrono>
#include <thread>
#include <fcntl.h>
#include <signal.h>

struct Client {
public:

	Client();

	void CreateSocket();
	
	bool Connect();

	int SendMessage(std::string& newMess);

	void Reconnect();

	~Client();

private:
	int sock;
	struct sockaddr_in server;
};