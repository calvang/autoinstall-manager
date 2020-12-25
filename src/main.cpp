/*
 * @author Calvin Huang, 2020
 * 
 * This file is part of Autoinstall Manager.
 *
 * See the LICENSE file at the top-level directory of this distribution
 * for details of code ownership.
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <getopt.h>

#include <sys/ioctl.h>
#include <stdio.h>
#include <wctype.h>
#include <unistd.h>
#include <pwd.h>

#include "Driver.h"

using std::cout;
using std::cerr;
using std::string;


struct Settings {
    Settings(string file, string home) {
        string line;
        if (access(file.c_str(), F_OK ) == -1) {
            cerr << "Error: settings.conf does not exist!\n";
            exit(1);
        }
        std::ifstream infile(file);
        if (!infile.is_open()) {
            cerr << "Error opening settings.conf!\n";
            exit(1);
        }
        while (getline(infile, line)){
            // skip comments and whitespace
            if (line.empty() || line[0] == '#' || iswspace(line[0])) continue;
            string key, val;
            std::stringstream lstream(line);
            getline(lstream, key, '=');
            getline(lstream, val);
            // trim whitespace
            key.erase(key.find_last_not_of(" \t\n\r\f\v") + 1);
            val.erase(val.find_last_not_of(" \t\n\r\f\v") + 1);
            if (key == "PREP_CONFIGS") pconf = insert_home_dir(val, home);
            else if (key == "INSTALL_CONFIGS") iconf = insert_home_dir(val, home);
            else if (key == "PREP_EXECUTION_DIR") pdir = insert_home_dir(val, home);
            else if (key == "INSTALL_EXECUTION_DIR") idir = insert_home_dir(val, home);
            if (infile.fail()) {
                cerr << "Error reading from settings.conf!\n";
                exit(1);
            }
        }
    }
    string insert_home_dir(string path, string home) {
        string new_path = path;
        if (path[0] == '~') new_path = home + path.substr(1);
        return new_path;
    }
    void print(int win_width) {
        cout << "\n";
        TitleBar titlebar("Current Settings", win_width);
        titlebar.print();
        cout << "PREP_CONFIGS: " << pconf << "\nINSTALL_CONFIGS: " << iconf
            << "\nPREP_EXECUTION_DIR: " << pdir << "\nINSTALL_EXECUTION_DIR: " << idir << "\n";
    }
    string pconf;
    string iconf;
    string pdir;
    string idir;
};

void parse_opts(std::unordered_map<char, bool>& opts, int argc, char** argv);

int main(int argc, char** argv) {
    std::unordered_map<char, bool> opts;
    parse_opts(opts, argc, argv);

    string USER_HOME = string(getpwuid(getuid())->pw_dir);
    string FSETTINGS = USER_HOME + "/.config/autoinstall-manager/settings.conf";

    // extract the terminal window dims
    struct winsize win_dims;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &win_dims);
    int win_width = int(win_dims.ws_col);

    // read in settings file
    Settings settings(FSETTINGS, USER_HOME);
    if (opts['v']) settings.print(win_width);

    // run stages
    if (opts['p']){
        Driver preparer(settings.pconf, settings.pdir, "Preparation", win_width);
        preparer.parse_driver_configs();
        if (opts['v']) preparer.print_utils();
        preparer.run();
        if (opts['v']) preparer.print_statuses();
    }
    if (opts['i']){
        Driver installer(settings.iconf, settings.idir, "Installation", win_width);
        installer.parse_driver_configs();
        if (opts['v']) installer.print_utils();
        installer.run();
        if (opts['v']) installer.print_statuses();
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