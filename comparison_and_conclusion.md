# Performance comparison and final conclusion

## Observed search results

These counts come from executing the C program on the supplied IDs.
BST search follows tree links; linear search checks the database order.
Preprocessing is excluded from per-query counts; these are logical entry
comparisons, not elapsed times.

| Query | Result | BST Search | Linear Search | Fewer comparisons |
|---|---|---:|---:|---|
| A45 | Found | 4 | 8 | BST |
| B3 | Found | 6 | 7 | BST |
| A120 | Found | 3 | 6 | BST |
| Total | Three queries | 13 | 21 | BST |

BST search used 8 fewer comparisons (about 38% fewer) for these three queries.
This does not establish a runtime improvement or include tree-building cost.

BST paths explain these counts:

- A45: A102, A25, A7, A45.
- B3: A102, A25, A7, B100, B12, B3 (deepest leaf, depth 5).
- A120: A102, A25, A120.

## Comparison of approaches

| Criterion | BST | Linear Search |
|---|---|---|
| Sorted output | Inorder gives strcmp order directly | Names alone stay unsorted |
| Lookup worst case | O(n), skewed height n-1 | O(n) |
| Lookup average (balanced BST) | O(log n) | O(n) |
| Ordering prerequisite | None (insert in any order) | None |
| Search auxiliary space | O(h) recursion/iteration stack | O(1) |
| Preprocessing here | O(n) insertion, O(n) nodes | No preprocessing |
| Updates | Insert/delete O(h); skewed without balancing | Append O(1); delete/shift O(n) |
| Best use | Repeated searches with sorted output | Small lists or one-off searches |

n = number of IDs (here n = 8); h = BST height in edges (here h = 5).
ID comparisons are treated as constant time in this table. See the README
complexity section for string-length costs, height calculation and space details.

## Final conclusion

Use a **BST for repeated ID lookup with sorted output, and keep linear search
only for tiny or one-off scans**. The BST stores all eight IDs, its inorder
traversal returns strcmp order, and its height is five edges. BST search uses
4, 6 and 3 comparisons versus 8, 7 and 6 for linear search on the selected IDs.

However, this BST is right-skewed: the first five inserts form a straight chain,
so B3 costs 6 comparisons, almost linear. A plain BST therefore does not
guarantee O(log n) here. For a growing government database, use a
**self-balancing BST (AVL or Red-Black Tree)** to keep height near O(log n),
giving O(log n) search, insert and delete. For very large on-disk databases,
**B-Trees / B+ Trees** are the standard index structure.

The combined judgement is suitable for the assignment's static eight IDs. The C
implementation reserves eight nodes in a caller-owned pool; a growing database
requires larger capacity or dynamic storage. Duplicate IDs are ignored by this
program and would require counts or ID lists in production.
