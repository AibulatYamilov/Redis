#include <fstream>
#include <cstdio>
#include <string>
#include <iostream>
#include <map>
#include <string>
#include <cstdlib>

const char data_file[] = "data.txt";
const char log_file[] = "log.txt";
const char temporary_file[] = "temporary.txt";

class Redis_API {
private:
    std::map<std::string, std::string> data;
    std::map<std::string, std::string> logs;

public:
    void read_log_file() {
        std::fstream file;
        file.open(log_file, std::fstream::in);
        std::string key, value;
        while (!file.eof()) {
            file >> key >> value;
            logs.emplace(key, value);
        }
        file.close();
    }

    void read_data_file() {
        std::fstream file;
        file.open(data_file, std::fstream::in);
        std::string key, value;
        while (!file.eof()) {
            file >> key >> value;
            data.emplace(key, value);
        }
        file.close();
    }

    void change_data() {
        for (auto it = logs.begin(); it != logs.end(); ++it)
            data[it->first] = it->second;
    }

    void move_to_data_file() {
        std::fstream infile;
        infile.open(temporary_file, std::fstream::in);
        if (infile.eof()) {
            infile.close();
            return;
        }
        infile.close();
        std::system("cp temporary.txt data.txt");
    }

    void write_data_file() {
        std::fstream file;
        file.open(temporary_file, std::fstream::out | std::fstream::trunc);
        for (auto it = data.begin(); it != data.end(); ++it) {
            file << it->first << " " << it->second << std::endl;
        }
        move_to_data_file();
        file.close();
    }

    void write_log_file() {
        std::fstream file;
        file.open(log_file, std::fstream::out | std::fstream::trunc);
        for (auto it = logs.begin(); it != logs.end(); ++it) {
            file << it->first << " " << it->second << std::endl;
        }
        file.close();
    }



    Redis_API() {
        read_log_file();
        read_data_file();
        change_data();
        write_data_file();
    }

    std::string get(const std::string key) {
        if (data.find(key) == data.end())
            return "Key is not found!";
        return data[key];
    }

    void set(const std::string key, const std::string value) {
        logs[key] = value;
        write_log_file();
        change_data();
        write_data_file();
    }

    void command() {
        std::string value, key;
        std::string cmd;
        while (std::cin >> cmd) {
            if (cmd == "get") {
                std::cin >> key;
                std::cout << get(key) << std::endl;
                continue;
            }
            if (cmd == "set") {
                std::cin >> key >> value;
                set(key, value);
                std::cout << "OK" << std::endl;
                continue;
            }
            std::cout << "Command is not correct!" << std::endl;
        }
    }

};
