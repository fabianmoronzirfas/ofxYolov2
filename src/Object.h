#pragma once
#include "ofMain.h"

class Object{
public:
    Object(int _class_id, string _name, float _p, float _x, float _y, float _w, float _h);
    ~Object();
    ofRectangle r;
    ofRectangle getScaledBB(float _w, float _h);
    string name;
    float p;
    int class_id;
};
