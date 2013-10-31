#include <iostream>
#include "TROOT.h"
#include "TSystem.h"
#include "TChain.h"
#include "TRegexp.h"
void scanRecoTracks(std::string eName, std::string branchReg = "recoTracks_*"){
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
      tc->Scan(Form("%s.obj.pt():%s.obj.eta():%s.obj.phi():%s.obj.chi2():%s.obj.ndof():\
%s.obj.hitPattern().numberOfValidHits():%s.obj.hitPattern().numberOfValidTrackerHits():%s.obj.hitPattern().numberOfValidMuonHits()",
 		    ts.Data(), ts.Data(), ts.Data(), ts.Data(),ts.Data(), ts.Data(), ts.Data(),ts.Data()),"", "");
    }
  } //> e.tecoTracks.recoT0.txt

}
