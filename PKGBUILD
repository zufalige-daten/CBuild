pkgname=cbuild
pkgver='v3.5.5'
pkgrel=1
epoch=1
pkgdesc='A simplistic build system inspired by Makefile for compiled languages that include other files from include directories and from local directories.'
arch=('x86_64' 'i386' 'pentium4' 'armv7h' 'aarch64')
url='https://zufalige-daten.github.io/CBuild.html'
license=('GPL-2.0')
groups=(cbuild)
depends=('bash>=5.2.37')
makedepends=('gcc' 'clang' 'make')
source=("https://github.com/zufalige-daten/CBuild/archive/refs/tags/${pkgver}.tar.gz")
sha256sums=("ed3ea6952688104bebdbe1d12eee894d20a2d89e8f539779235c3600968a306f")

build(){
	cd "${srcdir}/CBuild-3.5.5"
	make -j $(nproc)
}

package(){
	mkdir -p "${pkgdir}/usr/bin"
	cp "${srcdir}/CBuild-3.5.5/cbuild" "${pkgdir}/usr/bin/cbuild"
}

