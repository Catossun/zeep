#include "huffman.h"

#include <algorithm>
#include <deque>
#include <map>

Huffman::Huffman() {
    tree = new Node();
}

Huffman::~Huffman() {
    releaseTree(tree);
}

Node *Huffman::releaseTree(Node *root) {
    if (!root) return nullptr;
    if (!root->left && !root->right) return root;
    delete releaseTree(root->left);
    delete releaseTree(root->right);
    delete root;
    return nullptr;
}

string Huffman::compress(string &src) {
    this->tree = buildHuffmanTree(src);
    map<char, string> charTable = buildCharTable(this->tree);
    // Map each char into the Huffman code
    string dst;
    for (char c: src) {
        dst.append(charTable.at(c));
    }
    return dst;
}

Node *Huffman::buildHuffmanTree(string &src) {
    // Calculate show times of each character
    map<char, Node *> nodes;
    for (char c: src) {
        if (nodes.find(c) == nodes.end()) {
            Node *n = new Node();
            n->value = c;
            n->weight = 1;
            nodes.insert(pair<char, Node *>(c, n));
            continue;
        }
        ++nodes.at(c)->weight;
    }
    // Transform the map to a list
    deque<Node *> sortedNodes;
    for (pair<char, Node *> p: nodes) {
        sortedNodes.push_back(p.second);
    }
    // Sort the list
    sort(sortedNodes.begin(), sortedNodes.end(), nodeComparator);
    // Build Huffman tree
    while (sortedNodes.size() > 1) {
        // Pop first two nodes
        Node *first = sortedNodes.front();
        sortedNodes.pop_front();
        Node *second = sortedNodes.front();
        sortedNodes.pop_front();
        // Combine the nodes
        Node *combined = new Node();
        combined->left = first;
        combined->right = second;
        combined->weight = first->weight + second->weight;
        sortedNodes.push_front(combined);
        // Sort the list
        sort(sortedNodes.begin(), sortedNodes.end(), nodeComparator);
    }
    // The only one node in the list is the tree root
    return sortedNodes.front();
}

map<char, string> Huffman::buildCharTable(Node *root) {
    map<char, string> table;
    string charCode;
    recursiveBuildCharTable(root, table, charCode);
    return table;
}

void Huffman::recursiveBuildCharTable(Node *root, map<char, string> &table, string &charCode) {
    if (!root) return;
    // If the root is the end, save it into the table
    if (!root->left && !root->right) {
        //Save to the table
        table.insert(pair<char, string>(root->value, charCode));
        // Remove the last character in the stream
        charCode = charCode.substr(0, charCode.size() - 1);
        return;
    }
    // Walk to left child
    charCode.append("0");
    recursiveBuildCharTable(root->left, table, charCode);
    // Walk to right child
    charCode.append("1");
    recursiveBuildCharTable(root->right, table, charCode);
    charCode = charCode.substr(0, charCode.size() - 1);
}

bool Huffman::nodeComparator(Node *a, Node *b) {
    // Sort by ASC
    return a->weight < b->weight;
}

string Huffman::decompress(string &src) {
    string dst;
    size_t offset = 0;
    while (offset < src.size()) {
        char c = recursiveDecompress(this->tree, src, offset);
        dst.push_back(c);
    }
    return dst;
}

char Huffman::recursiveDecompress(Node *root, string &src, size_t &offset) {
    if (!root->left && !root->right)
        return root->value;
    if (src.at(offset) == '0')
        return recursiveDecompress(root->left, src, ++offset);
    else
        return recursiveDecompress(root->right, src, ++offset);

}

void Huffman::readTree(basic_istream<char> &stream) {
    char value;
    char separator;
    string charCode;
    // Read data from stream with "<value>:<char_code>\n" format
    while (stream.get(value) &&
           stream.get(separator) &&
           stream >> charCode) {
        // Insert value into the tree
        insertNodeToTree(&this->tree, charCode, value);
        // Remove the line separator at the end
        stream.get(separator);
    }
}

void Huffman::insertNodeToTree(Node **rootPtr, string &position, char &value) {
    // Create a Node for the pointer of root
    if (*rootPtr == nullptr) *rootPtr = new Node();
    // Save the value, if the position string is empty
    if (position.empty()) {
        (*rootPtr)->value = value;
        return;
    }
    // Use the first character to decide the next direction
    if (position.at(0) == '0') {
        // Remove the first character and move left
        position = position.substr(1, position.size() - 1);
        insertNodeToTree(&(*rootPtr)->left, position, value);
    } else {
        // Remove the first character and move right
        position = position.substr(1, position.size() - 1);
        insertNodeToTree(&(*rootPtr)->right, position, value);
    }
}

void Huffman::writeTree(basic_ostream<char> &stream) {
    // Build a char table from the current Huffman tree
    map<char, string> charTable = buildCharTable(this->tree);
    // Write the char table into a stream
    for (pair<char, string> p: charTable) {
        stream << p.first << ":" << p.second << endl;
    }
    stream.flush();
}
