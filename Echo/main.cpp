#include <iostream>
#include <WS2tcpip.h>

#pragma comment (lib, "ws2_32.lib")


using namespace std;

using namespace std;

// uses overloaded '=' operator from string class
// to convert character array to string
// did not work the way I needed to check for "fin"
/*
string convertToString(char* buff)
{
	string s = buff;
	return s;
}
*/


void main() {

	//init

	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	// implimented several checks throughout the process which exit on errors (initalize, no message sent from client, invalid socket etc)
	int wsok = WSAStartup(ver, &wsData);
	if (wsok != 0) {
		cerr << "cant init" << endl;
		return;
	}

	//create socket (matched to port and IP address)

	SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);

	if (listening == INVALID_SOCKET) {
		cerr << "cant make socket" << endl;
		return;
	}

	//bind socket (link socket to ip and port)

	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000);  //host to network short
	hint.sin_addr.S_un.S_addr = INADDR_ANY;

	bind(listening, (sockaddr*)&hint, sizeof(hint));


	//listen

	listen(listening, SOMAXCONN);  // maximum connections 7fffffff *A LOT

	//connection

	sockaddr_in client;
	int clientSize = sizeof(client);

	SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);

	if (clientSocket == INVALID_SOCKET) {
		cerr << "invalid client socket" << endl;
		return;
	}

	char host[NI_MAXHOST];     // client remote name
	char service[NI_MAXSERV];  //client port

	ZeroMemory(host, NI_MAXHOST);    // might need to use  memset(host, 0, NI_MAXHOST);
	ZeroMemory(service, NI_MAXSERV); // might need to use  memset(service, 0, NI_MAXHOST);

	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0) {
		cout << host << " connected to port " << service << endl;
	}
	else {
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		cout << host << " connected to port " << ntohs(client.sin_port) << endl;
	}

	//close listen

	closesocket(listening);


	//echo message

	char buff[4096];

	while (true) {
		ZeroMemory(buff, 4096);

		// wait for client data 
		int byteReceived = recv(clientSocket, buff, 4096, 0);

		if (byteReceived == SOCKET_ERROR) {
			cerr << "error when received" << endl;
			break;
		}

		if (byteReceived == 0) {
			cout << "dissconnected" << endl;
			break;
		}
		
		cout << string(buff, 0, byteReceived) << endl;

		// reverse and echo message back

		// reversing message

		int i, j, temp;
		int l = strlen(buff);


		for (i = 0, j = l - 1; i < j; i++, j--) {
			temp = buff[i];
			buff[i] = buff[j];
			buff[j] = temp;
		}

		cout << string(buff, 0, byteReceived) << endl;

		send(clientSocket, buff, byteReceived + 1, 0);

		// server terminates if it receives "fin" but it replies before terminating. 
		// the buff has been reversed already so we need to check for "nif"

		/*
		tried several methods and each one simply did nothing, would not even cout the string which tells me no string was getting created. program will Exit with no client input - just hit enter. 
		*/



	//close socket

	closesocket(clientSocket);


	//shutdown

	WSACleanup();
}


















