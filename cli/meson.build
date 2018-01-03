project('learn-gobject', 'c', default_options: 'c_std=c11')

require_glib = dependency('gobject-2.0')

am_cflags = [
    '-Wall',
    '-Wextra',
]

add_global_arguments(am_cflags, language: 'c')

sources = [
	'usb-desc.c',
	'main.c',
]

executable(
		'main', sources,
		dependencies: require_glib,
		include_directories: include_directories('.'),
)