# project definitions
AC_INIT([project], [1.0], [admin@noexec.org])

# automake definitions
AM_INIT_AUTOMAKE([-Wall -Werror foreign])

# source directory indicator
AC_CONFIG_SRCDIR([main.cpp])

# configuration header
AC_CONFIG_HEADERS([config.h])

# macro calls tracing
AC_CONFIG_MACRO_DIR([m4])

# check for programs
AC_PROG_CXX
AM_PROG_AR
#LT_INIT
AM_PROG_LIBTOOL

# check for library functions
AC_SEARCH_LIBS(sqlite3, fcgio)
AC_CHECK_FUNCS([FCGX_Init], [closeDB])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT

