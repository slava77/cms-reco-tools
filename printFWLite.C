#include <vector>
#include <map>
#include <TFile.h>
#include <iostream>
#include <TSystem.h>
#include "FWCore/Utilities/interface/thread_safety_macros.h"
#include "FWCore/FWLite/interface/FWLiteEnabler.h"
#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/FWLite/interface/Event.h"

class loadFWLite {
public:
  loadFWLite() {
    gSystem->Load("libFWCoreFWLite");
    FWLiteEnabler::enable();
  }
};

static loadFWLite lfw;

using namespace std;

void print_patJets_btags(const std::string& fName,
                         int nEvts = 1, const std::string& substr = "DeepDouble"){
  TFile* f = new TFile(fName.c_str());
  cout<<" loaded file "<<std::endl;

  fwlite::Event e(f);
  int eCount = 0;
  for (e.toBegin(); ! e.atEnd() && (eCount < nEvts || nEvts < 0); ++e, ++eCount){
    cout<<" loaded event "<<e.id().event()<<endl;
    
    fwlite::Handle<pat::JetCollection> jh;
    jh.getByLabel(e, "slimmedJetsAK8");
    
    int iJ = 0;
    for (auto const& j : jh.ref()){
      int iD = 0;
      for (auto const& pd : j.getPairDiscri()) {
        if(pd.first.find(substr)!= std::string::npos){cout<<"jet "<<iJ<<" "<<j.pt()<<" discr "<<iD<<" "<<pd.first<<" "<<pd.second<<endl;} ++iD;
      }
      ++iJ;
    }
  }//event loop
}


void compare_patJets_btags(const std::string& fNameRef, const std::string& fNameNew, const std::string jetName = "slimmedJetsAK8",
                           int nEvts = 1, bool allowNew = true, bool allowValueDiffs = true, float minRel = -1,
                           bool verbose = false){
  TFile* fRef = new TFile(fNameRef.c_str());
  cout<<" loaded file "<<fNameRef<<std::endl;

  TFile* fNew = new TFile(fNameNew.c_str());
  cout<<" loaded file "<<fNameNew<<std::endl;

  fwlite::Event eRef(fRef);
  fwlite::Event eNew(fNew);
  int eCount = 0;
  int nJetsRefAll = 0;
  int nTagsRefSameAll = 0;
  int nTagsRefSameAllGE0 = 0;
  int nTagsRefSameAllGT0 = 0;

  std::map<std::string, int> deltaCountsMap;
  std::map<std::string, int> deltaMinRelCountsMap;

  bool kFirst = true;

  eRef.toBegin();
  eNew.toBegin();
  for (; ! eRef.atEnd() && !eNew.atEnd() && (eCount < nEvts || nEvts < 0); ++eRef, ++eNew, ++eCount){
    if (verbose) cout<<" loaded event "<<eRef.id().event()<<endl;
    if (eRef.id().event() != eNew.id().event()){
      std::cout<<"Events are out of order"<<std::endl;
      return;
    }
    
    fwlite::Handle<pat::JetCollection> jhRef;
    jhRef.getByLabel(eRef, jetName.c_str());

    fwlite::Handle<pat::JetCollection> jhNew;
    jhNew.getByLabel(eNew, jetName.c_str());
    
    if (jhRef.ref().size() != jhNew.ref().size()){
      std::cout<<"Jet sizes do not match"<<std::endl;
      return;
    }
    int nJ = jhRef.ref().size();

    const pat::JetCollection& jvRef = jhRef.ref();
    const pat::JetCollection& jvNew = jhNew.ref();

    for (int iJ = 0; iJ < nJ; ++iJ){
      auto const& jRef = jvRef[iJ];
      auto const& jNew = jvNew[iJ];

      auto const& dvRef = jRef.getPairDiscri();
      auto const& dvNew = jNew.getPairDiscri();
      if (not allowNew){
        int nD = dvRef.size();
        if (nD != dvNew.size()){
          std::cout<<"Jet discriminant count does not match"<<std::endl;
          return;
        }
        for (int iD = 0; iD < nD; ++iD){
          if (dvRef[iD].first != dvNew[iD].first){
            std::cout<<"Jet discriminant mismatch at index "<<iD
                     <<" : Ref "<<dvRef[iD].first<<" New "<<dvNew[iD].first
                     <<std::endl;
            return;
          }
          auto deltaVal = std::abs(dvRef[iD].second - dvNew[iD].second);
          if (dvRef[iD].second != dvNew[iD].second){
            if ( (!allowValueDiffs) || (allowValueDiffs && verbose)){
              std::cout<<"Jet discriminant mismatch for "<<dvRef[iD].first
                       <<" : Ref "<<dvRef[iD].second<<" New "<<dvNew[iD].second
                       <<std::endl;
            }
            if (allowValueDiffs) {
              deltaCountsMap[dvRef[iD].first]++;
              auto aveVal = (std::abs(dvRef[iD].second) + std::abs(dvNew[iD].second))*0.5f;
              if (aveVal != 0 && minRel > 0 && deltaVal/aveVal > minRel) deltaMinRelCountsMap[dvRef[iD].first]++;
            } else
              return;
          }
        }
      } else {
        //assume the reference is required to always have a match in the New
        int nDRef = dvRef.size();
        int nDNew = dvNew.size();
        if (nDRef > nDNew){
          std::cout<<"Ref has more tags than New: try reruning with flipped order"<<std::endl;
          return;
        }
        for (int iDRef = 0; iDRef < nDRef; ++iDRef){
          auto const& keyRef = dvRef[iDRef].first;
          auto const& valRef = dvRef[iDRef].second;
          bool hasMatch = false;
          for (int iDNew = 0; iDNew < nDNew; ++iDNew){
            if (dvNew[iDNew].first == keyRef){
              auto const& valNew = dvNew[iDNew].second;
              hasMatch = (valRef == valNew);
              if (not hasMatch){
                if ( (!allowValueDiffs) || (allowValueDiffs && verbose)){
                  std::cout<<"Jet discriminant mismatch for "<<keyRef
                           <<" : Ref "<<valRef<<" New "<<valNew
                           <<std::endl;
                }
                if (allowValueDiffs) {
                  deltaCountsMap[keyRef]++;
                  auto deltaVal = std::abs(valRef - valNew);
                  auto aveVal = (std::abs(valRef) + std::abs(valNew))*0.5f;
                  if (aveVal != 0 && minRel > 0 && deltaVal/aveVal > minRel) deltaMinRelCountsMap[keyRef]++;
                } else
                  return;
              }
              break;
            }
          }
          if (hasMatch){
            nTagsRefSameAll++;
            if (valRef >= 0) nTagsRefSameAllGE0++;
            if (valRef >  0) nTagsRefSameAllGT0++;
          } else if (verbose) {
            std::cout<<"Jet tag difference for "<<std::endl;
          }
        }
        if (kFirst){
          std::cout<<"New has "<<nDNew<<" tags; Ref has "<<nDRef<<" tags"<<std::endl;
          for (int iDNew = 0; iDNew < nDNew; ++iDNew){
            bool hasMatch = false;
            int iRefMatch = 0;
            for (int iDRef = 0; iDRef < nDRef; ++iDRef){
              if (dvNew[iDNew].first == dvRef[iDRef].first){
                hasMatch = true;
                iRefMatch = iDRef;
              }
            }
            if (hasMatch) printf("%3d was %3d %s\n", iDNew, iRefMatch, dvNew[iDNew].first.c_str());
            else          printf("%3d is new  %s\n", iDNew, dvNew[iDNew].first.c_str());
          }
          kFirst = false;
        }
      }// else: allowNew
      nJetsRefAll++;
    }//iJ
  }//event loop

  std::cout<<"Comparison on "<<eCount<<" events "
           <<nJetsRefAll<<" refJets "
           <<nTagsRefSameAll<<" allRefTags "
           <<nTagsRefSameAllGE0<<" allRefTagsGE0 "
           <<nTagsRefSameAllGT0<<" allRefTagsGT0 "
           <<std::endl;
  if (not deltaCountsMap.empty()){
    std::cout<<"Common pairs have differences: "<<std::endl;
    for (auto const& mVal : deltaCountsMap){
      std::cout<<"\t "<<mVal.first<<" diff count "<<mVal.second;
      if (minRel > 0) std::cout<<" , "<<deltaMinRelCountsMap[mVal.first]<< " relD > "<<minRel;
      std::cout<<std::endl;
    }
  }
}

void compare_patTaus_IDs(const std::string& fNameRef, const std::string& fNameNew, const std::string tauName = "slimmedTaus",
                         int nEvts = 1, bool allowNew = true, bool allowValueDiffs = true, float minRel = -1,
                         bool verbose = false){
  TFile* fRef = new TFile(fNameRef.c_str());
  cout<<" loaded file "<<fNameRef<<std::endl;

  TFile* fNew = new TFile(fNameNew.c_str());
  cout<<" loaded file "<<fNameNew<<std::endl;

  fwlite::Event eRef(fRef);
  fwlite::Event eNew(fNew);
  int eCount = 0;
  int nTausRefAll = 0;
  int nIDsRefSameAll = 0;

  std::map<std::string, int> deltaCountsMap;
  std::map<std::string, int> deltaMinRelCountsMap;

  bool kFirst = true;

  eRef.toBegin();
  eNew.toBegin();
  for (; ! eRef.atEnd() && !eNew.atEnd() && (eCount < nEvts || nEvts < 0); ++eRef, ++eNew, ++eCount){
    if (verbose) cout<<" loaded event "<<eRef.id().event()<<endl;
    if (eRef.id().event() != eNew.id().event()){
      std::cout<<"Events are out of order"<<std::endl;
      return;
    }
    
    fwlite::Handle<pat::TauCollection> thRef;
    thRef.getByLabel(eRef, tauName.c_str());

    fwlite::Handle<pat::TauCollection> thNew;
    thNew.getByLabel(eNew, tauName.c_str());
    
    if (thRef.ref().size() != thNew.ref().size()){
      std::cout<<"Tau sizes do not match"<<std::endl;
      return;
    }
    int nT = thRef.ref().size();

    const pat::TauCollection& tvRef = thRef.ref();
    const pat::TauCollection& tvNew = thNew.ref();

    for (int iT = 0; iT < nT; ++iT){
      auto const& tRef = tvRef[iT];
      auto const& tNew = tvNew[iT];

      auto const& idvRef = tRef.tauIDs();
      auto const& idvNew = tNew.tauIDs();
      if (not allowNew){
        int nID = idvRef.size();
        if (nID != idvNew.size()){
          std::cout<<"Tau ID count does not match"<<std::endl;
          return;
        }
        for (int iID = 0; iID < nID; ++iID){
          if (idvRef[iID].first != idvNew[iID].first){
            std::cout<<"Tau ID mismatch at index "<<iID
                     <<" : Ref "<<idvRef[iID].first<<" New "<<idvNew[iID].first
                     <<std::endl;
            return;
          }
          auto deltaVal = std::abs(idvRef[iID].second - idvNew[iID].second);
          if (idvRef[iID].second != idvNew[iID].second){
            if ( (!allowValueDiffs) || (allowValueDiffs && verbose)){
              std::cout<<"Tau ID mismatch for "<<idvRef[iID].first
                       <<" : Ref "<<idvRef[iID].second<<" New "<<idvNew[iID].second
                       <<std::endl;
            }
            if (allowValueDiffs) {
              deltaCountsMap[idvRef[iID].first]++;
              auto aveVal = (std::abs(idvRef[iID].second) + std::abs(idvNew[iID].second))*0.5f;
              if (aveVal != 0 && minRel > 0 && deltaVal/aveVal > minRel) deltaMinRelCountsMap[idvRef[iID].first]++;
            } else
              return;
          }
        }
      } else {
        //assume the reference is required to always have a match in the New
        int nIDRef = idvRef.size();
        int nIDNew = idvNew.size();
        if (nIDRef > nIDNew){
          std::cout<<"Ref has more IDs than New: try reruning with flipped order"<<std::endl;
          return;
        }
        for (int iIDRef = 0; iIDRef < nIDRef; ++iIDRef){
          auto const& keyRef = idvRef[iIDRef].first;
          auto const& valRef = idvRef[iIDRef].second;
          bool hasMatch = false;
          for (int iIDNew = 0; iIDNew < nIDNew; ++iIDNew){
            if (idvNew[iIDNew].first == keyRef){
              auto const& valNew = idvNew[iIDNew].second;
              hasMatch = (valRef == valNew);
              if (not hasMatch){
                if ( (!allowValueDiffs) || (allowValueDiffs && verbose)){
                  std::cout<<"Tau ID mismatch for "<<keyRef
                           <<" : Ref "<<valRef<<" New "<<valNew
                           <<std::endl;
                }
                if (allowValueDiffs) {
                  deltaCountsMap[keyRef]++;
                  auto deltaVal = std::abs(valRef - valNew);
                  auto aveVal = (std::abs(valRef) + std::abs(valNew))*0.5f;
                  if (aveVal != 0 && minRel > 0 && deltaVal/aveVal > minRel) deltaMinRelCountsMap[keyRef]++;
                } else
                  return;
              }
              break;
            }
          }
          if (hasMatch){
            nIDsRefSameAll++;
          } else if (verbose) {
            std::cout<<"Tau ID difference for "<<std::endl;
          }
        }
        if (kFirst){
          std::cout<<"New has "<<nIDNew<<" IDs; Ref has "<<nIDRef<<" IDs"<<std::endl;
          for (int iIDNew = 0; iIDNew < nIDNew; ++iIDNew){
            bool hasMatch = false;
            int iRefMatch = 0;
            for (int iIDRef = 0; iIDRef < nIDRef; ++iIDRef){
              if (idvNew[iIDNew].first == idvRef[iIDRef].first){
                hasMatch = true;
                iRefMatch = iIDRef;
              }
            }
            if (hasMatch) printf("%3d was %3d %s\n", iIDNew, iRefMatch, idvNew[iIDNew].first.c_str());
            else          printf("%3d is new  %s\n", iIDNew, idvNew[iIDNew].first.c_str());
          }
          kFirst = false;
        }
      }// else: allowNew
      nTausRefAll++;
    }//iT
  }//event loop

  std::cout<<"Comparison on "<<eCount<<" events "
           <<nTausRefAll<<" refTaus "
           <<nIDsRefSameAll<<" allRefIDs "
           <<std::endl;
  if (not deltaCountsMap.empty()){
    std::cout<<"Common pairs have differences: "<<std::endl;
    for (auto const& mVal : deltaCountsMap){
      std::cout<<"\t "<<mVal.first<<" diff count "<<mVal.second;
      if (minRel > 0) std::cout<<" , "<<deltaMinRelCountsMap[mVal.first]<< " relD > "<<minRel;
      std::cout<<std::endl;
    }
  }
}

void print_patEGamma_userFloats(){
  TFile* f = new TFile("20034.0_TTbar_14TeV+TTbar_14TeV_TuneCUETP8M1_2023D17_GenSimHLBeamSpotFull14+DigiFullTrigger_2023D17+RecoFullGlobal_2023D17+HARVESTFullGlobal_2023D17/step3_inMINIAODSIM.root");
  cout<<" loaded file "<<std::endl;
  fwlite::Event e(f);
  cout<<" loaded 1 event "<<endl;
  
  fwlite::Handle<pat::ElectronCollection> elh;
  elh.getByLabel(e, "slimmedElectronsFromMultiCl");
  
  int iEl = 0;
  for (auto const& el : elh.ref()){
    int iD = 0;
    for (auto const& fs : el.userFloatNames()) {
      cout<<"el "<<iEl<<" userFloatName "<<iD<<" "<<fs<<endl; 
      ++iD;
    }
    break;
    ++iEl;
  }

  fwlite::Handle<pat::PhotonCollection> phh;
  phh.getByLabel(e, "slimmedPhotonsFromMultiCl");

  int iPh = 0;
  for (auto const& ph : phh.ref()){
    int iD = 0;
    for (auto const& fs : ph.userFloatNames()) {
      cout<<"ph "<<iPh<<" userFloatName "<<iD<<" "<<fs<<endl;
      ++iD;
    }
    break;
    ++iPh;
  }

}
