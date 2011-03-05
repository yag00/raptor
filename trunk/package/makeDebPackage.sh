echo "check out raptor version $1"
rm -rf raptor-0.1.0
svn checkout http://raptor.googlecode.com/svn/trunk/ raptor-0.1.0
cd raptor-0.1.0
echo "recursively removing .svn folders from"
pwd
rm -rf `find . -type d -name .svn`

echo "Create package"
dh_make -e christophe.duvernois@gmail.com -n -s -c gpl3
rm -rf debian/*.ex  debian/*.EX  debian/docs  debian/dirs
cp -r ../debian .
dpkg-buildpackage -rfakeroot
cd ../
ls
