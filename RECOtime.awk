/^TimeReport/{sumR+=$2; tm[$6]+=$2;}
/ initialStep/{sumIniTS+=$2; used[$6]=1}
/ lowPtTripletStep/{sumLpt3TS+=$2; used[$6]=1}
/ pixelPairStep/{sumPix2TS+=$2; used[$6]=1}
/ detachedTripletStep/{sumD3TS+=$2; used[$6]=1}
/ mixedTripletStep/{sumMix3TS+=$2; used[$6]=1}
/ pixelLessStep/{sumNoPixTS+=$2; used[$6]=1}
/ tobTecStep/{sumTTTS+=$2; used[$6]=1}
/ elPFIso| phPFIso| muPFIso/{sumEGMPFiso+=$2; used[$6]=1}
/CaloJets/{sumCJ+=$2; used[$6]=1; nCJ++;}
/PFJets$/{sumPFJ+=$2; used[$6]=1; nPFJ++;}
/ regionalCosmic/{sumRegCos+=$2; used[$6]=1;}
/ pf| particleFlow/{sumPFnoIso+=$2; used[$6]=1;}
/ shrinkingConePFTau/{sumSCTau+=$2; used[$6]=1;}
/ hpsPFTau/{sumHPSTau+=$2; used[$6]=1;}
/ hpsTancTaus/{sumHPSTTau+=$2; used[$6]=1;}
/ caloRecoTau/{sumCRTau+=$2; used[$6]=1;}
/ uncleanedOnly/{sumUnclean+=$2; used[$6]=1; unclean[$6]=$2;}
/ convTrack| convStep|onversionStep|onversionTrack/{sumConvTk+=$2; used[$6]=1;}
/osmicMu|muonsFromCosmics|cosmicsVeto/{sumCosMu+=$2; used[$6]=1;}
/ ancientMuonSeed$| standAloneMuons$| globalMuons$| refittedStandAloneMuons$| tevMuons$| glbTrackQual$| muons1stStep$| calomuons$| muonEcalDetId$| muonShowerInformation$| SETMuonSeed$| standAloneSETMuons$| globalSETMuons$| muons$| muIso| muid/{sumMuNoCos+=$2; used[$6]=1;}
/ pixelTracks| generalTracks| electronGsfTracks/{sumPixGenGsfTks+=$2; used[$6]=1;}
END{
    sumA = 0;
    printUnclean=1;
    print "IniTS\t "sumIniTS"\t "sumIniTS/sumR; sumA += sumIniTS;
    print "Lpt3TS\t "sumLpt3TS"\t "sumLpt3TS/sumR; sumA += sumLpt3TS;
    print "Pix2TS\t "sumPix2TS"\t "sumPix2TS/sumR; sumA += sumPix2TS;
    print "D3TS\t "sumD3TS"\t "sumD3TS/sumR; sumA += sumD3TS;
    print "Mix3TS\t "sumMix3TS"\t "sumMix3TS/sumR; sumA += sumMix3TS;
    print "NoPixTS\t "sumNoPixTS"\t "sumNoPixTS/sumR; sumA += sumNoPixTS;
    print "TTTS\t "sumTTTS"\t "sumTTTS/sumR; sumA += sumTTTS;
    sumTSall = sumIniTS + sumLpt3TS + sumPix2TS + sumD3TS + sumMix3TS + sumNoPixTS + sumTTTS;
    print "--";
    print "subtotal TS\t "sumTSall"\t "sumTSall/sumR;
    print "---";
    print "EGMPFiso\t "sumEGMPFiso"\t "sumEGMPFiso/sumR; sumA += sumEGMPFiso;
    print "CJ\t "sumCJ"\t "sumCJ/sumR" in "nCJ; sumA += sumCJ;
    print "PFJ\t "sumPFJ"\t "sumPFJ/sumR" in "nPFJ; sumA += sumPFJ;
    print "RegCos\t "sumRegCos"\t "sumRegCos/sumR; sumA += sumRegCos;
    print "PFnoIso\t "sumPFnoIso"\t "sumPFnoIso/sumR; sumA += sumPFnoIso;
    print "SCTau\t "sumSCTau"\t "sumSCTau/sumR; sumA += sumSCTau;
    print "HPSTau\t "sumHPSTau"\t "sumHPSTau/sumR; sumA += sumHPSTau;
    print "HPSTTau\t "sumHPSTTau"\t "sumHPSTTau/sumR; sumA += sumHPSTTau;
    print "CRTau\t "sumCRTau"\t "sumCRTau/sumR; sumA += sumCRTau;
    print "Unclean\t "sumUnclean"\t "sumUnclean/sumR; sumA += sumUnclean;
    if (printUnclean==1){
	for (md in unclean){ if(unclean[md]/sumR>0.005) print "\t "md"\t "unclean[md]"\t "unclean[md]/sumR;}
    }
    print "ConvTk\t "sumConvTk"\t "sumConvTk/sumR; sumA += sumConvTk;
    print "CosMu\t "sumCosMu"\t "sumCosMu/sumR; sumA += sumCosMu;
    print "MuNoCos\t "sumMuNoCos"\t "sumMuNoCos/sumR; sumA += sumMuNoCos;
    print "PixGenGsfTks\t "sumPixGenGsfTks"\t "sumPixGenGsfTks/sumR; sumA += sumPixGenGsfTks;

    print "subtotal\t " sumA"\t "sumA/sumR;
    for (md in tm) {if(tm[md]>0.1&&used[md]!=1) print md"\t "tm[md]; }
}