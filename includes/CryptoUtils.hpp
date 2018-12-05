#ifndef CRYPTO_UTILS_HPP
#define CRYPTO_UTILS_HPP

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <exception>
#include <iomanip>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include <openssl/obj_mac.h>
#include <openssl/ecdsa.h>
#include <openssl/ec.h>
#include <openssl/bn.h>
#include <openssl/ripemd.h>
#include <openssl/sha.h>

using byte = unsigned char;
using const_bytes = const unsigned char*;

const int DIGEST_LENGTH = 32;
const byte DER_HEADLINE = 0x30;
const byte BEGIN_OF_NUM = 0x02;
const byte SIGHASH_ALL = 0x01;  // signature is valid for all exits
// if not static the file is included more than once and occurs multiple definition
static const char* Base58 = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";

namespace cu
{
    std::vector<byte> from_hex_to_bytes(const std::string& hex);

    std::string from_bytes_to_hex(const_bytes bytes, int length);

    std::string from_bytes_to_hex(const std::vector<byte>& bytes);

    std::string to_littleendian_format(const std::string& string);

    void to_littleendian_format(std::vector<byte>& bytes);

    std::vector<byte> to_varint_byte(size_t n);

    uint64_t to_varint(size_t size);

    std::string get_addres_from_script(const std::vector<byte>& bytes);
    
    std::string SHA256(const std::string& string);

    std::string RIPEMD160(const std::string& string);

    std::vector<byte> SHA256(const std::vector<byte>& string);

    std::vector<byte> RIPEMD160(const std::vector<byte>& string);

    std::string to_base58(const_bytes pbegin, const_bytes pend);

    std::string to_base58(const std::vector<byte>& vch);

    bool from_base58(const char *psz, std::vector<byte>& vch);

    bool from_base58(const std::string& str, std::vector<byte>& vchRet);

    BIGNUM* get_private_key_from_wif(const std::string& private_key_wif);

    EC_KEY* get_ec_key_from_private(const std::string& private_key_wif);

    EC_KEY* get_ec_key_from_public(const std::string& public_key);

    ECDSA_SIG* sign(const std::string& private_key, const std::vector<byte>& text);

    bool is_validate_signature(const std::string& public_key, const ECDSA_SIG* signature, const std::vector<byte>& text);

    std::string signature_to_der(const ECDSA_SIG* signature);

    std::vector<byte> signature_to_der_byted(const ECDSA_SIG* signature);

    ECDSA_SIG* from_der_to_sig(const std::string& scriptSig);

    template<typename T>
    std::vector<byte> to_bytes(T i)
    {
        static_assert(std::is_fundamental<T>::value, "not a fundamental type");

        std::vector<byte> a(sizeof(i));
        std::memcpy(&a[0], &i, sizeof(i));
        return a;
    }

    template<typename T>
    std::string to_hex(T s)
    {
        static_assert(std::is_fundamental<T>::value, "not a fundamental type");

        std::stringstream ss;
        ss << std::hex << std::setw(2 * sizeof(T)) << std::setfill('0') << static_cast<int>(s);
        return ss.str();
    }

    template<typename T, class It>
    T to_type(It begin, It end)
    {
        static_assert(std::is_fundamental<T>::value, "not a fundamental type");
        using this_type = std::remove_reference<decltype(*begin)>::type;
        using data = std::remove_const<this_type>::type;
        static_assert(std::is_same<data, byte>::value, "not a chunk");

        T x;
        auto size = std::distance(begin, end);
        std::memcpy(&x, &(*begin), size);
        return x;
    }

} // namespace cu

#endif // CRYPTO_UTILS_HPP
