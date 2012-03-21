mountray: main.c gtk.c drives.c notify.c
	g++ -Wno-conversion-null -Wno-write-strings -g -o mountray main.c `pkg-config --cflags --libs gtk+-2.0` -lnotify

install: mountray
	cp mountray /usr/bin/mountray
	cp icons/drive-active-big.png /usr/share/pixmaps/mountray/drive-active-big.png
	cp icons/drive-active.png /usr/share/pixmaps/mountray/drive-active.png
	cp icons/drive-normal.png /usr/share/pixmaps/mountray/drive-normal.png
	cp icons/unmount.png /usr/share/pixmaps/mountray/unmount.png

uninstall: mountray
	rm /usr/bin/mountray
	rm /usr/share/pixmaps/mountray/drive-active-big.png
	rm /usr/share/pixmaps/mountray/drive-active.png
	rm /usr/share/pixmaps/mountray/drive-normal.png
	rm /usr/share/pixmaps/mountray/unmount.png
