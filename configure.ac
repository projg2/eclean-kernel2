AC_PREREQ([2.60])
AC_INIT([eclean-kernel], [1.99.1])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([build-aux])
AM_INIT_AUTOMAKE([1.6 foreign dist-bzip2 subdir-objects])

AC_LANG([C++])
AC_PROG_CXX
AC_SYS_LARGEFILE

AC_CHECK_FUNCS([fstatat lstat openat readlinkat unlinkat])
AC_STRUCT_DIRENT_D_TYPE

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
