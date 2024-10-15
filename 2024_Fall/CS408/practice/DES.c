#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INPUT_SIZE 1000000  // Adjust as needed
// Initial Permutations
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
const char IP[64] = {58, 50, 42, 34, 26, 18, 10, 2,  60, 52, 44, 36, 28,
                     20, 12, 4,  62, 54, 46, 38, 30, 22, 14, 6,  64, 56,
                     48, 40, 32, 24, 16, 8,  57, 49, 41, 33, 25, 17, 9,
                     1,  59, 51, 43, 35, 27, 19, 11, 3,  61, 53, 45, 37,
                     29, 21, 13, 5,  63, 55, 47, 39, 31, 23, 15, 7};
const char IPi[64] = {40, 8,  48, 16, 56, 24, 64, 32, 39, 7,  47, 15, 55,
                      23, 63, 31, 38, 6,  46, 14, 54, 22, 62, 30, 37, 5,
                      45, 13, 53, 21, 61, 29, 36, 4,  44, 12, 52, 20, 60,
                      28, 35, 3,  43, 11, 51, 19, 59, 27, 34, 2,  42, 10,
                      50, 18, 58, 26, 33, 1,  41, 9,  49, 17, 57, 25};
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-

// F(Ri, k)
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
const char e[48] = {32, 1,  2,  3,  4,  5,  4,  5,  6,  7,  8,  9,
                    8,  9,  10, 11, 12, 13, 12, 13, 14, 15, 16, 17,
                    16, 17, 18, 19, 20, 21, 20, 21, 22, 23, 24, 25,
                    24, 25, 26, 27, 28, 29, 28, 29, 30, 31, 32, 1};
const char p[32] = {16, 7, 20, 21, 29, 12, 28, 17, 1,  15, 23,
                    26, 5, 18, 31, 10, 2,  8,  24, 14, 32, 27,
                    3,  9, 19, 13, 30, 6,  22, 11, 4,  25};
const char s1[4][16] = {{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
                        {0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
                        {4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
                        {15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}};
const char s2[4][16] = {{15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
                        {3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
                        {0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
                        {13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}};
const char s3[4][16] = {{10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
                        {13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
                        {13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
                        {1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}};
const char s4[4][16] = {{7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
                        {13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
                        {10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
                        {3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}};
const char s5[4][16] = {{2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
                        {14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
                        {4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
                        {11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}};
const char s6[4][16] = {{12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
                        {10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
                        {9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
                        {4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}};
const char s7[4][16] = {{4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
                        {13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
                        {1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
                        {6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}};
const char s8[4][16] = {{13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
                        {1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
                        {7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
                        {2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}};
const char (*sboxes[8])[16] = {s1, s2, s3, s4, s5, s6, s7, s8};
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-

// Key generator permutations
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
const char pc1[56] = {57, 49, 41, 33, 25, 17, 9,  1,  58, 50, 42, 34, 26, 18,
                      10, 2,  59, 51, 43, 35, 27, 19, 11, 3,  60, 52, 44, 36,
                      63, 55, 47, 39, 31, 23, 15, 7,  62, 54, 46, 38, 30, 22,
                      14, 6,  61, 53, 45, 37, 29, 21, 13, 5,  28, 20, 12, 4};
const char pc2[48] = {14, 17, 11, 24, 1,  5,  3,  28, 15, 6,  21, 10,
                      23, 19, 12, 4,  26, 8,  16, 7,  27, 20, 13, 2,
                      41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48,
                      44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32};
const char keySchedule[] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-

// Utility Functions
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
// Print binary string for blocks
void print_uint48_binary(uint64_t n) {
  int i;
  for (i = 47; i >= 0; i--) {
    putchar((n & ((uint64_t)1 << i)) ? '1' : '0');
  }
  putchar('\n');
}
void print_uint56_binary(uint64_t n) {
  int i;
  for (i = 55; i >= 0; i--) {
    putchar((n & ((uint64_t)1 << i)) ? '1' : '0');
  }
  putchar('\n');
}
void print_uint28_binary(uint64_t n) {
  int i;
  for (i = 27; i >= 0; i--) {
    putchar((n & ((uint64_t)1 << i)) ? '1' : '0');
  }
  putchar('\n');
}
void print_uint64_binary(uint64_t n) {
  int i;
  for (i = 63; i >= 0; i--) {
    putchar((n & ((uint64_t)1 << i)) ? '1' : '0');
  }
  putchar('\n');
}
// Get Bit
char get_bit(uint64_t key, int index) {
  return ((key & ((uint64_t)1 << index)) != 0);
}
// Set Bit
void set_bit(uint64_t *key, int index, char bit) {
  if (bit)
    *key |= (uint64_t)1 << index;
  else
    *key &= ~((uint64_t)1 << index);
}
// Key Shifts
uint32_t shift(uint32_t bit28, int shiftVal) {
  uint32_t result = 0;
  result = (bit28 << shiftVal);
  uint32_t overflow = result >> 28;
  result |= overflow;
  return result & 0xFFFFFFF;
}
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-

// DES Algo functions
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
// Key Schedular
uint64_t *genKeys(uint64_t key) {
  uint64_t *round_keys = malloc(16 * sizeof(uint64_t));
  if (round_keys == NULL)
    return NULL;

  uint64_t keyPrime = 0;
  for (int i = 0; i < 56; i++)
    set_bit(&keyPrime, i, get_bit(key, pc1[i] - 1));

  uint32_t c0 = keyPrime >> 28;
  uint32_t d0 = (uint32_t)(keyPrime & 0xFFFFFFF);

  for (int i = 0; i < 16; i++) {
    uint64_t ci = shift(c0, keySchedule[i]);
    uint64_t di = shift(d0, keySchedule[i]);
    c0 = ci;
    d0 = di;
    uint64_t key_i = (ci << 28) + di;

    uint64_t key_i_perm = 0;
    for (int j = 0; j < 48; j++)
      set_bit(&key_i_perm, j, get_bit(key_i, pc2[j] - 1));
    round_keys[i] = key_i_perm;
  }
  return round_keys;
}

// Round Function
uint32_t f(int ri, uint64_t key) {
  uint64_t riE = 0;
  for (int i = 0; i < 48; i++) {
    set_bit(&riE, i, get_bit(ri, e[i] - 1));
  }

  char B[8] = {0, 0, 0, 0, 0, 0, 0, 0};
  uint64_t b = riE ^ key;
  for (int i = 0; i < 8; i++)
    B[i] = (b >> i * 6) & 0x3F;

  uint32_t C = 0;
  for (int i = 0; i < 8; i++) {
    char row = ((B[i] & 0x20) >> 4) ^ (B[i] & 0x01);
    char col = (B[i] & 0x1E) >> 1;
    uint32_t temp = sboxes[i][row][col];
    C = C ^ (temp << i * 4);
  }
  uint64_t Cperm = 0;
  for (int i = 0; i < 32; i++)
    set_bit(&Cperm, i, get_bit(C, p[i] - 1));
  return Cperm;
}

// Initial and Final Permutation
void des_permutation(uint64_t *PT, const char permutation_table[64]) {
  uint64_t PTip = 0;
  for (int i = 0; i < 64; i++)
    set_bit(&PTip, i, get_bit(*PT, permutation_table[i] - 1));
  *PT = PTip;
}

// Round
void desRound(uint32_t *l0, uint32_t *r0, uint64_t key) {
  uint32_t li = *l0;
  uint32_t ri = *r0;
  *r0 = li ^ f(ri, key);
  *l0 = ri;
}

uint64_t DES_Dk(uint64_t key, uint64_t CT) {
  uint64_t *round_keys = genKeys(key);
  des_permutation(&CT, IP);
  uint32_t l = CT >> 32;
  uint32_t r = CT & 0xFFFFFFFF;

  for (int i = 15; i >= 0; i--) {
    desRound(&l, &r, round_keys[i]);
  }
  CT = r;
  CT = (CT << 32) ^ l;
  // CT = ((uint64_t)l << 32) | r;

  des_permutation(&CT, IPi);
  free(round_keys);
  return CT;
}

uint64_t DES_Ek(uint64_t key, uint64_t PT) {
  uint64_t *round_keys = genKeys(key);
  des_permutation(&PT, IP);
  uint32_t l = PT >> 32;
  uint32_t r = PT & 0xFFFFFFFF;

  for (int i = 0; i < 16; i++) {
    desRound(&l, &r, round_keys[i]);
  }
  PT = r;
  PT = (PT << 32) ^ l;
  // PT = ((uint64_t)l << 32) | r;

  des_permutation(&PT, IPi);
  free(round_keys);
  return PT;
}
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-

// Convert Messge into blocks
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
uint64_t *message_blocks(const char *message, size_t *num_blocks) {
    size_t msgLen = strlen(message);
    *num_blocks = (msgLen + 8) / 8 + (msgLen % 8 == 0 ? 0 : 1);
    size_t padded_len = *num_blocks * 8;

    uint64_t *blocks = calloc(*num_blocks, sizeof(uint64_t));
    if (blocks == NULL)
        return NULL;

    memcpy(blocks, message, msgLen);
    uint8_t *pad_start = (uint8_t *)blocks + msgLen;
    uint8_t pad_value = padded_len - msgLen;
    memset(pad_start, pad_value, pad_value);

    return blocks;
}

char *blocks_to_string(const uint64_t *blocks, size_t num_blocks) {
    size_t max_len = num_blocks * 8;
    
    char *string = malloc(max_len + 1);  // +1 for null terminator
    if (string == NULL) {
        return NULL;
    }

    memcpy(string, blocks, max_len);

    // Find the actual length of the string by checking the padding
    size_t actual_len = max_len;
    while (actual_len > 0 && string[actual_len - 1] == string[max_len - 1]) {
        actual_len--;
    }

    string[actual_len] = '\0';
    return string;
}

// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
int main() {
    uint64_t key = 0x133457799BBCDFF1;
    printf("key: ");
    print_uint64_binary(key);
    printf("\n");

    char* message = malloc(MAX_INPUT_SIZE);
    if (message == NULL) {
        fprintf(stderr, "Failed to allocate memory\n");
        return 1;
    }

    size_t message_len = 0;
    int c;
    while ((c = getchar()) != EOF && message_len < MAX_INPUT_SIZE - 1) {
        message[message_len++] = c;
    }
    message[message_len] = '\0';

    size_t num_blocks;
    uint64_t *blocks = message_blocks(message, &num_blocks);
    if (blocks == NULL) {
        fprintf(stderr, "Failed to create message blocks\n");
        free(message);
        return 1;
    }

    // Encrypt
    for (size_t i = 0; i < num_blocks; i++) {
        blocks[i] = DES_Ek(key, blocks[i]);
    }
    char *encryptedMsg = blocks_to_string(blocks, num_blocks);
    printf("Encrypted Message: %s\n", encryptedMsg);

    // Decrypt
    for (size_t i = 0; i < num_blocks; i++) {
        blocks[i] = DES_Dk(key, blocks[i]);
    }
    char *decryptedMsg = blocks_to_string(blocks, num_blocks);
    printf("Decrypted Message: %s\n", decryptedMsg);

    free(blocks);
    free(message);
    free(encryptedMsg);
    free(decryptedMsg);
    return 0;
}
