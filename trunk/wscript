#! /usr/bin/env python
# encoding: utf-8
#
# This file is part of the raptor project.
#
# Copyright (C) 2011 Christophe Duvernois <christophe.duvernois@gmail.com>
#
# Raptor is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Raptor is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
#

# building waf :
# python waf-light --make-waf --tools=slow_qt4,compat15
#

import sys
import os
import platform
from waflib import Build, Task, Options, Logs, Utils, Scripting
from waflib.Build import BuildContext
from waflib.Errors import ConfigurationError

VERSION='0.3.0'
APPNAME='raptor'
DESCRIPTION="Raptor A Portable Text editOR"

top = '.'
out = 'wbuild'
available_doc = ['en', 'fr']

class debug(BuildContext):
	cmd = 'debug'
	variant = 'debug'

class release(BuildContext):
	cmd = 'release'
	variant = 'release'

def dist(ctx):
	ctx.algo      = 'tar.gz'

def install(ctx):
	import waflib.Options
	waflib.Options.commands = 'install_release'

def uninstall(ctx):
	import waflib.Options
	waflib.Options.commands = 'uninstall_release'

Scripting.default_cmd = "release"

def options(opt):
	opt.load('compiler_c compiler_cxx qt4 slow_qt4')

def configure(conf):
	#min waf version
	conf.check_waf_version(mini='1.6.5')

	conf.setenv('debug')
	conf.load('compiler_c')
	conf.load('compiler_cxx')
	conf.load('qt4')
	conf.load('slow_qt4')
	#conf.find_program('assistant')
	#conf.find_program('assistant-qt4')
	#conf.find_program('qcollectiongenerator')
	conf.env.CFLAGS = get_debug_cflags()
	conf.env.CXXFLAGS = get_debug_cxxflags()

	conf.setenv('release', env=conf.env.derive())
	conf.env.CFLAGS = get_release_cflags()
	conf.env.CXXFLAGS = get_release_cxxflags()

	"""try:
		conf.find_program("sphinx-build", var="SPHINX_BUILD")
	except conf.errors.ConfigurationError:
		conf.fatal("Can't find Sphinx! Try 'easy_install -U Sphinx' or check http://sphinx.pocoo.org/")"""

	# summary
	Logs.pprint('BLUE', 'Summary:')
	conf.msg('Install Raptor ' + VERSION + ' in', conf.env['PREFIX'])

def prebuild(ctx):
	print('before the build is complete')

def postbuild(ctx):
	print('after the build is complete')

def build(bld):
	bld.add_pre_fun(prebuild)
	bld.add_post_fun(postbuild)

	#########################################################
	# build astyle
	#########################################################
	astyle_sources = bld.path.ant_glob(
		['ext/AStyle/astyle-2.01/src/*.cpp'],
		excl=['ext/AStyle/astyle-2.01/src/astyle_main.cpp'])

	bld.new_task_gen(
		features        = 'cxx cxxstlib',
		source          = astyle_sources,
		name            = 'astyle',
		target          = 'astyle',
		includes        = '.',
		cxxflags        = ['-Wall', '-Werror'],
		install_path    = None) # do not install this library


	#########################################################
	# build ctags
	#########################################################
	if isWindows():
		ctags_patterns = ['ext/ctags/ctags-5.8/*.c', 'ext/ctags/ctags-5.8/gnu_regex/regex.c']
	else:
		ctags_patterns = ['ext/ctags/ctags-5.8/*.c']

	ctags_sources = bld.path.ant_glob(ctags_patterns,
			excl=[	'ext/ctags/ctags-5.8/argproc.c',
					'ext/ctags/ctags-5.8/mac.c',
					'ext/ctags/ctags-5.8/main.c',
					'ext/ctags/ctags-5.8/qdos.c',
					'ext/ctags/ctags-5.8/sort.c']
				)

	bld.new_task_gen(
		features        = 'c cstlib',
		source          = ctags_sources,
		name            = 'ctags',
		target          = 'ctags',
		includes        = ['ext/ctags/ctags-5.8', 'ext/ctags/ctags-5.8/gnu_regex'],
		defines         = get_ctags_defines(),
		cflags          = [],	# todo build with -Wall -Werror
		install_path    = None) # do not install this library


	#########################################################
	# build qscintilla
	#########################################################
	qscintilla_sources = bld.path.ant_glob(
		[	'ext/QScintilla/QScintilla-gpl-2.5.1/Qt4/*.cpp',
			'ext/QScintilla/QScintilla-gpl-2.5.1/lexers/*.cpp',
			'ext/QScintilla/QScintilla-gpl-2.5.1/lexlib/*.cpp',
			'ext/QScintilla/QScintilla-gpl-2.5.1/src/*.cpp'],
		excl=[''])

	bld.new_task_gen(
		features        = 'qt4 cxx cxxstlib',
		uselib          = 'QTCORE QTGUI',
		source          = qscintilla_sources,
		name            = 'qscintilla2',
		target          = 'qscintilla2',
		includes        = ['ext/QScintilla/QScintilla-gpl-2.5.1/include', 
							'ext/QScintilla/QScintilla-gpl-2.5.1/lexlib', 
							'ext/QScintilla/QScintilla-gpl-2.5.1/src', 
							'ext/QScintilla/QScintilla-gpl-2.5.1/Qt4'],
		defines         = ['QSCINTILLA_MAKE_DLL', 'QT', 'SCI_LEXER', 'QT_THREAD_SUPPORT', 'QT_NO_DEBUG', 'QT_GUI_LIB', 'QT_CORE_LIB'],
		cxxflags        = [],	# todo build with -Wall -Werror
		install_path    = None) # do not install this library


	#########################################################
	# build raptor
	#########################################################
	raptor_sources = bld.path.ant_glob(
		[	'ext/qt-solutions/qtsingleapplication/src/qtsingleapplication.cpp',
			'ext/qt-solutions/qtsingleapplication/src/qtlocalpeer.cpp',
			'src/**/*.cpp',
			'src/**/*.ui',
			'src/**/*.qrc'],
		excl=['src/rc/*.cpp'])

	bld.new_task_gen(
        features        = 'qt4 cxx cxxprogram',
		uselib			= 'QTCORE QTGUI QTNETWORK QTHELP',
		use				= 'astyle qscintilla2 ctags',
        source          = raptor_sources,
        name            = 'raptor',
        target          = 'raptor',
        includes        = ['.',
							'ext/AStyle/astyle-2.01/src',
							'ext/ctags',
							'ext/QScintilla/QScintilla-gpl-2.5.1/Qt4',
							'ext/qt-solutions/qtsingleapplication/src',
							'src'],
		defines         = [	'UNICODE', 'HAVE_FGETPOS', 
							'QT_DLL', 'QT_NO_DEBUG', 'QT_SQL_LIB', 'QT_XML_LIB', 'QT_GUI_LIB', 'QT_NETWORK_LIB', 'QT_CORE_LIB', 'QT_THREAD_SUPPORT',
							'PACKAGE_NAME="%s"' % APPNAME.capitalize(),
							'PACKAGE_VERSION="%s"' % VERSION,
							'PACKAGE_DESCRIPTION="%s"' % DESCRIPTION,
							'PACKAGE_BIN="%s/bin"' % bld.env['PREFIX'],
							'PACKAGE_LIB="/usr/local/lib"',
							'PACKAGE_DATA="/usr/local/share"'],
		cxxflags        = ['-Wall', '-Werror'],
		linkflags       = get_raptor_ldflags(),
        install_path    = '${PREFIX}/bin')


	#bld.install_files('${PREFIX}/bin', ['raptor'], chmod=Utils.O755)

	#build doc
	"""for lang in available_doc:
		docBuildPath 	= os.path.join(out, "doctrees/%s" % lang)
		docSrcPath		= os.path.join(top, "doc/src/%s/source" % lang)
		docOut			= "raptorhelp/%s" % lang
		docOutPath		= os.path.join(out, docOut)
		bld(
			name	= 'doc_%s' % lang,
			color	='PINK',
			rule	= "${SPHINX_BUILD} -b raptorhelp -d %s %s %s" % (docBuildPath, docSrcPath, docOutPath),
			cwd		= bld.path.abspath()
		)
		bld(
			name	= 'qtdoc_%s' % lang,
			color	= 'PINK',
			rule	= "qcollectiongenerator %s -o %s" % (os.path.join(docOutPath, "Raptor.%s.qhcp" % lang), os.path.join(docOutPath, "Raptor.%s.qhc" % lang)),
			cwd		= bld.path.abspath(),
			after	= 'doc_%s' % lang,
			target	= [docOut + '/Raptor.%s.qhc' % lang, docOut + '/Raptor.%s.qch' % lang],
			install_path = '${PREFIX}/doc'
		)
	"""


def isWindows():
	if platform.system() == "Windows":
		return 1
	else:
		return 0

def isLinux():
	if platform.system() == "Linux":
		return 1
	else:
		return 0

def printOS():
	if isLinux():
		(distname,version,id) =  platform.linux_distribution()
		print(distname + ' ' + version + ' ' + id + ' - (' + platform.system() + ' ' + platform.release() + ')')
	else:
		print(platform.system() + ' ' + platform.release())

def get_debug_cflags():
	return ['-c', '-g']

def get_release_cflags():
	if isWindows():
		return ['-c', '-O2']
	else:
		return ['-c', '-pipe','-O2', '-fPIC']

def get_debug_cxxflags():
	if isWindows():
		return ['-c', '-g', '-frtti', '-fexceptions', '-mthreads']
	else:
		return ['-c', '-pipe', '-g', '-fPIC']

def get_release_cxxflags():
	if isWindows():
		return ['-c', '-O2', '-frtti', '-fexceptions', '-mthreads']
	else:
		return ['-c', '-pipe','-O2', '-fPIC']

def get_ctags_defines():
	if isWindows():
		return ['UNICODE', 'HAVE_REGCOMP', 'WIN32', 'REGEX_MALLOC', 'STDC_HEADERS=1', '__USE_GNU', 'HAVE_STDBOOL_H']
	else:
		return ['HAVE_REGCOMP', 'HAVE_STDLIB_H', 'HAVE_FGETPOS', 'HAVE_SYS_STAT_H', 'HAVE_FCNTL_H', 'HAVE_REGEX', 'HAVE_UNISTD_H', 'HAVE_STRSTR', 'HAVE_MKSTEMP']

def get_raptor_ldflags():
	if isWindows():
		return ['-Wl,-s', '-mthreads', '-Wl,-subsystem,windows']
	else:
		return ['-Wl,-O1']
	
from waflib.Build import BuildContext, CleanContext, InstallContext, UninstallContext

for x in 'debug release'.split():
	for y in (BuildContext, CleanContext, InstallContext, UninstallContext):
		name = y.__name__.replace('Context','').lower()
		class tmp(y):
			cmd = name + '_' + x
			variant = x

# use the following if you want to add the include paths automatically
from waflib.TaskGen import feature, before, after
@feature('cxx')
@after('process_source')
@before('apply_incpaths')
def add_includes_paths(self):
    incs = set(self.to_list(getattr(self, 'includes', '')))
    for x in self.compiled_tasks:
        incs.add(x.inputs[0].parent.path_from(self.path))
    self.includes = list(incs)


