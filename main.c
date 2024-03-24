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

typedef struct s_kyou_options {
	char* szKyouPath;
	bool  bAllowResize;
	bool  bDrawFrame;
} t_kyou_options;


/*
*------------------------------------------------------------------------------
*	Events
*------------------------------------------------------------------------------
*/

/* On keypress */
static bool kyou_keypress(GtkEventControllerKey* ev, guint keyval, guint keycode, GdkModifierType state, gpointer user_data) {
	t_kyou_options* opt = (t_kyou_options*) user_data;
	GtkWidget* widget = gtk_event_controller_get_widget( (GtkEventController*)ev );

#ifdef DEBUG
	printf("Aloha~ '%c' (0x%X) %d %04X\n", (keyval & 0xFF00) ? ' ' : keyval, keyval, keycode, state);
#endif

	switch ( keyval ) {

		case 'q':
		case 0xFF1B: /* Escape */
			gtk_window_close(GTK_WINDOW(widget));
			break;
		
		case ' ':
		case 'f':
			opt->bDrawFrame = !opt->bDrawFrame; /* Toggle frame */
			gtk_window_set_decorated(GTK_WINDOW(widget), opt->bDrawFrame);
			break;
	}

	return true;
}

/* On click */
static bool kyou_clicked(GtkGestureClick* ev, gint n_press, gdouble x, gdouble y, gpointer user_data) {
	t_kyou_options* opt = (t_kyou_options*) user_data;
	GtkWidget* widget = gtk_event_controller_get_widget( (GtkEventController*)ev );

#ifdef DEBUG
	printf("Aloha~ click %d at %f %f\n", n_press, x, y);
#else
	printf( "Nfu~ !\n" );
#endif

	/* Double-click -> toggle frame */
	if ( n_press > 1 ) {
		opt->bDrawFrame = !opt->bDrawFrame;
		gtk_window_set_decorated(GTK_WINDOW(widget), opt->bDrawFrame);
	}
	return true;
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
	GtkGesture* ev_clic;

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
	gtk_window_set_default_size(GTK_WINDOW(window), 250, 120);

	/* Load Kyoukai */
	kyou = gtk_picture_new_for_filename( opt->szKyouPath );
	gtk_picture_set_can_shrink(GTK_PICTURE(kyou), FALSE);
	gtk_picture_set_content_fit(GTK_PICTURE(kyou), GTK_CONTENT_FIT_CONTAIN);

	/* Setup event listeners */
	ev_key = gtk_event_controller_key_new();
	g_signal_connect(ev_key, "key-pressed", G_CALLBACK(kyou_keypress), opt);
	gtk_widget_add_controller(window, ev_key);
	ev_clic = gtk_gesture_click_new();
	g_signal_connect(ev_clic, "pressed", G_CALLBACK(kyou_clicked), opt);
	gtk_widget_add_controller(window, GTK_EVENT_CONTROLLER(ev_clic));

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