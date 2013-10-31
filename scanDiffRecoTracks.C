#include <iostream>
#include "TROOT.h"
#include "TSystem.h"
#include "TChain.h"
#include "TRegexp.h"
void scanDiffRecoTracks(std::string eName, std::string eoName,
			std::string branchReg = "recoTracks_*"){
  gSystem->Load("libFWCoreFWLite");
  gROOT->ProcessLine("AutoLibraryLoader::enable();");
  TChain* e = new TChain("Events");
  e->SetScanField(0);

  e->Add(eName.c_str());

  TChain* eo = new TChain("Events");
  eo->SetScanField(0);

  eo->Add(eoName.c_str());
  e->AddFriend(eo, "eo");

  TRegexp regg(branchReg.c_str(), kTRUE);

  TChain* tc = e ;
  TObjArray* tl = tc->GetListOfBranches();
 
  Int_t nBr = tl->GetSize();
  for (int iB=0;iB<nBr;++iB){
    TBranch* br = (TBranch*)(tl->At(iB)); 
    TString ts(br->GetName()); 
    if(ts.Index(regg)>=0){
      std::cout<<ts.Data()<<std::endl;
      tc->Scan(Form("Sum$(%s.obj.pt()>0):Sum$(eo.%s.obj.pt()>0):Sum$(%s.obj.pt()):Sum$(%s.obj.pt())-Sum$(eo.%s.obj.pt())",
 		    ts.Data(), ts.Data(), ts.Data(), ts.Data(), ts.Data()),"", "");
    }
  } //> e.tecoTracks.recoT0.txt

}
