#!/bin/bash -eux
# -e: Exit immediately if a command exits with a non-zero status.
# -u: Treat unset variables as an error when substituting.
# -x: Display expanded script commands

if [ -n "${FT2_VERSION+x}" ]
then
	cd /tmp
	subdir=""
	if test ${FT2_VERSION} = "2.2.1"; then subdir="freetype-old/"; fi
	wget -q -O - "https://download.savannah.gnu.org/releases/freetype/${subdir}freetype-${FT2_VERSION}.tar.gz" | tar xzf -
	mv "freetype-${FT2_VERSION}" freetype
	cd -

	cd fvdi/modules/ft2 && ln -s "/tmp/freetype" && cd -
fi

case ${CPU_TARGET} in
*v4e*)
	# first cleanup the gcc mess
	sudo apt-get remove gcc-m68k-atari-mint
	# needed for pacf, which is a 32bit executable
	sudo dpkg --add-architecture i386
	sudo apt-get update
	# libc6:amd64 seems to be outdated
	sudo apt-get install --only-upgrade libc6
	# install 32-bit libc and m68k-atari-mint-gcc again
	sudo apt-get install libc6:i386 gcc-m68k-atari-mint
	# remove mintlib and pml again...
	sudo dpkg --remove --force-depends mintlib-m68k-atari-mint pml-m68k-atari-mint

	# PACF is defined as "/tmp/pacf/bin/pacf"
	mkdir -p "/tmp/pacf/bin"
	cd "/tmp/pacf/bin"
	wget -q -O - http://microapl.com/Porting/ColdFire/Download/pacflin.gz | gzip -dc > pacf
	chmod 755 pacf
	cd -
	;;
esac

BINTRAY_REPO="freemint" ./.travis/install_bintray.sh mintlib
