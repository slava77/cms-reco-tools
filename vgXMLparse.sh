#!/bin/bash
grep "<obj\|<dir\|<file\|<line\|<fn\|</error\|<kind" $1 |   awk '\
    /<\/error>/{prn=0;}\
   /kind>/{if(!match(msg, "TObject") && !match(msg, "R__zipLZMA") && !match(msg, "clang::Parser::Parse") && !match(msg, "clang::ASTReader") ){\
   print msgP"\n======\n";} msg=$0; msgP=msg; prn=1;cnt=0;}\
   /<fn>|<dir>|<file>|<line>/{if (prn==1){msg=msg"\n"$0; if(cnt<20)msgP=msgP"\n"$0}\
        if (prn==1)cnt++; else cnt=0;}' |\
 sed -e 's?<fn>??g;s?</fn>??g;s?<obj>??g;s?</obj>??g;s?<text>??g;s?</text>??g;s/\&gt;/>/g;s/\&amp;/\&/g;s/\&lt;/</g'