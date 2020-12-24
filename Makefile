all: install

build:
	@g++ src/main.cpp src/driver.cpp -std=c++11 -Wall -Werror -Wextra -pedantic -O3 -DNDEBUG -o src/autoinstall-manager

clean:
	@rm src/autoinstall-manager

install:
	@make build
	@cp src/autoinstall-manager /usr/local/bin/

uninstall:
	@make clean
	@rm /usr/local/bin/autoinstall-manager
