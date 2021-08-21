#!/bin/bash
#Author: Suraj Patel
echo -e "\n
$(tput setaf 2)..######..##....##....###....########..########..##....##
.##....##.###...##...##.##...##.....##.##.....##..##..##.
.##.......####..##..##...##..##.....##.##.....##...####..
..######..##.##.##.##.....##.########..########.....##...
.......##.##..####.#########.##........##...........##...
.##....##.##...###.##.....##.##........##...........##...
..######..##....##.##.....##.##........##...........##...
$(tput setaf 3)  A all in one command line utility for screenshots and screen records.
$(tput setaf 1)  github.com/real4suraj/snappy.git\n"

[ "$(id -u)" != "0" ] && echo -e "$(tput setaf 3)  This script must be run as root!\n Aborting" && exit

check_installed() {
  [ "$(pacman -Qi $1 &> /dev/null)" ] && return 0
  [ -f "/usr/bin/$1" ] && return 0
  [ -f "/usr/local/bin/$1" ] && return 0
  return 1
}
show_success() {
  echo -e "$(tput setaf 2)  $1 check passed" 
}
install_package() {
  echo -e "$(tput setaf 3)  Installing $1" && pacman --noconfirm -Sy $1 > /dev/null 2>&1
}

use_hack=""
set_snappy_font() {
  echo -e "$(tput setaf 4)  Installing Hack Nerd Font " && cp "$(pwd)/Hack.ttf" "/usr/share/fonts/" && fc-cache
  read -p " Size [default 12] : " sz
  [ ! $sz ] && sz="12"
  use_hack="Hack Nerd Font:size=$sz"
}
if check_installed "snappy"; then
  echo -e "$(tput setaf 4)  Found snappy in path!\n 1. Install snappy with the specified font size\n 2. Uninstall snappy"
  read -p " Choose (1, 2 [default 1]) : " ch 
  case $ch in
    "2") rm /usr/bin/xrectsel && rm /usr/local/bin/snappy && echo "$(tput setaf 1) Uninstalled snappy from system." && tput setaf 7 && exit ;; 
    *) set_snappy_font ;;
  esac
fi

echo -e "$(tput setaf 2)  Verfying | Installing Dependencies"
[ ! "$use_hack" ] && read -p "$(echo -e "$(tput setaf 4)    壘   Are these unicode visible ? [y/N] : ")" ch && case $ch in
  y|Y) echo -e "$(tput setaf 2)  unicode check passed" ;;
  *) set_snappy_font ;;
esac
check_installed "dmenu" && show_success "dmenu" || install_package "dmenu"
check_installed "ffmpeg" && show_success "ffmpeg" || install_package "ffmpeg"
check_installed "scrot" && show_success "scrot" || install_package "scrot"
check_installed "gcc" && show_success "gcc" || install_package "base-devel"
echo -e "$(tput setaf 2)  All checks passed! Installing snappy." 
gcc -lX11 "$(pwd)/xrectsel.c" -o "$(pwd)/xrectsel"
cp "$(pwd)/xrectsel" "/usr/bin"
echo $use_hack
[ ! "$use_hack" ] && cp "$(pwd)/snappy" "/usr/local/bin" || (sed "s/-fn/-fn \"$use_hack\"/g" snappy_font > /usr/local/bin/snappy)

chmod a+x /usr/bin/xrectsel
chmod a+x /usr/local/bin/snappy
tput setaf 7
