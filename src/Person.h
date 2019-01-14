#pragma once
#include "ofMain.h"

class Person{
  public:
    Person(int _x, int _y, int _w, int _h, int _id);
    ~Person();
  ofRectangle rect;
  bool available;
  bool del;
  int timer = 127;
  int id;
  void draw();
  void update(const ofRectangle& _r);
  void countDown();
  bool dead();
};
