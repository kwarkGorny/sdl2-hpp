#!/bin/bash -x

./preper_general.sh

mkdir ../external
mkdir ../external/win
pushd ../external/win

rm -rf SDL2
SDL2="SDL2-devel-2.0.12-VC"
wget https://www.libsdl.org/release/$SDL2.zip
unzip -q $SDL2.zip
mv "SDL2-2.0.12" SDL2
rm $SDL2.zip

rm -rf SDL2_image
SDL2Image="SDL2_image-devel-2.0.5-VC"
wget https://www.libsdl.org/projects/SDL_image/release/$SDL2Image.zip
unzip -q $SDL2Image.zip
mv "SDL2_image-2.0.5" SDL2_image
rm $SDL2Image.zip

rm -rf SDL2_ttf
SDL2ttf="SDL2_ttf-devel-2.0.15-VC"
wget https://www.libsdl.org/projects/SDL_ttf/release/$SDL2ttf.zip
unzip -q $SDL2ttf.zip
mv "SDL2_ttf-2.0.15" SDL2_ttf
rm $SDL2ttf.zip

rm -rf SDL2_mixer
SDL2mixer="SDL2_mixer-devel-2.0.4-VC"
wget https://www.libsdl.org/projects/SDL_mixer/release/$SDL2mixer.zip
unzip -q $SDL2mixer.zip
mv "SDL2_mixer-2.0.4" SDL2_mixer
rm $SDL2mixer.zip

popd
