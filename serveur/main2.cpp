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

#include "server.h"

#define PORT 4100 
#define SA struct sockaddr 
#define KNOWN_FREQUENCY 440
#define KNOWN_LENGTH 3000000
#define UNKNOWN_FREQUENCY 660
#define UNKNOWN_LENGTH 1000000
#define UNKNOWN_DELAY 500000

using namespace cv;
using namespace std;


void func(int connfd) 
{    
    Server server(connfd);
	string frequencyPath = "/sys/devices/pwm-ctrl.42/freq0";
	string dutyPath = "/sys/devices/pwm-ctrl.42/duty0";
	string enablePath = "/sys/devices/pwm-ctrl.42/enable0";
	std::ofstream freq_file, duty_file, enable_file;

	// Receive the message from the client
	server.receiveMessageFromClient();	

	// Configure the PWM
	system("cd /sys/devices && modprobe pwm-meson");
	system("cd /sys/deUNKNOWN_LENGTHvices && modprobe pwm-ctrl");
	
	//Configure the duty period
	duty_file.open(dutyPath.c_str());
	duty_file << 512;
	duty_file.close();	

	switch(server.face_detection_code_option)
	{
		case(0): // LEARN
			cout << "Learning mode" << endl;
			break;

		case(1): // KNOWN
			cout << "Known" << endl;

			// Ajuster la frequence
			freq_file.open(frequencyPath.c_str());
			freq_file << KNOWN_FREQUENCY;
			freq_file.close();

			// Play the music
			enable_file.open(enablePath.c_str());
			enable_file << 1;
			enable_file.close();
			usleep(KNOWN_LENGTH);
			enable_file.open(enablePath.c_str());
			enable_file << 0;
			enable_file.close();
			break;

		case(2): // UNKNOWN
			cout << "Play unknown song" << endl;

			// Ajuster la frequence
			freq_file.open(frequencyPath.c_str());
			freq_file << UNKNOWN_FREQUENCY;
			freq_file.close();

			// Play the music
			for (int i=0; i<3; i++)
			{
				enable_file.open(enablePath.c_str());
				enable_file << 1;
				enable_file.close();
				usleep(UNKNOWN_LENGTH);
				enable_file.open(enablePath.c_str());
				enable_file << 0;
				enable_file.close();
				usleep(UNKNOWN_DELAY);
			}

	}
} 

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
	// else
	// 	printf("Socket successfully binded..\n"); 

	// Now server is ready to listen and verification 
	if ((listen(sockfd, 5)) != 0) { 
		printf("Listen failed...\n"); 
		exit(0); 
	} 
	// else
    //     printf("2nd Server listening.. \n");
    len = sizeof(cli); 

	// Accept the data packet from client and verification 
	connfd = accept(sockfd, (SA*)&cli, &len); 
	if (connfd < 0) {
		printf("2nd server accept failed...\n"); 
		exit(0); 
	} 
	// else
	// 	printf("2nd server accept the client...\n"); 

	// Function for chatting between client and server 
	func(connfd); 

	// After chatting close the socket
	close(sockfd); 
	printf("closed connection to 2nd server..\n\n");
}