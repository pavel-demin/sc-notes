#! /bin/sh

git clone --recursive https://github.com/grame-cncm/faust.git
cd faust
make PREFIX=/opt/faust -j 2
make PREFIX=/opt/faust install