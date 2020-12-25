#include <string>

using std::string;


void initialize_settings_file(string src_file, string dst_file, string dst_dir);

struct Settings {
    Settings(string file, string home);
    string insert_home_dir(string path, string home);
    void print(int win_width);
    string pconf;
    string iconf;
    string pdir;
    string idir;
};