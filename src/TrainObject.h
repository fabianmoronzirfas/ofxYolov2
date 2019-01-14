#pragma once
#include "ofMain.h"

class TrainObject
{
public:
  TrainObject(int _id, string _name, ofRectangle _r);
  ~TrainObject();
  int id;
  string name;
  ofRectangle r;
  ofRectangle getScaledBB(float _w, float _h);
};
