project('hasanta', 'cpp',
  version: '1.0.0',
  default_options: ['cpp_std=c++20']
)

# Find Fcitx5 via pkg-config
fcitx5_dep = dependency('Fcitx5Core')

# Define installation paths
fcitx5_libdir = get_option('prefix') / get_option('libdir') / 'fcitx5'
fcitx5_datadir = get_option('prefix') / get_option('datadir') / 'fcitx5'

# 1. Compile the plugin
# We use shared_module with an empty name_prefix so it outputs "hasanta.so"
# instead of "libhasanta.so" (which Fcitx5 would fail to load).
shared_module('hasanta',
  ['src/engine.cpp', 'src/parser.cpp'],
  dependencies: fcitx5_dep,
  name_prefix: '',
  install: true,
  install_dir: fcitx5_libdir
)

# 2. Inject version info and install the Addon metadata
conf = configuration_data()
conf.set('PROJECT_VERSION', meson.project_version())

configure_file(
  input: 'hasanta-addon.conf.in',
  output: 'hasanta.conf',
  configuration: conf,
  install_dir: fcitx5_datadir / 'addon'
)

# 3. Install the Input Method layout metadata
install_data('hasanta-im.conf',
  rename: 'hasanta.conf',
  install_dir: fcitx5_datadir / 'inputmethod'
)
