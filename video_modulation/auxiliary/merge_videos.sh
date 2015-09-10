#!/bin/bash

# touch tmp.avi
rm all.avi 2> /dev/null

# find . -name "*.avi" | sort | xargs echo
find . -name "*.avi" | sort | xargs cat > tmp.avi
ffmpeg -i tmp.avi -acodec copy -vcodec copy all.avi

rm tmp.avi
