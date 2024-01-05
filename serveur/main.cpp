// Library includes
#include <inttypes.h>
#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <unistd.h> // read(), write(), close()
#include <opencv2/opencv.hpp>
// File includes
#include "server.h"

#define PORT 4099 
#define SA struct sockaddr 

using namespace cv;
using namespace std;

/**
 * \mainpage
 * Ceci est la documentation officiel du projet fait par Alfa Barri et Emmanuel Desilets.
 * 
 * Plusieurs precisions se retrouvent sur le Readme.md du projet.
 * 
 * Ce projet contient deux classes principales: Server et Client. Ces classes ont été conceptualisées
 * pour intéragir ensemble. Pour un fonctionnement complet du projet, deux instances de serveurs.
 * rouleront en même temps sur des procédés différents. Veuillez vous référer a main.cpp et main2.cpp 
 * pour une meilleure compréhension.
 * 
 * La classe ExternValue permet la lecture des peripheriques du cote serveur et la classe FaceDetector permet de faire les
 * manipulations de la detection et reconnaissance faciale du cote client.
 * 
 * Les fichiers main.cpp et main2.cpp etablissent les connections entre les serveurs et le client.
 * 
 * La lecture des périphériques est entièrement effectuée côté serveur et la détection/reconnaissance faciale
 * est entièrement effectuée du côté client.
 * 
**/

// Function designed for chat between client and server. 
void func(int connfd) 
{    
    Server server(connfd);

	// User options
	int frame_width_options[4] = {176, 320, 800, 960};
    int frame_height_options[4] = {144, 240, 600, 720};
	string format_options[2] = {".jpeg", ".png"};
	int* tmp_client_option_change;
    Mat frame;
    VideoCapture cap(0);


	// If the camera is not open, exit program
    if (cap.isOpened() == false)
    {
		cout << "CANNOT OPEN CAMERA" << endl; 
        return;
    } 
    
    // Receive the message from the client
    server.receiveMessageFromClient();

    // Set the resolution of the frame
    cap.set(CV_CAP_PROP_FRAME_WIDTH,frame_width_options[server.resolution_option]);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT,frame_height_options[server.resolution_option]);

    // Read the periph// Receive the message from the clienterals value and send the according message
	server.sendMessageToClient();

	// Capture and send the frame
	cap >> frame;
	server.sendFrameToClient(frame, format_options[server.format_option]);

	// infinite loop for exchange 
	while(true) { 
        // Receive the message from the client
		tmp_client_option_change = server.receiveMessageFromClient();

		// Reset resolution
		if (tmp_client_option_change[1]) {
			cap.set(CV_CAP_PROP_FRAME_WIDTH,frame_width_options[server.resolution_option]);
    		cap.set(CV_CAP_PROP_FRAME_HEIGHT,frame_height_options[server.resolution_option]);
		}

        // Terminate the program when the OK is false
        if(!(server.msg & ELE4205_OK)) {
			cap.release();
			cout << "QUIT" << endl;
			break;
        }
		
        // Read the peripherals value and send the according message
		server.sendMessageToClient();

		// Capture and send the frame
		cap >> frame; 
		server.sendFrameToClient(frame, format_options[server.format_option]);
	} 
} // Receive the message from the client

// Driver function 
int main() 
{
	socklen_t sockfd, connfd, len; 
	struct sockaddr_in servaddr, cli; 

	// socket create and verification 
	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd == -1) {
		printf("socket creation failed...\n"); 
		exit(0); 
	} 
	else
		printf("Socket successfully created..\n"); 
	bzero(&servaddr, sizeof(servaddr)); 

	// assign IP, PORT T
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
	servaddr.sin_port = htons(PORT); 

	// Binding newly created socket to given IP and veriserver.sendMessageToClient(connfd);
	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) { 
		printf("socket bind failed...\n"); 
		exit(0); 
	} 
	else
		printf("Socket successfully binded..\n"); 

	// Now server is ready to listen and verification 
	if ((listen(sockfd, 5)) != 0) { 
		printf("Listen failed...\n"); 
		exit(0); 
	} 
	else
        printf("Server listening.. \n");
    len = sizeof(cli); 

	// Accept the data packet from client and verification 
	connfd = accept(sockfd, (SA*)&cli, &len); 
	if (connfd < 0) {
		printf("server accept failed...\n"); 
		exit(0); 
	} 
	else
		printf("server accept the client...\n"); 

	// Function for chatting between client and server 
	func(connfd); 

	// After chatting close the socket
	close(sockfd); 
}