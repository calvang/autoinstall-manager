all: install

init:
	@mkdir -p ~/.config/autoinstall-manager
	@cp settings.conf ~/.config/autoinstall-manager

build:
	@make init
	@g++ src/main.cpp src/Driver.cpp src/TableProps.cpp -std=c++11 -Wall -Werror -Wextra -pedantic -O3 -DNDEBUG -o src/autoinstall-manager

run:
	@./src/autoinstall-manager

test:
	@make -s build
	@make -s run

clean:
	@rm src/autoinstall-manager

install:
	@make init
	@make build
	@cp src/autoinstall-manager /usr/local/bin/

uninstall:
	@make clean
	@rm /usr/local/bin/autoinstall-manager
	@rm -rf ~/.config/autoinstall-manager
