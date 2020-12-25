/*
 * @author Calvin Huang, 2020
 * 
 * This file is part of Autoinstall Manager.
 *
 * See the LICENSE file at the top-level directory of this distribution
 * for details of code ownership.
 */

#include "TableProps.h"

using std::cout;


TableProp::TableProp() : output(""){}

TableProp::TableProp(string input) : output(input){}

void TableProp::print(bool newline){
    if (newline) cout << output + "\n";
    else cout << output;
}


TitleBar::TitleBar(string title, size_t width) : TableProp::TableProp(){
    width = width / 2 - 4;
    if (width > output.length())
        output.append((width-(title.length()-3)/2), '=');
    output = output + "  " + title + "  " + output;
    if (output.length() < 2*width+8) output += "=";
}


HLine::HLine(size_t width) : TableProp::TableProp(){
    output.append((width/2)*2, '-');
}


ColumnCell::ColumnCell(size_t width) : TableProp::TableProp(){
    output.append(width, ' ');
    output = "| " + output + " |";
}

ColumnCell::ColumnCell(string text, size_t width) : TableProp::TableProp(text){
    adjust_print_cols(output, width);
    output = "| " + output + " |";
}

void ColumnCell::adjust_print_cols(string& col, size_t col_width){
    if (col.length() < col_width) // pad with spaces
        col.append(col_width - col.length(), ' ');
    else
        col = col.substr(0, col_width);
}


Header::Header(string* col_names, size_t col_width, size_t width, int num_cols)
    : hline(width), num_cols(num_cols){
    for (int i = 0; i < num_cols; ++i){
        ColumnCell* col_header = new ColumnCell(col_names[i], col_width);
        col_headers.push_back(col_header);
    }
}

Header::Header(string* col_names, size_t col_widths[], size_t width, int num_cols)
    : hline(width), num_cols(num_cols){
    for (int i = 0; i < num_cols; ++i){
        ColumnCell* col_header = new ColumnCell(col_names[i], col_widths[i]);
        col_headers.push_back(col_header);
    }
}

void Header::print(){
    hline.print();
    for (int i = 0; i < num_cols; ++i){
        (*col_headers[i]).print(false);
    }
    hline.print();
}


