#!/bin/bash

pset=$1

igprof -pp -z -o ig.gz -t cmsRun cmsRun $pset 
igprof-analyse --sqlite -v --demangle --gdb ig.gz > ig.txt
python ~/tools/fix-igprof-sql.py ig.txt > ig_fixed.txt
sqlite3 ig.sql3 < ig_fixed.txt
