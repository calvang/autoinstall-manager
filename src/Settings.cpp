#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <boost/filesystem.hpp>

#include <unistd.h>

#include "Settings.h"
#include "TableProps.h"

using std::cout;
using std::cerr;
using std::string;
namespace fs = boost::filesystem;

/**
 * Creates .config/autoinstall-manager/settings.conf if
 * it does not already exist.
 */
void initialize_settings_file(string src_file, string dst_file, string dst_dir){
    fs::path dir_path(dst_dir);
    if (!fs::is_directory(dir_path))
        fs::create_directory(dir_path);
    fs::path src(src_file);
    fs::path dst(dst_file);
    if (!fs::exists(dst))
        fs::copy(src, dst);
}


Settings::Settings(string file, string home) {
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

string Settings::insert_home_dir(string path, string home) {
    string new_path = path;
    if (path[0] == '~') new_path = home + path.substr(1);
    return new_path;
}

void Settings::print(int win_width) {
    cout << "\n";
    TitleBar titlebar("Current Settings", win_width);
    titlebar.print();
    cout << "PREP_CONFIGS: " << pconf << "\nINSTALL_CONFIGS: " << iconf
        << "\nPREP_EXECUTION_DIR: " << pdir << "\nINSTALL_EXECUTION_DIR: " << idir << "\n";
}