#pragma once
#include "ofMain.h"

class Person{
  public:
    Person(int _x, int _y, int _w, int _h, int _id);
    ~Person();
  ofRectangle rect;
  bool available;
  bool del;
  int timer;
  int id;
  int dist;
  ofRectangle getScaledBB(float _w, float _h);
  // void draw();
  void update(const ofRectangle _r);
  void countDown();
  void updateTimer();
  bool dead();
};
