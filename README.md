# Roscli

Awesome tool written in C,C++,Bash to make roslaunch more elegant

## Dependencies

- [Terminator](https://github.com/gnome-terminator/terminator) - Multiple GNOME terminals in one window
- [tmux](https://github.com/tmux/tmux) — A terminal multiplexer
- [fzf](https://github.com/junegunn/fzf) - A command-line fuzzy finder
- [ncurses]

```sh
sudo apt-get install terminator tmux libncurses5-dev

## Ubuntu 19.04+
sudo apt-get install fzf

## Below 19.04 (Using official installation script)
git clone --depth 1 https://github.com/junegunn/fzf.git ~/.fzf
~/.fzf/install
```

## Installation

```sh
git clone https://github.com/ms-jagadeeshan/roscli.git
cd roscli
make install
```

To run the program `make run`

## Todo

- [ ] roslaunch file from /opt/ros/${ROS_DISTRO}
- [ ] rosrun support
- [ ] Better Makefile
- [ ] Multiple window layouts
