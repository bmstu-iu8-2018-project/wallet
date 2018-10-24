#include "CryptoUtils.h"

std::vector<byte> cu::from_hex_to_bytes(const std::string& hex)
{
    std::vector<byte> vector_of_bytes;
    for (size_t i = 0; i < hex.length(); i += 2)
    {
        std::string byteString = hex.substr(i, 2);
        byte ibyte = static_cast<byte>(strtol(byteString.c_str(), NULL, 16));
        vector_of_bytes.push_back(ibyte);
    }
    return vector_of_bytes;
}

std::string cu::to_hex(unsigned char s)
{
    std::stringstream ss;
    ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(s);

    return ss.str();
};

std::string cu::SHA256(const std::string& string)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, string.c_str(), string.length());
    SHA256_Final(hash, &sha256);

    std::string output;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
        output += to_hex(hash[i]);

    return output;
};

std::string cu::RIPEMD160(const std::string& string)
{
    unsigned char hash[RIPEMD160_DIGEST_LENGTH];
    RIPEMD160_CTX ripemd160;
    RIPEMD160_Init(&ripemd160);
    RIPEMD160_Update(&ripemd160, string.c_str(), string.length());
    RIPEMD160_Final(hash, &ripemd160);

    std::string output;
    for (int i = 0; i < RIPEMD160_DIGEST_LENGTH; i++)
        output += to_hex(hash[i]);

    return  output;
};

std::string cu::to_base58(const unsigned char* pbegin, const unsigned char* pend)
{
    // Skip & count leading zeroes.
    int zeroes = 0;
    while (pbegin != pend && *pbegin == 0)
    {
        pbegin++;
        zeroes++;
    }
    // Allocate enough space in big-endian base58 representation.
    std::vector<unsigned char> b58((pend - pbegin) * 138 / 100 + 1); // log(256) / log(58), rounded up.
    // Process the bytes.
    while (pbegin != pend)
    {
        int carry = *pbegin;
        // Apply "b58 = b58 * 256 + ch".
        for (std::vector<unsigned char>::reverse_iterator it = b58.rbegin(); it != b58.rend(); it++)
        {
            carry += 256 * (*it);
            *it = carry % 58;
            carry /= 58;
        }
        assert(carry == 0);
        pbegin++;
    }
    // Skip leading zeroes in base58 result.
    std::vector<unsigned char>::iterator it = b58.begin();
    while (it != b58.end() && *it == 0)
        it++;
    // Translate the result into a string.
    std::string str;
    str.reserve(zeroes + (b58.end() - it));
    str.assign(zeroes, '1');
    while (it != b58.end())
        str += cu::Base58[*(it++)];
    return str;
};

std::string cu::to_base58(const std::vector<unsigned char>& vch)
{
    return cu::to_base58(&vch[0], &vch[0] + vch.size());
};
