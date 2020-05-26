#!/bin/bash -ex
# -e: Exit immediately if a command exits with a non-zero status.
# -u: Treat unset variables as an error when substituting.
# -x: Display expanded script commands

FT2_URL=https://download.savannah.gnu.org/releases/freetype

if test "${CPU_TARGET}" != ""; then
	ALL_CPUS="${CPU_TARGET}"
else
	ALL_CPUS="000 020 v4e"
fi
FT2_VERSIONS="2.2.1 2.5.2 2.8.1 2.10.2"

pwd="$PWD"
cd ..
mkdir -p freetype
cd freetype
for ft2 in ${FT2_VERSIONS}
do
	if ! test -d freetype-${ft2}; then
		subdir=""
		if test "$ft2" = "2.2.1"; then subdir="freetype-old/"; fi
		wget -q -O - $FT2_URL/${subdir}freetype-${ft2}.tar.gz | tar xzf -
	fi
done
FT2_DIR=`pwd`
cd "$pwd"

test -d fvdi && cd fvdi
INSTALL_DIR=`pwd`/release

case $ALL_CPUS in
*v4e*)
# needed for pacf, which is a 32bit executable
	sudo dpkg --add-architecture i386
	sudo apt-get update
	sudo apt-get install libc6:i386

	mkdir -p bin
	cd bin
	wget -q -O - http://microapl.com/Porting/ColdFire/Download/pacflin.gz | gzip -dc > pacf
	chmod 755 pacf
	export PACF=`pwd`/pacf
	cd ..
	;;
esac

for ft2 in "" ${FT2_VERSIONS}; do
	echo freetype $ft2
	rm -f modules/ft2/freetype
	ft2tag=
	if test "$ft2" != ""; then
		ln -s ${FT2_DIR}/freetype-${ft2} modules/ft2/freetype
		ft2tag=-ft${ft2}
	fi
	for CPU in $ALL_CPUS; do
		make clean >/dev/null
		make CPU="${CPU}" V=1 || exit 1
		rm -rf "${INSTALL_DIR}"
		make CPU="${CPU}" DESTDIR="${INSTALL_DIR}" install || exit 1
		tar -C "${INSTALL_DIR}" -cvzf fvdi-${CPU}${ft2tag}.tar.gz .
	done
done

echo "release files at: $PWD"
ls -l fvdi*tar.gz
