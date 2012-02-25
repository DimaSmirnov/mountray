#include <stdio.h>
#include "gtk.c"
#include "notify.c"
#include "drives.c"



int main(int argc, char *argv[]) {
	Gio gio;

	gtk_init(&argc, &argv);
	gio->monitor = g_volume_monitor_get();

    g_signal_connect(gio->monitor , "volume-added", G_CALLBACK(on_volume_connected), gio);
    g_signal_connect(gio->monitor , "volume-removed", G_CALLBACK(on_volume_disconnected), gio);
    g_signal_connect(gio->monitor , "mount-added", G_CALLBACK(on_volume_mounted), gio);
    g_signal_connect(gio->monitor , "mount-removed", G_CALLBACK(on_volume_unmounted), gio);
	icon_size = gtk_icon_size_register ("",18,18);
	g_timeout_add(1000, (GSourceFunc) TimerFuncReturnStatusIcon, response_widget);
	tray_icon = CreateTrayIcon();

	mountray_init(gio);

	gtk_main();
    return 0;
}
