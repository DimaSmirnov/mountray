#include <libnotify/notify.h>

NotifyNotification *notify;

void Create_notify(const char *notify_header, const char *notify_text, int delay) {
	notify_init(notify_text);
	notify = notify_notification_new(notify_text,notify_header,NULL);
	GdkPixbuf *image = gdk_pixbuf_new_from_file("/usr/share/pixmaps/mountray/drive-active-big.png", &error);
	notify_notification_set_image_from_pixbuf(notify, image);
	notify_notification_set_timeout(notify,delay); // set the timeout of the notification to DELAY milisecs
	notify_notification_show(notify,&error);
}
