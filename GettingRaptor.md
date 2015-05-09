# Windows #
![http://raptor.googlecode.com/svn/wiki/images/windows.png](http://raptor.googlecode.com/svn/wiki/images/windows.png) Download the windows installer, and follow the setup instructions.
Go to the [download page](http://code.google.com/p/raptor/downloads/list?q=label:Featured) or directly [get the last release](http://code.google.com/p/raptor/downloads/detail?name=raptor-0.4.0-setup.exe)

# Linux #
> # ![http://raptor.googlecode.com/svn/wiki/images/ubuntu.png](http://raptor.googlecode.com/svn/wiki/images/ubuntu.png) Ubuntu #
> You can get a binary ubuntu package from the repository since lucid lynx. Just add the repository following one of these two methods:
    * Add the repository using the add-apt-repository command
```
sudo add-apt-repository ppa:raptor-project/raptor
```
    * Or just edit your _sources.list_ file and import the key.
```
#sudo apt-key adv --keyserver keyserver.ubuntu.com --recv-keys 8B87FA0A 
deb http://ppa.launchpad.net/raptor-project/raptor/ubuntu oneiric main
```
> Then install raptor
```
sudo apt-get update
sudo apt-get install raptor-editor
```

> The deb package for ubuntu are also available on the [download page](http://code.google.com/p/raptor/downloads/list?q=label:Featured)

> # ![http://raptor.googlecode.com/svn/wiki/images/fedora.png](http://raptor.googlecode.com/svn/wiki/images/fedora.png) Fedora #
> raptor is not yet available via yum, but you can get the rpm for fedora on the [download page](http://code.google.com/p/raptor/downloads/list?q=label:Featured)

> Then install raptor
```
sudo rpm -ivh raptor-editor-(your_version).rpm
```

> # ![http://raptor.googlecode.com/svn/wiki/images/linux.png](http://raptor.googlecode.com/svn/wiki/images/linux.png) Others distributions #

> No binary packages are available yet for your distribution. _(contributions are welcomed)._

> Get the source and compile it. See the [How to build raptor](BuildingRaptor.md) wiki page.

# Mac OS #
![http://raptor.googlecode.com/svn/wiki/images/mac.png](http://raptor.googlecode.com/svn/wiki/images/mac.png) No binary packages are available for Mac OS . _(contributions are welcomed)._
> Get the source and compile it. See the [How to build raptor](BuildingRaptor.md) wiki page.