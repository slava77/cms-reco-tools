BEGIN{
    maxV=0;maxR=0;maxVI=-1;maxRI=-1;
    cn=0;
    maxC=0;maxCI=-1;avC=0;
    maxT=0;maxTI=-1;avT=0;
}
/^MemoryCheck/{
    cn++; 
    if(maxV<$5){maxV=$5; maxVI=cn;} 
    if(maxR<$8){maxR=$8; maxRI=cn;}
}
/^TimeEvent/{
    avT+=$4; if(maxT<$4){maxT=$4;maxTI=cn;}
    avC+=$5; if(maxC<$5){maxC=$5;maxCI=cn;}
}
END{
    print "Max VSIZ "maxV" on evt "maxVI" ; max RSS "maxR" on evt "maxRI; 
    print "Time av "avT/cn" s/evt   max "maxT" s on evt "maxTI;
    print "CPU av "avC/cn" s/evt   max "maxC" s on evt "maxCI;
}