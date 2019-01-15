#include "Person.h"
Person::Person(int _x, int _y, int _w, int _h, int _id){
  rect = ofRectangle(_x,_y,_w,_h);
  id = _id;
  available = true;
  del = false;
  timer = 10;
}
Person::~Person(){
}

ofRectangle Person::getScaledBB(float _w, float _h){
    ofRectangle r_return;
    r_return.set(rect.x * _w,
                 rect.y * _h,
                 rect.width * _w,
                 rect.height * _h);
    return r_return;
}

  // void Person::draw(){
  //           ofNoFill();
  //       ofSetLineWidth(3);
  //   ofDrawRectangle(rect);
  // }
  void Person::update(const ofRectangle _r) {
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