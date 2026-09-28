/* DSA Assignment 2. Build: gcc -std=c11 -Wall -Wextra -Wpedantic main.c -o govids */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ID_COUNT 8
#define ID_LEN 20

static int trace_mode = 0;

typedef struct Node {
    char id[ID_LEN];
    struct Node *left;
    struct Node *right;
} Node;

typedef struct {
    int found;
    int comparisons;
} SearchResult;

/* Supplied assignment data: database order is also the BST insertion order. */
static const char *IDS[ID_COUNT] = {
    "A102", "A25", "A7", "B100",
    "B12", "A120", "B3", "A45"
};

static const char *TARGETS[3] = {"A45", "B3", "A120"};

/* strcmp order: pure lexicographical (dictionary) order for these IDs. */
static int compare_ids(const char *left, const char *right)
{
    return strcmp(left, right);
}

/* The caller owns the node storage; pointers remain valid for its lifetime. */
static Node *build_bst(Node pool[ID_COUNT])
{
    Node *root = NULL;
    int i;
    for (i = 0; i < ID_COUNT; ++i) {
        Node *node;
        Node *cur;
        strncpy(pool[i].id, IDS[i], ID_LEN - 1);
        pool[i].id[ID_LEN - 1] = '\0';
        pool[i].left = NULL;
        pool[i].right = NULL;
        node = &pool[i];
        if (root == NULL) {
            root = node;
            if (trace_mode) {
                printf("| %d | %s | (root) | 0 | %s |\n", i + 1, node->id, node->id);
            }
            continue;
        }
        cur = root;
        if (trace_mode) {
            char path[256] = "";
            int comps = 0;
            const char *part[ID_COUNT];
            int part_count = 0;
            int k;
            while (cur != NULL) {
                char step[48];
                int cmp;
                snprintf(step, sizeof(step), "%s%s", (path[0] ? " -> " : ""), cur->id);
                strcat(path, step);
                cmp = compare_ids(node->id, cur->id);
                ++comps;
                if (cmp < 0) {
                    if (cur->left == NULL) {
                        cur->left = node;
                        break;
                    }
                    cur = cur->left;
                } else if (cmp > 0) {
                    if (cur->right == NULL) {
                        cur->right = node;
                        break;
                    }
                    cur = cur->right;
                } else {
                    break;
                }
            }
            inorder_collect(root, part, &part_count);
            printf("| %d | %s | %s | %d | ", i + 1, node->id, path, comps);
            for (k = 0; k < part_count; ++k) {
                printf("%s%s", k ? " " : "", part[k]);
            }
            puts(" |");
        } else {
            while (cur != NULL) {
                int cmp = compare_ids(node->id, cur->id);
                if (cmp < 0) {
                    if (cur->left == NULL) {
                        cur->left = node;
                        break;
                    }
                    cur = cur->left;
                } else if (cmp > 0) {
                    if (cur->right == NULL) {
                        cur->right = node;
                        break;
                    }
                    cur = cur->right;
                } else {
                    break;
                }
            }
        }
    }
    return root;
}

static void inorder_collect(const Node *root, const char *order[ID_COUNT], int *count)
{
    if (root == NULL) {
        return;
    }
    inorder_collect(root->left, order, count);
    order[*count] = root->id;
    (*count)++;
    inorder_collect(root->right, order, count);
}

static int tree_height(const Node *root)
{
    int lh, rh;
    if (root == NULL) {
        return -1;
    }
    lh = tree_height(root->left);
    rh = tree_height(root->right);
    return 1 + (lh > rh ? lh : rh);
}

static SearchResult linear_search(const char *db[ID_COUNT], int count, const char *target)
{
    SearchResult result = {0, 0};
    int i;
    if (trace_mode) {
        printf("\n### Linear Search: %s\n\n| Comparison | Index | Entry | Result |\n|---:|---:|---|---|\n", target);
    }
    for (i = 0; i < count; ++i) {
        int hit = compare_ids(db[i], target) == 0;
        ++result.comparisons;
        if (trace_mode) {
            printf("| %d | %d | %s | %s |\n", result.comparisons, i, db[i], hit ? "Found" : "Continue");
        }
        if (hit) {
            result.found = 1;
            break;
        }
    }
    if (trace_mode && !result.found) {
        puts("\nAll entries exhausted: not found.");
    }
    return result;
}

/* BST search follows strcmp left/right links from the root. */
static SearchResult bst_search(const Node *root, const char *target)
{
    SearchResult result = {0, 0};
    const Node *cur = root;
    if (trace_mode) {
        printf("\n### BST Search: %s\n\n| Comparison | Node | Action |\n|---:|---|---|\n", target);
    }
    while (cur != NULL) {
        int cmp = compare_ids(target, cur->id);
        ++result.comparisons;
        if (trace_mode) {
            const char *action = (cmp == 0) ? "Found" : ((cmp < 0) ? "go left" : "go right");
            printf("| %d | %s | %s |\n", result.comparisons, cur->id, action);
        }
        if (cmp == 0) {
            result.found = 1;
            break;
        } else if (cmp < 0) {
            cur = cur->left;
        } else {
            cur = cur->right;
        }
    }
    if (trace_mode && !result.found) {
        puts("\nReached NULL: not found.");
    }
    return result;
}

static void run_tests(Node *root, const char *inorder[ID_COUNT], int count)
{
    const char *sorted[ID_COUNT] = {
        "A102", "A120", "A25", "A45", "A7", "B100", "B12", "B3"
    };
    SearchResult a, b;
    const char *single_db[1] = {"A102"};
    int i;
    assert(count == ID_COUNT);
    assert(tree_height(root) == 5);
    assert(tree_height(root->left) == -1);
    for (i = 0; i < count; ++i) {
        assert(strcmp(inorder[i], sorted[i]) == 0);
    }
    a = bst_search(root, "A45");
    b = linear_search(IDS, ID_COUNT, "A45");
    assert(a.found == 1 && a.comparisons == 4);
    assert(b.found == 1 && b.comparisons == 8);
    a = bst_search(root, "B3");
    b = linear_search(IDS, ID_COUNT, "B3");
    assert(a.found == 1 && a.comparisons == 6);
    assert(b.found == 1 && b.comparisons == 7);
    a = bst_search(root, "A120");
    b = linear_search(IDS, ID_COUNT, "A120");
    assert(a.found == 1 && a.comparisons == 3);
    assert(b.found == 1 && b.comparisons == 6);
    assert(bst_search(root, "A102").comparisons == 1);
    assert(linear_search(IDS, ID_COUNT, "A102").comparisons == 1);
    assert(bst_search(root, "Z99").found == 0);
    assert(linear_search(IDS, ID_COUNT, "Z99").found == 0);
    assert(linear_search(IDS, ID_COUNT, "Z99").comparisons == 8);
    assert(bst_search(NULL, "A45").comparisons == 0);
    assert(linear_search(NULL, 0, "A45").comparisons == 0);
    assert(linear_search(single_db, 1, "A102").found == 1);
    assert(linear_search(single_db, 1, "ZZZ").found == 0);
    puts("All C self-tests passed.");
}

int main(int argc, char *argv[])
{
    Node pool[ID_COUNT];
    Node *root;
    const char *inorder[ID_COUNT];
    const char *db[ID_COUNT];
    int count = 0;
    int i;
    trace_mode = argc == 2 && strcmp(argv[1], "--trace") == 0;
    if (trace_mode) {
        puts("# Execution trace tables\n\nGenerated by `govids --trace`. One comparison means comparing the target with one ID entry.");
        puts("\n## BST construction (insertion in database order)\n\n| Step | Inserted ID | Path followed | Comparisons | Inorder after step |\n|---:|---|---|---:|---|");
    }
    root = build_bst(pool);
    inorder_collect(root, inorder, &count);
    for (i = 0; i < ID_COUNT; ++i) {
        db[i] = IDS[i];
    }
    if (trace_mode) {
        puts("\n## Inorder traversal (Left -> Root -> Right)\n");
        printf("Inorder: ");
        for (i = 0; i < count; ++i) {
            printf("%s%s", i ? " " : "", inorder[i]);
        }
        puts("\n");
        puts("## Search traces\n\nDatabase order is the original list; BST search follows tree links.");
        for (i = 0; i < 3; ++i) {
            (void)linear_search(db, ID_COUNT, TARGETS[i]);
            (void)bst_search(root, TARGETS[i]);
        }
        return EXIT_SUCCESS;
    }
    if (argc == 2 && strcmp(argv[1], "--test") == 0) {
        run_tests(root, inorder, count);
        return EXIT_SUCCESS;
    }
    if (argc != 1) {
        fprintf(stderr, "Usage: %s [--test|--trace]\n", argv[0]);
        return EXIT_FAILURE;
    }
    puts("GOVERNMENT ID DATABASE - BST INORDER TRAVERSAL");
    printf("Database order: ");
    for (i = 0; i < ID_COUNT; ++i) {
        printf("%s%s", i ? ", " : "", db[i]);
    }
    printf("\nInorder traversal: ");
    for (i = 0; i < count; ++i) {
        printf("%s%s", i ? " " : "", inorder[i]);
    }
    puts("\n(Inorder visits Left -> Root -> Right, giving strcmp-sorted order.)");
    printf("\nTree height: %d edges (%d levels)\n", tree_height(root), tree_height(root) + 1);
    puts("Longest root-to-leaf: A102 -> A25 -> A7 -> B100 -> B12 -> B3");
    puts("\nSEARCH COMPARISONS");
    printf("%-8s%-12s%8s%8s\n", "Target", "Result", "BST", "Linear");
    for (i = 0; i < 3; ++i) {
        SearchResult a = bst_search(root, TARGETS[i]);
        SearchResult b = linear_search(db, ID_COUNT, TARGETS[i]);
        printf("%-8s%-12s%8d%8d\n", TARGETS[i], a.found ? "Found" : "Not found",
               a.comparisons, b.comparisons);
    }
    puts("\nOne comparison means comparing the target with one ID entry.");
    return EXIT_SUCCESS;
}
