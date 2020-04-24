# fvdi
fVDI fork with additional fixes and drivers.

new branch "cf"
- builds for ColdFire (using MicroAPL's PortAsm) depending on CPU
- CPU can be set with setting the CPU variable
- Makefiles are sensitive to "CPU=v4e" (ColdFire build) or any other m68k-atari-mint CPU target
- provided build script "buildit.sh" (dependend on CPU setting as well) builds the fvdi engine and the bitplane.sys driver 
  (to allow testing using hatari). Build with "CPU=v4e ./buildit.sh" for ColdFire (needs pacf installed) or with 
  "CPU=000 ./buildit.sh if you want an m68000 build)
- untar the provided freetype-2.2.1.tar.bz2 (in fvdi/modules/ft2) for truetype library before building

ColdFire build not yet functional (crashes when opening virtual workstation). 
Works for other CPUs

