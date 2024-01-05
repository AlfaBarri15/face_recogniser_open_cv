#include "server.h"

using namespace std;
using namespace cv;


Server::Server(const socklen_t given_connfd) 
{
	connfd = given_connfd;
	pushb_counter = 0;
}


void Server::sendFrameToClient(const cv::Mat frame, const string format) 
{
	mem_buffer.clear();
	cv::imencode(format, frame, mem_buffer);

	// Send the size of the image and the image itself
	buff_size = mem_buffer.size();
	write(connfd, &buff_size, sizeof(buff_size));
	for (int x=0; x<buff_size; x++) 
	{
		write(connfd, &mem_buffer[x], byte_size_1); 
	}
};


void Server::sendMessageToClient()
{
	adc_value = externValue.getValue();
	while (adc_value[1] < ADC_TOLERANCE || adc_value[1] < 1) { // Il m'y a pas de la lumiere
		
		msg &= ~(SERVER_MSG_MASK << SERVER_MSG_BIT_POSITION); // clear server message
		write(connfd, &msg, sizeof(msg)); // send the message

		adc_value = externValue.getValue();
	}
	
	if (adc_value[0]) 
	{ 
		// button is not pushed
		// send READY
		msg &= ~(SERVER_MSG_MASK << SERVER_MSG_BIT_POSITION); // clear server message
		msg = (READY << SERVER_MSG_BIT_POSITION) | msg; 

		// Reset pushb_counter
		pushb_counter = 0;
	} 
	else 
	{	
		// button is pushed
		if(!pushb_counter)
		{
			pushb_counter++;

			// send PUSHB
			msg &= ~(SERVER_MSG_MASK << SERVER_MSG_BIT_POSITION); // clear server message
			msg = (PUSHB << SERVER_MSG_BIT_POSITION) | msg; 

			pid = fork();
			if (pid == 0) 
			{
				system("./server2");
				exit(0);
			}
		}
	}

	// send the message 
	write(connfd, &msg, sizeof(msg)); 
};


int* Server::receiveMessageFromClient() 
{

	old_format_option = format_option;
	old_resolution_option = resolution_option;
	old_face_detection_code_option = face_detection_code_option;

	// Receive the message
	read(connfd, &msg, sizeof(msg));

	// Logic to read the message
	format_option = 
		((FORMAT_MASK << FORMAT_BIT_POSITION) & msg) 
		>> FORMAT_BIT_POSITION;
	resolution_option = 
		((RESOLUION_MASK << RESOLUION_BIT_POSITION) & msg) 
		>> RESOLUION_BIT_POSITION; 
	face_detection_code_option = 
		((FACE_DETECTION_MSG_MASK << FACE_DETECTION_MSG_BIT_POSITION) & msg)
		>> FACE_DETECTION_MSG_BIT_POSITION;

	if(format_option != old_format_option) 
	{
		client_options_change[0] = 1;
	} else 
	{
		client_options_change[0] = 0;
	}

	if(resolution_option != old_resolution_option) 
	{
		client_options_change[1] = 1;
	} 
	else 
	{
		client_options_change[1] = 0;
	}
	
	return client_options_change; 
};