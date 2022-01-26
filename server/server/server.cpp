#include <iostream>
#include <string>
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

	listen(listenerSocket, SOMAXCONN);

	// Wait for connetction

	sockaddr_in client;
	int clientSize = sizeof(client);

	SOCKET clientSocket = accept(listenerSocket, (sockaddr*)&client, &clientSize);
	if (clientSocket == INVALID_SOCKET) {
		cerr << "Accepting client failed!" << endl;
		return 0;
	}

	char host[NI_MAXHOST]; // client remote name
	char service[NI_MAXSERV]; //Service (i.e. port) the client is connected on

	ZeroMemory(host, NI_MAXHOST); // same as memset(host, 0, NI_MAXHOST)
	ZeroMemory(service, NI_MAXSERV);
	
	if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0) {
		cout << host << " connected on port " << service << endl;
	}
	else {
		inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
		cout << host << " connected on port " << ntohs(client.sin_port) << endl;
	}

	// Close listening socket

	closesocket(listenerSocket);

	// While loop: accept and echo message back to client

	char buf[4096];

	while (true) {

		ZeroMemory(buf, 4096);

		// Wait for client to send data
		int bytesReceived = recv(clientSocket, buf, 4096, 0);
		if (bytesReceived == SOCKET_ERROR) {
			cerr << "Error in recv()" << endl;
			break;
		}

		if (bytesReceived == 0) {
			cout << "Client disconnected" << endl;
			break;
		}

		// Echo message back to the client
		cout << endl << "CLIENT: " << string(buf, 0, bytesReceived) << endl;
		send(clientSocket, buf, bytesReceived + 1, 0);
	}

	// Close socket

	closesocket(clientSocket);

	// Cleanup winsock

	WSACleanup();

	return 0;
}