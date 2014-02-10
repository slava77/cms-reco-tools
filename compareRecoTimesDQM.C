void compareRecoTimesDQM(const char* fName1, const char* fName2, int Nev = 200, float scaleF1 = 1.0, int runNumber = 1){
  //  std::cout<<"File "<<fName<<std::endl;
  TFile* f1 = new TFile(fName1);
  std::string timerPath = Form("/DQMData/Run %d/DQM/Run summary/TimerService/Paths", runNumber);
  f1->cd(timerPath.c_str());
  TH1F* h1 = gDirectory->Get("reconstruction_step_module_total");
  h1->Scale(scaleF1);
  const unsigned int n1 = h1->GetNbinsX();
  TAxis* x1 = h1->GetXaxis();
  h1->SetBit(TH1::kCanRebin, false); //just in case it's on by default

  TFile* f2 = new TFile(fName2);
  f2->cd(timerPath.c_str());
  TH1F* h2 = gDirectory->Get("reconstruction_step_module_total");
  const unsigned int n2 = h2->GetNbinsX();
  TAxis* x2 = h2->GetXaxis();
  h2->SetBit(TH1::kCanRebin, false); //just in case it's on by default

  double sum1 = h1->Integral(0,9999)/Nev;
  double sum2 = h2->Integral(0,9999)/Nev;

  std::vector<std::string > commonN;
  std::vector<double > commonT1;
  std::vector<double > commonT2;

  std::vector<std::string > only2N;
  std::vector<double > only2T;
  std::vector<std::string > only1N;
  std::vector<double > only1T;
  
  std::vector<int> isCommonFor2(n2, 0);

  for (int i1 = 1; i1<=n1;++i1){
    std::string mName = x1->GetBinLabel(i1);
    if (mName.find("(")!=std::string::npos) continue;

    int i2 = x2->FindBin(mName.c_str());
    if (i2>=1 && i2<= n2){
      commonN.push_back(mName);
      commonT1.push_back(h1->GetBinContent(i1)/Nev);
      commonT2.push_back(h2->GetBinContent(i2)/Nev);
      isCommonFor2[i2-1] = 1;
    } else {
      only1N.push_back(mName);
      only1T.push_back(h1->GetBinContent(i1)/Nev);
    }
  }

  for (int i2 = 1; i2<=n2;++i2){
    std::string mName = x2->GetBinLabel(i2);
    if (mName.find("(")!=std::string::npos) continue;

    if (isCommonFor2[i2-1]!=1){
      only2N.push_back(mName);
      only2T.push_back(h2->GetBinContent(i2)/Nev);
    }
  }

  std::cout<<"Only f1 module count: "<<only1N.size()<<std::endl;
  double sumOnly1 = 0;
  for (int i = 0; i< only1N.size(); ++i){
    double val1 = only1T[i];
    std::cout<<" \t"<<only1N[i]<<" \t "<<val1<<" ms/ev"<<std::endl;
    sumOnly1+= val1;
  }
  if (sumOnly1>0) std::cout<<" \t Total only f1: "<<sumOnly1<<std::endl;

  std::cout<<"Only f2 module count: "<<only2N.size()<<std::endl;
  double sumOnly2 = 0;
  for (int i = 0; i< only2N.size(); ++i){
    double val2 = only2T[i];
    std::cout<<" \t"<<only2N[i]<<" \t "<<val2<<" ms/ev"<<std::endl;
    sumOnly2+= val2;
  }
  std::cout<<" \t Total only f2: "<<sumOnly2<<std::endl;

  std::cout<<"Common modules count (only significant diffs shown): "<<commonN.size()<<std::endl;
  double sumCommon = 0;
  double verbFrac = 0.05;
  double verbSumForVerbFrac = 0.005*(sum2+sum1); // verbFrac difference from modules taking at least 1% of total time   
  double verbFracSmall = 0.1; //any 10% difference to be shown
  double verbDelta = 0.005*(sum2+sum1);  //1% of total CPU time

  double verbSum1 = 0.;
  double verbSum2 = 0.;
  for (int i = 0; i< commonN.size(); ++i){
    double val1 = commonT1[i];
    double val2 = commonT2[i];

    if ( (val1+val2>verbSumForVerbFrac && fabs(val1/val2-1.)> verbFrac)
	|| fabs(val1/val2-1.)>verbFracSmall
	 || fabs(val1-val2) > verbDelta ){
      std::cout<<"\t"<<commonN[i]<<" \t "<<val1<<" ms/ev -> "<<val2<<" ms/ev" <<std::endl;
      verbSum1 += val1;
      verbSum2 += val2;
    }
    
  }
  if (verbSum1 && verbSum2 > 0){
    std::cout<< "\t Total in detailed printout: "<<" \t "<<verbSum1<<" ms/ev -> "<<verbSum2<< " ms/ev"
	     <<" \t delta: "<<verbSum2-verbSum1<<std::endl;
  }
  std::cout<<" Total times: "<<sum1<<" ms/ev -> "<<sum2<<" ms/ev"
	   <<" \t delta: "<<sum2-sum1<<std::endl;
  //  for(int i=1;i<=n;++i)std::cout<<h->GetXaxis()->GetBinLabel(i)<<" "<<h->GetBinContent(i)<<std::endl; 
}
