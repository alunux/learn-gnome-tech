public class Main : WebKit.WebView
{
    public Main(WebKit.Settings settings) {
        set_settings(settings);
        load_uri("file:///home/alunux/mylabora/learn-vala/src/example.html");
    }

    static int main (string[] args) {
        Gtk.init(ref args);

        var context = WebKit.WebContext.get_default();
        context.initialize_web_extensions.connect((event) => {
            GLib.print("SIGNAL: initialize-web-extensions (webext) \n");
            context.set_web_extensions_directory(".");
            return;
        });

        var settings = new WebKit.Settings();
        settings.set_enable_javascript(true);
        var webview = new Main(settings);
        var win = new Gtk.Window();
        win.destroy.connect(Gtk.main_quit);
        win.add(webview);
        win.show_all();

        Gtk.main();

        return 0;
    }
}