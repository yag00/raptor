# include config file
include( ../config.pri )

#generate sphinx documentation
docs.depends = 
docs.commands = $(MKDIR) $$PACKAGE_DESTDIR/doc; \ 
				make -f SphinxMakefile raptorhelp DELIVERY_DIR=$$PACKAGE_DESTDIR; \
				$(COPY_DIR) build/raptorhelp/**/*.qhc $$PACKAGE_DESTDIR/doc/; \
				$(COPY_DIR) build/raptorhelp/**/*.qch $$PACKAGE_DESTDIR/doc/;

QMAKE_CLEAN += -r build
QMAKE_EXTRA_TARGETS += docs

TARGET = docs
