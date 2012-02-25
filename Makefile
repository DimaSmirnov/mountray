mountray: main.c gtk.c drives.c notify.c
	g++ -Wno-conversion-null -Wno-write-strings -g -o mountray main.c `pkg-config --cflags --libs gtk+-2.0` -lnotify

