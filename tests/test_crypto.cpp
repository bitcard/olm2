#include "axolotl/crypto.hh"

#include "unittest.hh"

int main() {


{ /* Curve25529 Test Case 1 */

TestCase test_case("Curve25529 Test Case 1");

std::uint8_t alice_private[32] = {
    0x77, 0x07, 0x6D, 0x0A, 0x73, 0x18, 0xA5, 0x7D,
    0x3C, 0x16, 0xC1, 0x72, 0x51, 0xB2, 0x66, 0x45,
    0xDF, 0x4C, 0x2F, 0x87, 0xEB, 0xC0, 0x99, 0x2A,
    0xB1, 0x77, 0xFB, 0xA5, 0x1D, 0xB9, 0x2C, 0x2A
};

std::uint8_t alice_public[32] = {
    0x85, 0x20, 0xF0, 0x09, 0x89, 0x30, 0xA7, 0x54,
    0x74, 0x8B, 0x7D, 0xDC, 0xB4, 0x3E, 0xF7, 0x5A,
    0x0D, 0xBF, 0x3A, 0x0D, 0x26, 0x38, 0x1A, 0xF4,
    0xEB, 0xA4, 0xA9, 0x8E, 0xAA, 0x9B, 0x4E, 0x6A
};

std::uint8_t bob_private[32] = {
    0x5D, 0xAB, 0x08, 0x7E, 0x62, 0x4A, 0x8A, 0x4B,
    0x79, 0xE1, 0x7F, 0x8B, 0x83, 0x80, 0x0E, 0xE6,
    0x6F, 0x3B, 0xB1, 0x29, 0x26, 0x18, 0xB6, 0xFD,
    0x1C, 0x2F, 0x8B, 0x27, 0xFF, 0x88, 0xE0, 0xEB
};

std::uint8_t bob_public[32] = {
    0xDE, 0x9E, 0xDB, 0x7D, 0x7B, 0x7D, 0xC1, 0xB4,
    0xD3, 0x5B, 0x61, 0xC2, 0xEC, 0xE4, 0x35, 0x37,
    0x3F, 0x83, 0x43, 0xC8, 0x5B, 0x78, 0x67, 0x4D,
    0xAD, 0xFC, 0x7E, 0x14, 0x6F, 0x88, 0x2B, 0x4F
};

std::uint8_t expected_agreement[32] = {
    0x4A, 0x5D, 0x9D, 0x5B, 0xA4, 0xCE, 0x2D, 0xE1,
    0x72, 0x8E, 0x3B, 0xF4, 0x80, 0x35, 0x0F, 0x25,
    0xE0, 0x7E, 0x21, 0xC9, 0x47, 0xD1, 0x9E, 0x33,
    0x76, 0xF0, 0x9B, 0x3C, 0x1E, 0x16, 0x17, 0x42
};

axolotl::Curve25519KeyPair alice_pair = axolotl::generate_key(alice_private);

assert_equals(alice_private, alice_pair.private_key, 32);
assert_equals(alice_public, alice_pair.public_key, 32);

axolotl::Curve25519KeyPair bob_pair = axolotl::generate_key(bob_private);

assert_equals(bob_private, bob_pair.private_key, 32);
assert_equals(bob_public, bob_pair.public_key, 32);

std::uint8_t actual_agreement[axolotl::CURVE25519_SHARED_SECRET_LENGTH] = {};

axolotl::curve25519_shared_secret(alice_pair, bob_pair, actual_agreement);

assert_equals(expected_agreement, actual_agreement, 32);

axolotl::curve25519_shared_secret(bob_pair, alice_pair, actual_agreement);

assert_equals(expected_agreement, actual_agreement, 32);

} /* Curve25529 Test Case 1 */


{ /* AES Test Case 1 */

TestCase test_case("AES Test Case 1");

axolotl::Aes256Key key = {};
axolotl::Aes256Iv iv = {};
std::uint8_t input[16] = {};

std::uint8_t expected[32] = {
    0xDC, 0x95, 0xC0, 0x78, 0xA2, 0x40, 0x89, 0x89,
    0xAD, 0x48, 0xA2, 0x14, 0x92, 0x84, 0x20, 0x87,
    0xF3, 0xC0, 0x03, 0xDD, 0xC4, 0xA7, 0xB8, 0xA9,
    0x4B, 0xAE, 0xDF, 0xFC, 0x3D, 0x21, 0x4C, 0x38
};

std::size_t length = axolotl::aes_encrypt_cbc_length(sizeof(input));
assert_equals(std::size_t(32), length);


std::uint8_t actual[32] = {};

axolotl::aes_encrypt_cbc(key, iv, input, sizeof(input), actual);
assert_equals(expected, actual, 32);

length = axolotl::aes_decrypt_cbc(key, iv, expected, sizeof(expected), actual);
assert_equals(std::size_t(16), length);
assert_equals(input, actual, length);

} /* AES Test Case 1 */


{ /* SHA 256 Test Case 1 */

TestCase test_case("SHA 256 Test Case 1");

std::uint8_t input[0] = {};

std::uint8_t expected[32] = {
    0xE3, 0xB0, 0xC4, 0x42, 0x98, 0xFC, 0x1C, 0x14,
    0x9A, 0xFB, 0xF4, 0xC8, 0x99, 0x6F, 0xB9, 0x24,
    0x27, 0xAE, 0x41, 0xE4, 0x64, 0x9B, 0x93, 0x4C,
    0xA4, 0x95, 0x99, 0x1B, 0x78, 0x52, 0xB8, 0x55
};

std::uint8_t actual[32];

axolotl::sha256(input, sizeof(input), actual);

assert_equals(expected, actual, 32);

} /* SHA 256 Test Case 1 */

{ /* HMAC Test Case 1 */

TestCase test_case("HMAC Test Case 1");

std::uint8_t input[0] = {};

std::uint8_t expected[32] = {
    0xb6, 0x13, 0x67, 0x9a, 0x08, 0x14, 0xd9, 0xec,
    0x77, 0x2f, 0x95, 0xd7, 0x78, 0xc3, 0x5f, 0xc5,
    0xff, 0x16, 0x97, 0xc4, 0x93, 0x71, 0x56, 0x53,
    0xc6, 0xc7, 0x12, 0x14, 0x42, 0x92, 0xc5, 0xad
};

std::uint8_t actual[32];

axolotl::hmac_sha256(input, sizeof(input), input, sizeof(input), actual);

assert_equals(expected, actual, 32);

} /* HMAC Test Case 1 */

{ /* HDKF Test Case 1 */

TestCase test_case("HDKF Test Case 1");

std::uint8_t input[22] = {
    0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,
    0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b,
    0x0b, 0x0b, 0x0b, 0x0b, 0x0b, 0x0b
};

std::uint8_t salt[13] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0a, 0x0b, 0x0c
};

std::uint8_t info[10] = {
    0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7,
    0xf8, 0xf9
};

std::uint8_t hmac_expected_output[32] = {
    0x07, 0x77, 0x09, 0x36, 0x2c, 0x2e, 0x32, 0xdf,
    0x0d, 0xdc, 0x3f, 0x0d, 0xc4, 0x7b, 0xba, 0x63,
    0x90, 0xb6, 0xc7, 0x3b, 0xb5, 0x0f, 0x9c, 0x31,
    0x22, 0xec, 0x84, 0x4a, 0xd7, 0xc2, 0xb3, 0xe5,
};

std::uint8_t hmac_actual_output[32] = {};

axolotl::hmac_sha256(
    salt, sizeof(salt),
    input, sizeof(input),
    hmac_actual_output
);

assert_equals(hmac_expected_output, hmac_actual_output, 32);

std::uint8_t hkdf_expected_output[42] = {
    0x3c, 0xb2, 0x5f, 0x25, 0xfa, 0xac, 0xd5, 0x7a,
    0x90, 0x43, 0x4f, 0x64, 0xd0, 0x36, 0x2f, 0x2a,
    0x2d, 0x2d, 0x0a, 0x90, 0xcf, 0x1a, 0x5a, 0x4c,
    0x5d, 0xb0, 0x2d, 0x56, 0xec, 0xc4, 0xc5, 0xbf,
    0x34, 0x00, 0x72, 0x08, 0xd5, 0xb8, 0x87, 0x18,
    0x58, 0x65
};

std::uint8_t hkdf_actual_output[42] = {};

axolotl::hkdf_sha256(
    input, sizeof(input),
    salt, sizeof(salt),
    info, sizeof(info),
    hkdf_actual_output, sizeof(hkdf_actual_output)
);

assert_equals(hkdf_expected_output, hkdf_actual_output, 42);

} /* HDKF Test Case 1 */

}
