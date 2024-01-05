#ifndef CLIENT_H_
#define CLIENT_H_

#include <inttypes.h>
#include <stdio.h> 
#include <netdb.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <unistd.h> // read(), write(), close()
#include <opencv2/opencv.hpp>
#include <bitset>
#include <sstream>

#include "FaceDetector.h"

#define PORT1 4099
#define PORT2 4100
#define SA struct sockaddr

#define ELE4205_OK 0b1
#define FORMAT_MASK 0b1
#define FORMAT_BIT_POSITION 1
#define RESOLUION_MASK 0b11
#define RESOLUION_BIT_POSITION 2
#define SERVER_MSG_MASK 0b11
#define SERVER_MSG_BIT_POSITION 4
#define FACE_DETECTION_MSG_MASK 0b11
#define FACE_DETECTION_MSG_BIT_POSITION 6
#define ADC_TOLERANCE 800
#define NUM_FRAMES_TRAINNING 300


/**
 * \class Client
 * Client est une classe qui permet la gestion du client
**/
class Client {
public:
	/**
     * \fn Client(const socklen_t given_sockfd)
     * \brief Constructeur de la classe Client
     * \param given_sockfd Le socket utilisé pour la communication avec le serveur
     * \return Instance de la classe Client.
    */
	Client(const socklen_t given_sockfd);

	/**
     * \fn setAndSendInitialMessageToServer()
     * \brief Configure et envoie le message au serveur
    */
	void setAndSendInitialMessageToServer();

	/**
     * \fn sendMessageToServer()
     * \brief Envoie le message au serveur
    */
	void sendMessageToServer();

	/**
     * \fn receiveMessageFromServer()
     * \brief Reçoit le message du serveur
    */
	void receiveMessageFromServer();

	/**
     * \fn receiveAndDecodeImageFromServer()
     * \brief Réception et décodage de l'image du serveur
    */
	void receiveAndDecodeImageFromServer();

	/**
     * \fn handleServerPUSHB()
     * \brief Opération à faire lors de la réception d'un signal PUSH_B du serveur
     * Lorsque le message du serveur est PUSH_B, la fonction analyse si c'est un front montant
     * du signal et si oui, la fonction fork est appelée pour faire une copie du programme qui 
     * fera la détection faciale.
    */
	void handleServerPUSHB();

	/**
     * \fn connectAndSendMessageToSecondServer()
     * \brief Connection et envoie du message avec le deuxième serveur
     * Connection TCP avec socket au deuxième serveur, envoie du message et fermeture
     * du socket.
    */
	void connectAndSendMessageToSecondServer();

	/**
     * \fn trainFaceDetectorModel()
     * \brief Fonction qui gère une session d'entraînement complète du reconnaisseur faciale.
     * Gère l'entrainement du reconnaisseur faciale et l'affichge de la progression dans le 
     * terminal. 
    */
	void trainFaceDetectorModel();

	/**
     * \fn configureFaceDetectionMessage(cv::Mat frame)
     * \brief Fonction qui configure le message de détection.
     * \param frame Image transmise par le serveur lors d'un PUSHB.
     * Le message de détection faciale est configuré selon les résultats des opérations faites
     * sur l'image capturé lors du signal PUSHB. Les opérations effectués sont des reconnaissances
     * faciales sur tous les visages détectés.
    */
	void configureFaceDetectionMessage(cv::Mat frame);

	/**
     * \fn recognizeFace(cv::Mat frame)
     * \brief Détecte et reconnait les visages présents dans l'image et émet une/des prédiction(s).
     * \param frame Une image à analyser pour la/les reconnaissance(s) faciales.
     * \return Un vecteur contenant une prédiction pour chaque visage détecté.
    */
	vector<int> recognizeFace(cv::Mat frame);

	/**
     * \fn getInitialInputFromUser()
     * \brief Fonction qui demande à l'usager d'entrer toutes les options désirées.
    */
	void getInitialInputFromUser();

	/**
     * \fn getResolutionOptionFromUser()
     * \brief Fonction qui demande à l'usager d'entrer la résolution d'image désirée.
    */
	void getResolutionOptionFromUser();

	/**
     * \fn getFormatOptionFromUser()
     * \brief Fonction qui demande à l'usager d'entrer le format d'image désiré.
    */
	void getFormatOptionFromUser();

	/**
     * \fn getFaceDetectionModeOptionFromUser()
     * \brief Fonction qui demande à l'usager d'entrer le mode de détection faciale désiré.
    */
	void getFaceDetectionModeOptionFromUser();

	/**
     * \fn getUserName()
     * \brief Fonction qui demande à l'usager d'entrer son nom dans le terminal.
    */
	void getUserName();

	/**
     * \fn changeOptions(char userKey)
     * \brief Fonction qui change les options choisies selon la touche appuyée par l'usager.
     * \param userKey Touche appuyée par l'usager
    */
	void changeOptions(char userKey);

	/**
     * \fn displayOptions()
     * \brief Affichage des options possibles pour l'usager dans le terminal.
    */
	void displayOptions();

	/**
     * \fn displaySelectedOptions()
     * \brief Affichage des options choisies par l'usager dans le terminal.
    */
	void displaySelectedOptions();

	/**
     * \fn setMessageAfterResolutionChange()
     * \brief Configurer le message selon la nouvelle option de résolution choisie.
    */
	void setMessageAfterResolutionChange();

	/**
     * \fn setMessageAfterFaceDetectionCodeChange()
     * \brief Configurer le message selon le nouveau code de détection faciale.
    */
	void setMessageAfterFaceDetectionCodeChange();

	/**
     * \var sockfd
     * \brief Valeur d'identifcation du socket utilisé par le client.
    */
	socklen_t sockfd;

    /**
     * \var byte_size_1
     * \brief Constante dont la valeur est d'un octet.
    */
	static const size_t byte_size_1 = 1;

    /**
     * \var buff_size
     * \brief Octet contenant la taille de l'image envoyé par le serveur
    */
	uint32_t buff_size;

    /**
     * \var msg
     * \brief Octet contenant le message pour la communication avec le serveur.
    */
	uint32_t msg;

    /**
     * \var tmp_byte_of_image
     * \brief Octet tampon qui contient reçoit les octets d'image encodés, envoyés par le serveur.
    */
	u_char tmp_byte_of_image;

    /**
     * \var mem_buffer_enc
     * \brief Vecteur contenant les octets d'image encodés, envoyés par le serveur.
    */
	std::vector<uchar> mem_buffer_enc;

    /**
     * \var server_msg
     * \brief Signal/message transmis par le serveur.
    */
	int server_msg;

	/**
     * \var frame_width_options
     * \brief Options de largeur pour l'image.
    */
	int frame_width_options[4] = {176, 320, 800, 960};

    /**
     * \var frame_height_options
     * \brief Options d'hauteur pour l'image.
    */
    int frame_height_options[4] = {144, 240, 600, 720};

    /**
     * \var selected_resolution_option
     * \brief Résolution d'image choisi par l'usager.
    */
    int selected_resolution_option;

    /**
     * \var format_options
     * \brief Options de format pour l'image.
    */
    std::string format_options[2] = {".jpg", ".png"};

    /**
     * \var selected_format_option
     * \brief Format d'image choisi par l'usager.
    */
    int selected_format_option;

    /**
     * \var face_detection_mode_options
     * \brief Options de mode de détection faciale.
    */
    std::string face_detection_mode_options[2] = {"Learning Mode", "Detection Mode"};

    /**
     * \var selected_face_detection_mode_option
     * \brief Mode de détection faciale choisi par l'usager.
    */
    int selected_face_detection_mode_option;

	/**
     * \var user_name
     * \brief Nom de l'usager courant.
    */
	std::string user_name;

	/**
     * \var decoded_image
     * \brief L'image décodée reçu du serveur.
    */
	cv::Mat decoded_image;

	/**
     * \var face_detector
     * \brief Instance de la classe face_detector
     * Cette classe permet la gestion de la détection et reconnaissance faciale.
     * Voir la classe FaceDetector.
    */
    FaceDetector face_detector;

	/**
     * \var pid
     * \brief Variable pour encapsuler l'identification du procede courant.
	 * Encapsule le retour de la fonction fork()
    */
	int pid;

    /**
     * \var PUSHB_flag
     * \brief Valeur du signal PUSHB transmis par le serveur.
    */
    bool PUSHB_flag = false; 

    /**
     * \var old_PUSHB_flag
     * \brief variable qui sert à détecter un front montant du signal PUSHB.
    */
	bool old_PUSHB_flag = false;

    /**
     * \var face_detection_code
     * \brief Code de l'état de la detection faciale.
    */
	int face_detection_code;
	
private:
};
#endif 
