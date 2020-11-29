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
	# needed for pacf, which is a 32bit executable
	sudo dpkg --add-architecture i386
	sudo apt-get update
	# libc6:amd64 seems to be outdated
	sudo apt-get install --only-upgrade libc6
	# install 32-bit libc
	sudo apt-get install libc6:i386

	# PACF is defined as "/tmp/pacf/bin/pacf"
	mkdir -p "/tmp/pacf/bin"
	cd "/tmp/pacf/bin"
	wget -q -O - http://microapl.com/Porting/ColdFire/Download/pacflin.gz | gzip -dc > pacf
	chmod 755 pacf
	cd -
	;;
esac

if [ "${TRAVIS_PULL_REQUEST}" = "false" -a "${TRAVIS_REPO_SLUG}" = "freemint/fvdi" ]
then
	BINTRAY_REPO="travis" SYSROOT_DIR="/" ./.travis/install_bintray.sh m68k-atari-mint-binutils-gdb m68k-atari-mint-gcc mintbin
	BINTRAY_REPO="lib" ./.travis/install_bintray.sh mintlib
else
	sudo add-apt-repository -y ppa:vriviere/ppa
	sudo apt-get update
	sudo apt-get install binutils-m68k-atari-mint gcc-m68k-atari-mint mintbin-m68k-atari-mint mintlib-m68k-atari-mint
fi
