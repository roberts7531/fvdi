SAGA.SYS - SAGA driver for fVDI
Copyright (C) 2017 Vincent Riviere
email: vincent.riviere@freesbee.fr

Homepage, updates, source code:
http://vincent.riviere.free.fr/soft/m68k-atari-mint/archives/mint/fvdi/

This driver is designed to run on Amiga hardware with Vampire V2 accelerator.
It allows usage of SAGA HDMI video modes from EmuTOS/FreeMiNT.
Currently, it only supports 16-bit 565 video modes, with some resolutions.
VDI primitives are not accelerated by any dedicated hardware. However, the
Apollo 68080 CPU is fast enough for comfortable user experience.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

How to use:

1) The preferred way to run EmuTOS is from floppy.
Such floppy is available from development snapshots:
https://sourceforge.net/projects/emutos/files/snapshots/
Download emutos-amiga-floppy-*.zip, and extract emutos-vampire.adf.
You can write that image to a real floppy, or use a floppy emulator.

2) Prepare your C: filesystem:
- Put FVDI.PRG in \AUTO folder
- Put SAGA.SYS in \GEMSYS folder
- Put the provided FVDI.SYS in the root folder

3) Select your resolution:
Edit FVDI.SYS with a text editor and uncomment one of the saga.sys lines at
bottom:
01r saga.sys mode 640x480x16@60
The mode is in the form: WIDTHxHEIGHTxDEPTH@FREQ
WIDTH and HEIGHT are currently limited to the provided examples.
DEPTH must always be 16. FREQ is ignored.
Note that due to memory bandwidth limitation on Vampire V2 hardware, high
resolutions may be unstable, especially when programs make many memory accesses.

4) Put the EmuTOS floppy in your drive, and start your Amiga. EmuTOS will start
with its famous monochrome welcome screen. Then it will activate the HDMI
output and display the familiar green desktop, with 65536 available colors.

5) Of course you can also run FreeMiNT and XaAES to get more benefits of these
new video modes.

Credits:
- fVDI engine and example drivers: Johan Klockars
- SAGA Picasso96 driver: Jason S. McMullan
- SAGA PLL initialization: Christoph Hoehne
- SAGA fVDI driver: Vincent Riviere
