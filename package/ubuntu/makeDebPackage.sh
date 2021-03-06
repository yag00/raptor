# Usage
if [ $# -lt 1 ]
then
    echo Usage: $0 version_to_package
	echo Checkout,compile and package Raptor
    echo version_to_package : Version of raptor to build and package. Enter the version number or trunk.
    exit 1
fi

#svn checkout
if [ $1 = "trunk" ]
then
	echo Checkout Raptor trunk
	rm -rf raptor-editor-0.3.0
	svn checkout http://raptor.googlecode.com/svn/trunk/ raptor-editor-0.3.0
	cd raptor-editor-0.3.0
else
	echo Checkout Raptor version $1
	rm -rf raptor-editor-$1
	svn checkout http://raptor.googlecode.com/svn/tags/raptor-$1 raptor-editor-$1
	cd raptor-editor-$1
fi

#clear svn file
echo "recursively removing .svn folders from"
pwd
rm -rf `find . -type d -name .svn`

# Create the package
echo "Create package"
echo "dh_make : " | dh_make -e christophe.duvernois@gmail.com -n -s -c gpl3
rm -rf debian/*.ex  debian/*.EX  debian/README.* debian/docs  debian/dirs
cp -r ../debian .
debuild -S -sa
dpkg-buildpackage -rfakeroot
cd ../
ls
