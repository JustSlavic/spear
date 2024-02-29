typedef struct Character {
  int codePoint, x, y, width, height, originX, originY;
} Character;

typedef struct Font {
  const char *name;
  int size, bold, italic, width, height, characterCount;
  Character *characters;
} Font;

static Character characters_Arial[] = {
  {' ', 272, 106, 3, 3, 1, 1},
  {'!', 130, 81, 7, 25, -1, 24},
  {'"', 178, 106, 12, 10, 0, 24},
  {'#', 312, 31, 20, 25, 1, 24},
  {'$', 117, 0, 19, 29, 1, 25},
  {'%', 194, 0, 28, 25, 0, 24},
  {'&', 97, 31, 22, 25, 0, 24},
  {'\'', 197, 106, 6, 10, 0, 24},
  {'(', 59, 0, 11, 31, 0, 24},
  {')', 70, 0, 11, 31, 0, 24},
  {'*', 164, 106, 14, 12, 1, 24},
  {'+', 109, 106, 19, 17, 0, 20},
  {',', 190, 106, 7, 10, -1, 4},
  {'-', 231, 106, 12, 5, 1, 11},
  {'.', 243, 106, 7, 5, -1, 4},
  {'/', 68, 81, 13, 25, 2, 24},
  {'0', 116, 56, 18, 25, 0, 24},
  {'1', 107, 81, 12, 25, -2, 24},
  {'2', 40, 56, 19, 25, 1, 24},
  {'3', 134, 56, 18, 25, 0, 24},
  {'4', 59, 56, 19, 25, 1, 24},
  {'5', 152, 56, 18, 25, 0, 24},
  {'6', 78, 56, 19, 25, 1, 24},
  {'7', 170, 56, 18, 25, 0, 24},
  {'8', 188, 56, 18, 25, 0, 24},
  {'9', 206, 56, 18, 25, 0, 24},
  {':', 64, 106, 7, 19, -1, 18},
  {';', 158, 81, 7, 24, -1, 18},
  {'<', 71, 106, 19, 18, 0, 20},
  {'=', 145, 106, 19, 12, 0, 17},
  {'>', 90, 106, 19, 18, 0, 20},
  {'?', 224, 56, 18, 25, 0, 24},
  {'@', 0, 0, 33, 31, 0, 24},
  {'A', 222, 0, 25, 25, 2, 24},
  {'B', 0, 56, 20, 25, -1, 24},
  {'C', 50, 31, 24, 25, 0, 24},
  {'D', 119, 31, 22, 25, -1, 24},
  {'E', 20, 56, 20, 25, -1, 24},
  {'F', 97, 56, 19, 25, -1, 24},
  {'G', 247, 0, 25, 25, 0, 24},
  {'H', 207, 31, 21, 25, -1, 24},
  {'I', 137, 81, 7, 25, -1, 24},
  {'J', 52, 81, 16, 25, 1, 24},
  {'K', 141, 31, 22, 25, -1, 24},
  {'L', 18, 81, 17, 25, -1, 24},
  {'M', 272, 0, 25, 25, -1, 24},
  {'N', 228, 31, 21, 25, -1, 24},
  {'O', 297, 0, 25, 25, 0, 24},
  {'P', 249, 31, 21, 25, -1, 24},
  {'Q', 136, 0, 25, 26, 0, 24},
  {'R', 74, 31, 23, 25, -1, 24},
  {'S', 270, 31, 21, 25, 0, 24},
  {'T', 163, 31, 22, 25, 1, 24},
  {'U', 291, 31, 21, 25, -1, 24},
  {'V', 322, 0, 25, 25, 2, 24},
  {'W', 161, 0, 33, 25, 1, 24},
  {'X', 0, 31, 25, 25, 2, 24},
  {'Y', 25, 31, 25, 25, 2, 24},
  {'Z', 185, 31, 22, 25, 1, 24},
  {'[', 81, 0, 10, 31, 0, 24},
  {'\\', 81, 81, 13, 25, 2, 24},
  {']', 91, 0, 10, 31, 1, 24},
  {'^', 128, 106, 17, 15, 1, 24},
  {'_', 250, 106, 22, 4, 2, -3},
  {'`', 222, 106, 9, 6, 0, 24},
  {'a', 218, 81, 19, 19, 1, 18},
  {'b', 242, 56, 18, 25, 0, 24},
  {'c', 0, 106, 17, 19, 0, 18},
  {'d', 260, 56, 18, 25, 1, 24},
  {'e', 237, 81, 19, 19, 1, 18},
  {'f', 94, 81, 13, 25, 1, 24},
  {'g', 278, 56, 18, 25, 1, 18},
  {'h', 35, 81, 17, 25, 0, 24},
  {'i', 144, 81, 7, 25, 0, 24},
  {'j', 101, 0, 10, 31, 3, 24},
  {'k', 296, 56, 18, 25, 0, 24},
  {'l', 151, 81, 7, 25, 0, 24},
  {'m', 192, 81, 26, 19, 0, 18},
  {'n', 17, 106, 17, 19, 0, 18},
  {'o', 256, 81, 19, 19, 1, 18},
  {'p', 314, 56, 18, 25, 0, 18},
  {'q', 332, 56, 18, 25, 1, 18},
  {'r', 51, 106, 13, 19, 0, 18},
  {'s', 275, 81, 18, 19, 1, 18},
  {'t', 119, 81, 11, 25, 1, 24},
  {'u', 34, 106, 17, 19, 0, 18},
  {'v', 293, 81, 18, 19, 1, 18},
  {'w', 165, 81, 27, 19, 2, 18},
  {'x', 311, 81, 18, 19, 1, 18},
  {'y', 0, 81, 18, 25, 1, 18},
  {'z', 329, 81, 18, 19, 1, 18},
  {'{', 33, 0, 13, 31, 1, 24},
  {'|', 111, 0, 6, 31, -1, 24},
  {'}', 46, 0, 13, 31, 1, 24},
  {'~', 203, 106, 19, 7, 0, 15},
};

static Font font_Arial = {"Arial", 32, 0, 0, 350, 125, 95, characters_Arial};