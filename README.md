# pter1342's build of dwm - dynamic window manager
This is my build of dwm, the extremely fast, small, and dynamic window manager for X.

## Patches applied
Here's the list of the patches I've applied to this build:
* fullscreen-compilation
* attachbelow
* layout patches: bottomstack, centeredmaster, columns, fibonacci
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

## Configuration
The configuration of dwm is done by creating a custom config.h
and (re)compiling the source code, or (preferred) adding the removal of config.h into the makefile clean function and editing config.def.h only.

## Installation
Enter the following commands to build and install  this build of dwm:
```bash
git clone https://github.com/pter1342/dwm.git
cd dwm
sudo make clean install
```

## Running dwm
Add the following line to your .xinitrc to start dwm using startx:

    exec dwm
