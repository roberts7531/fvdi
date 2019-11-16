#!/bin/bash -x

CPU=v4e
#CPU=020

FVDI_DIR=/home/mfro/Dokumente/Development/atari/fvdi/fvdi

FT2_DIR=$FVDI_DIR/modules/ft2
BPL_DIR=$FVDI_DIR/drivers/bitplane
ENG_DIR=$FVDI_DIR/engine

TARGET_DIR=/home/mfro/Dokumente/Development/atari/fvdi_test

(cd $FT2_DIR; CPU=$CPU M68K_ATARI_MINT_CROSS=yes make clean)
(cd $FT2_DIR; CPU=$CPU M68K_ATARI_MINT_CROSS=yes make -j)

(cd $ENG_DIR; CPU=$CPU M68K_ATARI_MINT_CROSS=yes make clean)
(cd $ENG_DIR; CPU=$CPU M68K_ATARI_MINT_CROSS=yes make -j)

(cd $BPL_DIR; CPU=$CPU M68K_ATARI_MINT_CROSS=yes make clean)
(cd $BPL_DIR; CPU=$CPU M68K_ATARI_MINT_CROSS=yes make -j)

#cp $BPL_DIR/bitplane.sys $FVDI_DIR/gemsys
#cp $ENG_DIR/fvdi_gnu.prg $FVDI_DIR/auto
cp $BPL_DIR/bitplane.sys .
cp $ENG_DIR/fvdi_gnu.prg .
m68k-atari-mint-strip bitplane.sys
m68k-atari-mint-strip fvdi_gnu.prg
