# DSA Assignment 2: Government IDs with BST and Linear Search

## Aim

Organise government identification numbers in a Binary Search Tree, display the
inorder traversal, compare BST Search and Linear Search, and evaluate the structures.

## Submission contents

This project answers the government identification numbers question supplied for this assignment.

| Required item | File or section |
|---|---|
| C source code | [main.c](main.c) |
| Given input data and selected queries | [input_data.txt](input_data.txt) |
| Executed output | [sample_output.txt](sample_output.txt) |
| Intermediate trace tables | [trace_tables.md](trace_tables.md) |
| Time and space complexity analysis | [Complexity](#complexity) below |
| Performance comparison table | [Comparison](comparison_and_conclusion.md#observed-search-results) |
| Justified final conclusion | [Final conclusion](comparison_and_conclusion.md#final-conclusion) |

Repository designated for submission: https://github.com/shivadath-art/DSA-ASSIGNMENT-2

## Run the project

Requires a C11 compiler (GCC, Clang or Microsoft Visual C). No external libraries are needed.

```sh
gcc -std=c11 -Wall -Wextra -Wpedantic main.c -o govids
./govids
./govids --test
./govids --trace > trace_tables.md
```

- `main.c`: BST construction, inorder traversal, height calculation, both searches and self-tests.
- `sample_output.txt`: output captured from an actual execution.

The given IDs and three queries are built into the C program and documented
in `input_data.txt`; no interactive input or input-file argument is required.
The `--trace` option records each BST insertion with its path, the inorder list,
and every comparison in both searches. Trace printing is diagnostic output and
is excluded from the normal-operation complexity analysis.

## Part (a): BST representation and construction

Use a **binary search tree ordered by `strcmp`** (lexicographical / dictionary
order). Each C struct stores an ID string plus left/right child pointers. This
fixed example stores eight nodes in a caller-owned pool; no heap allocation is needed.

Insertion order (database order):

```text
A102 -> A25 -> A7 -> B100 -> B12 -> A120 -> B3 -> A45
```

Resulting BST:

```text
        A102
           \
            A25
           /   \
       A120     A7
               /  \
            A45   B100
                     \
                     B12
                        \
                         B3
```

`build_bst()` inserts the IDs in the given order. There are **8 nodes and
7 links**. Only `A120` (left child of `A25`) and `A45` (left child of `A7`)
branch left; the rest form a right chain, so the tree is unbalanced.

### Inorder traversal

1. Visit the left subtree.
2. Visit the node itself.
3. Visit the right subtree.

Actual execution:

```text
Inorder traversal: A102 A120 A25 A45 A7 B100 B12 B3
```

Inorder visits IDs in `strcmp` order: `A102 < A120 < A25` because `'1' < '2'`
at the second character. This is dictionary order, not numeric order. Inorder
traversal is useful whenever IDs must be listed in sorted order without a
separate sort.

## Part (b): Searchable representation and measured comparisons

Both algorithms use the same eight IDs. BST Search follows `strcmp` left/right
links from the root. Linear Search checks the database array from index 0.
Both return found/not-found together with their comparison count.

**Counting convention:** one comparison means comparing the target with one ID
entry. BST Search counts one `strcmp` per visited node. These are entry
comparisons, not counts of C operators, individual character comparisons, or
loop-condition checks. Tree-building comparisons are preprocessing and are
excluded from the per-search counts.

Results from running `./govids`:

| Target | Result | BST comparisons | Linear comparisons |
|---|---|---:|---:|
| A45 | Found | 4 | 8 |
| B3 | Found | 6 | 7 |
| A120 | Found | 3 | 6 |

BST paths explain these counts:

- A45: A102, A25, A7, A45.
- B3: A102, A25, A7, B100, B12, B3 (deepest leaf).
- A120: A102, A25, A120.

BST Search wins in all three examples, but only narrowly for B3 because B3 sits
at depth 5 of this skewed tree. Across these three demonstrations, BST Search
makes 13 comparisons and Linear Search makes 21; these totals describe the
chosen examples, not a universal average.

## Part (c): Analysis

### Tree height

Height is the number of edges on the longest root-to-leaf path. The path
A102 -> A25 -> A7 -> B100 -> B12 -> B3 contains **5 edges**. Therefore the
height is **5**, or **6 levels** if height is expressed in nodes.
`tree_height()` calculates this recursively; a leaf's height is zero.

### Complexity

Let n be the number of IDs and h the BST height. Here n = 8 and h = 5.
The table treats an ID comparison as constant time.

| Operation | Time | Additional space |
|---|---|---|
| Build a BST of n IDs | O(n h) worst case, O(n log n) balanced | O(n) for stored tree |
| Inorder traversal | O(n) | O(h + 1) recursive stack |
| Calculate height | O(n) | O(h + 1) recursive stack |
| BST Search, best case | O(1) | O(1) besides the tree |
| BST Search, average (balanced) | O(log n) | O(1) besides the tree |
| BST Search, worst case | O(n) | O(1) besides the tree |
| Linear Search, best case | O(1) | O(1) |
| Linear Search, average/worst case | O(n) | O(1) |

The build function explicitly inserts these eight IDs; its work is constant for
this exact input. O(n h) describes extending the same approach to arbitrary
insertion orders. Every traversal visits each node once, and height calculation
examines both subtrees. All fixed capacities must be increased if the database
grows, or replaced with dynamic storage.

For successful Linear Search with equally likely targets, the average number of
comparisons is (n + 1) / 2 = 4.5. An unsuccessful Linear Search checks all
n = 8 entries. A balanced BST on 8 nodes examines at most 4 entries; this
skewed tree examines up to 6 for the selected queries.

IDs are strings: if their maximum length is L, `strcmp` can cost O(L) time.
Thus more precise worst-case search bounds are O(hL) and O(nL), respectively.
The usual DSA bounds above abstract those costs.

### Suitability and conclusion

The BST is suitable for ID storage because it keeps IDs ordered and supports
inorder listing plus repeated lookup without a separate sort. It would need
balancing for sorted insertion orders that otherwise produce a chain.

A BST alone does not guarantee fast lookup: a skewed tree degrades to O(n),
as B3 (6 comparisons) nearly shows. A sorted array with binary search is an
alternative, but this assignment compares the tree directly against linear scan
of the database order.

The BST representation is suitable for this small, mostly static database.
When IDs are added or removed, tree height must stay small; this program
rebuilds from the fixed list. Inserting into a plain BST can skew it, so a
larger system needing frequent exact-ID searches should use a self-balancing
BST (AVL or Red-Black Tree) for O(log n) lookup, or B-Trees for on-disk data.
Duplicate IDs are ignored here and would require counts or record lists.

## Validation

The C program follows standard C11 compatible with the build commands above.
Execution output is saved in `sample_output.txt`. Run `govids --test` to verify
inorder order, exact height, search paths, comparison counts, root/single-entry
cases, missing entries, and empty input. Assertions must remain enabled (do not
define `NDEBUG` when testing).

### Windows build alternative

From a Visual Studio Developer Command Prompt:

```bat
cl /nologo /std:c11 /W4 /WX main.c /Fe:govids.exe
govids.exe
govids.exe --test
```
