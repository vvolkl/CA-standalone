
#include "CACell.h"
#include "CellularAutomaton.h"
#include <iostream>
//#include "MyHits.h"

int main() {

  std::vector<MyHit> innerHits;
  innerHits.push_back(MyHit(1,1,0)); 
  innerHits.push_back(MyHit(3,1,0)); 
  innerHits.push_back(MyHit(4,3,0)); 
  innerHits.push_back(MyHit(4,4,0)); 
  innerHits.push_back(MyHit(2,3,0)); 
  innerHits.push_back(MyHit(4,2,0)); 
  std::vector<MyHit> outerHits;
  outerHits.push_back(MyHit(1,1,1)); 
  outerHits.push_back(MyHit(3,1,1)); 
  outerHits.push_back(MyHit(4,3,1)); 
  outerHits.push_back(MyHit(4,4,1)); 
  outerHits.push_back(MyHit(0,4,1)); 
  outerHits.push_back(MyHit(4,0,1)); 
  std::vector<MyHit> outermostHits;
  outermostHits.push_back(MyHit(1,1,2)); 
  outermostHits.push_back(MyHit(3,1,2)); 
  outermostHits.push_back(MyHit(4,3,2)); 
  outermostHits.push_back(MyHit(4,4,2)); 
  outermostHits.push_back(MyHit(1,3,2)); 
  outermostHits.push_back(MyHit(1,4,2)); 
  std::vector<MyHit> layer4Hits;
  layer4Hits.push_back(MyHit(1,1,5)); 
  layer4Hits.push_back(MyHit(3,1,5)); 
  layer4Hits.push_back(MyHit(4,3,5)); 
  layer4Hits.push_back(MyHit(4,4,5)); 
  layer4Hits.push_back(MyHit(1,3,5)); 
  layer4Hits.push_back(MyHit(1,4,5)); 
  auto doublets = new HitDoublets(innerHits, outerHits);
  doublets->add(0,0);
  doublets->add(1,0);
  doublets->add(2,0);
  doublets->add(3,0);
  doublets->add(0,1);
  doublets->add(1,1);
  doublets->add(2,1);
  doublets->add(3,1);
  auto outerDoublets = new HitDoublets(outerHits, outermostHits);
  outerDoublets->add(0,0);
  outerDoublets->add(1,0);
  outerDoublets->add(2,0);
  outerDoublets->add(3,0);
  outerDoublets->add(0,1);
  outerDoublets->add(1,1);
  outerDoublets->add(2,1);
  outerDoublets->add(3,1);
  auto doublets3 = new HitDoublets(outermostHits, layer4Hits);
  doublets3->add(0,0);
  doublets3->add(1,0);
  doublets3->add(2,0);
  doublets3->add(3,0);
  doublets3->add(0,1);
  doublets3->add(1,1);
  doublets3->add(2,1);
  doublets3->add(3,1);
  std::cout<< "r coordinate of inner hit of first doublet: "<<doublets->r(0,HitDoublets::inner) << "\t phi: "<< doublets->phi(0,HitDoublets::inner) << std::endl;
  std::vector<const HitDoublets*> doubletvector;
  doubletvector.push_back(doublets);
  std::cout << "doublets size " << doublets->size() << std::endl;
  doubletvector.push_back(outerDoublets);
  doubletvector.push_back(doublets3);
  auto automaton = new CellularAutomaton<4>();
  //const TrackingRegion myTrackingRegion = TrackingRegion();
  automaton->createAndConnectCells(doubletvector, 20, 2,2);
  std::cout << "found cells per layer 1 :" <<  automaton->theFoundCellsPerLayer[0].size() << std::endl;
  std::cout << "found cells per layer 2 :"  << automaton->theFoundCellsPerLayer[1].size() << std::endl;
  std::cout  << "found cells per layer 3 :" << automaton->theFoundCellsPerLayer[2].size() << std::endl;
  for (auto c: automaton->theFoundCellsPerLayer[0]) {
    std::cout << "\t" << "layer 1 cell status: " << c.getCAState();
  }
  std::cout << std::endl;
  for (auto c: automaton->theFoundCellsPerLayer[1]) {
    std::cout << "\t" << "layer 2 cell status: " << c.getCAState();
  }
  std::cout << std::endl;
  std::cout << "evolve!" << std::endl;
  automaton->evolve();
  for (auto c: automaton->theFoundCellsPerLayer[0]) {
    std::cout << "\t" << "layer 1 cell status: " << c.getCAState();
  }
  std::cout << std::endl;
  for (auto c: automaton->theFoundCellsPerLayer[1]) {
    std::cout << "\t" << "layer 2 cell status: " << c.getCAState();
  }
  std::cout << std::endl;
  std::cout << "evolve!" << std::endl;
  automaton->evolve();
  std::cout << std::endl;
  return 0;
}
