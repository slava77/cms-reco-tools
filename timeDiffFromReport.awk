/^ot/{to[$2]=$3;}
/^st/{ts[$2]=$3;}

END{
    for (m in to){
	ms[m]=1;
    }
    for (m in ts){
	if (ms[m]==1) ms[m]=2;
	else ms[m]=3;
    }

    toJob = 0;
    tsJob = 0;
    for (m in ms){
	toV = to[m];
	tsV = ts[m];
	tm=0.5*(toV+tsV); 
	dt=tsV-toV; 

	if(tm>0) dt=dt/tm; 
	adt = dt; if (adt< 0) adt = -adt;

	dta[m] = dt;
	adta[m] = adt;
	
	toJob += toV;
	tsJob += tsV;
    }
    nmod = asort(adta,adtaSV);

    for (m in dta) dtaT[m] = dta[m];
    nPrn=0;

    minAdt = 0.05;
    for (im=nmod; im>=1;--im){
	mName="";
	adt = adtaSV[im];
	if (adt< minAdt) continue;

	for (m in dtaT){
	    valT=dtaT[m];
	    if (valT==adt || valT==-adt){
		mName = m;
		dtaT[m]=9;
		break;
	    }
	}
	m = mName;
	tm = 0.5*(to[m] + ts[m]);
	dt=dta[m];
	dtJob = dt*tm*100/toJob;
	if ( (adt>0.05&&tm>0.005) || (adt>0.2&&tm>0.0005) || adt>1 ){
	    if (nPrn==0){
		printf("%12s %10s %12s          %12s       %s\n","delta/mean","delta/orJob", "original", "new","module name");
		printf("%12s %10s %12s          %12s       %s\n","----------","------------", "--------", "----","------------");
	    }
	    if (adt!=2){
		printf("%+12f %+10.2f%% %12.2f ms/ev -> %12.2f ms/ev %s\n",dt, dtJob, to[m]*1000, ts[m]*1000,m);
	    } else if (dt==-2){
		printf("%12s %+10.2f%% %12.2f ms/ev -> %12.2f ms/ev %s\n","removed", dtJob, to[m]*1000, 0, m);
	    } else {
		printf("%12s %+10.2f%% %12.2f ms/ev -> %12.2f ms/ev %s\n","added", dtJob, 0, ts[m]*1000, m);
	    }
	    nPrn++;
	} 
    }
    
    printf("%12s %10s %12s          %12s       %s\n","----------","------------", "--------", "----","------------");
    print "Job total:  "toJob" s/ev ==> "tsJob" s/ev";
}