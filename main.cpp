
#include "CACell.h"
#include <iostream>
//#include "MyHits.h"

int main() {

  //auto hit1 = MyHits();
  auto hit = new MyHit();
  auto doublet = new HitDoublets();
  auto c = CACell(doublet, 0, 1, 2,3); 
  std::cout<<c.getInnerX()<<std::endl;
  std::cout<<doublet->r(0,HitDoublets::inner) << "\t"<< doublet->phi(0,HitDoublets::inner) << std::endl;
  std::cout<<doublet->r(0,HitDoublets::outer) << "\t"<< doublet->phi(0,HitDoublets::outer) << std::endl;
  return 0;
}
