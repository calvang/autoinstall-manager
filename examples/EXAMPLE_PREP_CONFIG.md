# Preparation Configurations

## File System

- [x] Ubuntu Dotfiles

```
git clone <dotfiles_repo>
```

## Symlinks

- [x] .bashrc

```
rm -rf ~/.bashrc
ln ~/.dotfiles/.bashrc ~
```

- [x] .profile

```
rm -rf ~/.profile
ln ~/.dotfiles/.profile ~
```

- [ ] .gitconfig

```
ln ~/.dotfiles/.gitconfig ~
```

- [ ] .vimrc

```
ln ~/.dotfiles/.vimrc ~
```

- [ ] .alacritty.yml

```
ln ~/.dotfiles/.alacritty.yml ~
```

## Startup

- [x] Ubuntu Startup Applications

```
cp ~/.dotfiles/startup/startup.desktop ~/.config/autostart
```

## Refresh

- [ ] Rebash

```
source ~/.bashrc
```

