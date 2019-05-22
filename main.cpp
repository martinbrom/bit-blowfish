#include <iostream>
#include <cstring>
#include "blowfish.h"

int main(int argc, char** argv) {
//    Blowfish b1("testtest");
//
//    uint64_t pn = 0x0123456789ABCDEF;
//    std::cout << "pn = " << pn << std::endl;
//    std::cout << "pn = 0x0123456789ABCDEF" << std::endl;
//    uint64_t cn = b1.encode(pn);
//    std::cout << "cn = " << cn << std::endl;
//    uint64_t dn = b1.decode(cn);
//    std::cout << "dn = " << dn << std::endl;
//    std::cout << "dn = " << b1.toHex(dn) << std::endl;
//
//    std::cout << "---------------------------------------" << std::endl;
//    Blowfish b2("abcdefghijklmnopqrstuvwx");
//
//    std::string pt = "HeloWrld";
//    std::cout << "pt = \"HeloWrld\"" << std::endl;
//    uint64_t ct = b2.encode(b2.parse(pt));
//    std::cout << "ct = " << ct << std::endl;
//    std::cout << "ct = " << b2.toString(ct) << std::endl;
//    uint64_t dt = b2.decode(ct);
//    std::cout << "dt = " << dt << std::endl;
//    std::cout << "dt = " << b2.toString(dt) << std::endl;

    if (argc != 4) {
        return 1;
    }

    std::string key;
    std::string text;
    std::string flagName, flagValue;
    bool parseText;
    uint64_t number = 0;

    std::string temp = argv[1];
    flagName = temp.substr(0, temp.find("="));
    if (flagName != "--key") {
        std::cout << "Invalid first flag" << std::endl;
        return 1;
    }
    flagValue = temp.substr(temp.find("=") + 1, temp.npos);
    if (flagValue.length() < 4) {
        std::cout << "Invalid key length" << std::endl;
        return 1;
    }
    key = flagValue;

    temp = argv[2];
    flagName = temp.substr(0, temp.find("="));
    if (flagName == "--text") {
        flagValue = temp.substr(temp.find("=") + 1, temp.npos);
        if (flagValue.length() != 8) {
            std::cout << "Invalid second parameter length" << std::endl;
            return 1;
        }
        text = flagValue;
        parseText = true;
    } else if (flagName == "--number") {
        flagValue = temp.substr(temp.find("=") + 1, temp.npos);
        number = std::stoull(flagValue);
        parseText = false;
    } else {
        std::cout << "Invalid second flag" << std::endl;
        return 1;
    }
    text = flagValue;

    temp = argv[3];
    if (temp != "--encode" && temp != "--decode") {
        std::cout << "Invalid third flag" << std::endl;
        return 1;
    }

    Blowfish b(key);

    flagName = argv[3];
    bool encode = flagName == "--encode";
    number = parseText ? b.parse(text) : number;

    uint64_t result = encode
                      ? b.encode(number)
                      : b.decode(number);
    std::cout << result << std::endl;
    std::cout << (encode ? b.toHex(result) : b.toString(result)) << std::endl;

    return 0;
}