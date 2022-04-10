# zeep
Zeep is a program used to compress files

## Command Line Interface

```shell
$ zeep [options] <input_file> <output_file>
```

### Options

```text
Normal Options:
    -h, --help                  Show this help message.
    -c, --compress <type>       Compress the input file.
    -d, --decompress <type>     Decompress the input file.

Compress type:
    --huffman <tree_file>       Use Huffman Coding.
```

### Compress File

```shell
# Compress by Huffman Coding and save Huffman tree to a file
$ zeep -c --huffman <tree_file> <input_file> <output_file>
```

### Decompress File

```shell
# Decompress by Huffman Coding with loaded Huffman tree
$ zeep -d --huffman <tree_file> <input_file> <output_file>
```