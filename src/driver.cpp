#include <string>
#include <fstream>
#include <iostream>
#include <sstream>

#include "driver.h"

using std::cout;
using std::getline;
using std::string;


Driver::Driver(string dvr_configs, string ex_dir, string task, int width) : driver_configs(dvr_configs),
    execution_dir(ex_dir), task_name(task), win_width(width){}


/**
 * Parse the driver's configuration file and save scripts.
 */
void Driver::parse_driver_configs(){
    string out;
    string line;
    string curr_type, curr_name, curr_script = "";
    bool is_included = false; // true is checked in driver configs
    bool is_script = false; // true if in script block
    std::ifstream infile(driver_configs);
    while (getline(infile,line)){
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
        if (is_included && is_script){
            curr_script += line + "\n";
        }
    }
} 

void Driver::run(){
    // for (size_t i = 0; i < util_queue.size(); ++i){
    //     Utility* util = util_queue[i];
    //     cout << "Running " << util->name << "...\n";
    // }
}

/**
 * Print an exhaustive table of the relevant utilities as read from 
 * the driver's configuration file.
 */
void Driver::print_utils(){
    size_t col_width = win_width / 2 - 4;
    size_t inc_size = util_queue.size(), ex_size = excluded.size();
    size_t print_size = inc_size > ex_size ? inc_size : ex_size;
    // handle title bar
    string titlebar = "";
    if (col_width > task_name.length())
        titlebar.append((col_width-(task_name.length()-3)/2), '=');
    titlebar = titlebar + "  " + task_name + "  " + titlebar;
    if (titlebar.length() < 2*col_width+8) titlebar += "=";
    cout << titlebar + "\n";
    // horizontal table line
    string hline = "";
    hline.append((win_width/2)*2, '-');
    // blank column
    string blank = "";
    blank.append(col_width, ' ');
    // header block
    string left_h = "Included", right_h = "Excluded";
    adjust_print_cols(left_h, col_width);
    adjust_print_cols(right_h, col_width);
    cout << hline + "\n| " + left_h + " || " + right_h + " |\n"
        << hline + "\n";
    for (size_t i = 0; i < print_size; ++i){
        if (i < inc_size){
            string right_col = util_queue[i]->name;
            adjust_print_cols(right_col, col_width);
            cout << "| " + right_col + " |";
        }
        else cout << "| " + blank + " |";
        if (i < ex_size){
            string left_col = excluded[i];
            adjust_print_cols(left_col, col_width);
            cout << "| " + left_col + " |\n";
        }
        else cout << "| " + blank + " |\n";
    }
    cout << hline + "\n\n";
}

/**
 * Print statuses of all utilities (Success/failure) after
 * running the driver.
 */
void Driver::print_statuses(){
    
}