#include <string>
#include <unordered_map>
#include <vector>

using std::cout;
using std::string;
using std::vector;
using std::unordered_map;

class Driver {
    public:
        Driver(string dvr_configs, string ex_dir, string task, int width);
        void parse_driver_configs();
        void run();
        void print_utils();
        void print_statuses(); 
    private:
        void adjust_print_cols(string& col, size_t col_width){
            if (col.length() < col_width) // pad with spaces
                col.append(col_width - col.length(), ' ');
            else
                col = col.substr(0, col_width);
        }
        struct Utility {
            string name;
            string type;
            string cmd; // command to install utility
            Utility(string name, string type, string cmd) 
                : name(name), type(type), cmd(cmd){} 
        };
        vector<Utility*> util_queue; // utils to prepare/install
        vector<string> excluded; // utils in config to exclude
        unordered_map<string, bool> util_statuses;
        string driver_configs; // path to specific util configs
        string execution_dir; // directory to execute from
        string task_name; // title to output when printing (Prepare/Install)
        int win_width;
};