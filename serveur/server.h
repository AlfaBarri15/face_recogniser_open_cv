#ifndef SERVER_H_
#define SERVER_H_

#include <inttypes.h>
#include <stdio.h> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <unistd.h> // read(), write(), close()
#include <opencv2/opencv.hpp> // Imports from main.cpp
#include <bitset>

#include "ExternValue.h"

#define ELE4205_OK 0b1
#define FORMAT_MASK 0b1
#define FORMAT_BIT_POSITION 1
#define RESOLUION_MASK 0b11
#define RESOLUION_BIT_POSITION 2
#define SERVER_MSG_MASK 0b11
#define SERVER_MSG_BIT_POSITION 4
#define FACE_DETECTION_MSG_MASK 0b11
#define FACE_DETECTION_MSG_BIT_POSITION 6
#define READY 1
#define PUSHB 2
#define ADC_TOLERANCE 800


/**
 * \class Server
 * Server est une classe qui permet la gestion du serveur
**/
class Server {
public:
	/**
     * \fn Server(const socklen_t given_connfd)
     * \brief Constructeur de la classe Server
     * \param given_connfd  Le socket utilisé pour la communication avec le client
     * \return Instance de la classe Server.
    */
	Server(const socklen_t given_connfd);
	
	/**
     * \fn sendFrameToClient(const cv::Mat frame, const std::string format)
     * \brief Fonction qui encode et envoie une image au client.
     * \param frame L'image à envoyer au client
     * \param format   Format de l'image pour l'encodage
     * Encode l'image dans le format désiré et l'envoie au client .
    */
	void sendFrameToClient(const cv::Mat frame, const std::string format);

	/**
     * \fn sendMessageToClient()
     * \brief Fonction qui lit la valeur des peripheriques et qui envoit le message au client.
     * Lit les valeurs des périphériques, modifie le message en conséquence et envoie le message
     * au client.
    */
	void sendMessageToClient();

	/**
     * \fn receiveMessageFromClient()
     * \brief Fonction qui recoit le message du client.
     * \return Which client option has changed  
     * Reçoit le message du client et change les variables concernées en conséquence.
    */
	int* receiveMessageFromClient();

	/**
     * \var connfd
     * \brief Valeur d'identifcation du socket utilisé par le serveur.
    */
	socklen_t connfd;

	/**
     * \var byte_size_1
     * \brief Constante dont la valeur est d'un octet
    */
	static const size_t byte_size_1 = 1;

	/**
     * \var buff_size
     * \brief Octet contenant la taille de l'image à envoyer au client.
    */
	uint32_t buff_size;

	/**
     * \var msg
     * \brief Octet contenant le message pour la communication avec le client.
    */
	uint32_t msg;

	/**
     * \var mem_buffer
     * \brief Vecteur contenant les octets de l'image à envoyer au client.
    */
	std::vector<uchar> mem_buffer;

	/**
     * \var old_format_option
     * \brief Format d'image choisi par l'usager sur le client avant la reception de la nouvelle valeur.
    */
	int old_format_option;

	/**
     * \var format_option
     * \brief Format d'image choisi par l'usager sur le client.
    */
	int format_option;

	/**
     * \var old_resolution_option
     * \brief Resolution d'image choisie par l'usager sur le client avant la réception de la nouvelle valeur.
    */
	int old_resolution_option;	

	/**
     * \var resolution_option
     * \brief Resolution d'image choisie par l'usager sur le client
    */
	int resolution_option;	

	/**
     * \var old_face_detection_code_option
     * \brief Code de la détection faciale fournie par le client avant la réception de la nouvelle valeur.
    */
	int old_face_detection_code_option;

	/**
     * \var face_detection_code_option
     * \brief Code de la détection faciale fournie par le client.
    */
	int face_detection_code_option;

	/**
     * \var client_options_change
     * \brief Tableau qui indique si la valeur d'un peripherique a changé.
    */
	int client_options_change[2]; 

	/**
     * \var adc_value
     * \brief Valeur du convertisseur analogue à digital.
    */
	int* adc_value; 

	/**
     * \var externValue
     * \brief Instance de la classe ExternValue.
    */
	ExternValue externValue;

	/**
     * \var pid
     * \brief Variable pour encapsuler l'identification du procédé courant.
	 * Encapsule le retour de la fonction fork()
    */
	int pid;

	/**
     * \var pushb_counter
     * \brief Compteur pour la validation d'un front montant du bouton.
    */
	int pushb_counter;
private:
};
#endif 