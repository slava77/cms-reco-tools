#include <iostream>
#include "TROOT.h"
#include "TSystem.h"
#include "TChain.h"
#include "TRegexp.h"
void scanRecoMuons(std::string eName, std::string branchReg = "recoMuons_*"){
  gSystem->Load("libFWCoreFWLite");
  gROOT->ProcessLine("AutoLibraryLoader::enable();");
  TChain* e = new TChain("Events");
  e->SetScanField(0);

  e->Add(eName.c_str());

  TRegexp regg(branchReg.c_str(), kTRUE);

  TChain* tc = e ;
  TObjArray* tl = tc->GetListOfBranches();
 
  Int_t nBr = tl->GetSize();
  for (int iB=0;iB<nBr;++iB){
    TBranch* br = (TBranch*)(tl->At(iB)); 
    TString ts(br->GetName()); 
    if(ts.Index(regg)>=0){
      std::cout<<ts.Data()<<std::endl;
      tc->Scan(Form("%s.obj.pt():%s.obj.eta():%s.obj.phi():\
%s.obj.combinedMuon().get().pt():%s.obj.innerTrack().get().pt():%s.obj.outerTrack().get().pt():\
%s.obj.combinedMuon().get().charge():%s.obj.innerTrack().get().charge():%s.obj.outerTrack().get().charge():\
%s.obj.combinedMuon().get().hitPattern().numberOfValidHits():\
%s.obj.combinedMuon().get().hitPattern().numberOfValidTrackerHits():\
%s.obj.combinedMuon().get().hitPattern().numberOfValidMuonHits():\
%s.obj.innerTrack().get().numberOfValidHits():\
%s.obj.outerTrack().get().numberOfValidHits()",
 		    ts.Data(), ts.Data(), ts.Data(), //%s.obj.pt():%s.obj.eta():%s.obj.phi()
 		    ts.Data(), ts.Data(), ts.Data(), //%s.obj.combinedMuon().get().pt():%s.obj.innerTrack().get().pt():%s.obj.outerTrack().get().pt()
 		    ts.Data(), ts.Data(), ts.Data(), //%s.obj.combinedMuon().get().charge():%s.obj.innerTrack().get().charge():%s.obj.outerTrack().get().charge()
		    ts.Data(),                       //%s.obj.combinedMuon().get().hitPattern().numberOfValidHits()
		    ts.Data(),                       //%s.obj.combinedMuon().get().hitPattern().numberOfValidTrackerHits()
		    ts.Data(),                       //%s.obj.combinedMuon().get().hitPattern().numberOfValidMuonHits()
		    ts.Data(),                       //%s.obj.innerTrack().get().numberOfValidHits()
		    ts.Data()                        //%s.obj.outerTrack().get().numberOfValidHits() 
		    ),Form("%s.obj.isGlobalMuon()", ts.Data()), "");
    }
  } //> e.tecoTracks.recoT0.txt

}
