
#ifndef MYHITS_H
#define MYHITS_H


#include <vector>
#include <array>
#include <cmath>

class MyPoint {
public:
  MyPoint(float ax, float ay, float az): m_x(ax), m_y(ay), m_z(az), m_r(std::sqrt(ax*ax + ay*ay)), m_phi(std::atan2(ay, ax))  {}
  float x()const {return m_x;}
  float y()const {return m_y;}
  float z()const {return m_z;}
  float r()const {return m_r;}
  float phi()const {return m_phi;}

private:
  float m_x;
  float m_y;
  float m_z;
  float m_r;
  float m_phi;
};

class MyHit {
 public:
 MyHit(): thePoint(1,2,3) {}
 MyHit(float ax, float ay, float az): thePoint(ax, ay, az) {}
 double phi() {return thePoint.phi();}
 double x() const {return thePoint.x();}
 double y() const {return thePoint.y();}
 double z() const {return thePoint.z();}
 double r() const {return thePoint.r();}
 double phi() const {return thePoint.phi();}
 MyPoint thePoint;

};
class HitDoublets {
public:
  HitDoublets(): testHit_inner(1,2,3), testHit_outer(3,4,5) { 
    layers[1].push_back(testHit_outer);
    layers[0].push_back(testHit_inner);
    }
  enum layer { inner=0, outer=1};

  using Hit=MyHit;

  Hit const & hit(int i, layer l) const { return testHit;}
  float x(int i, layer l) const { return layers[l][i].x();}//testHit.x();}
  float y(int i, layer l) const { return layers[l][i].y();}
  float z(int i, layer l) const { return layers[l][i].z();}
  float r(int i, layer l) const { return layers[l][i].r();}
  float phi(int i, layer l) const { return layers[l][i].phi();}
  std::array<std::vector<Hit>,2> layers;  
  Hit testHit_inner;
  Hit testHit_outer;
  Hit testHit;

};


class MyTrackingRegion {
public:
 MyTrackingRegion(): point(0, 1, 2) {}

 MyPoint origin() {return point;}
 float originRBound() {return 0.;}
  
private:
 
  MyPoint point;
};

#endif
