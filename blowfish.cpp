#include <iostream>
#include "blowfish.h"

Blowfish::Blowfish(std::string key) {
    int keyLength = (int) key.size();
    for (int i = 0; i < 18; ++i) {
        int j = i * 4;
        uint32_t v = (uint32_t) (
                ((key[j % keyLength]) << 24) +
                ((key[(j + 1) % keyLength]) << 16) +
                ((key[(j + 2) % keyLength]) << 8) +
                (key[(j + 3) % keyLength])
        );
        P[i] ^= v;
    }

    uint32_t l = 0, r = 0;

    for (int i = 0; i < 18; i += 2) {
        encode(l, r);
        P[i] = l;
        P[i + 1] = r;
    }

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 256; ++j) {
            encode(l, r);
            S[i][j] = l;
            S[i][j+1] = r;
        }
    }
}

uint64_t Blowfish::encode(uint64_t number) {
    uint32_t l = 0, r = 0;
    split(number, l, r);
    encode(l, r);
    return ((uint64_t) l << 32) + r;
}

void Blowfish::encode(uint32_t& l, uint32_t& r) {
    for (int i = 0; i < 16; ++i) {
        l ^= P[i];
        r = F(l) ^ r;

        std::swap(l, r);
    }

    std::swap(l, r);
    r ^= P[16];
    l ^= P[17];
}

uint64_t Blowfish::decode(uint64_t number) {
    uint32_t l = 0, r = 0;
    split(number, l, r);
    decode(l, r);
    return ((uint64_t) l << 32) + r;
}

void Blowfish::decode(uint32_t& l, uint32_t& r) {
    for (int i = 17; i > 1; --i) {
        l ^= P[i];
        r = F(l) ^ r;

        std::swap(l, r);
    }

    std::swap(l, r);
    r ^= P[1];
    l ^= P[0];
}

uint64_t Blowfish::parse(std::string text) {
    uint64_t out = 0;
    for (int i = 0; i < 8; ++i) {
        out <<= 8;
        out += (text[i] % 0xFF);
    }

    return out;
}

std::string Blowfish::toString(uint64_t number) {
    std::string out;

    for (int i = 0; i < 8; ++i) {
        out += (uint8_t) (number >> ((7-i) * 8));
    }

    return out;
}

std::string Blowfish::toHex(uint64_t number) {
    std::string out = "0x";

    uint8_t part = 0;
    for (int i = 0; i < 8; ++i) {
        part = (uint8_t) (number >> ((7-i) * 8));
        out += hexChars[part / 16];
        out += hexChars[part % 16];
    }

    return out;
}

void Blowfish::split(uint64_t& n, uint32_t& l, uint32_t& r) {
    l = (uint32_t) (n >> 32);
    r = (uint32_t) (n & 0xFFFFFFFF);
}

uint32_t Blowfish::F(uint32_t l) {
    uint32_t a = (l & 0xFF000000) >> 24;
    uint32_t b = (l & 0x00FF0000) >> 16;
    uint32_t c = (l & 0x0000FF00) >> 8;
    uint32_t d = l & 0x000000FF;

    uint32_t out = (S[0][a] + S[1][b]) % 0xFFFFFFFF;
    out ^= S[2][c];
    out += S[3][d];
    out %= 0xFFFFFFFF;

    return out;
}
