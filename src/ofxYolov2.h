#pragma once

#include "ofMain.h"
#include <opencv2/dnn.hpp>
#include <opencv2/dnn/shape_utils.hpp>
#include <opencv2/core/ocl.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cstdlib>

#include "Object.h"
#include "TrainObject.h"
#include "Person.h"
#include "utilities.h"

using namespace std;
using namespace cv;
using namespace cv::dnn;

class ofxYolov2{
public:
    ofxYolov2();
    ~ofxYolov2();
    void setup(string _path_to_cfg, string _path_to_weights, string _path_to_names);
    void draw(float _x, float _y, float _w, float _h);
    void drawAnnotation(float _x, float _y, float _w, float _h);
    void update(ofPixels &op);
    void setConfidenceThreshold(float _threshold);

    void setNextAnnotation();
    void setPreviousAnnotation();

    void loadAnnotationDir(string _path_to_file);
    void loadAnnotationImage(string _path_to_file);
    void loadBoundingBoxFile(string _path_to_file);
    void saveAnnotation();
    void saveBoundingBoxToFile(string _path_to_file);

    void drawClassSelector(float _x, float _y, int _row);
    void addTrainObject(ofRectangle _r);
    int getPersonCount();

    vector<Object> objects;
    vector<TrainObject>train;
    vector<Object> persons;

    cv::Mat toCV(ofPixels &pix);
    dnn::Net net;
    int network_width = 416;
    int network_height = 416;
    vector<string> classNamesVec;
    vector<ofColor> detection_color;
    vector<Person> person_persistance_list;
    vector<String> outNames;

    float confidenceThreshold;
    ofTrueTypeFont font_info;

    string filename;
    string filename_jpg;
    string filename_txt;
    string filepath;
    ofDirectory dir_annotation;
    int pos_annotation_file;
    ofImage image_annotation;
    int class_id_selected;
    int yolo_version = 2;
    int person_count = 0;

};
