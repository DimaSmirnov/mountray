#include <gio/gio.h>
#include <stdlib.h>

GIcon *drive_icon;
typedef struct {
    GVolumeMonitor *monitor;
} Gio[1];
const char *drv_name;

/////////////////////////////////////
int is_mount(GVolume *volume) {
	int ismounted;

	GMount *is_mounted = g_volume_get_mount(volume);
	if (!is_mounted) ismounted = 0;
	else ismounted = 1;
	return ismounted;
}
/////////////////////////////////////
void fill_volumes_array(GVolumeMonitor *volume_monitor) {
	int c=0;
	GList *m, *k, *volus, *drives;

	drives = g_volume_monitor_get_connected_drives(volume_monitor);
	for (m = drives; m; m = m->next) {
		gchar *drive_name = g_drive_get_name((GDrive*)m->data);
		volus = g_drive_get_volumes((GDrive*)m->data);
		for (k = volus; k; k = k->next,c++) {
			const char *volume_name = g_volume_get_name((GVolume*)k->data);
			volumes[c].mount  = g_volume_get_mount((GVolume*)k->data);
			int is_mounted = is_mount((GVolume*)k->data);
			volumes[c].ismounted = is_mounted;
			volumes[c].number=c;
			volumes[c].name = volume_name;
			volumes[c].volume = (GVolume*)k->data;
			volumes[c].drive_name = drive_name;
			volumes[c].drive = (GDrive*)m->data;
			AddDriveToMenu(volume_name, volumes[c].ismounted, drive_icon);
		}
	}
	g_list_free(k); g_list_free(m); g_list_free(volus);
}
/////////////////////////////////////
int get_qty_connected_volumes(GVolumeMonitor *volume_monitor) {
	int c=0; GList *m, *k, *volus, *drives;
	drives = g_volume_monitor_get_connected_drives(volume_monitor);
	for (m = drives; m; m = m->next) {
		gchar *drive_name = g_drive_get_name((GDrive*)m->data);
		drive_icon = g_drive_get_icon((GDrive*)m->data);
		volus = g_drive_get_volumes((GDrive*)m->data);
		for (k = volus; k; k = k->next,c++) { }
	}
	drives_qty=c;
	g_list_free(k); g_list_free(m); g_list_free(volus); g_list_free(drives);
return c;
}
/////////////////////////////////////
static void create_menu(Gio gio) {

	menu = gtk_menu_new();
	int drives_qty = get_qty_connected_volumes(gio->monitor);
	volumes = new vols[drives_qty];
	if (drives_qty) {
		menuitem = gtk_menu_item_new_with_label ("Volumes:");
		gtk_widget_set_sensitive (menuitem,FALSE);
		gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
	}

	fill_volumes_array(gio->monitor); // заполняем массив и меню

	if (drives_qty) {
		sep = gtk_separator_menu_item_new(); // сепаратор
		gtk_menu_shell_append(GTK_MENU_SHELL(menu), sep);
	}
	menuitem = gtk_image_menu_item_new_from_stock(GTK_STOCK_QUIT, NULL);
	g_signal_connect(menuitem, "activate",(GCallback) ActionOnQuit,  widget);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);
}
/////////////////////////////////////
static void mountray_init(Gio gio) {
	create_menu(gio);
}
/////////////////////////////////////
static void on_volume_connected(GVolumeMonitor *volume_monitor, GVolume *volume, Gio gio) {

    gtk_status_icon_set_from_file (tray_icon, "/usr/share/pixmaps/mountray/drive-active.png");
	iconchangestatus=1;
	gtk_widget_destroy (menu);
	free(volumes);
	create_menu(gio);
	printf("Volume connected\n");
	for (int i=0; strlen(volumes[i].name)>0; i++) {
		if (volume == volumes[i].volume) { drv_name=volumes[i].drive_name; break; }
	}
	Create_notify("Устройство подключено", drv_name, 3000);
}
/////////////////////////////////////
static void on_volume_disconnected(GVolumeMonitor *volume_monitor, GVolume *volume, Gio gio) {
    gtk_status_icon_set_from_file (tray_icon, "/usr/share/pixmaps/mountray/drive-active.png");
	iconchangestatus=1;
	gtk_widget_destroy (menu);
	free(volumes);
	create_menu(gio);
	printf("Volume disconnected\n");
}
/////////////////////////////////////
static void on_volume_mounted(GVolumeMonitor *volume_monitor, GDrive *drive, Gio gio) {
    gtk_status_icon_set_from_file (tray_icon, "/usr/share/pixmaps/mountray/drive-active.png");
	iconchangestatus=1;
	gtk_widget_destroy (menu);
	free(volumes);
	create_menu(gio);
	printf("Volume mounted\n");
}
/////////////////////////////////////
static void on_volume_unmounted (GVolumeMonitor *volume_monitor, GDrive *drive, Gio gio) {

    gtk_status_icon_set_from_file (tray_icon, "/usr/share/pixmaps/mountray/drive-active.png");
	iconchangestatus=1;
	for (int i=0; strlen(volumes[i].name)>0; i++) {
		if (drive = volumes[i].drive) { drv_name=volumes[i].drive_name; break; }
	}
	Create_notify("Устройство может быть извлечено", drv_name,  3000);
	gtk_widget_destroy (menu);
	free(volumes);
	create_menu(gio);
	printf("Volume unmounted\n");


}
