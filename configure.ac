#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

#AC_PREREQ([2.69])
AC_INIT([kstack], [1.0], [cpu12.gems@googlemail.com])
AM_INIT_AUTOMAKE([subdir-objects foreign]) #(silent-rules)
AC_CONFIG_SRCDIR([./src/])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC	# ([clang gcc])
AC_PROG_INSTALL
#AC_PROG_AWK
#AC_PROG_LEX

#AC_CHECK_PROG(MY_FOO, kphoo, STD_ON, STD_OFF, , [/usr/sbin/oofoo])

# Checks for libraries.
AC_ARG_ENABLE([async-exec],
	[AS_HELP_STRING([--disable-async-exec],
	[disable asynchronous execution @<:@default: no@:>@])],
	[async_exec=${enableval}], [async_exec=yes]
)
    
have_pthreads=no
                
if test "x${async_exec}" = xyes; then
	have_pthreads=no
        AC_SEARCH_LIBS([pthread_create], [pthread], [have_pthreads=yes])
        if test "x${have_pthreads}" = xyes; then
        	AC_CHECK_HEADERS([pthread.h], [], [have_pthreads=no])
        fi
        if test "x${have_pthreads}" = xno; then
        	AC_MSG_WARN([
                -----------------------------------------
                Unable to find pthreads on this system.
                Building a single-threaded version.
                -----------------------------------------])
                async_exec=no
	fi
fi

if test "x${async_exec}" = xyes; then
	AC_DEFINE([ASYNC_EXEC], 1, [async execution enabled])
fi       

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_UINT16_T
 

# Checks for library functions.

AC_CONFIG_FILES([Makefile
                 ])
AC_OUTPUT

echo \
"-------------------------------------------------
${PACKAGE_NAME} Version ${PACKAGE_VERSION}

Prefix: '${prefix}'.
Compiler: '${CC} ${CFLAGS} ${CPPFLAGS}'

Package features:
	Async Execution: ${async_exec}
	
Now type 'make @<:@<target>@:>@'
where the optional <target> is:
	all                - build all binaries
	install            - install everything
--------------------------------------------------"