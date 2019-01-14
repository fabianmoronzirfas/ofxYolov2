#include "Person.h"
Person::Person(int _x, int _y, int _w, int _h, int _id){
  rect = ofRectangle(_x,_y,_w,_h);
  id = _id;
  available = true;
  del = false;
}
Person::~Person(){
}

  void Person::draw(){
            ofNoFill();
        ofSetLineWidth(3);
    ofDrawRectangle(rect);
  }
  void Person::update(const ofRectangle& _r){
    rect = ofRectangle(_r.getX(), _r.getY(), _r.getWidth(), _r.getHeight());
  }
  void Person::countDown(){
    timer--;
  }
  bool Person::dead(){
    if (timer < 0){
      return true;
    }
    return false;
  }