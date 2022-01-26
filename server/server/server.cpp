#include <iostream>
#include <WS2tcpip.h>

#pragma comment (lib, "WS2_32.lib") //Winsock Library

using namespace std;

int main() {

	// Initialize winsock
	WSADATA wsData;
	WORD ver = MAKEWORD(2, 2);

	int wSock = WSAStartup(ver, &wsData);
	if (wSock != 0) {
		cerr << "Winsock initialization failed!" << endl;
		return 0;
	}

	// Create socket

	SOCKET listenerSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (listenerSocket == INVALID_SOCKET) {
		cerr << "Socket creation failed!" << endl;
		return 0;
	}

	// Bind the ip address and port to a socket

	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(54000);
	hint.sin_addr.S_un.S_addr = INADDR_ANY;

	bind(listenerSocket, (sockaddr*)&hint, sizeof(hint));



	// Tell winsock the socket is for listening

	// Wait for connetction

	// Close listening socket

	// While loop: accept and echo message back to client

	// Close socket

	// Shutdown winsock

	return 0;
}