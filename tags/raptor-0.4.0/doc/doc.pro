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

# Dummy, just to avoid qmake error because we're not compiling any application
TEMPLATE = subdirs

# include config file
include( ../config.pri )

#generate sphinx documentation
docs.depends =
docs.commands = $(MAKE) -f SphinxMakefile raptorhelp

#generate debug, release, and default target
debug.depends = docs
debug.commands = $(MAKE) -f SphinxMakefile raptorhelp_debug_delivery
release.depends = docs
release.commands = $(MAKE) -f SphinxMakefile raptorhelp_release_delivery
make_default.depends = docs debug release


TMPDIRS = "build/"
QMAKE_CLEAN += "-r $$TMPDIRS"
QMAKE_DISTCLEAN += -r $$TMPDIRS
QMAKE_EXTRA_TARGETS += debug release make_default docs

TARGET = docs

# install docs
raptor_doc.path	= $${PACKAGE_DATA}/doc
raptor_doc.files = $$PACKAGE_DESTDIR/doc/*
raptor_doc.CONFIG *= no_check_exist
INSTALLS += raptor_doc

