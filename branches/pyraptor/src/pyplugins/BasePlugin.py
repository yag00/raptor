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

class BasePlugin(object):
    
    def name(self):
        """ return the name of the plugin """
        return ""
 
    def icon(self):
        """ return the icon of the plugin """
        return ""

    def shortDescription(self):
        """ return a short description of the plugin """
        return ""

    def description(self):
        """ return a short description of the plugin """
        return ""

    def author(self):
        """ return the author of the plugin """
        return ""

    def license(self):
        """ return the license name of the plugin """
        return ""
        
    def version(self):
        """ return the version of the plugin """
        return ""
        
    def about(self):
        """ return the about plugin informations"""
        return ""


    def actions(self):
        """ return a list tuple containing plugin action
        tuple = (name, icon, shortcut, functionName, parentActionName)
        'name'          : the name of the action, displayed in the menu
        'icon'          : the path to an icon associated with the action, 
                          displayed in the menu. Use None if this action has no icon
        'shortcut'      : the shortcut sequence key
        'functionName'  : the python method of the plugin to call when the action is triggered.
                          If None is used the run function will be called
        'parentAction'  : the parent action name. Use None if this action has no parent.
                          Use to create the menu tree.
        """
        return []


    def run(self, action):
        """ default function """
        return 0;
