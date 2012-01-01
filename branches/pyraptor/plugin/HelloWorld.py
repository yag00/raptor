import BasePlugin

class SimpleHelloWorld(BasePlugin.BasePlugin):
    
    def name(self):
        """ return the name of the plugin """
        return "SimpleHelloWorld"

    def shortDescription(self):
        """ return a short description of the plugin """
        return "Simple Demo Plugin"

    def description(self):
        """ return a short description of the plugin """
        return "Simple Demo Plugin\nTo be used as Example"

    def author(self):
        """ return the author of the plugin """
        return "christophe.duvernois"

    def license(self):
        """ return the license name of the plugin """
        return "GNU GPL v3"
        
    def version(self):
        """ return the version of the plugin """
        return "1.0"
        
    def actions(self):
        print os.path.abspath("python.png")
        path = '/home/chris/Dev/raptorpy/python.png'
        return [("SimpleHelloWorld", path, None, None, None)]

    def run(self, action):
        print self.name() + " -> " + action
        return 0;


class HelloWorld(BasePlugin.BasePlugin):
    
    def name(self):
        """ return the name of the plugin """
        return "HelloWorld"

    def shortDescription(self):
        """ return a short description of the plugin """
        return "HelloWorld Demo Plugin"

    def description(self):
        """ return a short description of the plugin """
        return "Simple Demo Plugin\nTo be used as Example"

    def author(self):
        """ return the author of the plugin """
        return "christophe.duvernois"

    def license(self):
        """ return the license name of the plugin """
        return "GNU GPL v3"
        
    def version(self):
        """ return the version of the plugin """
        return "1.0"

    def actions(self):
        return [("HelloWorld", None, None, "function1", None), \
                ("HelloWorld2", None, None, None, None), \
                ("HelloWorld3", None, None, None, "HelloWorld2"), \
                ("D", None, None, None, None)]

    def function1(self):
        print self.name() + " -> function1"
        return 0;

    def run(self, action):
        print self.name() + " -> " + action
        return 0;
