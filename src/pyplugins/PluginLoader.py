"""
 This file is part of the raptor project.

 Copyright (C) 2011 Christophe Duvernois <christophe.duvernois@gmail.com>

 Raptor is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 Raptor is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA
"""

import sys
import os
import glob
import BasePlugin

class PluginLoader():
	def load(self, path):
		d = {}
		for p in path:
			sys.path.append(p)
			for file in glob.glob(os.path.join(os.path.abspath(p), "*.py")):
				d[file] = []
				#name of file
				name = os.path.splitext(os.path.basename(file))[0]
				#import module
				module = __import__(name)
				#list member of the module
				for member in dir(module):
					if member == 'BasePlugin':
						continue
					if member.startswith('__'):
						continue

					#check plugins
					plugin = getattr(module, member)
					try:
						if issubclass(plugin, BasePlugin.BasePlugin):
							d[file].append(member)
					except:
						pass
		return d
