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
#include <string>
#include <unordered_map>
#include <getopt.h>

#include <sys/ioctl.h>
#include <wctype.h>
#include <unistd.h>
#include <pwd.h>

#include "Driver.h"
#include "Settings.h"

using std::cout;
using std::cerr;
using std::string;

void run_manager(std::unordered_map<char, bool>& opts, string USER_HOME, string DSETTINGS,
    string FSETTINGS, std::ostream& out, bool save);

void parse_opts(std::unordered_map<char, bool>& opts, int argc, char** argv);

int main(int argc, char** argv) {
    std::unordered_map<char, bool> opts;
    parse_opts(opts, argc, argv);

    string USER_HOME = string(getpwuid(getuid())->pw_dir);
    string DSETTINGS = USER_HOME + "/.config/autoinstall-manager"; // settings dir
    string FSETTINGS = USER_HOME + "/.config/autoinstall-manager/settings.conf";

    // initialize output stream for saving
    if (opts['s']) {
        string LOGFILE = USER_HOME + "/.config/autoinstall-manager/log.txt";
        std::ofstream out(LOGFILE);
        out << "hi";
        run_manager(opts, USER_HOME, DSETTINGS, FSETTINGS, out, true);
    } 
    else
        run_manager(opts, USER_HOME, DSETTINGS, FSETTINGS, cout, false);

    return 0;
}

/**
 * Run the utility with given options and an output stream.
 */
void run_manager(std::unordered_map<char, bool>& opts, string USER_HOME, string DSETTINGS,
    string FSETTINGS, std::ostream& out, bool save) {

    // extract the terminal window dims
    struct winsize win_dims;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &win_dims);
    int win_width = int(win_dims.ws_col);

    // initialize and read in settings file
    initialize_settings_file(FSETTINGS, DSETTINGS);
    Settings settings(FSETTINGS, USER_HOME);
    if (opts['v']) settings.print(win_width, out, save);

    // run stages
    if (opts['p']){
        Driver preparer(settings.pconf, settings.pdir, "Preparation", win_width, out, save);
        preparer.parse_driver_configs();
        if (opts['v']) preparer.print_utils();
        preparer.run();
        if (opts['v']) preparer.print_statuses();
    }
    if (opts['i']){
        Driver installer(settings.iconf, settings.idir, "Installation", win_width, out, save);
        installer.parse_driver_configs();
        if (opts['v']) installer.print_utils();
        installer.run();
        if (opts['v']) installer.print_statuses();
    }
}

/**
 * Parse options from the command line.
 * 
 * @param opts unordered_map of flags and bools
 */
void parse_opts(std::unordered_map<char, bool>& opts, int argc, char** argv) {
    // run full process by default
    if (argc == 1) opts['p'] = true, opts['i'] = true, opts['v'] = true, opts['s'] = true;

    int choice = 0, opt_idx = 0;
    option long_opts[] = {
        {"help", no_argument, nullptr, 'h'},
        {"verbose", no_argument, nullptr, 'v'},
        {"save", no_argument, nullptr, 's'},
        {"prepare", no_argument, nullptr, 'p'},
        {"install", no_argument, nullptr, 'i'}
    };

    while ((choice = getopt_long(argc, argv, "hvspi", long_opts, &opt_idx)) != -1) {
        switch (choice) {
            case 'h':
                cout << "Autoinstall Manager\n"
                    << "https://github.com/calvang/autoinstall-manager\n\n"
                    << "Usage: autoinstall [OPTIONS]\n"
                    << "  -h,  --help       Access help menu\n"
                    << "  -v,  --verbose    Output in verbose mode\n"
                    << "  -s,  --save       Save output to \"~/.config/autoinstall-manager/log.txt\"\n"
                    << "  -p,  --prepare    Prepare system by setting up directories and symlinks\n"
                    << "  -i,  --install    Install utilities and programs to system\n\n"
                    << "Default arguments are -vspi\n";
                exit(0);
            case 'v':
                opts['v'] = true;
                break;
            case 's':
                opts['s'] = true;
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