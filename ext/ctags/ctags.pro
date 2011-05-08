# include config file
include( ../../config.pri )

# For correctly include ctags headers
DEFINES *= HAVE_REGCOMP 
win32:DEFINES	*= WIN32 REGEX_MALLOC STDC_HEADERS=1 __USE_GNU HAVE_STDBOOL_H
unix:DEFINES	*= HAVE_STDLIB_H \
	HAVE_FGETPOS \
	HAVE_SYS_STAT_H \
	HAVE_FCNTL_H \
	HAVE_REGEX \
	HAVE_UNISTD_H \
	HAVE_STRSTR \
	HAVE_MKSTEMP
	
include( ctags.pri )

TEMPLATE = lib
TARGET = ctags

CONFIG  += debug_and_release
CONFIG	+= staticlib
CONFIG	-= qt

DESTDIR	= $${PACKAGE_BUILD_PATH}/lib

INCLUDEPATH 		*= $$CTAGS_VERSION 
win32:INCLUDEPATH	*= $$CTAGS_VERSION\gnu_regex

HEADERS	= $${CTAGS_VERSION}/ctags.h \
	$${CTAGS_VERSION}/debug.h \
	$${CTAGS_VERSION}/e_amiga.h \
	$${CTAGS_VERSION}/e_djgpp.h \
	$${CTAGS_VERSION}/e_mac.h \
	$${CTAGS_VERSION}/e_msoft.h \
	$${CTAGS_VERSION}/entry.h \
	$${CTAGS_VERSION}/e_os2.h \
	$${CTAGS_VERSION}/e_qdos.h \
	$${CTAGS_VERSION}/e_riscos.h \
	$${CTAGS_VERSION}/e_vms.h \
	$${CTAGS_VERSION}/general.h \
	$${CTAGS_VERSION}/get.h \
	$${CTAGS_VERSION}/keyword.h \
	#$${CTAGS_VERSION}/main.h \
	$${CTAGS_VERSION}/options.h \
	$${CTAGS_VERSION}/parse.h \
	$${CTAGS_VERSION}/parsers.h \
	$${CTAGS_VERSION}/read.h \
	$${CTAGS_VERSION}/readtags.h \
	$${CTAGS_VERSION}/routines.h \
	$${CTAGS_VERSION}/sort.h \
	$${CTAGS_VERSION}/strlist.h \
	$${CTAGS_VERSION}/vstring.h \
	$${CTAGS_VERSION}/LibCtags.h

SOURCES	= $${CTAGS_VERSION}/ant.c \
	#$${CTAGS_VERSION}/argproc.c \
	$${CTAGS_VERSION}/args.c \
	$${CTAGS_VERSION}/args.h \
	$${CTAGS_VERSION}/asm.c \
	$${CTAGS_VERSION}/asp.c \
	$${CTAGS_VERSION}/awk.c \
	$${CTAGS_VERSION}/basic.c \
	$${CTAGS_VERSION}/beta.c \
	$${CTAGS_VERSION}/c.c \
	$${CTAGS_VERSION}/cobol.c \
	$${CTAGS_VERSION}/debug.c \
	$${CTAGS_VERSION}/dosbatch.c \
	$${CTAGS_VERSION}/eiffel.c \
	$${CTAGS_VERSION}/entry.c \
	$${CTAGS_VERSION}/erlang.c \
	$${CTAGS_VERSION}/flex.c \
	$${CTAGS_VERSION}/fortran.c \
	$${CTAGS_VERSION}/get.c \
	$${CTAGS_VERSION}/html.c \
	$${CTAGS_VERSION}/jscript.c \
	$${CTAGS_VERSION}/keyword.c \
	$${CTAGS_VERSION}/lisp.c \
	$${CTAGS_VERSION}/lregex.c \
	$${CTAGS_VERSION}/lua.c \
	#$${CTAGS_VERSION}/mac.c \
	#$${CTAGS_VERSION}/main.c \
	$${CTAGS_VERSION}/make.c \
	$${CTAGS_VERSION}/matlab.c \
	$${CTAGS_VERSION}/ocaml.c \
	$${CTAGS_VERSION}/options.c \
	$${CTAGS_VERSION}/parse.c \
	$${CTAGS_VERSION}/pascal.c \
	$${CTAGS_VERSION}/perl.c \
	$${CTAGS_VERSION}/php.c \
	$${CTAGS_VERSION}/python.c \
	#$${CTAGS_VERSION}/qdos.c \
	$${CTAGS_VERSION}/read.c \
	$${CTAGS_VERSION}/readtags.c \
	$${CTAGS_VERSION}/rexx.c \
	$${CTAGS_VERSION}/routines.c \
	$${CTAGS_VERSION}/ruby.c \
	$${CTAGS_VERSION}/scheme.c \
	$${CTAGS_VERSION}/sh.c \
	$${CTAGS_VERSION}/slang.c \
	$${CTAGS_VERSION}/sml.c \
	#$${CTAGS_VERSION}/sort.c \
	$${CTAGS_VERSION}/sql.c \
	$${CTAGS_VERSION}/strlist.c \
	$${CTAGS_VERSION}/tcl.c \
	$${CTAGS_VERSION}/tex.c \
	$${CTAGS_VERSION}/verilog.c \
	$${CTAGS_VERSION}/vhdl.c \
	$${CTAGS_VERSION}/vim.c \
	$${CTAGS_VERSION}/vstring.c \
	$${CTAGS_VERSION}/yacc.c \
	$${CTAGS_VERSION}/LibCtags.c

	
win32:DEFINES *= __USE_GNU HAVE_STDBOOL_H
win32:SOURCES	*= \
		$${CTAGS_VERSION}/gnu_regex/regex.c \
		$${CTAGS_VERSION}/gnu_regex/regcomp.c \
		$${CTAGS_VERSION}/gnu_regex/regexec.c \
		$${CTAGS_VERSION}/gnu_regex/regex_internal.c

win32:HEADERS	*= \
		$${CTAGS_VERSION}/gnu_regex/regex.h \
		$${CTAGS_VERSION}/gnu_regex/regex_internal.h
