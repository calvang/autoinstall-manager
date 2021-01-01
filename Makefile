all: install

build:
	@g++ src/main.cpp src/Driver.cpp src/Settings.cpp src/TableProps.cpp -std=c++11 -lboost_filesystem -lboost_system -Wall -Werror -Wextra -pedantic -O3 -DNDEBUG -o autoinstall-manager

run:
	@./autoinstall-manager

test:
	@make -s build
	@make -s run

package:
	@make -s build
	@cp autoinstall-manager usr/bin
	@cd ..; dpkg-deb --build autoinstall-manager; \
		mv autoinstall-manager.deb autoinstall-manager

clean:
	@rm autoinstall-manager

install:
	@make init
	@make build
	@cp autoinstall-manager /usr/local/bin/

uninstall:
	@make clean
	@rm /usr/local/bin/autoinstall-manager
	@rm -rf ~/.config/autoinstall-manager
