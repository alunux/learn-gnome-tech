namespace WebExt {
    private const JSCore.StaticFunction[] js_funcs = {
        { "funcJS", funcJS_function, JSCore.PropertyAttribute.ReadOnly },
        { null, null, 0 }
    };
    
    private const JSCore.ClassDefinition js_class = {
        0,
        JSCore.ClassAttribute.None,
        "FuncJSClass",
        null,
        null,
        js_funcs,
        null,
        null,
        null,
        null,
        null,
        null,
        null,
        null,
        null,
        null,
        null
    };
    
    private static void setup_js_class(JSCore.GlobalContext context) {
        GLib.print("WebExt: Setup JS Class\n");
    
        var JSKlazz = new JSCore.Class(js_class);
        var JSObj = new JSCore.Object(context, JSKlazz, context);
        var JSGlobalCtx = context.get_global_object();
        var id = new JSCore.String.with_utf8_c_string("FuncJSClass");
        JSGlobalCtx.set_property(context, id, JSObj, JSCore.PropertyAttribute.None, null);
    }
    
    private static JSCore.Value funcJS_function(JSCore.Context ctx,
                                               JSCore.Object function,
                                               JSCore.Object thisObject,
                                               JSCore.Value[] arguments,
                                               out JSCore.Value exception) {
        GLib.print("WebExt: Setup JS Function\n");
    
        exception = null;
        var text = new JSCore.String.with_utf8_c_string("Hello from JSCore web-extension");
        return new JSCore.Value.string(ctx, text);
    }
}

public static void webkit_web_extension_initialize (WebKit.WebExtension web_extension) {
    web_extension.page_created.connect((extension, web_page) => {
        GLib.print("Webext: page created signal halder from web-extension\n");
        return;
    });

    WebKit.ScriptWorld.get_default().window_object_cleared.connect((page, frame) => {
        GLib.print("WebExt: Adding some JS\n");
        WebExt.setup_js_class((JSCore.GlobalContext) frame.get_javascript_global_context());
        return;
    });    
}