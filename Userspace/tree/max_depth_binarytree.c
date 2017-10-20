#include <stdio.h>
#include <stdlib.h>

struct node {
    int data;
    struct node *left;
    struct node *right;
};

int max_depth(struct node *node)
{
    if (node == NULL) return 0;

    int ldepth = max_depth(node->left);
    int rdepth = max_depth(node->right);
    if (ldepth > rdepth) return (ldepth + 1);
    else return (rdepth + 1);
}

struct node *add_node(int data)
{
    struct node *node = (struct node *)malloc(sizeof(struct node));
    if (node == NULL) return;
    node->data  = data;
    node->left  = NULL;   
    node->right = NULL;   
 
    return node;
}

void padding(char ch, int n)
{
    int i;
    for (i = 0; i < n; i++)
    printf("%c%c%c%c%c%c%c", ch, ch, ch, ch, ch, ch, ch);
}

void dump_tree(struct node *node, int depth)
{
    if (node == NULL) return;
    dump_tree(node->left, depth+1);
    padding(' ', depth);   
    printf("[%d]%d\n", depth, node->data); 
    dump_tree(node->right, depth+1);
}

void main()
{
    struct node *root = NULL;
    
    root = add_node(60);

    root->left = add_node(41);
    root->right = add_node(74);

    root->left->left = add_node(16);
    root->left->right = add_node(53);
    root->right->left = add_node(65);

    root->left->left->right = add_node(25);
    root->left->right->left = add_node(46);
    root->left->right->right = add_node(55);
    root->right->left->left = add_node(63);
    root->right->left->right = add_node(70);
  
    root->left->right->left->left = add_node(42);
    root->right->left->left->left = add_node(62);
    root->right->left->left->right = add_node(64);

    dump_tree(root, 1);
    printf("Maximum Depth or Height of a Tree = %d\n", max_depth(root));
}    
