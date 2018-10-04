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

void print_patJets_btags(){
  TFile* f = new TFile("pass-1acac95/136.8311_RunJetHT2017F_reminiaod+RunJetHT2017F_reminiaod+REMINIAOD_data2017+HARVEST2017_REMINIAOD_data2017/step2.root");
  cout<<" loaded file "<<std::endl;
  fwlite::Event e(f);
  cout<<" loaded event "<<endl;
  
  if (1<2){
    fwlite::Handle<pat::JetCollection> jh;
    jh.getByLabel(e, "slimmedJetsAK8");
    
    int iJ = 0;
    for (auto const& j : jh.ref()){
      int iD = 0;
      for (auto const& pd : j.getPairDiscri()) {
        if(pd.first.find("DeepDouble")!= std::string::npos){cout<<"jet "<<iJ<<" discr "<<iD<<" "<<pd.first<<" "<<pd.second<<endl;} ++iD;
      }
      ++iJ;
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
