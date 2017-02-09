
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

class Hits {
public:

};

class HitDoublets {
public:
  using Hit=MyHit;
  enum layer { inner=0, outer=1};

  HitDoublets(std::vector<Hit>& innerHits, std::vector<Hit>& outerHits): layers{{innerHits, outerHits}}{}


  Hit const & hit(int i, layer l) const { return testHit;}
  float x(int i, layer l) const { return layers[l][i].x();}
  float y(int i, layer l) const { return layers[l][i].y();}
  float z(int i, layer l) const { return layers[l][i].z();}
  float r(int i, layer l) const { return layers[l][i].r();}
  float phi(int i, layer l) const { return layers[l][i].phi();}
  void add(int innerHit, int outerHit) { indices.push_back(innerHit); indices.push_back(outerHit); } 
  size_t size() const {return indices.size()/2;}
  int innerHitId(int i) const {return indices[2*i];}
  int outerHitId(int i) const {return indices[2*i+1];}
  std::array<std::vector<Hit>,2> layers;  
  Hit testHit_inner;
  Hit testHit_outer;
  Hit testHit;
  std::vector<int> indices;

};


class MyTrackingRegion {
public:
 MyTrackingRegion(): point(0, 1, 2) {}

 MyPoint origin() {return point;}
 float originRBound() {return 0.;}
  
private:
 
  MyPoint point;
};

typedef MyTrackingRegion TrackingRegion;

#endif
