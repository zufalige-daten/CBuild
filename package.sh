mkdir cbuild_1.0-1_amd64
mkdir -p cbuild_1.0-1_amd64/usr/local/bin

cp cbuild cbuild_1.0-1_amd64/usr/local/bin
mkdir cbuild_1.0-1_amd64/DEBIAN
cp pkg_info.txt cbuild_1.0-1_amd64/DEBIAN/control

dpkg-deb --build --root-owner-group cbuild_1.0-1_amd64

rm -rf cbuild_1.0-1_amd64
