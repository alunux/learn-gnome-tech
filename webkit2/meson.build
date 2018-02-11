project('learn-vala', 'vala', 'c')

gnome = import('gnome')

vapi = join_paths(meson.source_root(), 'vapi')

# webext = library(
#     'webext', 'src/webexten.c',
#     dependencies: [
#         dependency('glib-2.0'),
#         dependency('webkit2gtk-web-extension-4.0'),
#     ],
#     install: true,
# )

vapidir = join_paths(meson.source_root(), 'vapi')

webext = library(
    'webext', 'src/webexten.vala',
    vala_args: [
        '--vapidir', vapidir,
        '--pkg', 'javascriptcore',
        '--library', 'webkit_webextension_02',
    ],
    dependencies: [
        dependency('glib-2.0'),
        dependency('webkit2gtk-web-extension-4.0'),
    ],
    install: true,
)

executable(
    meson.project_name(),
    'src/main.vala',
    dependencies: [
        dependency('glib-2.0'),
        dependency('gtk+-3.0'),
        dependency('webkit2gtk-4.0'),
        dependency('libsoup-2.4'),
    ],
    install: true
)