# Prerequisites #

  * [Qt 4.x](http://qt.nokia.com/products/)
  * [Python](http://www.python.org/) (use by waf and sphinx)
  * [Sphinx](http://sphinx.pocoo.org/) (use for generate the documentation)

Raptor use waf build system: more information on [waf build system](http://code.google.com/p/waf/)

# Building raptor on linux #

To build raptor, just type these command from raptor root path:
```
./waf configure
./waf
```

for more advanced command
```
./waf --help
```

# Building raptor on windows #
Define the environnement variables as the following example:

  * QTDIR
    * C:\Qt\xxx\qt

  * QMAKESPEC
    * win32-g++

  * PATH=
    * C:\Qt\xxx\mingw\bin
    * C:\Qt\xxx\bin
    * C:\Qt\xxx\qt\bin
    * C:\Python27
    * C:\Python27\Scripts

Then launch a terminal (cmd.exe) or another console
( [msys](http://www.mingw.org/wiki/msys) or [console](http://sourceforge.net/projects/console/) ) and build raptor :

```
python waf configure
python waf
```

for more advanced command
```
./waf --help
```