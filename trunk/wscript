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

import sys, os, platform, shutil, tarfile, re
from waflib import Build, Task, Options, Logs, Utils, Scripting
from waflib.Build import BuildContext, CleanContext, InstallContext, UninstallContext
from waflib.Errors import ConfigurationError

VERSION='0.5.0'
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

class install(InstallContext):
	cmd = 'install'
	variant = 'release'

class uninstall(UninstallContext):
	cmd = 'uninstall'
	variant = 'release'

def dist(ctx):
	"makes a tarball for redistributing the sources"
	ctx.excl = '**/build **/*~ **/wbuild* **/delivery* **/.*'
	if isWindows():
		ctx.algo      = 'zip'
	else:
		ctx.base_name = 'raptor-editor-' + VERSION
		ctx.algo      = 'tar.gz'

def install(ctx):
	waflib.Options.commands = 'install_release'

def uninstall(ctx):
	waflib.Options.commands = 'uninstall_release'

Scripting.default_cmd = "release"

def options(opt):
	opt.load('compiler_c compiler_cxx qt4 slow_qt4')
	opt.add_option('--deb-src', action='store_true', default=False, help = '(ubuntu) package target will only build the source package', dest = 'debSrcOnly')
	opt.add_option('--deb-dist', action='store', default="", help = '(ubuntu) package target will only build the source package for the specified ubuntu (ex: --deb-dist=oneiric)', dest = 'debDist')
	opt.add_option('--deb-version', action='store', default="", help = '(ubuntu) package target will only build the source package with special version (ex: --deb-version=1)', dest = 'debVersion')

def distclean(ctx):
	Scripting.distclean(ctx)
	lst = os.listdir('.')
	for f in lst:
		if f.startswith('package.') and not Options.commands:
			shutil.rmtree(f, ignore_errors=True)

def configure(conf):
	#min waf version
	conf.check_waf_version(mini='1.6.8')

	conf.load('compiler_c')
	conf.load('compiler_cxx')

	if isWindows():
		conf.check_tool('winres')

	configurePackage(conf)
	configureDoc(conf);
		
	conf.load('qt4')
	conf.load('slow_qt4')

	#check qt version
	QT_MIN_VERSION = '4.6.0'
	qtversion = (conf.cmd_and_log([conf.env.QMAKE, '-query', 'QT_VERSION']).strip())
	if(qtversion.split('.') < QT_MIN_VERSION.split('.')):
		conf.msg("Checking for Qt version", "Qt should be at least in version " + QT_MIN_VERSION + " (" +qtversion + " found)", "RED")
		conf.fatal("Upgrade Qt !")
	else:
		conf.msg("Checking for Qt version", "" + qtversion)

	conf.setenv('debug', env=conf.env.derive())
	conf.env.CFLAGS = get_debug_cflags()
	conf.env.CXXFLAGS = get_debug_cxxflags()

	conf.setenv('release', env=conf.env.derive())
	conf.env.CFLAGS = get_release_cflags()
	conf.env.CXXFLAGS = get_release_cxxflags()

	# summary
	Logs.pprint('BLUE', 'Summary:')
	conf.msg('Install Raptor ' + VERSION + ' in', conf.env['PREFIX'])


def configureDoc(conf):
	try:
		conf.find_program("sphinx-build", var="SPHINX_BUILD")
	except conf.errors.ConfigurationError:
		Logs.pprint('YELLOW', "WARNING : Can't find Sphinx! Documentation generation is disabled!")
		Logs.pprint('YELLOW', "WARNING : To install sphinx, try 'easy_install -U Sphinx' or check http://sphinx.pocoo.org/")
	try:
		conf.find_program("qcollectiongenerator", var="QCOLLECTIONGENERATOR")
	except conf.errors.ConfigurationError:
		Logs.pprint('YELLOW', "WARNING : Can't find qcollectiongenerator! Documentation generation is disabled!")
		Logs.pprint('YELLOW', "WARNING : Check your qt installation!")

def configurePackage(conf):
	if isWindows():
		try:
			conf.find_program("ISCC", var="ISCC")
		except conf.errors.ConfigurationError:
			pass
	else:
		(distname,version,id) =  platform.linux_distribution()
		if distname == "Ubuntu":
			try:
				conf.find_program("debuild", var="DEBUILD")
			except conf.errors.ConfigurationError:
				pass
			try:
				conf.find_program("dpkg-buildpackage", var="DPKG_BUILDPACKAGE")
			except conf.errors.ConfigurationError:
				pass
		elif distname == "Fedora":
			try:
				conf.find_program("rpmbuild", var="RPMBUILD")
			except conf.errors.ConfigurationError:
				pass

def build(bld):
	if ((not bld.variant) and (bld.cmd == 'build')):
		Options.commands = ['release'] + Options.commands
		return

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
		cflags          = ['-Wformat=0'],	# todo build with -Wall -Werror
		install_path    = None) # do not install this library


	#########################################################
	# build qscintilla
	#########################################################
	qscintilla_sources = bld.path.ant_glob(
		[	'ext/QScintilla/QScintilla-gpl-2.6/Qt4/*.cpp',
			'ext/QScintilla/QScintilla-gpl-2.6/lexers/*.cpp',
			'ext/QScintilla/QScintilla-gpl-2.6/lexlib/*.cpp',
			'ext/QScintilla/QScintilla-gpl-2.6/src/*.cpp'],
		excl=[''])

	bld.new_task_gen(
		features        = 'qt4 cxx cxxstlib',
		uselib          = 'QTCORE QTGUI',
		source          = qscintilla_sources,
		name            = 'qscintilla2',
		target          = 'qscintilla2',
		includes        = ['ext/QScintilla/QScintilla-gpl-2.6/include',
							'ext/QScintilla/QScintilla-gpl-2.6/lexlib',
							'ext/QScintilla/QScintilla-gpl-2.6/src',
							'ext/QScintilla/QScintilla-gpl-2.6/Qt4'],
		defines         = ['QT', 'SCI_LEXER', 'QT_THREAD_SUPPORT', 'QT_NO_DEBUG'],
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

	if isWindows():
		raptor_sources += bld.path.ant_glob('src/**/*.rc')

	bld.new_task_gen(
		features        = 'qt4 cxx cxxprogram' + (' winrc' if isWindows() else ''),
		uselib          = 'QTCORE QTGUI QTNETWORK QTXML QTHELP',
		use             = 'astyle qscintilla2 ctags',
		source          = raptor_sources,
		lang            = bld.path.ant_glob('src/translations/*.ts'),
		name            = 'raptor',
		target          = 'raptor',
		includes        = ['.',
							'ext/AStyle/astyle-2.01/src',
							'ext/ctags',
							'ext/QScintilla/QScintilla-gpl-2.6/Qt4',
							'ext/qt-solutions/qtsingleapplication/src',
							'ext/dtl/dtl-1.15',
							'src'],
		defines         = [	'UNICODE', 'HAVE_FGETPOS', 'QT_NO_DEBUG','QT_THREAD_SUPPORT',
							'PACKAGE_NAME="%s"' % APPNAME.capitalize(),
							'PACKAGE_VERSION="%s"' % VERSION,
							'PACKAGE_DESCRIPTION="%s"' % DESCRIPTION,
							'PACKAGE_BIN="%s/bin"' % bld.env['PREFIX'],
							'PACKAGE_LIB="%s/lib"' % bld.env['PREFIX'],
							'PACKAGE_DATA="%s/share"' % bld.env['PREFIX'],
							'PACKAGE_OS="%s"' % getSystemOsString()],
		cxxflags        = ['-Wall', '-Werror'],
		linkflags       = (['-Wl,-s', '-mthreads', '-Wl,-subsystem,windows'] if isWindows() else ['-Wl,-O1']),
		install_path    = getBinaryInstallationPath(bld.env['PREFIX']))

	#install translations file
	bld.install_files(getTranslationInstallationPath(bld.env['PREFIX']), bld.bldnode.ant_glob('**/*.qm'))

	if isWindows():
		#install qt dll
		qtbin = (bld.cmd_and_log([bld.env.QMAKE, '-query', 'QT_INSTALL_BINS'], quiet=True).strip())
		qtdlls = bld.root.find_node(qtbin).ant_glob(['QtCore4.dll', 'QtGui4.dll', 'QtNetwork4.dll', 'QtXml4.dll', 'QtHelp4.dll', 'mingwm10.dll', 'libgcc_s_dw2-1.dll'])
		bld.install_files(bld.env['PREFIX'], qtdlls)


	#########################################################
	# build raptor documentation
	#########################################################
	"""if (bld.env.SPHINX_BUILD != []) and (bld.env.QCOLLECTIONGENERATOR != []):
		for lang in available_doc:
			sphinxlog		= os.path.join(bld.bldnode.abspath(), "sphinx_%s.log" % lang)
			qcglog			= os.path.join(bld.bldnode.abspath(), "qcglog_%s.log" % lang)
			docBuildPath 	= os.path.join(bld.bldnode.abspath(), "doctrees/%s" % lang)
			docOutPath		= os.path.join(bld.bldnode.abspath(), "raptorhelp/%s" % lang)
			docSrcPath		= os.path.join(bld.srcnode.abspath(), "doc/src/%s/source" % lang)
			
			rstsource = bld.path.ant_glob(['doc/config/**/*.py', 'doc/src/%s/**/*.rst' % lang, 'doc/src/%s/source/conf.py' % lang])
			
			qhcpNode = bld.path.find_or_declare('raptorhelp/%s/Raptor.%s.qhcp' % (lang, lang)) 
			qhpNode = bld.path.find_or_declare('raptorhelp/%s/Raptor.%s.qhp' % (lang, lang))
			qchNode = bld.path.find_or_declare('raptorhelp/%s/Raptor.%s.qch' % (lang, lang))
			qhcNode = bld.path.find_or_declare('raptorhelp/%s/Raptor.%s.qhc' % (lang, lang))
			
			bld(
				name	= 'doc_%s' % lang,
				color	='PINK',
				rule	= "${SPHINX_BUILD} -b raptorhelp -d %s %s %s > %s" % (docBuildPath, docSrcPath, docOutPath, sphinxlog),
				source	= rstsource,
				target	= [qhcpNode, qhpNode],
			)
			bld(
				name	= 'qtdoc_%s' % lang,
				color	= 'PINK',
				rule	= "${QCOLLECTIONGENERATOR} %s -o %s > %s" % (qhcpNode.abspath(), qhcNode.abspath(), qcglog),
				cwd		= bld.path.abspath(),
				after	= 'doc_%s' % lang,
				source	= [qhcpNode, qhpNode],
				target	= [qchNode, qhcNode],
				install_path = '${PREFIX}/doc',
			)
	"""

#########################################################
# Packaging
#########################################################

class package(BuildContext):
	cmd = 'package'
	fun = 'package'
class packageWindows(BuildContext):
	cmd = 'packageWindows'
	fun = 'packageWindows'
class packageUbuntu(BuildContext):
	cmd = 'packageUbuntu'
	fun = 'packageUbuntu'
class packageFedora(BuildContext):
	cmd = 'packageFedora'
	fun = 'packageFedora'

def package(ctx):
	"create package for your system"
	if isWindows():
		Options.commands = ['clean', 'release','install', 'packageWindows', 'uninstall'] + Options.commands
	else:
		(distname,version,id) =  platform.linux_distribution()
		if distname == "Ubuntu":
			Options.commands = ['clean', 'dist', 'packageUbuntu'] + Options.commands
		elif distname == "Fedora":
			Options.commands = ['clean', 'dist', 'packageFedora'] + Options.commands
		else:
			ctx.fatal('no packaging available for ' + distname + ' ' + version + ' ' + id)

def packageWindows(ctx):
	if ctx.env.ISCC != []:
		command = '"' + ctx.env.ISCC + '"'
		command += ' "/dAppVersion=' + VERSION + '"'
		command += ' "/dAppSrcDir=' + ctx.path.abspath() + '"'
		command += ' "/dAppInstallDir=' + ctx.env.PREFIX + '"'
		command += ' "/dOutputDir=' + os.path.abspath("package.windows") + '"'
		command += ' package/windows/package.iss'
		print(command)
		ctx.exec_command(command)
	else:
		ctx.fatal("InnoSetup compiler is not available. Run configure")

def packageUbuntu(ctx):
	if (ctx.env.DEBUILD != []) and (ctx.env.DPKG_BUILDPACKAGE != []):
		#setup deb build root tree
		try:
			os.mkdir(os.path.abspath("debbuild"))
		except:
			pass
		#extract tar
		tar = tarfile.open("raptor-editor-" + VERSION + ".tar.gz")
		tar.extractall()
		tar.close()
		os.remove(os.path.abspath("raptor-editor-" + VERSION + ".tar.gz"))
		shutil.move(os.path.abspath("raptor-editor-" + VERSION), os.path.abspath("debbuild/"));
		#copy debian directory
		shutil.copytree(os.path.abspath("package/ubuntu/debian"), os.path.abspath("debbuild/raptor-editor-" + VERSION + "/debian"), ignore=shutil.ignore_patterns('.svn'))

		#update changelog
		fchangelog = open(os.path.abspath("debbuild/raptor-editor-" + VERSION + "/debian/changelog"), "r")
		changelog = fchangelog.read()
		fchangelog.close()

		# the dictionary has target_word:replacement_word pairs
		from email.Utils import formatdate
		(distname,version,id) =  platform.linux_distribution()
		
		if ctx.options.debDist:
			id = ctx.options.debDist
		RAPTOR_VERSION_DEB = VERSION
		if ctx.options.debVersion:
			RAPTOR_VERSION_DEB += '.' + ctx.options.debVersion
		
		word_dic = {
			'${RAPTOR_VERSION_DEB}': RAPTOR_VERSION_DEB,
			'${DEB_DIST}': id,
			'${RAPTOR_VERSION_DISPLAY}': VERSION,
			'${DEB_DATE}': formatdate(localtime=True),
		}

		changelog = replace_words(changelog, word_dic)
		# write updated changelog
		fchangelog = open(os.path.abspath("debbuild/raptor-editor-" + VERSION + "/debian/changelog"), "w")
		fchangelog.write(changelog)
		fchangelog.close()

		#command
		os.chdir("debbuild/raptor-editor-" + VERSION)
		command = ctx.env.DEBUILD + ' '
		command += "-S -sa"
		ctx.exec_command(command)

		if ctx.options.debSrcOnly == False and ctx.options.debDist == ""  and ctx.options.debVersion == "":
			command = ctx.env.DPKG_BUILDPACKAGE + ' '
			command += "-rfakeroot"
			ctx.exec_command(command)
	
		os.chdir("../../")

		os.rename("debbuild", "package.ubuntu")

	else:
		ctx.fatal("debbuild/dpkg-buildpackage are not available.\nYou can install it with 'sudo apt-get install devscripts'.\nThen run configure.")

def packageFedora(ctx):
	if ctx.env.RPMBUILD != []:
		#setup rpm build root tree
		rpmSetupTree()
		#copy spec file
		shutil.copy(os.path.abspath("package/fedora/raptor-editor.spec"), os.path.abspath("rpmbuild/SPECS"))
		#copy source archive
		try:
			os.remove(os.path.abspath("rpmbuild/SOURCES/raptor-editor-" + VERSION + ".tar.gz"))
		except:
			pass
		shutil.move(os.path.abspath("raptor-editor-" + VERSION + ".tar.gz"), os.path.abspath("rpmbuild/SOURCES/"))

		#command
		command = ctx.env.RPMBUILD + ' '
		command += "--define '_topdir "
		command += os.path.abspath("rpmbuild") + "' "
		command += '-ba '
		command += ' ' + os.path.abspath("rpmbuild/SPECS/raptor-editor.spec")
		ctx.exec_command(command)

		#get rpms/srpms
		shutil.move(os.path.abspath("rpmbuild/RPMS"), os.path.abspath("package.fedora"))
		shutil.move(os.path.abspath("rpmbuild/SRPMS"), os.path.abspath("package.fedora"))

		#clean rpmbuild directory
		rpmCleanupTree()

	else:
		ctx.fatal("rpmbuild is not available.\nYou can install it with 'yum install rpmdevtools'.\nThen run configure.")

def rpmSetupTree():
	#setup rpm build root tree
	try:
		os.mkdir(os.path.abspath("rpmbuild"))
		os.mkdir(os.path.abspath("rpmbuild/BUILD"))
		os.mkdir(os.path.abspath("rpmbuild/RPMS"))
		os.mkdir(os.path.abspath("rpmbuild/SOURCES"))
		os.mkdir(os.path.abspath("rpmbuild/SPECS"))
		os.mkdir(os.path.abspath("rpmbuild/SRPMS"))
	except:
		pass

def rpmCleanupTree():
	#clea rpm build root tree
	try:
		shutil.rmtree(os.path.abspath("rpmbuild"))
	except:
		pass

#########################################################
# system & compilation utils function
#########################################################

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

def getSystemOsString():
	if isLinux():
		(distname,version,id) =  platform.linux_distribution()
		return (platform.system() + ' ' + distname + ' ' + version + ' ' + id)
	else:
		return (platform.system() + ' ' + platform.release())

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

def getBinaryInstallationPath(prefix):
	if isWindows():
		return prefix
	else:
		return prefix + '/bin'

def getTranslationInstallationPath(prefix):
	if isWindows():
		return prefix + '/translations'
	else:
		return prefix + '/share/raptor/translations'


def replace_words(text, word_dic):
	"""
	take a text and replace words that match a key in a dictionary with
	the associated value, return the changed text
	"""
	rc = re.compile('|'.join(map(re.escape, word_dic)))
	def translate(match):
		return word_dic[match.group(0)]
	return rc.sub(translate, text)

# create debug and release target
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


