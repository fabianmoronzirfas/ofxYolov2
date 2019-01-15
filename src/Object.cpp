#include "Object.h"
Object::Object(int _class_id, string _name, float _confidence, float _x, float _y, float _w, float _h)
{
    class_id = _class_id;
    name = _name;
    confidence = _confidence;
    r.set(_x, _y, _w, _h);
}

Object::~Object()
{

}


ofRectangle Object::getScaledBB(float _w, float _h)
{
    ofRectangle r_return;
    r_return.set(r.x*_w,
                 r.y*_h,
                 r.width * _w,
                 r.height * _h);
    return r_return;
}
