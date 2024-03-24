/*
*------------------------------------------------------------------------------
*
*								TINY-KYOUKAI
*
*	Displays a little Kyoukai at the bottom of your screen !
*	Nfu~ !
*
*	Author	: HerrCrazi <herrcrazi@gmail.com>
*	Date	: 24/03/2024
*------------------------------------------------------------------------------
*/


/*
*------------------------------------------------------------------------------
*	Includes
*------------------------------------------------------------------------------
*/

#include <gtk/gtk.h>
#include <stdio.h>
#include <X11/Xlib.h>
#include <gdk/x11/gdkx.h>

#include "test.h"
#include "version.h"
#include "types.h"

/*
*------------------------------------------------------------------------------
*	Definitions
*------------------------------------------------------------------------------
*/

#define KYOU_ICON_PATH 	"ressources/"
#define KYOU_ICON_NAME 	TINYKYOU_NAME
#define KYOU_PATH		"kyoukai_peek.png"

#define KYOU_WIN_WIDTH	250
#define KYOU_WIN_HEIGHT	120

typedef struct s_kyou_options {
	char* szKyouPath;
	bool  bAllowResize;
	bool  bDrawFrame;
} t_kyou_options;


/*
*------------------------------------------------------------------------------
*	GTK devs are RETARDED greasy fucks (imagine being a gnome dev!)
*------------------------------------------------------------------------------
*/ /* Move Kyoukai */
static void kyou_window_move(GtkWindow* window, int16 x, int16 y) {

	Window   xw = gdk_x11_surface_get_xid( GDK_SURFACE(gtk_native_get_surface( GTK_NATIVE(window) )) );
	Display *xd = gdk_x11_display_get_xdisplay( gtk_widget_get_display( GTK_WIDGET(window) ) );
	/* I WILL move my windows no matter what gnometards think is bad! */
	XMoveWindow( xd, xw, x, y );
}

/* Get Kyoukai's screen pos */
static void kyou_get_pos(GtkWindow* window, int16* x, int16* y) {
	XWindowAttributes xwa;

	Window   xw = gdk_x11_surface_get_xid( GDK_SURFACE(gtk_native_get_surface( GTK_NATIVE(window) )) );
	Display *xd = gdk_x11_display_get_xdisplay( gtk_widget_get_display( GTK_WIDGET(window) ) );
	
	XGetWindowAttributes( xd, xw, &xwa );
	XTranslateCoordinates( xd, xwa.root, xw, 0, 0, &xwa.x, &xwa.y, &(Window){0} );
	*x = -xwa.x;
	*y = -xwa.y;
}


/*
*------------------------------------------------------------------------------
*	Events
*------------------------------------------------------------------------------
*/

/* On keypress */
static bool kyou_keypress(GtkEventControllerKey* ev, guint keyval, guint keycode, GdkModifierType state, gpointer user_data) {

	t_kyou_options* opt = (t_kyou_options*) user_data;
	GtkWidget* widget = gtk_event_controller_get_widget( GTK_EVENT_CONTROLLER(ev) );

#ifdef DEBUG
	printf("Aloha~ '%c' (0x%X) %d %04X\n", (keyval & 0xFF00) ? ' ' : keyval, keyval, keycode, state);
#endif

	switch ( keyval ) {

		case 'q':
		case 0xFF1B: /* Escape */
			gtk_window_close(GTK_WINDOW(widget));
			break;
		
		case ' ':
		case 'f': /* Toggle frame */
			opt->bDrawFrame = !opt->bDrawFrame;
			gtk_window_set_decorated(GTK_WINDOW(widget), opt->bDrawFrame);
			break;

		case 'm': /* Snap to bottom corner of current monitor */
			GdkRectangle geom = {0};
			gdk_monitor_get_geometry(
				gdk_display_get_monitor_at_surface(
					gdk_display_get_default(), 
					GDK_SURFACE(gtk_native_get_surface( GTK_NATIVE(widget) ))
				),
				&geom );
			kyou_window_move( GTK_WINDOW(widget), geom.x+geom.width-KYOU_WIN_WIDTH, geom.y+geom.height-KYOU_WIN_HEIGHT );
	}

	return true;
}

/* On click */
static bool kyou_clicked(GtkGestureClick* ev, gint n_press, gdouble x, gdouble y, gpointer user_data) {

	t_kyou_options* opt = (t_kyou_options*) user_data;
	GtkWidget* widget = gtk_event_controller_get_widget( GTK_EVENT_CONTROLLER(ev) );
	uint8 btn = gtk_gesture_single_get_current_button(GTK_GESTURE_SINGLE(ev));

#ifdef DEBUG
	printf( "Aloha~ click %d at %f %f (btn: %d)\n", n_press, x, y, btn );
#else
	printf( "Nfu~ !\n" );
#endif

	/* Middle click -> exit window */
	if ( btn == GDK_BUTTON_MIDDLE ) gtk_window_close(GTK_WINDOW(widget));

	/* Double-click -> toggle frame */
	if ( n_press > 1 ) {
		opt->bDrawFrame = !opt->bDrawFrame;
		gtk_window_set_decorated(GTK_WINDOW(widget), opt->bDrawFrame);
	}

	return true;
}

static bool kyou_mousemove(GtkGestureDrag* ev, gdouble x, gdouble y, gpointer user_data) {
	
	int16 px, py;
	GtkWidget* widget = gtk_event_controller_get_widget( GTK_EVENT_CONTROLLER(ev) );

	kyou_get_pos( GTK_WINDOW(widget), &px, &py);
#ifdef DEBUG
	printf("move (%f,%f)\n", x, y);
	printf("pos: (%d,%d)\n", px, py);
#endif
	kyou_window_move( GTK_WINDOW(widget), px+x, py+y );
	return true;
}

/* Move Kyoukai as soon as window is created */
static void kyou_on_display( GtkWidget* widget, gpointer user_data ) {

	GdkRectangle geom = {0};
	GdkDisplay* disp = gdk_display_get_default();
	GdkSurface* surf = GDK_SURFACE(gtk_native_get_surface( GTK_NATIVE(widget) ));
	GdkMonitor* mon = gdk_x11_display_get_primary_monitor( disp );

	gdk_monitor_get_geometry( mon, &geom );

	kyou_window_move( GTK_WINDOW(widget), geom.x+geom.width-KYOU_WIN_WIDTH, geom.y+geom.height-KYOU_WIN_HEIGHT );
}


/*
*------------------------------------------------------------------------------
*	ACTIVATE - Main GTK setup function
*------------------------------------------------------------------------------
*/
static void activate(GtkApplication *app, gpointer user_data) {

	GtkWidget *window;
	GtkWidget *kyou;
	GtkWidget *layout;
	GtkIconTheme* icon_theme;
	GtkEventController* ev_key;
	GtkGesture* ev_clic, * ev_move;

	t_kyou_options* opt = (t_kyou_options*) user_data;

	/* Load icon theme (kyoukai icon) */
	icon_theme = gtk_icon_theme_get_for_display(gdk_display_get_default());
	gtk_icon_theme_add_search_path(icon_theme, KYOU_ICON_PATH );

	if ( !gtk_icon_theme_has_icon(icon_theme, KYOU_ICON_NAME) ) {
		printf("WARN: Icon not found\n");
	}
	
	/* Set application icon */
	gtk_window_set_default_icon_name( KYOU_ICON_NAME);

	/* Create window */
	window = gtk_application_window_new(app);
	gtk_window_set_title(GTK_WINDOW(window), "Tiny Kyoukai !");
	gtk_window_set_default_size(GTK_WINDOW(window), KYOU_WIN_WIDTH, KYOU_WIN_HEIGHT);

	/* Load Kyoukai */
	kyou = gtk_picture_new_for_filename( opt->szKyouPath );
	gtk_picture_set_can_shrink(GTK_PICTURE(kyou), FALSE);
	gtk_picture_set_content_fit(GTK_PICTURE(kyou), GTK_CONTENT_FIT_CONTAIN);

	/* Setup event listeners */
	ev_key = gtk_event_controller_key_new();
	g_signal_connect(ev_key, "key-pressed", G_CALLBACK(kyou_keypress), opt);
	gtk_widget_add_controller(window, ev_key);
	ev_clic = gtk_gesture_click_new();
	gtk_gesture_single_set_button(GTK_GESTURE_SINGLE(ev_clic), AnyButton);
	g_signal_connect(ev_clic, "released", G_CALLBACK(kyou_clicked), opt);
	ev_move = gtk_gesture_drag_new();
	g_signal_connect(ev_move, "drag-update", G_CALLBACK(kyou_mousemove), opt);
	gtk_widget_add_controller(window, GTK_EVENT_CONTROLLER(ev_clic));
	gtk_widget_add_controller(window, GTK_EVENT_CONTROLLER(ev_move));

	/* Setup box layout aligned to bottom right, no padding */
	layout = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
	gtk_box_set_homogeneous(GTK_BOX(layout), FALSE);
	gtk_widget_set_valign(layout, GTK_ALIGN_END);
	gtk_widget_set_halign(layout, GTK_ALIGN_END);
	/* Add Kyoukai */
	gtk_box_append(GTK_BOX(layout), kyou);

	/* Make window background transparent */
	GtkCssProvider *provider = gtk_css_provider_new();
	char* css = "window {background: transparent;}";
	gtk_css_provider_load_from_string(provider, css);
	gtk_style_context_add_provider_for_display(gdk_display_get_default(),GTK_STYLE_PROVIDER(provider),GTK_STYLE_PROVIDER_PRIORITY_USER);

	/* Hide window frame and title bar */
	gtk_window_set_decorated(GTK_WINDOW(window), opt->bDrawFrame);

	/* Add everything to window */
	gtk_window_set_child(GTK_WINDOW(window), layout);

	/* Move Kyoukai as soon as the window is ready */
	g_signal_connect( window, "show", G_CALLBACK(kyou_on_display), opt);

	/* Display window */
	gtk_window_present(GTK_WINDOW(window));
}


/*
*------------------------------------------------------------------------------
*	MAIN - tiny-kyoukai - little Kyoukai peeks at the screen
*------------------------------------------------------------------------------
*/
int main(int argc, char **argv) {

	GtkApplication *app;
	int status;

	t_kyou_options options = {
		.szKyouPath = KYOU_PATH,
		.bDrawFrame = false
	};

	printf( "-----------------------\n" \
			"~\e[31m" TINYKYOU_NAME "\e[0m~ - " TINYKYOU_VERSION "\n"\
			"-----------------------\n" );
	printf( "~\e[31m羌瘣小ささ\e[0m!~\n" );

	app = gtk_application_new("dev.chenco.tiny-kyoukai",G_APPLICATION_DEFAULT_FLAGS);
	g_signal_connect(app, "activate", G_CALLBACK(activate), &options);
	status = g_application_run(G_APPLICATION(app), argc, argv);
	g_object_unref(app);

	printf( "\nO ancient darkness\n\t...primordial flame, what is it that lies\n\t...within your eyes?\nO bright light that" 	\
					" splits the Heavens,\n\trumbling tremors that shake the Earth, what pulse beats\n\t...within your ears?\nMan" 	\
					" walks adrift, a vessel of the Earth.\nI dance for thee, O God of Lightning!\nAh...Ryoku Sui..." 				\
					"\n\tLet blood boil over!\n" );

	return status;
}