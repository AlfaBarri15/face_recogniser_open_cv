#include <bits/stdc++.h>
#include <typeinfo>

#include "FaceDetector.h"

using namespace std;


FaceDetector::FaceDetector() 
{
    face_cascade_file = "../xml_files/haarcascade_frontalface_alt.xml";
    //double treshold = 20000.0;
    face_recognizer = cv::createLBPHFaceRecognizer();

    try
    {
        existing_faces = loadVector();
        face_recognizer->load(MODEL_FILE_LOCATION);
        cout << "Loaded latest model" << endl;
    }
    catch(const std::exception& e)
    {
        cout << "No model to load" << endl;
        cout << "Choose the Learning Mode" << endl;
    }
};


vector<string> FaceDetector::loadVector()
{
    vector<string> loadedVector;
    std::ifstream input_file("output.txt");

    if (input_file.is_open())
    {
        string line;

        while (getline(input_file, line))
        {
            loadedVector.push_back(line);
        }
        input_file.close();

        cout << "The data is loaded!" << endl;
    }
    else
    {
        cout << "Failed to load the data." << endl;
    }

    return loadedVector;
}


void FaceDetector::saveFaces()
{
    std::ofstream output_file("output.txt");

    if (output_file.is_open())
    {
        for (int i = 0; i < existing_faces.size(); i++)
        {
            output_file << existing_faces[i] << '\n';
        }
        output_file.close();

        face_recognizer->save(MODEL_FILE_LOCATION);

        cout << "The data is saved!" << endl;
    }
    else
    {
        cout << "Failed to save the data." << endl;
    }
}


int FaceDetector::findFaceNameIndex(string username)
{
    vector<string>::iterator it = find(existing_faces.begin(), existing_faces.end(), username);

    if (it != existing_faces.end())
    {
        int index = it - existing_faces.begin();
        return index;
    }
    else
    {
        return -1;
    }
};


cv::Mat FaceDetector::detectFaceRectangles(cv::Mat frame, vector<int> predictions) 
{
    if(!face_cascade.load(face_cascade_file)) 
    {
        cerr << "Erreur de chargement du fichier XML de classification!" << endl;
        return frame;
    }

    // Convert the image in shades of gray for a better recognition
    cv::Mat gray_frame;
    cv::cvtColor(frame, gray_frame, cv::COLOR_BGR2GRAY); 
    cv::equalizeHist(gray_frame, gray_frame);

    // Face detection
    std::vector<cv::Rect> faces;
    face_cascade.detectMultiScale(frame, faces, 1.1, 2, 0|cv::CASCADE_SCALE_IMAGE, cv::Size(30,30));

    // Draw rectangles around the detected faces
    for(int i=0; i < faces.size(); i++) 
    {
        if(predictions[i] != -1)
        {
            cv::rectangle(frame, faces[i], cv::Scalar(0, 255, 0), 2);
            
            string text = existing_faces[predictions[i]];
            int text_x = faces[i].x + (faces[i].width - text.size()*8)/2;
            int text_y = faces[i].y - 10;

            cv::putText(frame, text, cv::Point(text_x, text_y),cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255,255,255), 1, false);
        }
    }

    // Return the frame with the detected faces
    return frame;
};


vector<cv::Mat> FaceDetector::detectAndCropFace(cv::Mat frame) {
    if(!face_cascade.load(face_cascade_file)) {
        cerr << "Erreur de chargement du fichier XML de classification!" << endl;
        return frame;
    }

    // Convert the image in shades of gray for a better recognition
    cv::Mat gray_frame;
    cv::cvtColor(frame, gray_frame, cv::COLOR_BGR2GRAY); 
    cv::equalizeHist(gray_frame, gray_frame);

    // Face detection
    std::vector<cv::Rect> faces;
    face_cascade.detectMultiScale(gray_frame, faces, 1.1, 2, 0|cv::CASCADE_SCALE_IMAGE, cv::Size(35,35));

    vector<cv::Mat> cropped_images;
    cv::Mat cropped_frame;

    // Crop the detected faces
    if (faces.size() > 1) 
    {
        cout << "More than 1 face!!" << endl;
    }
    else if (faces.size() < 1)
    {
        string errorMessage = "No face";
        throw (errorMessage);
    }

    for (int i=0; i<faces.size(); i++)
    {
        cv::Rect roi_rect(faces[i].x, faces[i].y, faces[i].width, faces[i].height);
        cropped_frame = frame(roi_rect);

        cv::resize(cropped_frame, cropped_frame, cv::Size(300, 300));
        cv::cvtColor(cropped_frame, cropped_frame, cv::COLOR_BGR2GRAY); 
        cv::equalizeHist(cropped_frame, cropped_frame);
        
        cropped_images.push_back(cropped_frame);
    }

    return cropped_images;
}


void FaceDetector::trainFaceRecognizer(cv::Mat frame, string username) {
    int label = findFaceNameIndex(username);
    if(label == -1) { cout << "Name not recognized" << endl; label = 100;}

    label_vector.push_back(label);
    frame_vector.push_back(frame);
};


int FaceDetector::recognizeFace(cv::Mat frame)
{
    int label;
    double confidence;
    
    frame_vector.push_back(frame);
    face_recognizer->predict(frame, label, confidence);
    frame_vector.clear();

    if (confidence > 75)
    {
        label = -1;
    }

    if (label > -1)
    {
        if (confidence < 75)
        {
            cout << "Prediction: " << existing_faces[label] << endl;
            cout << "Confidence : " << confidence << endl;
        } else {
            cout << "Face not detected!" << endl;
            cout << "Prediction: " << existing_faces[label] << endl;
            cout << "Confidence : " << confidence << endl;
        }

    } else if(label = -1) 
    {
        cout << "Face not recognized!" << endl;
        cout << "Confidence : " << confidence << endl;
    }
    cout << endl;
    
    return label;
}


void FaceDetector::savePicture(cv::Mat decoded_image, vector<int> predictions, string format)
{
    Mat decoded_image_with_rectangles = detectFaceRectangles(decoded_image, predictions);
    ss << number_image_saved;
    name_image_saved = "../pictures/Belle_personne_enregistree_" + ss.str() + format;
    imwrite(name_image_saved, decoded_image_with_rectangles);
};