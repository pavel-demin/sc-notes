#! /bin/sh

export SUPERCOLLIDER_HEADERS=/opt/sc/include/SuperCollider

cd guitarix

for file in *.dsp
do
  echo $file
  faust2supercollider $file
done

dir=/opt/sc/share/SuperCollider/Extensions/GXplugins
mkdir -p $dir
cp *.sc *.so $dir

cd -
