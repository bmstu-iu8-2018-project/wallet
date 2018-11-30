#include <includes/CryptoUtils.hpp>
#include <includes/Key.hpp>

namespace cu
{
    std::vector<byte> from_hex_to_bytes(const std::string& hex)
    {
        std::vector<byte> vector_of_bytes;
        for (size_t i = 0; i < hex.length(); i += 2)
        {
            byte ibyte = static_cast<byte>(strtol(hex.substr(i, 2).c_str(), NULL, 16));
            vector_of_bytes.push_back(ibyte);
        }
        return vector_of_bytes;
    };

    std::string from_bytes_to_hex(const_bytes bytes, int length)
    {
        std::string hex;
        for (int i = 0; i < length; i++)
        {
            hex += to_hex(bytes[i]);
        }
        return hex;
    };

    std::vector<byte> to_varint_byte(size_t size)
    {
        std::vector<byte> var_int;
        size_t storage_length;

        if (size < 0xfd)
        {
            var_int.push_back(size);
            return var_int;
        }
        else if (size <= 0xffff)
        {
            var_int.push_back('\xfd');
            storage_length = 3;
        }
        else if (size <= 0xffffffff)
        {
            var_int.push_back('\xfe');
            storage_length = 5;
        }
        else
        {
            var_int.push_back('\xff');
            storage_length = 9;
        }

        var_int.resize(storage_length);

        for (byte i = 1; i < storage_length; ++i)
            var_int[i] = (size >> ((i - 1) * 8)) % 256;

        return var_int;
    }

    uint64_t to_varint(size_t size)
    {
        std::vector<uint8_t> buf = to_bytes(size);
        if (size > 8 && buf[0] == 0xff)
        {
            buf.resize(9);
            return (static_cast<uint64_t>(buf[1]) << 0)
                | (static_cast<uint64_t>(buf[2]) << 8)
                | (static_cast<uint64_t>(buf[3]) << 0x10)
                | (static_cast<uint64_t>(buf[4]) << 0x18)
                | (static_cast<uint64_t>(buf[5]) << 0x20)
                | (static_cast<uint64_t>(buf[6]) << 0x28)
                | (static_cast<uint64_t>(buf[7]) << 0x30)
                | (static_cast<uint64_t>(buf[8]) << 0x38);
        }
        else if (size > 4 && buf[0] == 0xfe)
        {
            buf.resize(5);
            return (static_cast<uint32_t>(buf[1]) << 0)
                | (static_cast<uint32_t>(buf[2]) << 8)
                | (static_cast<uint32_t>(buf[3]) << 0x10)
                | (static_cast<uint32_t>(buf[4]) << 0x18);
        }
        else if (size > 2 && buf[0] == 0xfd)
        {
            buf.resize(3);
            return (static_cast<uint16_t>(buf[1]) << 0)
                | (static_cast<uint16_t>(buf[2]) << 8);
        }
        else if (size > 0 && buf[0] <= 0xfc)
        {
            buf.resize(1);
            return buf[0];
        }
    }

    std::string to_littleendian_format(const std::string& string)
    {
        auto bytes = from_hex_to_bytes(string);
        std::reverse(bytes.begin(), bytes.end());
        return from_bytes_to_hex(bytes); // littleendian format
    }

    void to_littleendian_format(std::vector<byte>& bytes)
    {
        std::reverse(bytes.begin(), bytes.end());
    }

    std::string get_addres_from_script(const std::vector<byte>& bytes)
    {
        std::vector<byte> address;
        address.push_back(TEST_NETWORK);
        address.insert(address.end(), bytes.begin() + 3, bytes.begin() + 23);

        auto tmp_vec = SHA256(SHA256(address));
        address.insert(address.end(), tmp_vec.begin(), tmp_vec.begin() + 4);

        return to_base58(address);
    }

    std::string from_bytes_to_hex(const std::vector<byte>& bytes)
    {
        return from_bytes_to_hex(&bytes[0], bytes.size());
    };

    std::string SHA256(const std::string& string)
    {
        return from_bytes_to_hex(SHA256(from_hex_to_bytes(string)));
    };

    std::string RIPEMD160(const std::string& string)
    {
        const auto byte_string = from_hex_to_bytes(string);

        byte hash[RIPEMD160_DIGEST_LENGTH];
        RIPEMD160_CTX ripemd160;
        RIPEMD160_Init(&ripemd160);
        RIPEMD160_Update(&ripemd160, &byte_string[0], byte_string.size());
        RIPEMD160_Final(hash, &ripemd160);

        return  from_bytes_to_hex(hash, RIPEMD160_DIGEST_LENGTH);
    };

    std::vector<byte> SHA256(const std::vector<byte>& byte_string)
    {
        byte hash[SHA256_DIGEST_LENGTH];
        SHA256_CTX sha256;
        SHA256_Init(&sha256);
        SHA256_Update(&sha256, &byte_string[0], byte_string.size());
        SHA256_Final(hash, &sha256);

        std::vector<byte> a(SHA256_DIGEST_LENGTH);
        std::memcpy(&a[0], &hash, SHA256_DIGEST_LENGTH);

        return a;
    };

    std::vector<byte> RIPEMD160(const std::vector<byte>& byte_string)
    {
        byte hash[RIPEMD160_DIGEST_LENGTH];
        RIPEMD160_CTX ripemd160;
        RIPEMD160_Init(&ripemd160);
        RIPEMD160_Update(&ripemd160, &byte_string[0], byte_string.size());
        RIPEMD160_Final(hash, &ripemd160);

        std::vector<byte> a(RIPEMD160_DIGEST_LENGTH);
        std::memcpy(&a[0], &hash, RIPEMD160_DIGEST_LENGTH);

        return a;
    };

    std::string to_base58(const_bytes pbegin, const_bytes pend)
    {
        // Skip & count leading zeroes.
        int zeroes = 0;
        while (pbegin != pend && *pbegin == 0)
        {
            pbegin++;
            zeroes++;
        }
        // Allocate enough space in big-endian base58 representation.
        std::vector<byte> b58((pend - pbegin) * 138 / 100 + 1); // log(256) / log(58), rounded up.
        // Process the bytes.
        while (pbegin != pend)
        {
            int carry = *pbegin;
            // Apply "b58 = b58 * 256 + ch".
            for (std::vector<byte>::reverse_iterator it = b58.rbegin(); it != b58.rend(); it++)
            {
                carry += 256 * (*it);
                *it = carry % 58;
                carry /= 58;
            }
            assert(carry == 0);
            pbegin++;
        }
        // Skip leading zeroes in base58 result.
        std::vector<byte>::iterator it = b58.begin();
        while (it != b58.end() && *it == 0)
            it++;
        // Translate the result into a string.
        std::string str;
        str.reserve(zeroes + (b58.end() - it));
        str.assign(zeroes, '1');
        while (it != b58.end())
            str += Base58[*(it++)];
        return str;
    };

    std::string to_base58(const std::vector<byte>& vch)
    {
        return to_base58(&vch[0], &vch[0] + vch.size());
    };

    bool from_base58(const char *psz, std::vector<byte>& vch)
    {
        // Skip leading spaces.
        while (*psz && isspace(*psz))
            psz++;
        // Skip and count leading '1's.
        int zeroes = 0;
        while (*psz == '1') {
            zeroes++;
            psz++;
        }
        // Allocate enough space in big-endian base256 representation.
        std::vector<byte> b256(strlen(psz) * 733 / 1000 + 1); // log(58) / log(256), rounded up.
        // Process the characters.
        while (*psz && !isspace(*psz))
        {
            // Decode base58 character
            const char *ch = strchr(Base58, *psz);
            if (ch == NULL)
                return false;
            // Apply "b256 = b256 * 58 + ch".
            int carry = ch - Base58;
            for (std::vector<byte>::reverse_iterator it = b256.rbegin(); it != b256.rend(); it++)
            {
                carry += 58 * (*it);
                *it = carry % 256;
                carry /= 256;
            }
            assert(carry == 0);
            psz++;
        }
        // Skip trailing spaces.
        while (isspace(*psz))
            psz++;
        if (*psz != 0)
            return false;
        // Skip leading zeroes in b256.
        std::vector<byte>::iterator it = b256.begin();
        while (it != b256.end() && *it == 0)
            it++;
        // Copy result into output vector.
        vch.reserve(zeroes + (b256.end() - it));
        vch.assign(zeroes, 0x00);
        while (it != b256.end())
            vch.push_back(*(it++));
        return true;
    };

    bool from_base58(const std::string& str, std::vector<byte>& vchRet)
    {
        return from_base58(str.c_str(), vchRet);
    };

    BIGNUM* get_private_key_from_wif(const std::string& private_key_wif)
    {
        BIGNUM* prv_key = BN_new();
        std::vector<byte> private_k;
        if (!from_base58(private_key_wif, private_k))
        {
            throw std::runtime_error("wrong format of private key");
        }

        private_k.erase(private_k.begin(), private_k.begin() + 1);
        private_k.erase(private_k.end() - 4, private_k.end());

        BN_hex2bn(&prv_key, from_bytes_to_hex(private_k).c_str());

        return prv_key;
    };

    EC_KEY* get_ec_key_from_private(const std::string& private_key_wif)
    {
        EC_KEY* ec_key = EC_KEY_new_by_curve_name(NID_secp256k1);
        BIGNUM* prv_key = get_private_key_from_wif(private_key_wif);
        EC_KEY_set_private_key(ec_key, prv_key);

        BN_CTX* ctx = BN_CTX_new();
        BN_CTX_start(ctx);
        const EC_GROUP* group = EC_KEY_get0_group(ec_key);
        EC_POINT* pub_key = EC_POINT_new(group);
        EC_POINT_mul(group, pub_key, prv_key, NULL, NULL, ctx);
        EC_KEY_set_public_key(ec_key, pub_key);

        EC_POINT_free(pub_key);
        BN_CTX_end(ctx);
        BN_CTX_free(ctx);
        BN_clear_free(prv_key);

        return ec_key;
    };

    EC_KEY* get_ec_key_from_public(const std::string& public_key)
    {
        EC_KEY* ec_key = EC_KEY_new_by_curve_name(NID_secp256k1);
        const EC_GROUP* group = EC_KEY_get0_group(ec_key);
        EC_POINT* pub_key = EC_POINT_new(group);
        BN_CTX* ctx = BN_CTX_new();
        BN_CTX_start(ctx);

        EC_POINT_hex2point(group, public_key.c_str(), pub_key, ctx);
        EC_KEY_set_public_key(ec_key, pub_key);

        EC_POINT_free(pub_key);
        BN_CTX_end(ctx);
        BN_CTX_free(ctx);

        return ec_key;
    };

    ECDSA_SIG* sign(const std::string& private_key, const std::vector<byte>& text)
    {
        const auto hash = SHA256(SHA256(text));

        EC_KEY* ec_key = get_ec_key_from_private(private_key);

        ECDSA_SIG* signature = ECDSA_do_sign(&hash[0], hash.size(), ec_key);
        if (signature == NULL)
        {
            throw std::runtime_error("Failed to generate signature");
        }

        EC_KEY_free(ec_key);

        return signature;
    };

    bool is_validate_signature(const std::string& public_key, const ECDSA_SIG* signature, const std::vector<byte>& text)
    {
        const auto hash = RIPEMD160(SHA256(text));
        
        EC_KEY* ec_key = get_ec_key_from_public(public_key);
        const auto flag = ECDSA_do_verify(&hash[0], hash.size(), signature, ec_key);
        EC_KEY_free(ec_key);

        return flag == 1;
    };

    std::string signature_to_der(const ECDSA_SIG* signature)
    {
        const std::string r = BN_bn2hex(signature->r);
        const std::string s = BN_bn2hex(signature->s);

        std::string der;

        der += to_hex(DER_HEADLINE);

        int size = (r.size() + s.size()) / 2 + 3;
        std::stringstream oss;
        oss << std::hex << size;
        der += oss.str(); //  length of next data

        der += to_hex(BEGIN_OF_NUM);
        der.insert(der.end(), r.begin(), r.end());
        der += to_hex(BEGIN_OF_NUM);
        der.insert(der.end(), s.begin(), s.end());
        der += to_hex(SIGHASH_ALL);

        return der;
    }

    std::vector<byte> signature_to_der_byted(const ECDSA_SIG* signature)
    {
        return from_hex_to_bytes(signature_to_der(signature));
    }

    ECDSA_SIG* from_der_to_sig(const std::string& scriptSig)
    {
        ECDSA_SIG* signature = ECDSA_SIG_new();

        const int begin = 6;
        std::string s = scriptSig.substr(begin, DIGEST_LENGTH * 2);
        std::string r = scriptSig.substr(begin + DIGEST_LENGTH * 2 + 2, DIGEST_LENGTH * 2);

        return signature;
    }

} // namaspase cu
