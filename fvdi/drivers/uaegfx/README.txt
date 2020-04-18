UAEGFX.SYS - WinUAE RTG driver for fVDI
Copyright (C) 2017-2020 Vincent Riviere
email: vincent.riviere@freesbee.fr

Homepage, updates, source code:
http://vincent.riviere.free.fr/soft/m68k-atari-mint/archives/mint/fvdi/

This driver allows usage of WinUAE extended video modes from EmuTOS/FreeMiNT.
Currently, it only supports 16-bit 565 video modes, with any resolution.
VDI primitives are accelerated by WinUAE, for a significant speed boost.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

How to use:

1) Your WinUAE emulated Amiga needs at least 1 MB of RAM (any kind).

2) You need EmuTOS 0.9.12 or higher, Amiga ROM version.
This is mandatory for proper initialization of Zorro cards through
AUTOCONFIG mechanism.

3) Prepare your filesystem:
- Put FVDI.PRG in \AUTO folder
- Put UAEGFX.SYS in \GEMSYS folder
- Put the provided FVDI.SYS in the root folder

4) Select your resolution:
Edit FVDI.SYS with a text editor and change the following line at bottom:
01r uaegfx.sys mode 1024x768x16@60
The mode is in the form: WIDTHxHEIGHTxDEPTH@FREQ
WIDTH and HEIGHT can be any standard values. You can look at the WinUAE debug
log to see possible resolutions.
DEPTH must always be 16. FREQ is ignored.

5) In WinUAE properties, go to Hardware > RTG board. Then select an RTG Graphics
card in the list. UAE Zorro II or III work fine.

6) Start WinUAE. EmuTOS will start with its famous monochrome welcome screen.
Then it will switch to the extended video mode and display the familiar green
desktop, with 65536 available colors.

7) Of course you can also run FreeMiNT and XaAES to get more benefits from these
new video modes.
