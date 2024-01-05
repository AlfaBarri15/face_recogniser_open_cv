/*
 * ExternValue.cpp
 *
 *  Created on: Aug 13, 2015
 *      Author: Lucas Malo Bélanger
 */

#include "ExternValue.h"
using namespace std;


ExternValue::ExternValue() {
	// Initialize the path of the peripherals
	BoutonPath = "/sys/class/gpio/gpio228/value";
	ADCPath = "/sys/class/saradc/ch0";

	// Configure the button
	std::ofstream button_export_file, button_direction_file;

	button_export_file.open("/sys/class/gpio/export");
	button_export_file << 228;
	button_export_file.close();

	button_direction_file.open("/sys/class/gpio/gpio228/direction");
	button_direction_file << "in";
	button_direction_file.close();
};


int* ExternValue::getValue()
{
	std::ifstream bouton_file, adc_file;	

	bouton_file.open(BoutonPath.c_str());
	bouton_file >> BoutonValue;
	bouton_file.close();

	adc_file.open(ADCPath.c_str());
	adc_file >> ADCValue;
	adc_file.close();

	static int values[2]; 
	values[0] = BoutonValue;
	values[1] = ADCValue;

	return values;
};

