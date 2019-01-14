#include "TrainObject.h"

TrainObject::TrainObject(int _id, string _name, ofRectangle _r)
{
  id = _id;
  name = _name;
  r.set(_r);
}
TrainObject::~TrainObject()
{
}
ofRectangle TrainObject::getScaledBB(float _w, float _h)
{
  ofRectangle r_return;
  r_return.set(r.x * _w,
               r.y * _h,
               r.width * _w,
               r.height * _h);
  return r_return;
}