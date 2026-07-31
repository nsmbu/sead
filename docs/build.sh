#!/usr/bin/env sh
set -eu

curl -fsSLO https://github.com/doxygen/doxygen/releases/download/Release_1_14_0/doxygen-1.14.0.linux.bin.tar.gz
tar -xzf doxygen-1.14.0.linux.bin.tar.gz
curl -fsSL https://docs.nsmbu.net/sys/Sys.tag -o Sys.tag
curl -fsSL https://docs.nsmbu.net/eft/Eft.tag -o Eft.tag
curl -fsSL https://docs.nsmbu.net/ffl/FFL.tag -o FFL.tag
curl -fsSL https://docs.nsmbu.net/g3d/G3d.tag -o G3d.tag
curl -fsSL https://docs.nsmbu.net/lyt/Lyt.tag -o Lyt.tag
curl -fsSL https://docs.nsmbu.net/snd/Snd.tag -o Snd.tag
./doxygen-1.14.0/bin/doxygen ./docs/Doxyfile
