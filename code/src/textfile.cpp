#include "textfile.hpp"
#include <algorithm>
#include <cmath>
#include <cstdint> // for UINT32_MAX
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <vector>

std::unordered_map<char, unsigned int>
TextFile::get_character_frequencies(bool verbose) {
  std::unordered_map<char, unsigned int> frequencies_table; // table declaration
  std::ifstream file(this->name);
  if (!file.is_open()) {
    std::cerr << "Error opening file!" << std::endl;
    return frequencies_table;
  }

  std::string line;
  while (std::getline(file, line)) {
    for (char character : line) {
      frequencies_table[character]++;
    }
  }
  file.close();

  if (verbose) {
    // 1. Pretty print the map
    std::cout << "Character Frequencies:\n";
    for (auto &[character, frequency] : frequencies_table) {
      if (character == ' ') {
        std::cout << "' '"
                  << " -> " << frequency << "\n";
      } else {
        std::cout << character << " -> " << frequency << "\n";
      }
    }

    // 2. Find most and least common non-space characters
    unsigned int max_freq = 0, min_freq = UINT32_MAX;
    std::vector<char> most_common, least_common;
    for (auto &[character, frequency] : frequencies_table) {
      if (character == ' ')
        continue;
      if (frequency > max_freq) {
        max_freq = frequency;
        most_common = {character};
      } else if (frequency == max_freq) {
        most_common.push_back(character);
      }

      if (frequency < min_freq) {
        min_freq = frequency;
        least_common = {character};
      } else if (frequency == min_freq) {
        least_common.push_back(character);
      }
    }

    std::cout << "Most common non-space character(s): ";
    for (char c : most_common)
      std::cout << c << " ";
    std::cout << "(" << max_freq << " times)\n";

    std::cout << "Least common non-space character(s): ";
    for (char c : least_common)
      std::cout << c << " ";
    std::cout << "(" << min_freq << " times)\n";

    // 3. Mean and median frequency
    std::vector<unsigned int> non_space_freqs;
    for (auto &[character, frequency] : frequencies_table) {
      if (character != ' ')
        non_space_freqs.push_back(frequency);
    }

    double mean = 0;
    for (unsigned int f : non_space_freqs)
      mean += f;
    mean /= non_space_freqs.size();

    std::sort(non_space_freqs.begin(), non_space_freqs.end());
    double median;
    size_t n = non_space_freqs.size();
    if (n % 2 == 0)
      median = (non_space_freqs[n / 2 - 1] + non_space_freqs[n / 2]) / 2.0;
    else
      median = non_space_freqs[n / 2];

    std::cout << "Mean frequency: " << mean << "\n";
    std::cout << "Median frequency: " << median << "\n";

    // 4. Standard deviation
    double variance = 0;
    for (unsigned int f : non_space_freqs) {
      variance += (f - mean) * (f - mean);
    }
    variance /= non_space_freqs.size();
    double std_dev = std::sqrt(variance);

    std::cout << "Standard deviation: " << std_dev << "\n";
  }

  return frequencies_table;
}
