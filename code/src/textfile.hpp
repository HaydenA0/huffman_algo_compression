#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

class TextFile {
public:
  std::string name;
  void encode(); // to be implemented
  void decode(); // to be implemented
  std::unordered_map<char, unsigned int>
  get_character_frequencies(bool verbose);
};
