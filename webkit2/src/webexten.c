#include <stdio.h>
#include <glib.h>
#include <webkit2/webkit-web-extension.h>
#include <JavaScriptCore/JavaScript.h>
#include <unistd.h>


// Javascript can call C function.
// Losley copied and apadpted from: https://github.com/Aeva/webkit2gtk-experiment/blob/master/module.cpp


static void
web_page_created_callback(WebKitWebExtension *extension, WebKitWebPage *web_page,
                          gpointer user_data)
{
	g_print("Webext: page created signal halder from webextension\n");
	// Observation: This is called only once when webkit extension loads.
}


// Reached by calling "window.myCFunction();" in javascript console.
// Some basic c function to be exposed to the javascript environment
static JSValueRef javascript_function (JSContextRef context,
                               JSObjectRef function,
                               JSObjectRef thisObject,
                               size_t argumentCount,
                               const JSValueRef arguments[],
                               JSValueRef *exception) {
	g_print("C method was called by Javascript");
	JSStringRef string = JSStringCreateWithUTF8CString("Value returned from C");
    system("budgie-desktop-settings");
	return JSValueMakeString(context, string);
}

static void
window_object_cleared_callback (WebKitScriptWorld *world,
                                WebKitWebPage     *web_page,
                                WebKitFrame       *frame,
                                gpointer           user_data)
{
	g_print("Adding some JavaScript..\n");
	// Observation: This is called everytime a webpage is loaded.

    JSGlobalContextRef jsContext;
    JSObjectRef        globalObject;
    JSValueRef exception = 0;

    jsContext = webkit_frame_get_javascript_context_for_script_world (frame, world);
    globalObject = JSContextGetGlobalObject (jsContext);

    JSStringRef function_name = JSStringCreateWithUTF8CString("myCFunction");
    JSObjectRef jsFunction = JSObjectMakeFunctionWithCallback(jsContext, function_name, javascript_function);
    JSObjectSetProperty(jsContext, globalObject, function_name, jsFunction,
    		kJSPropertyAttributeDontDelete | kJSPropertyAttributeReadOnly, &exception);

    if (exception) {
    	g_print("OJSObjectSetProperty exception occurred");
    }
}

G_MODULE_EXPORT void
webkit_web_extension_initialize(WebKitWebExtension *extension)
{
	g_print("extension is being initialized. (Javascript core)\n");
    g_signal_connect(extension, "page-created",  G_CALLBACK(web_page_created_callback), NULL);


    g_signal_connect (webkit_script_world_get_default (),
                      "window-object-cleared",
                      G_CALLBACK (window_object_cleared_callback),
                      NULL);
}