# Maintainer: Suraj Patel <real4suraj@gmail.com>
pkgname=suraj-snappy-git
pkgver=1.0
pkgrel=1
pkgdesc="A all in one command line utility for screenshots and screenrecords"
arch=(x86_64)
url="https://github.com/real4suraj/snappy.git"
license=('GPL')
depends=(ffmpeg scrot base-devel dmenu)
provides=(snappy)
conflicts=(snappy)
source=("git+$url")
md5sums=('SKIP')

pkgver() {
  cd "${_pkgname}"
  printf "1.0.r%s.%s" "$(git rev-list --count HEAD)" "$(git rev-parse --short HEAD)"
}

package() {
  cd "${_pkgname}"
  install -Dm755 xrectsel "${pkgdir}"/usr/bin/xrectsel
  install -Dm755 snappy_aur "${pkgdir}"/usr/local/bin/snappy
}
