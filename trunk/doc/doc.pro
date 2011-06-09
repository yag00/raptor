# include config file
include( ../config.pri )

clean.commands = make -f SphinxMakefile clean
generate.commands = make -f SphinxMakefile raptorhelp

#done by Sphinx build
#collect.depends = generate
#collect.commands = qcollectiongenerator build/qthelp/Raptor.qhcp -o build/qthelp/Raptor.qhc

docdelivery.depends = generate
docdelivery.commands = $(COPY_DIR) build/qthelp/* $$PACKAGE_DESTDIR/doc

docs.depends = generate
docs.commands = @echo "Documentation built"

QMAKE_EXTRA_TARGETS += clean generate docdelivery docs

TARGET = docs

#do not work, $(TARGET) is not called ...
#QMAKE_POST_LINK += "@echo COPY_DIR build/qthelp $$PACKAGE_DESTDIR/doc"
