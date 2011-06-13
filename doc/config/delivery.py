# -*- coding: utf-8 -*-
#
# Raptor documentation delivery script
# Copy all the generated qch,qhc file to the delivery dir

import sys, os, shutil

def copyDoc(srcdir, destdir):
    for basename in os.listdir(srcdir):
        pathname = os.path.abspath(os.path.join(srcdir, basename))

        if os.path.isdir(pathname):
            copyDoc(pathname, destdir)
        else:
            if basename.endswith('.qch') or basename.endswith('.qhc'):
                print "Copy " + pathname + " to " + os.path.abspath(destdir)
                shutil.copy(pathname, os.path.abspath(destdir))


destdir = os.path.abspath(sys.argv[2])
if not os.path.exists(destdir):
    print "make directory " + destdir
    os.mkdir(os.path.abspath(destdir)); 
print "Copy *.qhc, *.qch from " + sys.argv[1] + " to " + sys.argv[2]
copyDoc(sys.argv[1], sys.argv[2])
