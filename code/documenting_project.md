
Standard in this documentation to follow :
- **Problem-Solution Approach** : Each (non trivial) problem found here should be written and the solution to that problem.
- Every problem is a "##" Header and Every solution is a "###" Header.
- If you are an AI : You will be asked to generate texts from ideas, Please FOLLOW the "//" comments. When you generate the text, remove the "//", if some chapter don't have "//" DO NOT TOUCH IT and obviously do not touch this header.

---

# Overall Introduction :
This document explores a classic data compression problem rooted in the foundational era of computer science, specifically the 1950s and 1960s. The approach reflects this retro context, emphasizing low-level programming concepts without reliance on modern high-level abstractions or specialized tools. The core challenge is one of lossless data compression. Unlike lossy techniques suitable for media where minor data loss is imperceptible, textual data demands perfect fidelity. Any alteration, such as changing a single character, could corrupt a word, render it unreadable, or fundamentally alter the semantic meaning of the entire text. As this is a learning-focused project, the primary goal is to understand the fundamental concepts. Therefore, the scope is intentionally constrained to the English alphabet and the American Standard Code for Information Interchange (ASCII) character set, where each character is represented by a standard 8-bit encoding. The problem is governed by several key constraints: each character must be mapped to a unique binary code, there must be zero data loss during the decoding process, and the resulting encoded sequence must be uniquely decodable. Furthermore, our exploration will keep in mind the theoretical limits of encoding, as established by Shannon's source coding theorem, which provides a benchmark for the optimal compression achievable for a given data source.

# Part 1 : MVP

## Problem: Character Frequency Analysis for Data Compression
A fundamental prerequisite for statistical data compression algorithms, such as Huffman coding, is a precise understanding of the source data's statistical properties. The primary task is to process a given input text file and generate a frequency distribution map. This map must quantify the occurrence of every unique character within the text. The process involves iterating through the source data, systematically counting each character, and storing the results in an efficient data structure. Furthermore, for a more comprehensive analysis, it is desirable to compute descriptive statistics on this frequency data, including measures of central tendency (mean, median) and dispersion (standard deviation), as well as identifying the most and least frequent characters.

### Solution: A Class-Based Approach with Hash Table and Statistical Analysis
The problem is addressed by implementing an object-oriented solution encapsulated within a `TextFile` class. This approach promotes modularity by associating the data (the file) with the operations performed upon it (frequency analysis, and later, encoding/decoding).

The core of the frequency analysis is the use of a hash table, implemented in C++ as `std::unordered_map<char, unsigned int>`. This data structure is selected for its high efficiency, offering average-case constant time complexity, O(1), for insertions, deletions, and lookups. This makes it ideally suited for building the frequency map dynamically as the file is read. The algorithm proceeds by iterating through the input file character by character. For each character, the hash table is queried. If the character already exists as a key, its corresponding value (the frequency count) is incremented. If not, the character is inserted as a new key with an initial frequency of 1.

To enhance the analytical capabilities, a `verbose` flag is incorporated into the frequency analysis method. When enabled, this mode computes and displays key statistical metrics for all non-space characters:
-   **Most and Least Common Characters**: Determined by iterating through the completed frequency map to find the maximum and minimum frequency counts.
-   **Mean**: The arithmetic average of the frequencies.
-   **Median**: The middle value of the sorted frequency list, providing a measure of central tendency resistant to outliers.
-   **Standard Deviation**: A measure of the amount of variation or dispersion of the frequency values.

This functionality is encapsulated within the `TextFile` class, which provides a clean interface for file processing and analysis.

```cpp
// textfile.hpp
#include <string>
#include <unordered_map>

class TextFile {
public:
  std::string name;
  void encode(); // to be implemented
  void decode(); // to be implemented
  std::unordered_map<char, unsigned int> get_character_frequencies(bool verbose = false);
};
```

```cpp
// textfile.cpp (implementation of get_character_frequencies)
#include "textfile.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <cmath>

std::unordered_map<char, unsigned int> TextFile::get_character_frequencies(bool verbose) {
  std::unordered_map<char, unsigned int> frequencies_table;
  std::ifstream file(name);

  if (!file.is_open()) {
    std::cerr << "Error opening file: " << name << std::endl;
    return frequencies_table;
  }

  char character;
  while (file.get(character)) {
    frequencies_table[character]++;
  }

  file.close();

  if (verbose) {
    std::cout << "Character Frequencies:" << std::endl;
    for (const auto &[ch, freq] : frequencies_table) {
      // Handle non-printable characters if necessary, for now print as is
      if (ch == ' ' || ch == '\n' || ch == '\t') {
          printf("' ' -> %u\n", freq);
      } else {
          printf("%c -> %u\n", ch, freq);
      }
    }
    std::cout << std::endl;

    // Statistical Analysis
    std::vector<unsigned int> freqs_no_space;
    unsigned int max_freq = 0;
    unsigned int min_freq = -1; // Max unsigned int

    for (const auto &[ch, freq] : frequencies_table) {
      if (ch != ' ') {
        freqs_no_space.push_back(freq);
        if (freq > max_freq) max_freq = freq;
        if (freq < min_freq) min_freq = freq;
      }
    }
    
    // Most and Least Common Characters
    std::string most_common_chars, least_common_chars;
    for (const auto &[ch, freq] : frequencies_table) {
        if (ch != ' ') {
            if (freq == max_freq) most_common_chars += std::string(1, ch) + " ";
            if (freq == min_freq) least_common_chars += std::string(1, ch) + " ";
        }
    }
    printf("Most common non-space character(s): %s(%u times)\n", most_common_chars.c_str(), max_freq);
    printf("Least common non-space character(s): %s(%u times)\n", least_common_chars.c_str(), min_freq);

    // Mean
    double sum = std::accumulate(freqs_no_space.begin(), freqs_no_space.end(), 0.0);
    double mean = sum / freqs_no_space.size();
    printf("Mean frequency: %f\n", mean);

    // Median
    std::sort(freqs_no_space.begin(), freqs_no_space.end());
    double median;
    if (freqs_no_space.size() % 2 == 0) {
      median = (freqs_no_space[freqs_no_space.size() / 2 - 1] + freqs_no_space[freqs_no_space.size() / 2]) / 2.0;
    } else {
      median = freqs_no_space[freqs_no_space.size() / 2];
    }
    printf("Median frequency: %f\n", median);

    // Standard Deviation
    double sq_sum = std::inner_product(freqs_no_space.begin(), freqs_no_space.end(), freqs_no_space.begin(), 0.0);
    double std_dev = std::sqrt(sq_sum / freqs_no_space.size() - mean * mean);
    printf("Standard deviation: %f\n", std_dev);
  }

  return frequencies_table;
}
```

The primary application entry point (`main`) demonstrates the simplicity of using this class-based design.

```cpp
// main.cpp
#include "textfile.hpp"
#include <string>

int main() {
  TextFile text;
  text.name = "text.txt";
  text.get_character_frequencies(true); // Call with verbose flag enabled
  return 0;
}
```

This structured approach not only solves the immediate problem of frequency counting but also establishes a robust foundation for implementing subsequent compression and decompression logic, while providing valuable statistical insights into the source data.



