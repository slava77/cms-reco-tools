ls -d all_* | while read -r d; do nd=`ls $d | grep -c png`; echo -e $nd" \t "$d; done 
# ls -d all_* | while read -r d; do nd=`ls $d | grep -c png`; echo -e $nd" \t "$d; done | grep  -v ^0
