# -*- coding: utf-8 -*-
#
# Raptor documentation build configuration file
#patch qhcp file

import sys, os

def patch_qhcp(filename):
	qhcp = ""
	file = open(filename, 'r')
	for line in file:
		if line.strip() == "<assistant>":
			qhcp += line
			qhcp += "        <applicationIcon>_static/raptor.png</applicationIcon>\n"
			qhcp += "        <aboutMenuText>\n"
			qhcp += "            <text>About Raptor</text>\n"
			qhcp += "        </aboutMenuText>\n"
			qhcp += "        <aboutDialog>\n"
			qhcp += "            <file>_static/about.txt</file>\n"
			qhcp += "            <icon>_static/raptor.png</icon>\n"
			qhcp += "        </aboutDialog>\n"
		else:
			qhcp += line
	file.close()

	file = open(filename, 'w')
	file.write(qhcp)
	file.close()

print "patch " + sys.argv[1]
patch_qhcp(sys.argv[1])
