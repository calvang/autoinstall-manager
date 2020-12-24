/*
 * This file is part of Autoinstall Manager.
 *
 * See the LICENSE file at the top-level directory of this distribution
 * for details of code ownership.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <getopt.h>

#include <sys/ioctl.h>
#include <stdio.h>
#include <unistd.h>
#include <pwd.h>

#include "driver.h"

using std::cout;
using std::cerr;
using std::string;


void parse_opts(std::unordered_map<char, bool>& opts, int argc, char** argv);

int main(int argc, char** argv) {
    std::unordered_map<char, bool> opts;
    parse_opts(opts, argc, argv);
    string home = string(getpwuid(getuid())->pw_dir);
    string prep_dir = home + "/.dotfiles/setup/PREPARE_CONFIGS.md";
    string install_dir = home + "/.dotfiles/setup/INSTALL_CONFIGS.md";
    string prep_ex = home;
    string installer_ex = home + "/Downloads";
    // extract the terminal window dims
    struct winsize win_dims;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &win_dims);
    int win_width = int(win_dims.ws_col);
    if (opts['p']){
        Driver preparer(prep_dir, prep_ex, "Preparation", win_width);
        preparer.parse_driver_configs();
        if (opts['v']) preparer.print_utils();
    }
    if (opts['i']){
        Driver installer(install_dir, prep_ex, "Installation", win_width);
        installer.parse_driver_configs();
        if (opts['v']) installer.print_utils();
    }
    return 0;
}

/**
 * Parse options from the command line.
 * 
 * @param opts unordered_map of flags and bools
 */
void parse_opts(std::unordered_map<char, bool>& opts, int argc, char** argv) {
    // run full process by default
    if (argc == 1) opts['p'] = true, opts['i'] = true, opts['v'] = true;

    int choice = 0, opt_idx = 0;
    option long_opts[] = {
        {"help", no_argument, nullptr, 'h'},
        {"verbose", no_argument, nullptr, 'v'},
        {"prepare", no_argument, nullptr, 'p'},
        {"install", no_argument, nullptr, 'i'}
    };

    while ((choice = getopt_long(argc, argv, "hvpi", long_opts, &opt_idx)) != -1) {
        switch (choice) {
            case 'h':
                cout << "Autoinstall Manager\n"
                    << "Repo: https://github.com/calvang/autoinstall-manager\n\n"
                    << "Usage: autoinstall [OPTIONS]\n"
                    << "Default: -vpi\n"
                    << "\t-h: Access help menu\n"
                    << "\t-v: Output in verbose mode.\n"
                    << "\t-p: Prepare system by setting up directories and symlinks\n"
                    << "\t-i: Install utilities and programs to system\n";
                exit(0);
            case 'v':
                opts['v'] = true;
                break;
            case 'p':
                opts['p'] = true;
                break;
            case 'i':
                opts['i'] = true;
                break;
            default:
                cerr << "Invalid option. See -h for help.\n";
                exit(1);
        }
    }
}