#include "client.h"

using namespace cv;
using namespace std;


void func(int sockfd)
{
    // variable declaration
    Client client(sockfd);
    char user_key;

    client.getInitialInputFromUser();
    client.setAndSendInitialMessageToServer();

    while(1) {

        client.receiveMessageFromServer();
    
        client.receiveAndDecodeImageFromServer();

        // Show image in a window
        imshow("Belle personne", client.decoded_image);   

        // Act if the server message is PUSHB
        client.handleServerPUSHB();

        /* Deal with an user key interuption */    
        user_key = waitKey(30);

        // reset message of client fidelity
        if (user_key == 27) {      
            client.msg &= !ELE4205_OK; // set message to not Ok
            cout << "QUIT" << endl;
            break;
        } else {
            client.msg |= ELE4205_OK; // set message to OK
        }

        client.changeOptions(user_key);
        client.trainFaceDetectorModel();
        client.sendMessageToServer();
    }
}
 
int main()
{
    int sockfd, connfd;
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
 
    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("192.168.7.2");
    servaddr.sin_port = htons(PORT1);
 
    // connect the client socket to server socket
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
        printf("connected to the server..\n");
 
    // function for chat
    func(sockfd);
 
    // close the socket
    close(sockfd);
}
