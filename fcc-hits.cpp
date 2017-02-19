#include "datamodel/ParticleCollection.h"
#include "datamodel/EventInfo.h"
#include "datamodel/EventInfoCollection.h"
#include "datamodel/Particle.h"
#include "datamodel/ParticleCollection.h"
#include "datamodel/PositionedTrackHitCollection.h"
#include "datamodel/LorentzVector.h"


// ROOT
#include "TBranch.h"
#include "TFile.h"
#include "TTree.h"
#include "TROOT.h"
#include "TLorentzVector.h"

// STL
#include <vector>
#include <iostream>
#include <array>
#include <cmath>

// podio specific includes
#include "podio/EventStore.h"
#include "podio/ROOTReader.h"

#include "FKDTree.h"

#include "KDDoublets.h"

#include "CACell.h"
#include "CellularAutomaton.h"

int main(){
  bool verbose = 1;
  const unsigned int  numLayers = 3;
  std::array<std::vector<FKDPoint<float, 3>>, numLayers> layerVectors;
  std::array<std::vector<fcc::PositionedTrackHit>, numLayers> layerHitVectors;

  auto store = podio::EventStore();
  auto reader = podio::ROOTReader();

  reader.openFile("input.root");
  store.setReader(&reader);
  unsigned nevents= reader.getEntries();

  // event loop -- here we aggregate single particle events
  for(unsigned iev=0; iev<nevents; ++iev) {
    const fcc::PositionedTrackHitCollection* hitColl;
    bool hitsPresent = store.get("positionedHits", hitColl);

    if (hitsPresent) {
      for(const auto ptc: *hitColl) {
        if (ptc.bits() == 1) { // check if primary particle
          int system = ptc.cellId() % 32; // hackish, should use ddecoder for this
          int layer = (ptc.cellId() >> 5) % 32; // see above
          if (layer < numLayers && system == 0) { /// @todo: remove hardcoded values for system id
          auto pos = ptc.position();
          float t = ptc.time();
          // KDtree with 3 dimensions: phi, z, time
          layerVectors[layer].push_back(FKDPoint<float, 3> (std::atan2(pos.y, pos.x), pos.z, t, layerVectors[layer].size()));
          // need to clone hit
          layerHitVectors[layer].push_back(ptc.clone());
          if (verbose)
          std::cout << "found hit in layer " << layer << ": \t"  << std::atan2(pos.y, pos.x) << ", " << layerHitVectors[layer].back().position().z << "==" << layerVectors[layer].back()[1] << ", " << t << std::endl;
          }
          
        }
      }
    }

    store.clear();
    reader.endOfEvent();
  }

  // filter criteria for kdtree point search
  double deltaPhi = 0.2;
  double deltaZ = 50;
  double deltaT = 1;

  std::vector<const HitDoublets*> doubletvector;

  // loop over layer pairs
  /// @todo: loop from outside in
  for (int layerCounter = 1; layerCounter < layerVectors.size(); ++layerCounter) {
    auto points = layerVectors[layerCounter];
    auto doublets = new HitDoublets(layerHitVectors[layerCounter - 1], layerHitVectors[layerCounter]);
    FKDTree<float, 3> kdtree(points.size(), points);
    kdtree.build();
    if (verbose) std::cout << "points in layerVector (size " << points.size() << "):" << std::endl;
    int pointCounter = 0;
    for (auto p: layerVectors[layerCounter - 1]) {
      auto result_ids = kdtree.search_in_the_box(FKDPoint<float, 3>(p[0] - deltaPhi, p[1] - deltaZ, p[2] - deltaT, 0), FKDPoint<float,3>(p[0] + deltaPhi,p[1] + deltaZ, p[2] + deltaT, 0));
      if (verbose)
        std::cout << "number of search results: " << result_ids.size() << std::endl;
      
      for (auto r: result_ids) {
          doublets->add(pointCounter, r);
          if (verbose)
          std::cout << r << "\t" << layerVectors[layerCounter][r][1] << "\t" << layerHitVectors[layerCounter][r].position().z << std::endl;
        }
       ++pointCounter;
      }
      if (kdtree.test_correct_build()) {
        if (verbose) {
          std::cout << "FKDTree built correctly" << std::endl;
        }
      }
      else
        std::cerr << "FKDTree wrong" << std::endl;
  if (verbose)
    std::cout << "found " << doublets->size() << " doublets" << std::endl;
  doubletvector.push_back(doublets);
  }
  auto doublets = doubletvector[0];

  auto automaton = new CellularAutomaton<numLayers>();
  automaton->createAndConnectCells(doubletvector, 40, 2, 2); // values for cuts not used at the moment
  automaton->evolve();
  automaton->evolve();
  std::vector<CACell::CAntuplet> theNTuplets;
  automaton->findNtuplets(theNTuplets, 3);
  if (verbose) {
  std::cout << "found cells per layer 1 :" <<  automaton->theFoundCellsPerLayer[0].size() << std::endl;
  std::cout << "found cells per layer 2 :"  << automaton->theFoundCellsPerLayer[1].size() << std::endl;
  std::cout << "evolve!" << std::endl;
  for (auto c: automaton->theFoundCellsPerLayer[0]) {
    std::cout << "\t" << "layer 1 cell status: " << c.getCAState();
  }
  std::cout << std::endl;
  for (auto c: automaton->theFoundCellsPerLayer[1]) {
    std::cout << "\t" << "layer 2 cell status: " << c.getCAState();
  }
  std::cout << std::endl;
  std::cout << theNTuplets.size() << std::endl;
  // dump cell positions
  for (auto nt: theNTuplets) {
    std::cout << nt[0]->getInnerR() << "\t" << nt[0]->getInnerPhi() << std::endl;
    std::cout << nt[0]->getOuterR() << "\t" << nt[0]->getOuterPhi() << std::endl;
    std::cout << nt[1]->getInnerR() << "\t" << nt[1]->getInnerPhi() << std::endl;
    std::cout << nt[1]->getOuterR() << "\t" << nt[1]->getOuterPhi() << std::endl;
  }
  }
  
  return 0;
}

