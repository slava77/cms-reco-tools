void getRecoTimesDQM(const char* fName){
  //  std::cout<<"File "<<fName<<std::endl;
  TFile* f = new TFile(fName);
  f->cd("/DQMData/Run 1/DQM/Run summary/TimerService/Paths");
  TH1F* h = gDirectory->Get("reconstruction_step_module_total");
  int n = h->GetNbinsX();
  for(int i=1;i<=n;++i)std::cout<<h->GetXaxis()->GetBinLabel(i)<<" "<<h->GetBinContent(i)<<std::endl; 
}
