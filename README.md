# Organising Government Identification Numbers — BST vs Linear Search

**Course:** Data Structures Lab
**Input IDs:** `A102, A25, A7, B100, B12, A120, B3, A45` (n = 8)

This repo implements and compares BST Search and Linear Search on the ID set above, as required by the assignment (parts a, b, c).
IDs contain letters + digits, so they are compared as **strings with `strcmp` (lexicographical / dictionary order)**.

## Files

| File | Purpose |
|------|---------|
| `bst.c` | Part (a) — BST insert, prints path after every insertion + final inorder, height and structure |
| `search_compare.c` | Part (b) — BST Search vs Linear Search, prints path and comparisons for each target |
| `input.txt` | Input data — database order + search targets |
| `output.txt` | Captured program output (see How to run) |
| `README.md` | This file — trace tables, complexity analysis, comparison table, conclusion (part c) |

## How to run

```
gcc -o bst bst.c && ./bst
gcc -o search_compare search_compare.c && ./search_compare
```

## a) BST trace

Insertion order (database order):

```
A102 → A25 → A7 → B100 → B12 → A120 → B3 → A45
```

Program output (one line per insertion):

```
Initial order           : [A102, A25, A7, B100, B12, A120, B3, A45]

Insert #1  A102 : (root, 0 comparisons)
           inorder-so-far : [A102]
Insert #2  A25  : path [A102]  (1 comparison)
           inorder-so-far : [A102 A25]
Insert #3  A7   : path [A102 -> A25]  (2 comparisons)
           inorder-so-far : [A102 A25 A7]
Insert #4  B100 : path [A102 -> A25 -> A7]  (3 comparisons)
           inorder-so-far : [A102 A25 A7 B100]
Insert #5  B12  : path [A102 -> A25 -> A7 -> B100]  (4 comparisons)
           inorder-so-far : [A102 A25 A7 B100 B12]
Insert #6  A120 : path [A102 -> A25]  (2 comparisons)
           inorder-so-far : [A102 A120 A25 A7 B100 B12]
Insert #7  B3   : path [A102 -> A25 -> A7 -> B100 -> B12]  (5 comparisons)
           inorder-so-far : [A102 A120 A25 A7 B100 B12 B3]
Insert #8  A45  : path [A102 -> A25 -> A7]  (3 comparisons)
           inorder-so-far : [A102 A120 A25 A45 A7 B100 B12 B3]

Final inorder traversal : [A102 A120 A25 A45 A7 B100 B12 B3]
Tree height             : 5 edges (6 levels)
Longest root-to-leaf   : A102 -> A25 -> A7 -> B100 -> B12 -> B3
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

Inorder traversal visits **Left → Root → Right**, so it returns the IDs in sorted `strcmp` order:

```text
A102  A120  A25  A45  A7  B100  B12  B3
```

### Analysis of the tree structure

* The tree is **unbalanced / right-skewed**. `A25 > A102` (`'2' > '1'`), `A7 > A25` (`'7' > '2'`), and all `B... > A...`, so the first five inserts form a straight right chain.
* Only `A120` (left child of `A25`) and `A45` (left child of `A7`) branch left.
* Height is **5 edges (6 levels)**, close to the worst case `n-1 = 7` edges, far from the balanced height `floor(log2(8)) = 3` levels.
* This is why the BST still works but does not show full `O(log n)` benefit on this particular insertion order.

> Note on ordering: `strcmp` compares character-by-character, so `A102 < A120 < A25` because `'1' < '2'` at the second character. This is pure dictionary order, not numeric order (`7 < 25 < 102`). The code uses `strcmp` exactly, so the trace above is what the program really produces.

## b) BST Search vs Linear Search trace

Targets: `A45, B3, A120`. Database order for linear search is the original list.

```
Database order : [A102, A25, A7, B100, B12, A120, B3, A45]
```

### Search for A45

```text
BST Search    : path [A102 -> A25 -> A7 -> A45]  (4 comparisons)
Linear Search : A102(1) -> A25(2) -> A7(3) -> B100(4) -> B12(5) -> A120(6) -> B3(7) -> A45(8) [FOUND]
```

**BST = 4, Linear = 8**

### Search for B3

```text
BST Search    : path [A102 -> A25 -> A7 -> B100 -> B12 -> B3]  (6 comparisons)
Linear Search : A102(1) -> A25(2) -> A7(3) -> B100(4) -> B12(5) -> A120(6) -> B3(7) [FOUND]
```

**BST = 6, Linear = 7**

### Search for A120

```text
BST Search    : path [A102 -> A25 -> A120]  (3 comparisons)
Linear Search : A102(1) -> A25(2) -> A7(3) -> B100(4) -> B12(5) -> A120(6) [FOUND]
```

**BST = 3, Linear = 6**

### Comparison table

| ID | BST Search | Linear Search |
|----|-----------:|--------------:|
| A45 | 4 | 8 |
| B3 | 6 | 7 |
| A120 | 3 | 6 |
| **Total** | **13** | **21** |

BST needs fewer comparisons for all three IDs, but the gap is small for `B3` because `B3` sits at the deepest leaf (depth 5) of this unbalanced tree.

## c) Comparative analysis

| Criterion | BST Search | Linear Search |
|-----------|------------|---------------|
| Best case | O(1) — key at root / first element | O(1) — key at index 0 |
| Average case (balanced BST) | O(log n) | O(n) |
| Worst case | O(n) — skewed tree, height = n-1 | O(n) — key last / absent |
| Extra space | O(n) nodes + O(h) recursion stack | O(1) |
| Sorted output | Yes (inorder) | No |

Observed vs theory (n = 8):

* Balanced height would be ~`log2(8) = 3` levels → ~3 comparisons. `A120` (3 comps) matches this; `A45` (4) is close.
* `B3` (6 comps) shows the skew penalty — almost linear. Worst case for n = 8 would be 8 comparisons; we observe 6.
* Linear search observed 6–8 comparisons, i.e. exactly the theoretical `O(n)` behaviour.

### Effect of key length

IDs are strings. `strcmp` scans characters until a difference or `'\0'`. E.g. `A102345678` vs `A102345679` must compare 9 chars before deciding. So **longer common prefixes make each single comparison more expensive**, but they do not change tree height by themselves. Total cost ≈ (comparisons) × (cost per `strcmp`).

### Effect of insertion order

Insertion order decides shape and height:

Balanced insertion → short tree → fast search:

```text
          D
        /   \
       B     F
      / \   / \
     A   C E   G
```

Sorted insertion → skewed chain → search degrades to a linked list:

```text
A
 \
  B
   \
    C
     \
      D
       \
        E
```

* **Balanced → height ≈ log2(n) → search O(log n)**
* **Skewed → height = n-1 → search O(n)**, same as linear search

Our run is a near-worst-case example: ascending `strcmp` order for the first five keys produced a right chain of length 5.

### Which is more appropriate as the database grows?

For a small fixed set, a plain BST already beats linear search (13 vs 21 total comparisons here).

For a growing government database, **a plain BST is not enough**, because adversarial / sorted insertion order makes it `O(n)`. The suitable approach is a **self-balancing BST such as AVL or Red-Black Tree**, which keeps height ≈ `O(log n)`:

* Search: **O(log n)**
* Insert: **O(log n)**
* Delete: **O(log n)**

For very large on-disk databases, **B-Trees / B+ Trees** (the index structure behind most DBMSs) are the standard choice.

> A note for completeness: since these IDs have a fixed prefix + numeric part, a **trie / digital tree** or a **hash table** (average O(1) lookup, O(L) hash cost where L = key length) would outperform both BST and linear search at scale. That is outside this assignment, which asks specifically for BST vs Linear Search, but it is worth knowing for the viva if asked "is there anything faster?".
