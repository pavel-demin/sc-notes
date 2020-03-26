#! /bin/sh

git clone --recursive https://github.com/supercollider/supercollider.git
mkdir supercollider/build
cd supercollider/build
cmake -DCMAKE_INSTALL_PREFIX=/opt/sc -DCMAKE_BUILD_TYPE=Release -DNATIVE=ON -DSC_EL=OFF ..
make -j 2
make install
cd -

git clone --recursive https://github.com/supercollider/sc3-plugins.git
mkdir sc3-plugins/build
cd sc3-plugins/build
cmake -DSC_PATH=../../supercollider -DCMAKE_INSTALL_PREFIX=/opt/sc -DCMAKE_BUILD_TYPE=Release -DNATIVE=ON ..
make -j 2
make install
cd -
