project('cgol', 'cpp',
  version : '0.1',
  default_options : ['warning_level=3', 'cpp_std=c++20', 'buildtype=release'])

raylib = dependency('raylib', version : '4.2.0')

executable('cgol',
           'main.cpp',
           install : true,
           dependencies : raylib)
