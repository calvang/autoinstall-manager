# Install Configurations

## Core

- [x] git

```
sudo add-apt-repository ppa:git-core/ppa
sudo apt update
sudo apt install git
ln ~/.dotfiles/.gitconfig ~
```

## Repositories

- [x] Ubuntu universe

```
sudo add-apt-repository universe
```

## Utilities

- [x] ffmpeg

```
sudo apt-get install ffmpeg
```

- [x] mpv

```
sudo add-apt-repository ppa:mc3man/mpv-tests
sudo apt update
sudo apt install mpv
```

## Programming Languages

- [x] Python3-venv (Ubuntu)

```
sudo apt install -y python3-venv
```

- [x] Pip3 (Ubuntu)

```
sudo apt install -y python3-pip
```

- [x] NVM (Node Version Manager)

```
get_latest_release() {
  curl --silent "https://api.github.com/repos/$1/releases/latest" | # Get latest release from GitHub api
    grep '"tag_name":' |                                            # Get tag line
    sed -E 's/.*"([^"]+)".*/\1/'                                    # Pluck JSON value
}
latest=$(get_latest_release nvm-sh/nvm)

wget -qO- https://raw.githubusercontent.com/nvm-sh/nvm/${latest}/install.sh | bash
```

- [x] Node.js

```
nvm install-latest-npm
```

- [x] NPM (Node Package Manager)

```
nvm install lts/* --reinstall-packages-from=default --latest-npm
```

- [x] Rust

```
sudo curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh
rustc --version
```