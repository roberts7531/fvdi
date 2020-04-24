# fvdi
fVDI fork with additional fixes and drivers.

new branch "cf"
- builds for ColdFire (using MicroAPL's PortAsm) depending on CPU
- CPU can be set with setting the CPU variable
- Makefiles are sensitive to "CPU=v4e" (ColdFire build) or any other m68k-atari-mint CPU target
- provided build script "buildit.sh" (dependend on CPU setting as well) builds the fvdi engine and the bitplane.sys driver 
  (to allow testing using hatari)

Not yet functional (crashes when opening virtual workstation)
