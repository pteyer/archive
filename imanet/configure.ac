#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.65])
AC_INIT(imanet, 0.1, peyman AT cs.ucla.edu)
AC_CONFIG_SRCDIR(src/main.c)
AM_CONFIG_HEADER(src/config.h)

AM_INIT_AUTOMAKE

# Checks for programs.
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([arpa/inet.h fcntl.h netdb.h netinet/in.h stddef.h stdlib.h string.h sys/ioctl.h sys/socket.h termio.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T

# Checks for library functions.
AC_CHECK_FUNCS([atexit memset socket strerror])

AC_CONFIG_FILES([
Makefile
src/Makefile])

AC_OUTPUT
