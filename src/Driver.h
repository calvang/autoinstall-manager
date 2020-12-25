/*
 * @author Calvin Huang, 2020
 * 
 * This file is part of Autoinstall Manager.
 *
 * See the LICENSE file at the top-level directory of this distribution
 * for details of code ownership.
 */

#include <string>
#include <unordered_map>
#include <vector>

#include "TableProps.h"

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
        struct Utility {
            Utility(string name, string type, string cmd);
            bool exec(string exec_dir);
            string name;
            string type;
            string cmd; // command to install utility
        };
        vector<Utility*> util_queue; // utils to prepare/install
        vector<string> excluded; // utils in config to exclude
        unordered_map<string, bool> util_statuses;
        string driver_configs; // path to specific util configs
        string execution_dir; // directory to execute from
        string task_name; // title to output when printing (Prepare/Install)
        int win_width;
};