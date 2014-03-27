cms-reco-tools
==============

cms-reco-tools

generally, you can expect that tools here are expecting that the scripts are located in your HOME/tools directory

--------------
#Using FWLite validate.C script
- provides comparison of different values in RECO objects (open the script to see which ones)
- script is avaibale to process multiple input files, assuming they are coming from runTheMatrix workflows
```
source validateJR.sh relDirNew relDirOld OldVSNew matrix_70X.txt
```
- relDirNew and Old are the base names of directories containing new and reference outputs of (various) matrix workflows
- matrix_70X.txt here is a map file that provides a connection between wflow directory name and the input file, cmsRun process name to look at, and the short name for outputs
- png file outputs will show up in all_OldVSNew_WFlowName directory if there are any differences.

---------------
