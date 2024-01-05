#include "client.h"

using namespace std;
using namespace cv;


/* Constructor */
Client::Client(const socklen_t given_sockfd) 
{
	sockfd = given_sockfd;
	face_detector.number_pictures_taken_for_trainning = -1;
};


/* Communication related methods */
void Client::setAndSendInitialMessageToServer() 
{
	//Set the format bits
    if (selected_format_option) 
	{
		//Set the bit to 1
        msg |= 1 << FORMAT_BIT_POSITION; 
    } 
	else 
	{
		// Clear the bit to 0
        msg &= ~(1 << FORMAT_BIT_POSITION);   
    }

	// Clear the resolution bits 
    msg &= ~(RESOLUION_MASK << RESOLUION_BIT_POSITION);  
	// Set the resolution bits     
    msg = (selected_resolution_option << 2) | msg;  
    
    // Send the selection to the server
    write(sockfd, &msg, sizeof(msg)); 
};


void Client::sendMessageToServer() 
{
	write(sockfd, &msg, sizeof(msg));
};


void Client::receiveMessageFromServer() 
{
	do { 
		// receive server message
		read(sockfd, &msg, sizeof(msg)); 

		server_msg = ((SERVER_MSG_MASK << SERVER_MSG_BIT_POSITION) & msg) >> 2;
	} while (server_msg == 0); // while IDOWN

};


void Client::receiveAndDecodeImageFromServer() {
	// Receiving image length from the server
	read(sockfd, &buff_size, sizeof(buff_size));    
	
	// Receive image from the server
	mem_buffer_enc.clear();
	for (int i=0; i<buff_size; i++) 
	{
		read(sockfd, &tmp_byte_of_image, byte_size_1);
		mem_buffer_enc.push_back(tmp_byte_of_image);
	}

	// Decode the image vector
	decoded_image = imdecode(mem_buffer_enc, cv::IMREAD_UNCHANGED); 
};


void Client::handleServerPUSHB() 
{

	// Update the state of PUSHB
	if (server_msg == 8) 
	{
		PUSHB_flag = true;
	} 
	else 
	{
		PUSHB_flag = false;
	}

	if(PUSHB_flag && !old_PUSHB_flag) // Verify that it's the first PUSHB
	{  
		pid = fork();
		face_detector.number_image_saved++;
		if (pid == 0) 
		{
			configureFaceDetectionMessage(decoded_image);
			connectAndSendMessageToSecondServer();
			exit(0);
		} 
	}

	old_PUSHB_flag = PUSHB_flag;
};


void Client::connectAndSendMessageToSecondServer()
{
	int sockfd, connfd;
    struct sockaddr_in servaddr, cli;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    // else
    //     printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));
 
    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("192.168.7.2");
    servaddr.sin_port = htons(PORT2);
 
    // connect the client socket to server socket
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("connection with the 2nd server failed...\n");
        exit(0);
    }
    // else
    //     printf("connected to the 2nd server..\n");
 
    // function for chat
    write(sockfd, &msg, sizeof(msg));
 
    // close the socket
    close(sockfd);
	// printf("closed connection to 2nd server..\n");
};


void Client::trainFaceDetectorModel() 
{
	if(selected_face_detection_mode_option == 0) 
	{
		if (face_detector.number_pictures_taken_for_trainning == 0)
		{
			getUserName();
			face_detector.existing_faces.push_back(user_name);

			face_detector.number_pictures_taken_for_trainning++;
			cout << "Trainning Started!" << endl;
		} 
		else if (face_detector.number_pictures_taken_for_trainning > 0)
		{
			if (face_detector.number_pictures_taken_for_trainning > NUM_FRAMES_TRAINNING)
			{
				// Reset counter
				face_detector.number_pictures_taken_for_trainning = -1;
				
				// Train the model
				face_detector.face_recognizer->train(
					face_detector.frame_vector, 
					face_detector.label_vector);

				// Save the new model
				face_detector.saveFaces();
				
				cout << "Trainning Completed!" << endl << endl;
				displayOptions();
			}
			else
			{	
				vector<Mat> cropped_frames;
				
				try
				{
					cropped_frames = face_detector.detectAndCropFace(decoded_image);
				}
				catch(string errorMessage)
				{
					cout << errorMessage << endl << endl;
					return;
				}

				face_detector.trainFaceRecognizer(cropped_frames[0], user_name);
				
				cout << "Progress " << face_detector.number_pictures_taken_for_trainning/3 << "%" << endl;
				face_detector.number_pictures_taken_for_trainning++;
			}
			
		}
	}
};


void Client::configureFaceDetectionMessage(Mat frame) 
{
	if(selected_face_detection_mode_option) // DETECTION MODE
	{
		vector<int> is_detected = recognizeFace(frame);
		bool allPredictionsFailed = true;

		face_detector.savePicture(decoded_image, is_detected, format_options[selected_format_option]);

		for (int i=0; i<is_detected.size(); i++)
		{
			if (is_detected[i] != -1)
			{
				allPredictionsFailed = false;
			}
		}

		if (allPredictionsFailed) 
		{
			face_detection_code = 2; // UNKNOWN
		} 
		else 
		{
			face_detection_code = 1; // KNOWN
		}
	} 
	else // LEARNING MODE
	{
		face_detection_code = 0; // LEARN
	}

	setMessageAfterFaceDetectionCodeChange();
};


vector<int> Client::recognizeFace(Mat frame) 
{
	vector<Mat> adjusted_frames;
	vector<int> predictions;

	try
	{
		adjusted_frames = face_detector.detectAndCropFace(frame);
		cout << "number of faces: " << adjusted_frames.size() <<endl;
	}
	catch (string errorMessage)
	{
		cout << errorMessage << endl << endl;
		predictions.push_back(-1);
		return predictions;
	}

	cout << "------------------------" << endl;
	for (int i = 0; i < adjusted_frames.size(); i++)
	{
		predictions.push_back(face_detector.recognizeFace(adjusted_frames[i]));
	}

	return predictions;
}


/* Options related methods */
void Client::getInitialInputFromUser() 
{
	/* Resolution OPTION */
	getResolutionOptionFromUser();  

	/* FORMAT OPTION */
	getFormatOptionFromUser();

	/* FACE DETECTION MODE OPTION */
    getFaceDetectionModeOptionFromUser();

	displayOptions();
};


void Client::getResolutionOptionFromUser() 
{
	cout << endl;

	// User selection of the resolution
    for(int i = 0; i < 4; i++)
    {
        cout << i << " - " << frame_width_options[i] << " x " << frame_height_options[i] << endl;
    }
    cout << "Select the resolution (0 to 3) of the video : " << endl;
    cin >> selected_resolution_option;

    // Check if the entry is valid
    if(selected_resolution_option < 0 || selected_resolution_option >= 4)
    {
        cout << "This resolution option is not valid!" << endl;
        return ;
    }

    int frame_width = frame_width_options[selected_resolution_option]; //get the width of frames of the video
    int frame_height = frame_height_options[selected_resolution_option]; //get the height of frames of the video
    cout << "You selected the " << frame_width << " x " << frame_height << " resolution" << endl << endl;
};


void Client::getFormatOptionFromUser() 
{
	// User selection of the format
    for(int i = 0; i < 2; i++)
    {
        cout << i << " - " << format_options[i] << endl;
    }
    cout << "Select the format (0 or 1) of the video : " << endl;
    cin >> selected_format_option; 

    // Check if the entry is valid
    if(selected_format_option < 0 && selected_format_option > 1)
    {
        cout << "This format option is not valid!" << endl;
        return ;
    }

    string format = format_options[selected_format_option];
    cout << "You selected the " << format << " format." << endl << endl;
};


void Client::getFaceDetectionModeOptionFromUser() 
{
	// User selection of the face detection option
    for(int i = 0; i < 2; i++)
    {
        cout << i << " - " << face_detection_mode_options[i] << endl;
    }
    cout << "Select the Face detection Mode (0 or 1) : " << endl;
    cin >> selected_face_detection_mode_option; 
// Verify that it's the first PUSHB
    // Check if the entry is valid
    if(selected_face_detection_mode_option < 0 && selected_face_detection_mode_option > 1)
    {
        cout << "This face detection mode option is not valid!" << endl;
        return ;
    }

    string face_detection_mode = face_detection_mode_options[selected_face_detection_mode_option];
    cout << "You selected the " << face_detection_mode << "." << endl << endl;

	if(selected_face_detection_mode_option == 0) 
	{
		face_detector.number_pictures_taken_for_trainning = 0;
	}
};


void Client::getUserName() 
{
	cout << "Enter your name: ";
	cin >> user_name;
	cout << endl << endl;

	// vector<string>::iterator it;
	// it = find(face_detector.existing_faces.begin(),
	// 	face_detector.existing_faces.end(),
	// 	user_name);

	// if (it != face_detector.existing_faces.end())
	// {
	// 	getUserName();
	// }
};


void Client::changeOptions(char user_key) 
{
	switch(user_key) {

		case 49: // key 1
			selected_format_option = 0; // JPEG 
			msg &= ~(FORMAT_MASK << FORMAT_BIT_POSITION); // Clear the bit to 0       
			displaySelectedOptions();
			break;

		case 50: // key 2
			selected_format_option = 1; // PNG 
			msg |= FORMAT_MASK << FORMAT_BIT_POSITION; //Set the bit to 1
			displaySelectedOptions();
			break;

		case 51: // key 3
			selected_resolution_option = 0; // 176x144
			setMessageAfterResolutionChange();
			displaySelectedOptions();
			break;

		case 52: // key 4
			selected_resolution_option = 1; // 352x288
			setMessageAfterResolutionChange();
			displaySelectedOptions();
			break;

		case 53: // key 5
			selected_resolution_option = 2; // 960x544
			setMessageAfterResolutionChange();
			displaySelectedOptions();
			break;

		case 54: // key 6
			selected_resolution_option = 3; // 1280x960
			setMessageAfterResolutionChange();
			displaySelectedOptions();
			break;
		
		case 55: // key 7
			if (face_detector.number_pictures_taken_for_trainning > -1)
			{
				cout << "Please wait for the current trainning to be over." << endl;
			} 
			else 
			{
				face_detector.number_pictures_taken_for_trainning = 0;

				selected_face_detection_mode_option = 0;
				displaySelectedOptions();
			}
			break;

		case 56: // key 8
			if (selected_face_detection_mode_option == 0)
			{
				if (face_detector.number_pictures_taken_for_trainning < 0)
				{
					selected_face_detection_mode_option = 1;
					displaySelectedOptions();
				} else {
					cout << "Cannot Switch to Detection Mode during the trainning" << endl;
					cout << "Wait for the "<< NUM_FRAMES_TRAINNING; 
					cout << "frames to be taken" << endl;
				}
				
			} 
			else 
			{
				selected_face_detection_mode_option = 1;
				displaySelectedOptions();
			}			
			break;	
    }
};


void Client::displayOptions() {
	cout << "To change the the options press : " << endl;
	cout << "1 - .jpg" << endl;
	cout << "2 - .png" << endl;
	cout << "3 - 176 x 144" << endl;
	cout << "4 - 320 x 240" << endl;
	cout << "5 - 800 x 600" << endl;
	cout << "6 - 900 x 720" << endl;
	cout << "7 - Learning Mode" << endl;
	cout << "8 - Detection Mode" << endl << endl;
};


void Client::displaySelectedOptions() {
	/* Fetch the selected options */
	int frame_width = frame_width_options[selected_resolution_option];
    int frame_height = frame_height_options[selected_resolution_option];
	string format = format_options[selected_format_option];
	string face_detection_mode = face_detection_mode_options[selected_face_detection_mode_option];

	/* Display the selected options */
	cout << "You selected the " << frame_width << " x " << frame_height << " resolution" << endl; 
    cout << "You selected the " << format << " format." << endl;
	cout << "You selected the " << face_detection_mode << "." << endl << endl;

	/* Display the options available */
	displayOptions();
};


/* Message and options related methods */
void Client::setMessageAfterResolutionChange() {
	msg &= ~(RESOLUION_MASK << RESOLUION_BIT_POSITION);
	msg = (selected_resolution_option << RESOLUION_BIT_POSITION) | msg;
};


void Client::setMessageAfterFaceDetectionCodeChange() {
	msg &= ~(FACE_DETECTION_MSG_MASK << FACE_DETECTION_MSG_BIT_POSITION);
	msg = (face_detection_code << FACE_DETECTION_MSG_BIT_POSITION) | msg;
};