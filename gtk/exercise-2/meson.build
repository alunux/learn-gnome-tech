project('learn-gobject', 'c', default_options: 'c_std=c11')

gnome = import('gnome')
require_gobject = dependency('gobject-2.0')
require_gtk3 = dependency('gtk+-3.0')
require_glib = dependency('glib-2.0')
require_json = dependency('json-c')
require_libudev = dependency('libudev')

usbip_app_res = gnome.compile_resources(
  'usbip_app-res', 'usbip_app.gresource.xml',
  source_dir : '.',
  c_name : 'usbip_app'
)

am_cflags = [
    '-Wall',
    '-Wextra',
	'-g',
]

add_global_arguments(am_cflags, language: 'c')

deps = [
	require_gobject,
	require_gtk3,
	require_glib,
	require_json,
	require_libudev,
]

sources = [
	'main.c',
	'usb_get_list.c',
	'usb_desc.c',
	'usbip_app.c',
	'usbip_app_win.c',
	usbip_app_res,
]

executable(
	'main', sources,
	dependencies: deps,
	include_directories: include_directories('.'),
)