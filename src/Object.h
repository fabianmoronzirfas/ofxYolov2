#pragma once
#include "ofMain.h"

class Object{
public:
    Object(int _class_id, string _name, float _confidence, float _x, float _y, float _w, float _h);
    ~Object();
    ofRectangle r;
    ofRectangle getScaledBB(float _w, float _h);
    string name;
    float confidence;
    int class_id;
    int x;
    int y;
    int w;
    int h;
};
