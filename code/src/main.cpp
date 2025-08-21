#include "textfile.hpp"
#include <string>

int main() {
  TextFile text;
  text.name = "text.txt";
  text.get_character_frequencies(true);
  return 0;
}
