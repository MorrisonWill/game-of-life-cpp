project('cgol', 'cpp',
  version : '0.1',
  default_options : ['warning_level=0', 'cpp_std=c++20', 'buildtype=release'])

raylib = dependency('raylib', version : '4.2.0')
incdir = include_directories('include')

executable('cgol',
           'main.cpp',
           install : true,
           dependencies : raylib,
           include_directories : incdir)
