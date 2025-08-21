
### 1. The Pinned Location

The core of the algorithm is described succinctly in the "Basic technique" section. This paragraph provides a perfect pseudo-code for the implementation:

> The simplest construction algorithm uses a priority queue where the node with lowest probability is given highest priority:
>
> 1.  Create a leaf node for each symbol and add it to the priority queue.
> 2.  While there is more than one node in the queue:
>     1.  Remove the two nodes of highest priority (lowest probability) from the queue
>     2.  Create a new internal node with these two nodes as children and with probability equal to the sum of the two nodes' probabilities.
>     3.  Add the new node to the queue.
> 3.  The remaining node is the root node and the tree is complete.

### 2. The Justification

This is an ideal candidate for a **Low-Level Programming** challenge. Here’s why:

*   **Classic Algorithm:** Huffman coding is a foundational algorithm in computer science and data compression. Implementing it from scratch is a rite of passage for many programmers.
*   **Data Structure Implementation:** The problem requires the practical application of two fundamental data structures: a **binary tree** to represent the code and a **priority queue** (usually implemented with a min-heap) to efficiently find the nodes with the lowest frequencies.
*   **Bit Manipulation:** A full implementation requires encoding the output as a stream of bits rather than a string of '0's and '1's, which is a great exercise in low-level bitwise operations.
*   **Clear, Verifiable Steps:** The process is deterministic and easy to verify. You can check your implementation against known examples (like the one in the article) to see if your generated codes are correct.

### 3. The Programming Problem

Here is a multi-level programming challenge to implement Huffman coding.

---

#### **Level 1: The Code Generator**

The goal of this level is to take a piece of text, analyze its character frequencies, and generate the corresponding Huffman code table.

**Objective:** Write a program that accepts a string of text and outputs a mapping of each unique character to its binary Huffman code.

**Tasks:**

1.  **Character Frequency Counter:** Write a function that takes a string and returns a map or dictionary of characters to their frequencies (e.g., `{"a": 4, "b": 2, " ": 7, ...}`).
2.  **Node and Tree Structure:** Define a class or struct for a tree node. It should contain:
    *   The character.
    *   The frequency (or weight).
    *   Pointers to left and right child nodes.
3.  **Build the Huffman Tree:**
    *   Create a leaf node for each unique character and its frequency.
    *   Use a priority queue to store these nodes. The priority should be determined by the frequency, with lower frequencies having higher priority (a min-heap is perfect for this).
    *   Implement the algorithm described in the "Pinned Location": repeatedly extract the two lowest-frequency nodes, create a new internal parent node that combines their frequencies, and add the new node back into the priority queue.
    *   Continue until only one node—the root of the tree—remains.
4.  **Generate the Code Table:** Traverse the final Huffman tree from the root. Keep track of the path taken (e.g., '0' for left, '1' for right). When you reach a leaf node, the accumulated path is the Huffman code for that character. Store these in a map.

**Example Input:** `"this is an example of a huffman tree"`

**Example Output (Code Table):**
```
{
  ' ': '111', 'a': '010', 'e': '000', 'f': '1101', 'h': '1010', 
  'i': '1000', 'm': '0111', 'n': '0010', 's': '1011', 't': '0110', 
  'l': '11001', 'o': '00110', 'p': '10011', 'r': '11000', 
  'u': '00111', 'x': '10010'
}
```
*(Note: Your exact codes might differ based on how you handle ties in frequency, but the lengths of the codes should be the same.)*

---

#### **Level 2: The Encoder and Decoder**

Now, use the code table from Level 1 to perform actual compression and decompression.

**Objective:** Create two functions: `encode` and `decode`.

**Tasks:**

1.  **Encoder:**
    *   Take an input string and the generated Huffman code table.
    *   Iterate through the string and concatenate the binary codes for each character to produce a single binary string (e.g., `"101101001000..."`).
    *   **Sub-challenge:** Instead of storing the output as a string of '1's and '0's, pack the bits into actual bytes. This is a true low-level task that requires bitwise operations. For example, the string `"10110001"` would become a single byte with the value `177`.
2.  **Decoder:**
    *   Take the encoded bitstream and the *original Huffman tree* (not just the code table, as the tree is the natural structure for decoding).
    *   Read the stream bit by bit. For each bit, traverse the tree from the root (left for '0', right for '1').
    *   When you reach a leaf node, you have decoded one character. Append it to your result string and return to the root to start decoding the next character.
    *   Continue until the bitstream is exhausted.

**Goal:** Ensure that `decode(encode(text, tree), tree)` returns the original `text`.

---

#### **Level 3: A Self-Contained Compression Tool**

Create a command-line tool that can compress and decompress files. The key challenge here is that the compressed file must contain all the information needed for decompression.

**Objective:** Build a tool that can be run as `my_huffman compress <input_file> <output_file>` and `my_huffman decompress <input_file> <output_file>`.

**Tasks:**

1.  **File Format Design:** A compressed file needs two parts: a header containing the decompression data and a body with the compressed content.
    *   **Header:** You need to serialize the Huffman tree or the frequency table. A simple way is to write the character frequencies to the start of the file. A more efficient way, mentioned in the article, is to serialize the tree structure itself. For example: a `0` bit represents an internal node, and a `1` bit represents a leaf node, followed by the 8 bits of the character itself.
    *   **Body:** The bit-packed data from Level 2.
    *   **Padding:** The compressed bitstream might not end on a perfect byte boundary. The header should include information about how many bits in the last byte are padding and should be ignored by the decompressor.
2.  **Compressor Logic:**
    *   Read the input file.
    *   Build the frequency table and Huffman tree.
    *   Write the header (the serialized tree/frequencies and padding info) to the output file.
    *   Encode the input file's content and write the resulting bytes to the output file.
3.  **Decompressor Logic:**
    *   Read the header from the compressed file.
    *   Reconstruct the Huffman tree from the header information.
    *   Read the rest of the file (the body) and use the tree to decode the bitstream until the original file size is reached (which you might also want to store in the header).
    *   Write the decoded data to the output file.

---

#### **Bonus Challenge: Graphics Simulation**

Visualize the tree-building process. This is a great way to debug your logic and understand the algorithm intuitively.

**Objective:** Create a simple graphical application that animates the construction of the Huffman tree.

**Tasks:**

1.  Represent each initial character/frequency pair as a box on the screen.
2.  In each step of the algorithm, animate the two lowest-frequency nodes moving from their positions, connecting under a new parent node, and the new parent node being placed back into the sorted collection of nodes.
3.  Repeat until only one tree remains. You can then display the final tree with the '0' and '1' labels on its branches.
