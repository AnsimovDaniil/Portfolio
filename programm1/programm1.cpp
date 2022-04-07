#include <iostream>
#include <algorithm>
#include <string>
#include <stdio.h>
#include <vector>
#include <mutex>
#include <queue>
#include <chrono>
#include <thread>
#include <future>
#include <sys/socket.h>
#include <sys/types.h>
#include <cstring>
#include <sys/un.h>
#include <unistd.h>
#include "Sender.h"


using namespace std;

struct Buffer {
    queue<string> buffer;
    mutex m;
};

class Message {
public:
    Message () {}
    Message(string& data) {  

        if (data.size() > 64) {
            throw invalid_argument("Message size can't be more than 64 digits");
        }

        for (const auto& digit : data) {
            int i = digit - '0';
            if (i > 9 || i < 0) {
                throw invalid_argument("Message could include only digits");
            }
        }

        sort(data.rbegin(), data.rend());

        for (int i = 0; i < data.size(); ++i) {
            int t = data[i] - '0';
            if (t % 2 == 0) {
                data.erase(i,1);
                data.insert(i, "KB");
                i = ++i;
            }
        }
        data_ = data;
    }

    void AddToBuffer(Buffer& common) {
        lock_guard<mutex> m{ common.m };
        common.buffer.push(data_);
    }

    void Print() {
        cout << data_ << endl;
    }
private:
    string data_;
};

void Thr1(Message& a, Buffer& common) {    

    string input = "";
    while (getline(cin >> ws, input)) {           
        if (input == "EXIT") {
            exit(0);
        }
        try {
            Message a(input);                      
            a.AddToBuffer(common);
        }
        catch (invalid_argument e) {
            cerr << e.what() << endl;
        }
    }
}

void Thr2(Buffer& common) {                 
    struct sigaction act;                   
	act.sa_handler = SIG_IGN;
	act.sa_flags = SA_RESTART;
	sigaction(SIGPIPE, &act, 0);
    
    string mess, toserv;
    int sum, test;
    Client S1;

    S1.CreateSocket();                    
    if (!S1.Connect()) {                
        S1.Reconnect();                 
    };

    while (true) {
        while (common.buffer.size() == 0) {  
            sleep(2);
        }

        lock_guard<mutex> m{ common.m };
        mess = common.buffer.front();                       
        sum = 0;
        for (const auto& digit : mess) {      
            int i = digit - '0';
            if (i < 9 && i > 0) {
                sum += i;
            }
        }
        toserv = to_string(sum);
        test = S1.SendMessage(toserv);                       
        if (test > 0) {
            common.buffer.pop();                            
            cout << "\n" << test << " " << mess << endl;
            mess.clear();
        } else {
            cout << test << " Sending failed" << endl;
            S1.Reconnect();
        };
    }
}

int main()
{
    Buffer common;
    Message a;
    future<void> first, second;
    first = async(Thr1, ref(a), ref(common));
    second = async(Thr2, ref(common));
    return 0;
}

