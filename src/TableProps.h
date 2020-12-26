/*
 * @author Calvin Huang, 2020
 * 
 * This file is part of Autoinstall Manager.
 *
 * See the LICENSE file at the top-level directory of this distribution
 * for details of code ownership.
 */

#include <string>
#include <vector>
#include <iostream>

using std::string;
using std::vector;


class TableProp {
    public:
        virtual void print(std::ostream& out = std::cout, bool save = false, bool newline = true);
    protected:
        TableProp();
        TableProp(string input);
        string output;
};


class TitleBar: public TableProp {
    public:
        TitleBar(string title, size_t width);
};


class HLine: public TableProp {
    public:
        HLine(size_t width);
};


class ColumnCell: public TableProp {
    public:
        ColumnCell(size_t width); // empty cell
        ColumnCell(string text, size_t width); // cell with text
    protected:
        void adjust_print_cols(string& col, size_t col_width);
};


class Header: public TableProp {
    public:
        Header(string* col_names, size_t col_width, size_t width, int num_cols); // all col same width
        Header(string* col_names, size_t col_widths[], size_t width, int num_cols);
        void print(std::ostream& out = std::cout, bool save = false);
    private:
        vector<ColumnCell*> col_headers;
        HLine hline;
        int num_cols;
};