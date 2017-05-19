---
title: CS 246 Assignment 8
---

<div id="header">

| **CS 246 Assignment 8**
| due 12:55pm on Thursday, April 27, 2017

</div>

[Link back to syllabus](http://cs.brynmawr.edu/cs246/syllabus.html)

[Click here to start assignment](https://classroom.github.com/group-assignment-invitations/ec803a72033fd8a51d4876f186f3ebd5)

Like all assignments, you will complete this assignment via
GitHub. See [the submission instructions](../submission.html)
for how to get the starter code and how to submit this
assignment.

A Huffman File Compressor and Decompressor
==========================================

This assignment is to write `huff` and `puff`, a file compressor and decompressor, respectively.

From the user's standpoint, running `huff infile.txt outfile.huff` at the command line reads in
`infile.txt` and produces `outfile.huff`. The contents of `infile.txt` are compressed in `outfile.huff`,
making the size of `outfile.huff` smaller than that of `infile.txt`. (This new file might not actually
be smaller than the input, if the input is too short, due to the extra header in the `huff` file. See
below.) One can then, for example, send `outfile.huff` over the internet, and it will transmit faster.
To decompress, run `puff outfile.huff puffed.txt`, which reads in `outfile.huff` and creates `puffed.txt`,
which should have the same exact contents as the original `infile.txt`.

These programs work by using Huffman compression. The Huffman compression scheme is explained in full
in class. Here is a summary, with a view to writing `huff`:

```c
typedef struct huff_tree_node {
  int ch;
  int freq;

  struct huff_tree_node* left;
  struct huff_tree_node* right;
} huff_tree_node;
```

1. Calculate the frequencies of all the characters in the input file. For example, if the file contains
`abracadabra`, you will calculate that there are 5 `a`s, 2 `b`s, 1 `c`, 1 `d`, and 2 `r`s.

2. For each character with a non-zero frequency, create a leaf `huff_tree_node` containing the
character (in the `ch` field) and the frequency (in the `freq` field). These trees (each initially
containing one node) are collectively
known as the forest. 

3. Let `x` be the minimum tree in the forest and `y` be the next-minimum tree in the forest. (Comparing
trees is done by the `freq` stored in the root, using `ch` to break ties.) Remove `x` and `y` from
the forest. Create a new `huff_tree_node` (call it `n`), whose left child is `x` and whose right child
is `y`. Choose a fresh negative number---that is, a negative number that hasn't been used yet. A convenient
way to do this is to count down from `-1`. Let `n`'s `ch` field be this negative number, and let `n`'s
`freq` field be the sum of `x`'s `freq` and `y`'s `freq`. Put `n` into the forest.

4. If the forest has more than one element, repeat step 3. Otherwise, we have arrived at the final
Huffman tree.

5. The Huffman tree tells you how to encode every character. To encode a character (all of which are
stored in leaves of the tree), consider the path from the root to the leaf containing that character.
For every left child traversed, add a `0` to the encoding. For every right child, add a `1`. So, if the
path from the root to the leaf containing `r` requires going left, then left, then right, the encoding
for `r` is `001`. Using this notion of encoding, build a Huffman table that allows you to look up a character
and find its encoding.

6. There are 256 possible characters. Write 256 `int`s to the output file; the $n$th `int` is the frequency
for the $n$th character. In the case of `abracadabra`, `int` number 97 will have the value 5, whereas all
`int`s before that will be 0.

7. For each character in the input file, write its encoding in the output file as a sequence of bits. With
`r`'s encoding above, this would mean writing three bits to the file. Do not use three *bytes*.

8. We're done. Close the files and free your memory.

As you can see from these instructions, a compressed file has a 1024-byte header: 256 4-byte `int`s describing
the frequencies of all possible characters. This header can make a compressed file larger than the original,
for small enough originals.

To decompress a file:

1. Read in the table of character frequencies in the first 256 `int`s in the input file.

2. Build the Huffman tree (as in steps 2-4 above).

3. For every bit read in from the input file, traverse down the Huffman tree (going left for `0` bits
and right for `1` bits). When you reach a leaf, there will be a character stored in the leaf node. Write
this character to the output file.

4. Repeat step 3 for every character in the input file (as determined by looking at the frequency of the
root of the Huffman tree, which is necessarily the total number of characters in the input).

5. We're done. Close the files and free your memory.

You may test your `huff` and `puff` against my versions, available in `/home/rae/pub/huff` and `/home/rae/pub/puff`
on powerpuff. So, if you're `ssh`d into powerpuff, you can say `/home/rae/pub/huff test_file.txt test_file.huff`
and then `/home/rae/pub/puff test_file.huff puffed.txt` and `puffed.txt` should be the same as `test_file.txt`.
Naturally, your `huff` and `puff` should work with mine.

Some hints:

 * Writing and reading compressed files means that you have to work with *bits* not *bytes*. This is awkward
   with the functions in `stdio.h`, all of which work with bytes. Consider writing bits. You will need a structure
   that stores the bits to write, as well as the number of bits that have been written. When this number goes above
   8 (the number of bits in a byte), write out the byte and clear the bits from your buffer. When reading, you will
   have to do the opposite, reading in a byte and storing it, while offering up one bit at a time via a function.

 * When writing a compressed file, you might read the last input character and still have some unwritten bits
   in your buffer. Make sure to flush the buffer before close the file.

 * When reading a compressed file, there may be some leftover bits at the end of reading. (That's because the
   writer might have written a number of bits that is not divisible by 8.) That's OK---just don't read them.

 * You have choices when it comes to implementing your forest. Best would be using a min-heap. (The first few
   of [these slides](https://www.cs.cmu.edu/~tcortina/15-121sp10/Unit06B.pdf) offer a nice refresher of heaps.)
   But it's OK if you use some other structure. You can get 10 bonus, extra-credit points for using a heap,
   though.

Reflections
-----------

Include in your submission a `refl.txt`. This file should contain reflections on this assignment (what was
hard? what wasn't so bad? how much time did it take?) as well as any general reflections you wish to share
about the course.


