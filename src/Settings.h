#include <string>

using std::string;


void initialize_settings_file(string file, string dir);

struct Settings {
    Settings(string file, string home);
    string insert_home_dir(string path, string home);
    void print(int win_width, std::ostream& out = std::cout, bool save = false);
    string pconf;
    string iconf;
    string pdir;
    string idir;
};