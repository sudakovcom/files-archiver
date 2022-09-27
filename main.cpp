#include <iostream>
#include <stdexcept>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <cmath>
#include <utility>

bool CheckLastSuf(std::string &string) {
    char c = string.back();
    return (c == string[string.size() - 1] && c == string[string.size() - 2] && c == string[string.size() - 3] && c == string[string.size() - 4]);
}

void pack_rle_debug(const std::string &input_text, std::string &output_text) {


    size_t cur_len = 0;
    std::string cur_string;
    bool is_equal = false;
    for (auto c: input_text) {

        if (cur_len >= 4) {
            if (cur_len == 999999999) {
                output_text += std::to_string(9);
                output_text += "-999999999";
                cur_len = 1;
                cur_string = c;
            } else {
                if (is_equal) {
                    if (c == cur_string[0]) {
                        cur_string += c;
                        cur_len += 1;
                    } else {
                        is_equal = false;
                        output_text += std::to_string(int(std::ceil(std::log10(static_cast<double>(cur_len) + 0.1))));
                        output_text += std::to_string(cur_len);
                        output_text += cur_string[0];
                        cur_len = 1;
                        cur_string = c;
                    }
                } else {
                    if (CheckLastSuf(cur_string) && cur_len != 4) {
                        output_text += std::to_string(int(std::ceil(std::log10(static_cast<double>(cur_len - 4) + 0.1))));
                        output_text += '-';
                        output_text += std::to_string(cur_len - 4);
                        output_text += cur_string.substr(0, cur_len - 4);

                        cur_string = cur_string.substr(cur_len - 4, 4);
                        cur_len = 4;
                        is_equal = true;


                        if (c == cur_string[0]) {
                            cur_string += c;
                            cur_len += 1;
                        } else {
                            is_equal = false;
                            output_text += std::to_string(int(std::ceil(std::log10(static_cast<double>(cur_len) + 0.1))));
                            output_text += std::to_string(cur_len);
                            output_text += cur_string[0];
                            cur_len = 1;
                            cur_string = c;
                        }
                    } else {
                        cur_len += 1;
                        cur_string += c;
                    }
                }
            }
        } else {
            cur_len += 1;
            cur_string += c;
            if (CheckLastSuf(cur_string)) {
                is_equal = true;
            }
        }
    }

    if (cur_len != 0) {
        if (!is_equal) {
            output_text += std::to_string(int(std::ceil(std::log10(static_cast<double>(cur_len) + 0.1))));
            output_text += '-';
            output_text += std::to_string(cur_len);
            output_text += cur_string;
        } else {
            output_text += std::to_string(int(std::ceil(std::log10(static_cast<double>(cur_len) + 0.1))));
            output_text += std::to_string(cur_len);
            output_text += cur_string[0];
        }
    }


}

void unpack_rle_debug(const std::string &input_text, std::string &output_text) {

    int q = 0;
    int len_q;
    bool is_equal = true;
    for (size_t i = 0; i < input_text.size();) {
        len_q = int(input_text[i]) - int('0');
        i += 1;
        is_equal = true;
        if (input_text[i] == '-') {
            i += 1;
            is_equal = false;
        }
        for (int j = 0; j < len_q; ++j) {
            q += (int(input_text[i]) - int('0')) * int(round(std::pow(10, len_q - 1 - j)));
            i += 1;
        }
        if (is_equal) {
            for (size_t j = 0; j < q; ++j) {
                output_text += input_text[i];
            }
            i += 1;
        } else {
            for (size_t j = 0; j < q; ++j) {
                output_text += input_text[i];
                i += 1;
            }
        }
        is_equal = true;
        q = 0;
    }

}


void pack_lwz(const std::string &input_file, const std::string &output_file) {
    std::cout << "pack(\"" << input_file << "\", \"" << output_file << "\")" << std::endl;

    std::ifstream file(input_file, std::ios_base::in);
    std::string input_text;
    std::string output_text;

    if (!file.is_open()) {
        std::cout << "ERROR: input file can't be opened!\n";
        return;
    } else {
        std::cout << "--Input file is being read...\n";
        char c;
        while (!file.eof()) {
            file >> c;
            input_text += c;
        }
        file.close();
        std::cout << "--Input file was read.\n";
    }


    std::map<std::string, int> map;
    std::string str;
    for (int i = 0; i < 128; ++i) {
        str += char(i);
        map[str] = i;
        str = "";
    }
    std::vector<int> codes;

    for (char i: input_text) {
        str += i;
        if (map.find(str) == map.end()) {
            str.pop_back();
            codes.push_back(map[str]);
            str += i;
            map[str] = map.size();
            str = "";
            str += i;
        }
    }

    codes.push_back(map[str]);

    for (auto el: codes) {
        output_text += std::to_string(el) + " ";
    }
    output_text.pop_back();


    std::ofstream file1;
    file1.open(output_file, std::ios::out);
    if (!file1.is_open()) {
        std::cout << "ERROR: output file can't be opened!\n";
        return;
    } else {
        std::cout << "--Output file is being generating...\n";
        for (auto el: output_text) {
            file1 << el;
        }
        file1.close();
        std::cout << "--Output file was generated.\n";
    }

}

void unpack_lwz(const std::string &input_file, const std::string &output_file) {
    std::cout << "unpack(\"" << input_file << "\", \"" << output_file << "\")" << std::endl;

    std::ifstream file(input_file, std::ios_base::in);
    std::string input_text;
    std::string output_text;

    if (!file.is_open()) {
        std::cout << "ERROR: input file can't be opened!\n";
        return;
    } else {
        std::cout << "--Input file is being read...\n";
        char c;
        while (!file.eof()) {
            file >> c;
            input_text += c;
        }
        file.close();
        std::cout << "--Input file was read.\n";
    }


    std::vector<int> codes;
    std::string cd;
    for (auto el: input_text) {
        if (el == ' ') {
            codes.push_back(std::stoi(cd));
            cd = "";
        } else {
            cd += el;
        }
    }
    codes.push_back(std::stoi(cd));


    std::map<std::string, int> map;
    std::map<int, std::string> map1;
    std::string str;
    for (int i = 0; i < 128; ++i) {
        str += char(i);
        map[str] = i;
        map1[i] = str;
        str = "";
    }


    str = "";
    for (int i = 0; i < codes.size(); ++i) {
        if (map1.find(codes[i]) != map1.end()) {
            output_text += map1[codes[i]];
            if (map.find(str + map1[codes[i]][0]) == map.end()) {
                map1[map1.size()] = str + map1[codes[i]][0];
                str = map1[codes[i]];
            } else {
                str += map1[codes[i]];
            }
        } else {
            str += str[0];
            output_text += str;
            map1[map1.size()] = str;
        }
    }

    std::ofstream file1;
    file1.open(output_file, std::ios::out);
    if (!file1.is_open()) {
        std::cout << "ERROR: output file can't be opened!\n";
        return;
    } else {
        std::cout << "--Output file is being generating...\n";
        for (auto el: output_text) {
            file1 << el;
        }
        file1.close();
        std::cout << "--Output file was generated.\n";
    }
}

void pack_rle(const std::string &input_file, const std::string &output_file) {
    std::cout << "pack(\"" << input_file << "\", \"" << output_file << "\")" << std::endl;

    std::ifstream file(input_file, std::ios_base::in);
    std::string input_text;
    std::string output_text;

    if (!file.is_open()) {
        std::cout << "ERROR: input file can't be opened!\n";
        return;
    } else {
        std::cout << "--Input file is being read...\n";
        char c;
        while (!file.eof()) {
            file.get(c);
            input_text += c;
        }
        input_text.pop_back();
        file.close();
        std::cout << "--Input file was read.\n";
    }

    pack_rle_debug(input_text, output_text);

    std::ofstream file1;
    file1.open(output_file, std::ios::out);
    if (!file1.is_open()) {
        std::cout << "ERROR: output file can't be opened!\n";
        return;
    } else {
        std::cout << "--Output file is being generating...\n";

        for (auto el: output_text) {
            file1 << el;
        }

        file1.close();
        std::cout << "--Output file was generated.\n";
    }
}

void unpack_rle(const std::string &input_file, const std::string &output_file) {
    std::cout << "unpack(\"" << input_file << "\", \"" << output_file << "\")" << std::endl;

    std::ifstream file(input_file, std::ios_base::in);
    std::string input_text;
    std::string output_text;

    if (!file.is_open()) {
        std::cout << "ERROR: input file can't be opened!\n";
        return;
    } else {
        std::cout << "--Input file is being read...\n";
        char c;
        while (!file.eof()) {
            file.get(c);
            input_text += c;
        }
        input_text.pop_back();
        file.close();
        std::cout << "--Input file was read.\n";
    }


    unpack_rle_debug(input_text, output_text);

    std::ofstream file1;
    file1.open(output_file, std::ios::out);
    if (!file1.is_open()) {
        std::cout << "ERROR: output file can't be opened!\n";
        return;
    } else {
        std::cout << "--Output file is being generating...\n";
        for (auto el: output_text) {
            file1 << el;
        }
        file1.close();
        std::cout << "--Output file was generated.\n";
    }
}


struct Args {
    std::string input_file;
    std::string output_file;
    enum Command {
        PACK_LWZ,
        UNPACK_LWZ,
        PACK_RLE,
        UNPACK_RLE
    } command;
};

Args parse_args(int argc, const char **argv) {

    if (argc != 5) {
        throw std::runtime_error("too few command line arguments");
    }
    Args result = {};
    if (argv[1] != std::string("pack") && argv[1] != std::string("unpack")) {
        throw std::runtime_error(std::string("unknown command: ") + argv[1]);
    }
    result.command = Args::PACK_LWZ;
    if (argv[1] == std::string("unpack")) {
        if (argv[2] == std::string("lwz")) {
            result.command = Args::UNPACK_LWZ;
        }
        if (argv[2] == std::string("rle")) {
            result.command = Args::UNPACK_RLE;
        }
    }

    if (argv[1] == std::string("pack")) {
        if (argv[2] == std::string("lwz")) {
            result.command = Args::PACK_LWZ;
        }
        if (argv[2] == std::string("rle")) {
            result.command = Args::PACK_RLE;
        }
    }

    result.input_file = argv[3];
    if (result.input_file.empty()) {
        throw std::runtime_error("no input file specified");
    }
    result.output_file = argv[4];
    if (result.output_file.empty()) {
        throw std::runtime_error("no output file specified");
    }
    return result;
}

int main(int argc, const char **argv) {
    try {
        Args args = parse_args(argc, argv);

        if (args.command == Args::PACK_LWZ) {
            pack_lwz(args.input_file, args.output_file);
            return 0;
        }

        if (args.command == Args::UNPACK_LWZ) {
            unpack_lwz(args.input_file, args.output_file);
            return 0;
        }

        if (args.command == Args::PACK_RLE) {
            pack_rle(args.input_file, args.output_file);
            return 0;
        }

        if (args.command == Args::UNPACK_RLE) {
            unpack_rle(args.input_file, args.output_file);
            return 0;
        }

    } catch (const std::exception &e) {
        std::cerr << "An error has occured: " << e.what()
                  << std::endl;
        std::cerr << "Usage: " << argv[0]
                  << " <pack|unpack> <input_file> <output_file>"
                  << std::endl;
        return 1;
    }
    return 0;
}