#include <iostream>
#include <string>
#include <WS2tcpip.h>
#pragma comment (lib, "ws2_32.lib");

using namespace std;

int main() {

	string ipAddress = "192.168.56.1"; // server ip address
	int port = 54000; // Listening port # on the server

	// Intitialize WinSock

	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0) {
		cerr << "Winsock initialization failed: " << wsResult << endl;
		return 0;
	}

	// Create socket

	SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == INVALID_SOCKET) {
		cerr << "Socket creation failed: " << WSAGetLastError << endl;
		WSACleanup();
		return 0;
	}

	// Fill in a hint structure

	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

	// Connect to server

	int connctedResult = connect(clientSocket, (sockaddr*)&hint, sizeof(hint));
	if (connctedResult == SOCKET_ERROR) {
		cerr << "Connection to server failed: " << WSAGetLastError() << endl;
		closesocket(clientSocket);
		WSACleanup();
		return 0;
	}

	// Do-While loop to send and receive data
	char buf[4096];
	string userInput;

	do {

		// prompt user for input
		cout << endl << "> ";
		getline(cin, userInput);

		
		if (userInput.size() > 0) { // checking if the user has entered the data

			// send the text
			int sendResult = send(clientSocket, userInput.c_str(), userInput.size() + 1, 0);
			
			if (sendResult != SOCKET_ERROR) {
				// wait for response
				ZeroMemory(buf, 4096);
				int bytesReceived = recv(clientSocket, buf, 4096, 0);
				if (bytesReceived > 0) {
					// echo the response to console
					cout << endl << "SERVER(ECHO): " << string(buf, 0, bytesReceived) << endl;
				}	
			}
		}

	} while (userInput.size() > 0);

	// Exit

	closesocket(clientSocket);
	WSACleanup();
	return 0;
}





