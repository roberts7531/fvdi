#!/bin/bash

CPU=${CPU:-"020"}			# assign defaults if not set
DEBUG=${DEBUG:-"yes"}

echo "\$CPU=$CPU"
echo "\$DEBUG=$DEBUG"

FVDI_DIR=fvdi

FT2_DIR=${FVDI_DIR}/modules/ft2
BPL_DIR=${FVDI_DIR}/drivers/bitplane
ENG_DIR=${FVDI_DIR}/engine

TARGET_DIR=/home/mfro/Dokumente/Development/atari/fvdi_test

rm -f bitplane.sys
rm -f fvdi_gnu.prg

(cd ${FT2_DIR}; DEBUG=${DEBUG} CPU=${CPU} M68K_ATARI_MINT_CROSS=yes make clean) || exit
(cd ${FT2_DIR}; DEBUG=${DEBUG} CPU=${CPU} M68K_ATARI_MINT_CROSS=yes make -j) || exit

(cd ${ENG_DIR}; DEBUG=${DEBUG} CPU=${CPU} M68K_ATARI_MINT_CROSS=yes make clean) || exit
(cd ${ENG_DIR}; DEBUG=${DEBUG} CPU=${CPU} M68K_ATARI_MINT_CROSS=yes make -j) || exit

(cd ${BPL_DIR}; DEBUG=${DEBUG} CPU=${CPU} M68K_ATARI_MINT_CROSS=yes make clean) || exit
(cd ${BPL_DIR}; DEBUG=${DEBUG} CPU=${CPU} M68K_ATARI_MINT_CROSS=yes make -j) || exit

cp ${BPL_DIR}/bitplane.sys .
cp ${ENG_DIR}/fvdi_gnu.prg .

m68k-atari-mint-strip bitplane.sys
m68k-atari-mint-strip fvdi_gnu.prg

# copy files to test directory if existing and if CPU is supported by hatari
if [ ${CPU} = '020' ]; then
    cp bitplane.sys $TARGET_DIR/gemsys
    cp fvdi_gnu.prg $TARGET_DIR/auto
fi
