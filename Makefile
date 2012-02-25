main: main.c gtk.c drives.c notify.c
	g++ -Wno-conversion-null -Wno-write-strings -g -o main main.c `pkg-config --cflags --libs gtk+-2.0` -lnotify

#gtk.o: gtk.c
#	g++ -g  -c gtk.c -o gtk.o `pkg-config --cflags --libs gtk+-2.0`

#drives.o: drives.c
#	g++ -g  -c drives.c -o drives.o `pkg-config --cflags --libs gtk+-2.0`

