#ifndef TERMINATOR_LAYOUT_GENERATER_H
#define TERMINATOR_LAYOUT_GENERATER_H

#include <iostream>
#include <stack>

/**
 * @brief Struct which contains data part of tree node
 *
 */
struct datanode
{
    short int type; //  0 - terminal, 1 - vsplit child, 2 - hsplit child, 4 - window,
    int value;
};

/**
 * @brief Struct which is node in tree
 *
 */
struct treenode
{
    struct datanode data;
    struct treenode *left_child;
    struct treenode *right_child;
};

struct queue
{
    struct treenode *node_ptr;
    struct queue *next;
};

int height(treenode *root);
void free_tree(treenode *root);
int power(int base, int power);
void printspace(int n);
void enqueue(treenode *node_ptr, queue **FRONT, queue **REAR);
void dequeue(queue **FRONT);
void preOrder(treenode *root);
void inOrder(treenode *root);
void postOrder(treenode *root);
void rowOrder(struct treenode *root);

int height(treenode *root)
{
    if (root != NULL)
    {
        return 0;
    }
    else
    {
        int left_height = height(root->left_child);
        int right_height = height(root->right_child);
        if (left_height > right_height)
        {
            return left_height + 1;
        }
        else
        {
            return right_height + 1;
        }
    }
}

void free_tree(treenode *root)
{
    if (root != NULL)
    {
        free_tree(root->left_child);
        free_tree(root->right_child);
        free(root);
    }
}

int power(int base, int power)
{
    int result = 1;
    for (int i = 0; i < power; i++)
    {
        result = result * base;
    }
    return result;
}

void printspace(int n)
{
    for (int i = 0; i < n; i++)
    {
        printf(" ");
    }
}

void enqueue(treenode *node_ptr, queue **FRONT, queue **REAR)
{
    if (*FRONT == NULL)
    {
        queue *tmp = (queue *)malloc(sizeof(queue));
        tmp->node_ptr = node_ptr;
        tmp->next = NULL;
        *FRONT = tmp;
        *REAR = tmp;
    }
    else
    {
        queue *tmp = (queue *)malloc(sizeof(queue));
        tmp->node_ptr = node_ptr;
        tmp->next = NULL;
        (*REAR)->next = tmp;
        *REAR = tmp;
    }
}

void dequeue(queue **FRONT)
{
    if (*FRONT != NULL)
    {
        queue *tmp = *FRONT;
        *FRONT = (*FRONT)->next;
        free(tmp);
    }
}

void preOrder(treenode *root)
{
    if (root != NULL)
    {
        printf(" %d", root->data.type);
        preOrder(root->left_child);
        preOrder(root->right_child);
    }
}

void inOrder(treenode *root)
{
    if (root != NULL)
    {
        inOrder(root->left_child);
        printf(" %d", root->data.type);
        inOrder(root->right_child);
    }
}

void postOrder(treenode *root)
{
    if (root != NULL)
    {
        postOrder(root->left_child);
        postOrder(root->right_child);
        printf(" %d", root->data.type);
    }
}

void rowOrder(struct treenode *root)
{
    if (root != NULL)
    {
        queue *FRONT = NULL;
        queue *REAR = NULL;
        enqueue(root, &FRONT, &REAR);

        while (FRONT != NULL)
        {
            printf("%d ", FRONT->node_ptr->data.type);
            if (FRONT->node_ptr->left_child != NULL)
                enqueue(FRONT->node_ptr->left_child, &FRONT, &REAR);
            if (FRONT->node_ptr->right_child != NULL)
                enqueue(FRONT->node_ptr->right_child, &FRONT, &REAR);

            dequeue(&FRONT);
        }
    }
}

#endif