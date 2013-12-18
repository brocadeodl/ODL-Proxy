#!/bin/sh
fileList=`find $1 -name "*.html"`;

for mfile in $fileList
do
        outFile=$mfile.mod;
        #echo "*** $outFile ***";
        #sed 's/.*files\.html.*//g' $mfile > $outFile;
        sed 's/<a class="qindex" href="files.html">File&nbsp;List<\/a>//g' $mfile > $outFile;
        mv $outFile $mfile
done

