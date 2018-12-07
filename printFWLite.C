#include <vector>
#include <TFile.h>
#include <iostream>
#include <TSystem.h>
#include "FWCore/Utilities/interface/thread_safety_macros.h"
#include "FWCore/FWLite/interface/FWLiteEnabler.h"
#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
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


void compare_patJets_btags(const std::string& fNameRef, const std::string& fNameNew,
                           int nEvts = 1, bool allowNew = true, bool verbose = false){
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

  bool kFirst = true;

  eRef.toBegin();
  eNew.toBegin();
  for (; ! eRef.atEnd() && !eNew.atEnd() && (eCount < nEvts || nEvts < 0); ++eRef, ++eNew, ++eCount){
    if (verbose) cout<<" loaded event "<<eRef.id().event()<<endl;
    assert(eRef.id().event() == eNew.id().event());
    
    fwlite::Handle<pat::JetCollection> jhRef;
    jhRef.getByLabel(eRef, "slimmedJetsAK8");

    fwlite::Handle<pat::JetCollection> jhNew;
    jhNew.getByLabel(eNew, "slimmedJetsAK8");
    
    assert(jhRef.ref().size() == jhNew.ref().size());
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
        assert(nD == dvNew.size());
        for (int iD = 0; iD < nD; ++iD){
          assert(dvRef[iD].first == dvNew[iD].first);
          assert(dvRef[iD].second == dvNew[iD].second);
        }
      } else {
        //assume the reference is required to always have a match in the New
        int nDRef = dvRef.size();
        int nDNew = dvNew.size();
        assert(nDRef <= nDNew);
        for (int iDRef = 0; iDRef < nDRef; ++iDRef){
          auto const& keyRef = dvRef[iDRef].first;
          auto const& valRef = dvRef[iDRef].second;
          bool hasMatch = false;
          for (int iDNew = 0; iDNew < nDNew; ++iDNew){
            if (dvNew[iDNew].first == keyRef){
              hasMatch = (valRef == dvNew[iDNew].second);
              break;
            }
          }
          assert(hasMatch);
          nTagsRefSameAll++;
          if (valRef >= 0) nTagsRefSameAllGE0++;
          if (valRef >  0) nTagsRefSameAllGT0++;
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

  std::cout<<"Comparison OK on "<<eCount<<" events "
           <<nJetsRefAll<<" refJets "
           <<nTagsRefSameAll<<" allRefTags "
           <<nTagsRefSameAllGE0<<" allRefTagsGE0 "
           <<nTagsRefSameAllGT0<<" allRefTagsGT0 "
           <<std::endl;
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
