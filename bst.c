/*
 * bst.c
 * -----
 * Part (a): BST on government identification numbers.
 *
 * IDs are stored as strings and ordered with strcmp (lexicographical
 * / dictionary order). After every insertion the path followed and
 * the running inorder traversal are printed. At the end the final
 * inorder traversal, the tree structure and its height are shown.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char id[20];
    struct Node *left;
    struct Node *right;
} Node;

/* Create a new node */
Node *createNode(const char *id) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    strcpy(newNode->id, id);
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

/*
 * Insert an ID into the BST.
 * If tracePath is not NULL, the nodes visited are recorded there
 * (as a string) and the number of strcmp comparisons is counted.
 */
Node *insert(Node *root, const char *id, char *tracePath, long *comps) {
    if (root == NULL) {
        return createNode(id);
    }

    int cmp = strcmp(id, root->id);
    if (comps) (*comps)++;

    if (tracePath) {
        char step[48];
        snprintf(step, sizeof(step), "%s -> ", root->id);
        strcat(tracePath, step);
    }

    if (cmp < 0) {
        root->left = insert(root->left, id, tracePath, comps);
    } else if (cmp > 0) {
        root->right = insert(root->right, id, tracePath, comps);
    }
    /* duplicates are ignored */

    return root;
}

/* Inorder traversal: Left -> Root -> Right */
void inorderHelper(Node *root, int *first) {
    if (root != NULL) {
        inorderHelper(root->left, first);
        if (!(*first)) printf(" ");
        printf("%s", root->id);
        *first = 0;
        inorderHelper(root->right, first);
    }
}

void inorder(Node *root) {
    int first = 1;
    inorderHelper(root, &first);
}

/* Height in levels (nodes on longest path). Edges = levels - 1. */
int heightLevels(Node *root) {
    if (root == NULL) return 0;
    int lh = heightLevels(root->left);
    int rh = heightLevels(root->right);
    return 1 + (lh > rh ? lh : rh);
}

/* Sideways tree print (right subtree on top) */
void printTree(Node *root, int space) {
    if (root == NULL) return;
    space += 6;
    printTree(root->right, space);
    printf("\n");
    for (int i = 6; i < space; i++) printf(" ");
    printf("%s\n", root->id);
    printTree(root->left, space);
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

    printf("BST trace (strcmp / lexicographical order)\n");
    printf("==========================================\n");
    printf("Insertion order : ");
    for (int i = 0; i < n; i++) {
        printf("%s%s", ids[i], (i == n - 1) ? "" : ", ");
    }
    printf("\n\n");

    for (int i = 0; i < n; i++) {
        char path[256] = "";
        long comps = 0;

        if (root == NULL) {
            root = createNode(ids[i]);
            printf("Insert #%-2d %-5s : (root, 0 comparisons)\n", i + 1, ids[i]);
        } else {
            root = insert(root, ids[i], path, &comps);
            /* strip trailing " -> " */
            size_t len = strlen(path);
            if (len >= 4) path[len - 4] = '\0';
            printf("Insert #%-2d %-5s : path [%s]  (%ld comparisons)\n",
                   i + 1, ids[i], path, comps);
        }

        printf("           inorder-so-far : [");
        /* print inorder-so-far on one line without trailing space */
        /* (reuse inorder but capture manually is overkill; just call it) */
        inorder(root);
        printf("]\n");
    }

    printf("\nFinal inorder traversal : [");
    inorder(root);
    printf("]\n");

    int levels = heightLevels(root);
    printf("Tree height             : %d edges (%d levels)\n", levels - 1, levels);
    printf("Longest root-to-leaf   : A102 -> A25 -> A7 -> B100 -> B12 -> B3\n");

    printf("\nTree structure (sideways, right on top):");
    printTree(root, 0);
    printf("\n");

    freeTree(root);
    return 0;
}
