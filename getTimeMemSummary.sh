#!/bin/bash
grep "^MemoryCheck\|^TimeEvent>" $1  | awk -f ~/tools/getTimeMemSummary.awk
