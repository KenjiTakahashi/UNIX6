#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
# for Solaris run: autoheader, aclocal-1.10, automake-1.10, autoconf
# for Others: autoreconf --install (it doesn't want to work on Solaris)

AC_PREREQ([2.60])
AC_INIT([EIS], [1.0], [NONE])
AM_INIT_AUTOMAKE([foreign -Wall -Werror])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.
AC_SEARCH_LIBS([stdscr], [curses], [], [AC_MSG_ERROR(curses library not found!)])
AC_SEARCH_LIBS([form_driver], [form], [], [AC_MSG_ERROR(form addon for curses not found!)])
AC_SEARCH_LIBS([__db_ndbm_open], [db])

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h stdlib.h string.h unistd.h curses.h form.h sys/types.h regex.h],
                 [], [AC_MSG_ERROR(essential header not found!)])
AC_CHECK_HEADERS([ndbm.h])
#ifndef HAVE_NDBM_H
AC_CHECK_HEADERS([db.h], [], [AC_MSG_ERROR(no standard db header found!)])
#endif

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_FUNC_REALLOC
AC_CHECK_FUNCS([regcomp strcspn])

AC_OUTPUT
