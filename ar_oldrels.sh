arDir=/castor/cern.ch/user/s/slava77/oldrels
echo $# directories will be archived to ${arDir}
echo $@ | tr ' ' '\n' |  while read -r d
  do echo do ${d} at `date`
  td=${d}.tar.gz 
  tar czf ${td} ${d}  && rfcp ${td} ${arDir}/${td} && rm -rf ${td} && rm -rf ${d}
  echo done ${d} at `date`
done