import BasePlugin

class SimpleHelloWorld(BasePlugin.BasePlugin):
    
    def name(self):
        return "SimpleHelloWorld"

    def version(self):
        return "0.1"
        
    def about(self):
        return "SimpleHelloWorld Demo Plugin"

    def actions(self):
        print os.path.abspath("python.png")
        path = '/home/chris/Dev/raptorpy/python.png'
        return [("SimpleHelloWorld", path, None, None, None)]

    def run(self, action):
        print self.name() + " -> " + action
        return 0;


class HelloWorld(BasePlugin.BasePlugin):
    
    def name(self):
        return "HelloWorld"

    def version(self):
        return "0.1"
        
    def about(self):
        return "HelloWorld Demo Plugin"

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
