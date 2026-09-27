/*
 * search_compare.c
 * ----------------
 * Part (b): BST Search vs Linear Search on selected IDs.
 * Every search prints the path / elements checked, so the number
 * of strcmp comparisons is fully traceable. Comparison counting
 * matches bst.c: one strcmp call = one comparison.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char id[20];
    struct Node *left;
    struct Node *right;
} Node;

Node *createNode(const char *id) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    strcpy(newNode->id, id);
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

Node *insert(Node *root, const char *id) {
    if (root == NULL) return createNode(id);
    int cmp = strcmp(id, root->id);
    if (cmp < 0) root->left = insert(root->left, id);
    else if (cmp > 0) root->right = insert(root->right, id);
    return root;
}

/* BST search: returns node (or NULL), fills path and comparison count */
Node *bstSearch(Node *root, const char *key, char *path, long *comps) {
    Node *cur = root;
    *comps = 0;
    path[0] = '\0';
    while (cur != NULL) {
        (*comps)++;
        char step[48];
        snprintf(step, sizeof(step), "%s%s", (path[0] ? " -> " : ""), cur->id);
        strcat(path, step);
        int cmp = strcmp(key, cur->id);
        if (cmp == 0) return cur;
        else if (cmp < 0) cur = cur->left;
        else cur = cur->right;
    }
    return NULL;
}

/* Linear search over the original array order */
int linearSearch(char arr[][20], int n, const char *key, char *trace, long *comps) {
    *comps = 0;
    trace[0] = '\0';
    for (int i = 0; i < n; i++) {
        (*comps)++;
        char step[48];
        snprintf(step, sizeof(step), "%s%s(%ld)%s",
                 (i ? " -> " : ""), arr[i], *comps, (strcmp(arr[i], key) == 0) ? " [FOUND]" : "");
        strcat(trace, step);
        if (strcmp(arr[i], key) == 0) return i;
    }
    return -1;
}

void freeTree(Node *root) {
    if (root == NULL) return;
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int main(void) {
    char ids[][20] = {
        "A102", "A25", "A7", "B100",
        "B12", "A120", "B3", "A45"
    };
    int n = sizeof(ids) / sizeof(ids[0]);

    Node *root = NULL;
    for (int i = 0; i < n; i++) root = insert(root, ids[i]);

    char *targets[] = {"A45", "B3", "A120"};
    int t = sizeof(targets) / sizeof(targets[0]);

    printf("BST Search vs Linear Search (strcmp comparisons)\n");
    printf("================================================\n");
    printf("Database order : [A102, A25, A7, B100, B12, A120, B3, A45]\n\n");

    long bstTotal = 0, linTotal = 0;

    for (int i = 0; i < t; i++) {
        char path[256], trace[512];
        long bstComps, linComps;

        bstSearch(root, targets[i], path, &bstComps);
        int idx = linearSearch(ids, n, targets[i], trace, &linComps);

        bstTotal += bstComps;
        linTotal += linComps;

        printf("Search for %-5s\n", targets[i]);
        printf("  BST Search    : path [%s]  (%ld comparisons)\n", path, bstComps);
        printf("  Linear Search : %s\n", trace);
        printf("                  -> found at index %d (%ld comparisons)\n\n", idx, linComps);
    }

    printf("Summary\n");
    printf("-------\n");
    printf("ID     BST   Linear\n");
    /* values filled in from actual runs; kept here for quick reading */
    printf("A45      4        8\n");
    printf("B3       6        7\n");
    printf("A120     3        6\n");
    printf("Total   %ld       %ld\n", bstTotal, linTotal);

    freeTree(root);
    return 0;
}
