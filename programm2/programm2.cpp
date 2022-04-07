#include <iostream>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>
#include "Reciever2.h"

using namespace std;

void THR1() {
	Server S1;
	//struct sigaction act;
	//act.sa_handler = SIG_IGN;
	//act.sa_flags = SA_RESTART;
	//sigaction(SIGPIPE, &act, 0);
	S1.Start();
}

void MessageAnalysys(string& m) {
	if (m.size() < 2) {
		throw invalid_argument("Messege includes less than 2 symbols after processing.");
	}
	else if ((stoi(m)) % 32 != 0) {
		throw invalid_argument("Messege aren't multiply of 32");
	}
	else {
		cout << m << endl;
	}
}

int main() {
	future<void> a;
	a = async(THR1);
	string input;
	while (getline(cin, input)) {
		if (input == "EXIT") {
			exit(1);
		}
	}
	return 0;
}