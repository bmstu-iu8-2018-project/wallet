#include "src/CryptoUtils.cpp"
#include "src/http_client_async_ssl.cpp"
#include "src/Key.cpp"
#include "src/NetworkUtils.cpp"
#include "src/OfflineWallet.cpp"
#include "src/Script.cpp"
#include "src/Transaction.cpp"
#include "src/TxIn.cpp"
#include "src/TxOut.cpp"

namespace {

TEST(CryptoUtilsTest, Convertation)
{
    const std::vector<byte> bytes = { 0xD0, 0xF1, 0xAF };
    const std::string hex = "d0f1af";

    EXPECT_EQ(bytes, cu::from_hex_to_bytes(hex));
    EXPECT_EQ(cu::from_bytes_to_hex(bytes), hex);

    std::string hex_not_lef = "aff1d0";
    cu::to_littleendian_format(hex_not_lef);
    EXPECT_EQ(cu::to_littleendian_format(hex_not_lef), hex);

    const auto hex_hash160 = cu::RIPEMD160(cu::SHA256(hex));
    const auto hash160 = cu::RIPEMD160(cu::SHA256(bytes));

    EXPECT_EQ(cu::from_bytes_to_hex(hash160), hex_hash160);

    const auto bytes_in58 = cu::to_base58(bytes);
    std::vector<byte> buff;
    EXPECT_TRUE(cu::from_base58(bytes_in58, buff));
    EXPECT_EQ(buff, bytes);
}

TEST(KeyTest, Generate)
{
    Key main_key;
    ASSERT_NO_THROW(main_key.create_wallet_key(MAIN_NETWORK));
    Key test_key;
    ASSERT_NO_THROW(test_key.create_wallet_key(TEST_NETWORK));
}

TEST(OfflineWalletTest, Address)
{
    OfflineWallet test_wallet;
    ASSERT_NO_THROW(test_wallet.create_wallet(TEST_NETWORK));
    OfflineWallet main_wallet;
    ASSERT_NO_THROW(main_wallet.create_wallet(MAIN_NETWORK));

    const auto test_address = test_wallet.get_address();

    auto public_key = cu::to_hex(TEST_NETWORK) +
        cu::RIPEMD160(cu::SHA256(test_wallet.get_public_key()));
    const auto check_sum = cu::SHA256(cu::SHA256(public_key));
    public_key.insert(public_key.end(), check_sum.begin(), check_sum.begin() + 8);
    const auto address = cu::to_base58(cu::from_hex_to_bytes(public_key));
    EXPECT_EQ(address, test_address);

    const auto main_address = main_wallet.get_address();

    auto public_key1 = cu::to_hex(MAIN_NETWORK) + 
        cu::RIPEMD160(cu::SHA256(main_wallet.get_public_key()));
    const auto check_sum1 = cu::SHA256(cu::SHA256(public_key1));
    public_key1.insert(public_key1.end(), check_sum1.begin(), check_sum1.begin() + 8);
    const auto address_main = cu::to_base58(cu::from_hex_to_bytes(public_key1));
    EXPECT_EQ(address_main, main_address);
}

TEST(CryptoUtilsTest, SignTest)
{
    OfflineWallet our_wallet;
    ASSERT_NO_THROW(our_wallet.create_wallet(TEST_NETWORK));
    const auto public_key = our_wallet.get_public_key();
    const auto wif_prvivate_key = our_wallet.get_private_key();

    OfflineWallet alien_wallet;
    ASSERT_NO_THROW(alien_wallet.create_wallet(TEST_NETWORK));
    const auto alien_key = alien_wallet.get_public_key();
    
    std::vector<byte> randombytes = { 0x00, 0xed, 0xa3, 0x34, 0xff, 0x12 };
    const auto signature = cu::sign(wif_prvivate_key, randombytes);
    EXPECT_TRUE(cu::is_validate_signature(public_key, signature, randombytes));
    EXPECT_FALSE(cu::is_validate_signature(alien_key, signature, randombytes));
}

TEST(TransactionTest, Parsing)
{
    //----------------------------input----------------------------------
    TxIn input;
    input.set_previous_output("413eeed1a70fe02d8f8c73e49672905f374ccabe3697334718ff01fda19e9640", 0);
    input.set_default_sequance();
    Script input_script;
    input_script.create_pkh_script("mhKokvEkgqMjJ2fE1vCcoLjA3iVYmoPbnN");
    input.set_pkh_script(input_script);
    //----------------------------outputs----------------------------------
    Script output_script;
    output_script.create_pkh_script("mv4rnyY3Su5gjcDNzbMLKBQkBicCtHUtFB");
    TxOut output_0(0.05, output_script);
    Script output_script_1;
    output_script_1.create_pkh_script("mhKokvEkgqMjJ2fE1vCcoLjA3iVYmoPbnN");
    TxOut output_1(0.06, output_script_1);
    //--------------------------------------------------------------------
    outputs vec_out;
    vec_out.push_back(output_0);
    vec_out.push_back(output_1);
    inputs vec_in;
    vec_in.push_back(input);
    //--------------------------------------------------------------------
    Transaction tx(1, vec_in, vec_out, 0);
    std::ofstream ofs("C:\Transaction.txt");
    ofs << tx.get_hex_tx();
    ofs.close();
    const auto t = Transaction::parse("C:\Transaction.txt");
    
    EXPECT_EQ(tx.get_byte_tx(), t.get_byte_tx());
    EXPECT_EQ(tx.get_hex_tx(), t.get_hex_tx());
    EXPECT_EQ(tx.get_in_count(), t.get_in_count());
    EXPECT_EQ(tx.get_out_count(), t.get_out_count());
    EXPECT_EQ(tx.get_locktime(), t.get_locktime());
    EXPECT_EQ(tx.get_version(), t.get_version());

    const auto tx_inputs = tx.get_inputs();
    const auto t_inputs = t.get_inputs();
    for (size_t i = 0; i < tx_inputs.size(); ++i)
    {
        EXPECT_EQ(tx_inputs[i].get_sequance(), t_inputs[i].get_sequance());
        EXPECT_EQ(tx_inputs[i].get_hex_input(), t_inputs[i].get_hex_input());
        EXPECT_EQ(tx_inputs[i].get_byte_input(), t_inputs[i].get_byte_input());
    }

    const auto tx_outputs = tx.get_outputs();
    const auto t_outputs = t.get_outputs();
    for (size_t i = 0; i < tx_outputs.size(); ++i)
    {
        EXPECT_EQ(tx_outputs[i].get_hex_output(), t_outputs[i].get_hex_output());
        EXPECT_EQ(tx_outputs[i].get_byte_output(), t_outputs[i].get_byte_output());
        EXPECT_EQ(tx_outputs[i].get_value(), t_outputs[i].get_value());
        EXPECT_EQ(tx_outputs[i].get_script().get_length(), t_outputs[i].get_script().get_length());
        EXPECT_EQ(tx_outputs[i].get_script().data(), t_outputs[i].get_script().data());
    }
}

TEST(NetworkTest, Requests)
{
    OfflineWallet w;
    ASSERT_NO_THROW(w.create_wallet(TEST_NETWORK));
    EXPECT_TRUE(nu::get_address_balance(w.get_address()).find("success") != std::string::npos);
    EXPECT_TRUE(nu::get_unspent_transactions(w.get_address()).find("success") != std::string::npos);
    EXPECT_TRUE(nu::get_spent_transactions(w.get_address()).find("success") != std::string::npos);
    EXPECT_TRUE(nu::get_received_transactions(w.get_address()).find("success") != std::string::npos);
}
}  // namespace