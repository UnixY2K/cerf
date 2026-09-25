libwin32coded_inc = include_directories('include')

libwin32coded_lib = static_library(
	'win32coded',
	'src/win_compat.cpp',
	include_directories: libwin32coded_inc,
)
libwin32coded_dep = declare_dependency(
	link_with: libwin32coded_lib,
	include_directories: libwin32coded_inc,
)
