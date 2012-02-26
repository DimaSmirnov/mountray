#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>

GtkWidget *menu, *response_widget, *menuitem, *image, *widget, *sep;
GtkStatusIcon *tray_icon;
GdkPixbuf *buf;

GtkIconSize icon_size;
GError *error = NULL;
struct  vols{   // -- массив разделов
		int number; // Порядковый номер
		const char *name; // Название раздела
		const char *drive_name;
		int ismounted; //  // 0 - no, 1 - yes
		GVolume *volume;
		GDrive *drive;
		GMount *mount;
}; vols *volumes;
int iconchangestatus = 0;
int drives_qty;

/////////////////////////////////////
void ActionOnQuit() {

	printf("Exiting. See you again!\n");
	gtk_widget_destroy (menu);
	//g_object_unref(gio->monitor);
	free(volumes);
	gtk_main_quit ();
}

/////////////////////////////////////
static void ActionEjectFinish(GObject *source, GAsyncResult *result, gpointer user_data) {
	g_volume_eject_with_operation_finish (G_VOLUME (source),result, &error);
}
void ActionOnDriveClick(GtkWidget *widget, GdkEvent *event) {
	const gchar *name = gtk_widget_get_name(widget);
	//printf("Action with volume: %s\n", name);
	for (int i=0;i<drives_qty;i++) {
		if (!strcmp(volumes[i].name,name)) {
				if (!volumes[i].ismounted) g_volume_mount(volumes[i].volume,G_MOUNT_MOUNT_NONE,NULL,NULL,NULL,NULL);
				else g_volume_eject_with_operation (volumes[i].volume,G_MOUNT_UNMOUNT_NONE,NULL,NULL,ActionEjectFinish,NULL);
		}
	}
}
/////////////////////////////////////
void AddDriveToMenu(const char *volume_name, int ismounted, GIcon *drive_icon) {

	if (!ismounted) {
		image = gtk_image_new_from_gicon(drive_icon,icon_size);
		menuitem = gtk_image_menu_item_new_with_label(volume_name);
		gtk_widget_set_tooltip_text (menuitem,"Volume not mounted");
		gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM (menuitem), image);
	}
	else {
		GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file("/usr/share/pixmaps/mountray/unmount.png", &error);
		image = gtk_image_new_from_pixbuf(pixbuf);
		menuitem = gtk_image_menu_item_new_with_label(volume_name);
		gtk_widget_set_tooltip_text (menuitem,"Volume mounted");
		gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM (menuitem), image);
	}


	gtk_widget_set_name(menuitem, volume_name);
	g_signal_connect(menuitem, "activate",(GCallback) ActionOnDriveClick,  menuitem);
	gtk_menu_shell_append(GTK_MENU_SHELL(menu), menuitem);

}
/////////////////////////////////////
void ShowTrayMenu(GtkWidget *widget, GdkEvent *event) {
	gtk_widget_show_all(menu);
	gtk_menu_popup (GTK_MENU(menu), NULL, NULL, NULL, NULL, NULL, NULL);
}
/////////////////////////////////////
static GtkStatusIcon *CreateTrayIcon() {

        tray_icon = gtk_status_icon_new();
        g_signal_connect(G_OBJECT(tray_icon), "activate", G_CALLBACK(ShowTrayMenu), NULL);
        g_signal_connect_swapped(G_OBJECT(tray_icon), "popup-menu",G_CALLBACK(ShowTrayMenu), NULL);
        gtk_status_icon_set_from_file (tray_icon, "/usr/share/pixmaps/mountray/drive-normal.png");
        gtk_status_icon_set_tooltip(tray_icon, "Mountray. Tray mounter");
        gtk_status_icon_set_visible(tray_icon, TRUE);
        return tray_icon;
}
/////////////////////////////////////
gboolean TimerFuncReturnStatusIcon(GtkWidget *label) {

	 if (iconchangestatus) {
		gtk_status_icon_set_from_file (tray_icon, "/usr/share/pixmaps/mountray/drive-normal.png");
		iconchangestatus=0;
	}
	return TRUE;
}
