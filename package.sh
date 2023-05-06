mkdir cbuild_$1_amd64
mkdir -p cbuild_$1_amd64/usr/local/bin

cp cbuild_x86_64 cbuild_$1_amd64/usr/local/bin/cbuild
mkdir cbuild_$1_amd64/DEBIAN
cp arch/x86_64/linux/pkg_info.txt cbuild_$1_amd64/DEBIAN/control

dpkg-deb --build --root-owner-group cbuild_$1_amd64

cp cbuild_$1_amd64.deb arch/x86_64/linux/

rm -rf cbuild_$1_amd64


mkdir cbuild_$1_i386
mkdir -p cbuild_$1_i386/usr/local/bin

cp cbuild_x86_32 cbuild_$1_i386/usr/local/bin/cbuild
mkdir cbuild_$1_i386/DEBIAN
cp arch/x86_32/linux/pkg_info.txt cbuild_$1_i386/DEBIAN/control

dpkg-deb --build --root-owner-group cbuild_$1_i386

cp cbuild_$1_i386.deb arch/x86_32/linux/

rm -rf cbuild_$1_i386

