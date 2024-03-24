/**/

#include <gtk/gtk.h>



static void print_hello(GtkWidget *widget, gpointer data) {
  g_print("Hello World\n");
}

static void activate(GtkApplication *app, gpointer user_data) {
  GtkWidget *window;
  GtkWidget *button;

  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "Hello");
  gtk_window_set_default_size(GTK_WINDOW(window), 170, 50);

  GtkWidget* fixed = gtk_fixed_new();
  button = gtk_button_new_with_label("Hello World");
  gtk_button_set_can_shrink(button, TRUE);
  gtk_widget_set_size_request(button, 150, 30);
  g_signal_connect(button, "clicked", G_CALLBACK(print_hello), NULL);
  gtk_fixed_put(GTK_FIXED(fixed), button, 10, 10);
  gtk_window_set_child(GTK_WINDOW(window), fixed);

  gtk_window_present(GTK_WINDOW(window));
}

int main(int argc, char **argv) {
  GtkApplication *app;
  int status;

  app = gtk_application_new("dev.chenco.tiny-kyoukai", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
  status = g_application_run(G_APPLICATION(app), argc, argv);
  g_object_unref(app);

  return status;
}
