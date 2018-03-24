bin_PROGRAMS = project
project_SOURCES = main.cpp
project_LDADD = libhelper.la

lib_LTLIBRARIES = libhelper.la
libhelper_la_SOURCES = game.cpp
libhelper_la_LDFLAGS = -version-info 0:0:0
AM_LDFLAGS = -lfcgi++ -lsqlite3 -lfcgi
ACLOCAL_AMFLAGS = -I m4
