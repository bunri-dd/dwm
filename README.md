# dwm - dynamic window manager
dwm is an extremely fast, small, and dynamic window manager for X.

## Patches applied
Here's the list of the patches I've applied to this build:
* fullscreen-compilation
* attachbelow
* layout patches: bottomstack, centeredmaster, columns
* sticky
* swallow
* pertag
* awesomebar
* hide vacant tags
* statuscmd (for working clickable dwmblocks-async status bar)
* shiftview function
* scratchpads
* autostart
* uselessgap

## Requirements
In order to build dwm you need the Xlib header files.

## Configuration
The configuration of dwm is done by creating a custom config.h
and (re)compiling the source code, or (preferred) adding the removal of config.h into the makefile clean function and editing config.def.h only.

## Installation
------------
Edit config.mk to match your local setup if you need to (dwm is installed into
the /usr/local namespace by default).

Afterwards enter the following command to build and install dwm (if
necessary as root):

    make clean install


## Running dwm
Add the following line to your .xinitrc to start dwm using startx:

    exec dwm
