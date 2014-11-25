BEGIN{
    maxV=0;maxR=0;maxVI=-1;maxRI=-1;
    cn=0;
    maxC=0;maxCI=-1;avC=0;
}
/^MemoryCheck/{
    cn++; 
    if(maxV<$5){maxV=$5; maxVI=cn;} 
    if(maxR<$8){maxR=$8; maxRI=cn;}
}
/^TimeEvent/{
    avC+=$4; if(maxC<$4){maxC=$4;maxCI=cn;}
}
END{
    print "Max VSIZ "maxV" on evt "maxVI"  max RSS "maxR" on evt "maxRI; 
    print "CPU av "avC/cn" s/evt   max "maxC" s on evt "maxCI;
}