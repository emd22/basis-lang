#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <basis/file.hpp>
#include <basis/lexer.hpp>
#include <basis/parser.hpp>


int main(int argc, char **argv) {
    if (argc == 2) {
        std::string filename = argv[1];
        
        std::ifstream file;
        file.open(filename, std::ios::in | std::ios::binary | std::ios::ate);

        size_t filesize = file.tellg();
        file.seekg(0, std::ios::beg);

        // create buffer
        char *data = new char[filesize + 1];
        data[filesize] = '\0';

        // read into buffer
        file.read(data, filesize);

        std::vector<char> data_vec(&data[0], &data[filesize]);

        basis::File file_data;
        file_data.filename = filename;
        file_data.data = data_vec;

        basis::Lexer lex(file_data);
        auto tokens = lex.Lex();
        
        basis::Parser parser(tokens);
        parser.Parse();

        delete[] data;


    } else {
        std::cout << "Usage: " << argv[0] << " <filename>\n";
    }
    
    return 0;

}
