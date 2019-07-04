#!/bin/bash

clear

echo "Compiling $1 to PDF ..."
ARGUMENT="$1"
pandoc $ARGUMENT.md --template template.latex --listings --pdf-engine=xelatex -o $ARGUMENT.pdf --toc --toc-depth=3 -V colorlinks -M lang=sr -M langserbianlatin=true

#-V links-as-notes=true
#--reference-links 
#--number-sections
#--filter pandoc-crossref
#-M toccolor=blue

ps axf | grep epdfview | grep -v grep | awk '{print "kill -s SIGHUP " $1}' | bash
