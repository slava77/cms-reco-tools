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

template<typename Collection>
void print_patCollection_userFloats(const std::string& fName, const std::string& cName,
                                    const std::string& oName, int nEvts = 1){
  TFile* f = new TFile(fName.c_str());
  cout<<" loaded file "<<std::endl;

  fwlite::Event e(f);
  int eCount = 0;
  for (e.toBegin(); ! e.atEnd() && (eCount < nEvts || nEvts < 0); ++e, ++eCount){
    cout<<" loaded event "<<e.id().event()<<endl;
    
    fwlite::Handle<Collection> ch;
    ch.getByLabel(e, cName.c_str());
    
    int iC = 0;
    for (auto const& c : ch.ref()){
      int iF = 0;
      for (auto const& fn : c.userFloatNames()) {
        cout<<oName.c_str()<<" "<<iC<<" "<<c.pt()<<" userFloat "<<iF<<" "<<fn<<" "<<c.userFloat(fn)<<endl;
        ++iF;
      }
      ++iC;
    }
  }//event loop
}
void print_patJets_userFloats(const std::string& fName, const std::string& cName = "slimmedJets",
                              int nEvts = 1){
  print_patCollection_userFloats<pat::JetCollection>(fName, cName, "jet", nEvts);
}
void print_patElectrons_userFloats(const std::string& fName, const std::string& cName = "slimmedElectrons",
                                   int nEvts = 1){
  print_patCollection_userFloats<pat::ElectronCollection>(fName, cName, "ele", nEvts);
}
void print_patPhotons_userFloats(const std::string& fName, const std::string& cName = "slimmedPhotons",
                                 int nEvts = 1){
  print_patCollection_userFloats<pat::PhotonCollection>(fName, cName, "pho", nEvts);
}


void compare_patJets_btags(const std::string& fNameRef, const std::string& fNameNew, const std::string jetName = "slimmedJetsAK8",
                           int nEvts = 1, bool allowNew = true, bool allowValueDiffs = true, float minRel = -1,
                           bool verbose = false, bool verboseRel = false){
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
    if (verbose || verboseRel) cout<<" loaded event "<<eRef.id().event()<<endl;
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
          auto const& keyRef = dvRef[iD].first;
          auto const& valRef = dvRef[iD].second;
          auto const& valNew = dvNew[iD].second;
          auto deltaVal = std::abs(valRef - valNew);
          if (valRef != valNew){
            deltaCountsMap[keyRef]++;
            auto aveVal = (std::abs(valRef) + std::abs(valNew))*0.5f;
            bool hasDiffRel = false;
            if (aveVal != 0 && minRel > 0 && deltaVal/aveVal > minRel){
              deltaMinRelCountsMap[keyRef]++;
              hasDiffRel = true;
            }
            
            if ( (!allowValueDiffs) || (allowValueDiffs && (verbose || (hasDiffRel && verboseRel)) ) ){
              std::cout<<"Jet discriminant mismatch for "<<keyRef
                       <<" : Ref "<<valRef<<" New "<<valNew
                       <<std::endl;
            }
            if (!allowValueDiffs) return;
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
                deltaCountsMap[keyRef]++;
                auto deltaVal = std::abs(valRef - valNew);
                auto aveVal = (std::abs(valRef) + std::abs(valNew))*0.5f;
                bool hasDiffRel = false;
                if (aveVal != 0 && minRel > 0 && deltaVal/aveVal > minRel){
                  deltaMinRelCountsMap[keyRef]++;
                  hasDiffRel = true;
                }

                if ( (!allowValueDiffs) || (allowValueDiffs && (verbose || (hasDiffRel && verboseRel)) ) ){
                  std::cout<<"Jet discriminant mismatch for "<<keyRef
                           <<" : Ref "<<valRef<<" New "<<valNew
                           <<std::endl;
                }
                if (!allowValueDiffs) return;
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
                         bool verbose = false, const std::vector<std::string>& ignoreInDebug = {}){
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
            std::cout<<"Tau ID mismatch for iT "<< iT <<"at index "<<iID
                     <<" : Ref "<<idvRef[iID].first<<" New "<<idvNew[iID].first
                     <<std::endl;
            return;
          }
          auto const& keyRef = idvRef[iID].first;
          auto const& valRef = idvRef[iID].second;
          auto const& valNew = idvNew[iID].second;
          auto deltaVal = std::abs(valRef - valNew);
          if (valRef != valNew){
            auto aveVal = (std::abs(valRef) + std::abs(valNew))*0.5f;
            bool passDeltaMinRel = aveVal != 0 && minRel > 0 && deltaVal/aveVal > minRel;
            bool needed = std::find(ignoreInDebug.begin(), ignoreInDebug.end(), keyRef) == ignoreInDebug.end();
            if ( (!allowValueDiffs) || (allowValueDiffs && passDeltaMinRel && needed && verbose)){
              std::cout<<"Tau ID value mismatch for iT "<<iT <<" for "<<keyRef
                       <<" : Ref "<<valRef<<" New "<<valNew
                       <<" Ref-New "<<valRef-valNew
                       <<std::endl;
            }
            if (allowValueDiffs) {
              deltaCountsMap[keyRef]++;
              if (passDeltaMinRel) deltaMinRelCountsMap[keyRef]++;
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
                auto aveVal = (std::abs(valRef) + std::abs(valNew))*0.5f;
                auto deltaVal = std::abs(valRef - valNew);
                bool passDeltaMinRel = aveVal != 0 && minRel > 0 && deltaVal/aveVal > minRel;
                bool needed = std::find(ignoreInDebug.begin(), ignoreInDebug.end(), keyRef) == ignoreInDebug.end();
                if ( (!allowValueDiffs) || (allowValueDiffs && passDeltaMinRel && needed && verbose)){
                  std::cout<<"Tau ID value mismatch for iT "<< iT <<" for "<<keyRef
                           <<" : Ref "<<valRef<<" New "<<valNew
                           <<" Ref-New "<<valRef-valNew
                           <<std::endl;
                }
                if (allowValueDiffs) {
                  deltaCountsMap[keyRef]++;
                  if (passDeltaMinRel) deltaMinRelCountsMap[keyRef]++;
                } else
                  return;
              }
              break;
            }
          }
          if (hasMatch){
            nIDsRefSameAll++;
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

