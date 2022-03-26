#include <iostream>
#include <string>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

using namespace std;


void main() {

	string ipAddress = "127.0.0.1";  // Server IP
	int port = 54000;                // Server listen port number

	// init

	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);

	if (wsResult != 0) {
		cerr << "cant start error number -> " << wsResult << endl;
		return;
	}

	// create socket

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);   // internet family/type sock/protocol

	if (sock == INVALID_SOCKET) {
		cerr << "cant make socket error number -> " << WSAGetLastError() << endl;
		WSACleanup();
		return;
	}

	//hint struct

	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

	//connect

	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));

	if (connResult == SOCKET_ERROR) {
		cerr << "cant connect with server error number -> " << WSAGetLastError << endl;
		WSACleanup();
		return;
	}

	// send/receive

	char buff[4096];
	string userInput;

	do {

		// ask user for input
		cout << ">>";
		getline(cin, userInput);

		if (userInput.size() > 0) {

			// send user input to server
			int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0);  //the string will have another character because C++ terminates each string with a 0

			if (sendResult != SOCKET_ERROR) {

				//wait respon
				ZeroMemory(buff, 4096);
				int byteReceived = recv(sock, buff, 4096, 0);

				if (byteReceived > 0) {
					cout << "server -> " << string(buff, 0, byteReceived) << endl;
				}
			}
		}
		
	} while (userInput.size() > 0);

	//close down

	closesocket(sock);
	WSACleanup();
}
















