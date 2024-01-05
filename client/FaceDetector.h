#ifndef FACEDETECTOR_H_
#define FACEDETECTOR_H_

#include <iostream>
#include <opencv2/opencv.hpp> 
#include <string>
#include <dirent.h>
#include <sys/types.h>

#define MODEL_FILE_LOCATION "model.xml"

using namespace std;
using namespace cv;

/**
 * \class FaceDetector
 * Classe qui permet la détection et reconnaissance faciale.
**/
class FaceDetector {
public:
    /**
     * \fn FaceDetector()
     * \brief Constructeur de la classe FaceDetector. Initialisation des emplacements de fichiers, creation de du reconnaisseur
     * facial et chargement des donnees existantes.
     * \return  Instance de la classe FaceDetector.
     * 
    */
    FaceDetector();

    /**
     * \fn loadVector()
     * \brief Charge les noms associés aux labels dans la base de donnée
     * \return La liste des noms ordonnés dans la base de donnée
    */
    vector<string> loadVector();

    /**
     * \fn saveFaces()
     * \brief Sauvergade dans la base de donnée les noms de façon ordonnée
    */
	void saveFaces();
    
    /**
     * \fn findFaceNameIndex(string username)
     * \brief Retourne l'index du nom sauvergardé
     * \param username Usager dont nous voulons trouver l'index
     * \return L'index du nom dans le vecteur des noms des visages sauvegardés
    */
    int findFaceNameIndex(string username);
    
    /**
     * \fn detectFaceRectangles(Mat frame, vector<int> predictions)
     * \brief Dessine des rectangles autour des visages reconnus avec le nom des visages
     * \param frame Une image transmise par le serveur
     * \param predictions   Prédiction par visage detecté
     * \return Image avec des rectangles autour des visages reconnus
    */
    Mat detectFaceRectangles(Mat frame, vector<int> predictions);

    /**
     * \fn detectAndCropFace(Mat frame)
     * \brief Détecte les visages et les découpent en rectangles 
     * \param frame Une image transmise par le serveur
     * \return  Un vecteur avec toutes les images ajustés des visages détectés
     * Fait la détection faciale et coupe chaque visage en rectangle. Ajustement des visages
     * en taille et en noir et blanc avant de les retourner dans un vecteur.
    */
    vector<Mat> detectAndCropFace(Mat frame);

    /**
     * \fn trainFaceRecognizer(Mat frame, string username)
     * \brief Ajoute l'image et le nom aux vecteur utilisé pour l'entrainement de la reconnaissance faciale
     * \param frame Image du visage ajusté sur lequel on fait l'entrainement de la reconnaissance faciale
     * \param username  Nom de l'usage
     * Cette fonction ne fait pas l'entrainement, mais ajoute l'image et le label associé à
     * l'usager courant aux vecteurs utilisés lors de l'entraînement.
    */
    void trainFaceRecognizer(Mat frame, string username);

    /**
     * \fn recognizeFace(Mat frame)
     * \brief Prédiction du label à partir d'une image d'un seul visage
     * \param frame Image du visage ajusté sur lequel on fait la reconnaissance faciale
     * \return Label associé au visage dans l'image
     * Prédiction du visage sur l'image formatté et affichage dans le terminal de la prédiction
     * et du niveau de confiance.
    */
    int recognizeFace(Mat frame);

    /**
     * \fn savePicture(Mat decoded_image, vector<int> predictions, string format)
     * \brief Dessine les rectangles autour des visages prédits et enregistre l'image.
     * \param decoded_image Image
     * \param predictions   Prédiction de la reconnaissance faciale pour chaque visage detecté
     * \param format        Format dans lequel l'image doit être enregistré
    */
    void savePicture(Mat decoded_image, vector<int> predictions, string format);
    
    /**
     * \var face_recognizer
     * \brief Pointeur vers une instance de la classe FaceRecognizer
     * Instance qu'on peut entraîner pour la reconnaissance faciale. Prédit également le 
     * label à partir d'une image. Voir la documentation de opencv 2.4.5 sur FaceRecognizer.
    */
    Ptr<FaceRecognizer> face_recognizer;

    /**
     * \var existing_faces
     * \brief Vecteur avec toutes les noms des visages enregistrés dans le LEARNING MODE.
    */
    vector<string> existing_faces;

    /**
     * \var frame_vector
     * \brief Vecteur avec toutes les images ordonnées pour le LEARNING MODE.
    */
    vector<Mat> frame_vector;

    /**
     * \var label_vector
     * \brief Vecteur avec tout les labels ordonnés pour le LEARNING MODE.
    */
    vector<int> label_vector;

    /**
     * \var number_pictures_taken_for_trainning
     * \brief Compteur du nombre d'images prises lors d'un entraînement.
    */
    int number_pictures_taken_for_trainning;

    /**
     * \var face_cascade_file
     * \brief Emplacement et nom du fichier xml pour la détection de visages.
    */
    string face_cascade_file;

    /**
     * \var face_cascade
     * \brief Instance de la classe CascadeClassifier.
     * Instance qui nous permet de faire de la détection de visages dans une image.
    */
    CascadeClassifier face_cascade;

    /**
     * \var ss
     * \brief Stringstream utilisé pour le nom des images sauvegardées.
    */
    std::stringstream ss;

    /**
     * \var number_image_saved
     * \brief Nombre d'image sauvergardé pendant un cycle de vie du programme.
    */
    int number_image_saved = 0;

    /**
     * \var name_image_saved
     * \brief Nom de l'image sauvegardé après un signal PUSH_B.
    */
    std::string name_image_saved;
private:
};
#endif 