/*
 * @author Calvin Huang, 2020
 * 
 * This file is part of Autoinstall Manager.
 *
 * See the LICENSE file at the top-level directory of this distribution
 * for details of code ownership.
 */

#include <string>
#include <fstream>
#include <iostream>

#include <unistd.h>
#include <stdio.h>

#include "Driver.h"

using std::cout;
using std::cerr;
using std::getline;
using std::string;


Driver::Driver(string dvr_configs, string ex_dir, string task, int width) : driver_configs(dvr_configs),
    execution_dir(ex_dir), task_name(task), win_width(width){}

Driver::Utility::Utility(string name, string type, string cmd) 
    : name(name), type(type), cmd(cmd){} 

/**
 * Execute utility's associated shell commands from a given dir
 */
bool Driver::Utility::exec(string exec_dir){
    char buff[128];
    string script =  "cd " + exec_dir + "\n" + cmd;
    string res = "";
    cout << "CMD: " << script << "\n";
    FILE *pipe = popen(script.c_str(), "r");
    if (!pipe) return false;
    while (!feof(pipe)) {
        if (fgets(buff, 128, pipe) != NULL)
            res += buff;
    }
    pclose(pipe);
    cout << "RESULT: " << res << "\n";
    return true;
}

/**
 * Parse the driver's configuration file and save scripts.
 */
void Driver::parse_driver_configs(){
    string out;
    string line;
    string curr_type, curr_name, curr_script = "";
    bool is_included = false; // true is checked in driver configs
    bool is_script = false; // true if in script block
    if (access(driver_configs.c_str(), F_OK) == -1) {
        cerr << "Error: config file for " << task_name << " does not exist!\n";
        exit(1);
    }
    std::ifstream infile(driver_configs);
    if (!infile.is_open()) {
        cerr << "Error opening config file for " << task_name << "!\n";
        exit(1);
    }
    while (getline(infile, line)){
        string first_three = line.substr(0,3);
        if (first_three == "## "){ // type declaration
            curr_type = line.substr(3);
        }
        else if (first_three == "```" && is_included){ // script block
            if (is_script){ // end of block
                is_script = false;
                Utility* new_util = new Utility(curr_name, curr_type, curr_script);
                util_queue.push_back(new_util);
                util_statuses.insert({curr_name,false});
                curr_script = "";
            }
            else is_script = true; // start of block
        }
        else if (first_three == "- [" && line[4] == ']'){
            char checkbox = line[3];
            curr_name = line.substr(6);
            if (checkbox == 'x') is_included = true;
            else {
                is_included = false;
                excluded.push_back(curr_name);
            }
        }
        else if (is_included && is_script){
            curr_script += line + "\n";
        }
        if (infile.fail()) {
            cerr << "Error reading config file for " << task_name << "!\n";
            exit(1);
        }
    }
} 

void Driver::run(){
    for (size_t i = 0; i < util_queue.size(); ++i){
        cout << "Running " << util_queue[i]->name << "...\n";
        bool status = util_queue[i]->exec(execution_dir);
        util_statuses[util_queue[i]->name] = status;
    }
}

/**
 * Print an exhaustive table of the relevant utilities as read from 
 * the driver's configuration file.
 */
void Driver::print_utils(){
    size_t col_width = win_width / 2 - 4;
    size_t inc_size = util_queue.size(), ex_size = excluded.size();
    size_t print_size = inc_size > ex_size ? inc_size : ex_size;
    string col_names[] = {"Included", "Excluded"};

    TitleBar titlebar("Detected "+ task_name + " Utilities", win_width);
    HLine hline(win_width);
    ColumnCell blank(col_width);
    Header header(col_names, col_width, win_width, 2);
    
    cout << "\n";
    titlebar.print();
    header.print();
    for (size_t i = 0; i < print_size; ++i){
        if (i < inc_size){
            ColumnCell right_col(util_queue[i]->name, col_width);
            right_col.print(false); // no newline
        }
        else blank.print(false);
        if (i < ex_size){
            ColumnCell left_col(excluded[i], col_width);
            left_col.print();
        }
        else blank.print();
    }
    hline.print();
    cout << "\n";
}

/**
 * Print statuses of all utilities (Success/failure) after
 * running the driver.
 */
void Driver::print_statuses(){
    size_t col_width = win_width / 2 - 4;
    string col_names[] = {"Process", "Status"};

    TitleBar titlebar("Executed "+ task_name + " Processes", win_width);
    HLine hline(win_width);
    ColumnCell blank(col_width);
    Header header(col_names, col_width, win_width, 2);

    cout << "\n";
    titlebar.print();
    header.print();
    for (size_t i = 0; i < util_queue.size(); ++i){
        string status = "Failure";
        ColumnCell right_col(util_queue[i]->name, col_width);
        right_col.print(false); // no newline
        if (util_statuses[util_queue[i]->name]) status = "Success";
        ColumnCell left_col(status, col_width);
        left_col.print();
    }
    hline.print();
    cout << "\n";
}