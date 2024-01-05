

#ifndef EXTERNVALUE_H_
#define EXTERNVALUE_H_

#include <string>
#include <stdint.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <unistd.h>
#include <algorithm>

/**
 * \class ExternValue
 * ExternValue est une classe qui permet la gestion des peripheriques du serveur
**/
class ExternValue {
public:
	/**
     * \fn ExternValue()
     * \brief Constructeur de la classe ExternValue
     * \return Instance de la classe ExternValue.
	 * Initialise les variables contenant les emplacements des fichiers des peripheriques et
	 * congifure les fichiers des peripheriques.
    */
	ExternValue();

	/**
     * \fn getValue()
     * \brief Fonction qui retourne les valeurs des peripheriques
	 * \return La valeur des peripheriques
	 * Lit les valeurs dans les fichiers des peripheriques et retourne leurs valeurs.
    */
	int* getValue();

	/**
     * \var BoutonValue
     * \brief Valeur du bouton
    */
	int BoutonValue;

	/**
     * \var ADCValue
     * \brief Valeur du convertisseur analogue a digital
    */
	int ADCValue;

	/**
     * \var BoutonPath
     * \brief Emplacement du fichier du bouton
    */
	std::string BoutonPath;

	/**
     * \var ADCPath
     * \brief Emplacement du fichier du convertisseur analogue a digital
    */
	std::string ADCPath;
private:
};
#endif 
