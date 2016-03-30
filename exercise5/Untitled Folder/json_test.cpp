#include "json/json/json.h"
#include <iostream>
#include <fstream>

int main(){
    Json::Value root;
    std::ifstream testfile("testfile.txt", std::ifstream::binary);
    testfile >> root;
    std::cout << root << std::endl;
    std::cout << root.get("my_plug_ins", "Placeholder")[1];
    std::cout << "Im alive" << std::endl;
    return 0;


}
