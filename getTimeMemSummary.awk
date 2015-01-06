BEGIN{
    maxV=0;maxR=0;maxVI=-1;maxRI=-1;
    cn=0;
    maxC=0;maxCI=-1;avC=0;
    maxC1=0;maxCI1=-1;avC1=0;
    maxT=0;maxTI=-1;avT=0;
    maxT1=0;maxTI1=-1;avT1=0;
}
/^MemoryCheck/{
    cn++; 
    if(maxV<$5){maxV=$5; maxVI=cn;} 
    if(maxR<$8){maxR=$8; maxRI=cn;}
}
/^TimeEvent/{
    avT+=$4; if(maxT<$4){maxT=$4;maxTI=cn;}
    avC+=$5; if(maxC<$5){maxC=$5;maxCI=cn;}
    if (cn>1){
	avT1+=$4; if(maxT1<$4){maxT1=$4;maxTI1=cn;}
	avC1+=$5; if(maxC1<$5){maxC1=$5;maxCI1=cn;}
    }
}
END{
    print "Summary for "cn" events";
    print "Max VSIZ "maxV" on evt "maxVI" ; max RSS "maxR" on evt "maxRI; 
    if (cn>=1){
	print "Time av "avT/cn" s/evt   max "maxT" s on evt "maxTI;
	print "CPU av "avC/cn" s/evt   max "maxC" s on evt "maxCI;
    }
    if (cn>=2){
	print "M1 Time av "avT1/(cn-1)" s/evt   max "maxT1" s on evt "maxTI1;
	print "M1 CPU av "avC1/(cn-1)" s/evt   max "maxC1" s on evt "maxCI1;
    }
}