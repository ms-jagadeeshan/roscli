#include "terminator-layout-generater.h"

enum
{
    TERMINAL,
    VSPLIT_CHILD,
    HSPLIT_CHILD,
    WINDOW
};

/**
 * @brief Function to insert child to node
 *
 * @param node_d_ptr
 * @param data_ptr
 */
struct treenode *insert(treenode **node_d_ptr, struct datanode *data_ptr);

std::string get_type(struct treenode *root);
std::string get_name(struct treenode *root);
void iterate(struct treenode *root);
int main(int argc, char **argv)
{
    /**
     * @brief counting variables
     *
     */
    int child_count = 0;
    int window_count = 0;
    int terminal_count = 0;

    short int node_type;
    if (argc < 2)
    {
        node_type = TERMINAL;
        terminal_count++;
    }
    else if (atoi(argv[1]) == 1)
    {
        node_type = VSPLIT_CHILD;
        child_count++;
    }
    else if (atoi(argv[1]) == 2)
    {
        node_type = HSPLIT_CHILD;
        child_count++;
    }
    else
    {
        std::cerr << "Invalid arguments!\n";
        exit(1);
    }

    /**
     * @brief pointer for root node of tree
     *
     */
    struct treenode *root = NULL;
    struct datanode data = {node_type, 0};
    insert(&root, &data);

    std::stack<struct treenode *> tree_node_stack;
    tree_node_stack.push(root);
    int arg_i = 2, arg;

    // removing terminal from stack
    if (!tree_node_stack.empty() && tree_node_stack.top()->data.type == TERMINAL)
        tree_node_stack.pop();

    while (!tree_node_stack.empty())
    {

        // generating argument as 0 in the end
        if (arg_i >= argc)
            arg = 0;
        else
            arg = atoi(argv[arg_i++]);

        switch (arg)
        {
        case 0:
        {
            data = {TERMINAL, terminal_count++};
            insert(&(tree_node_stack.top()), &data);
            break;
        }
        case 1:
        {
            data = {VSPLIT_CHILD, child_count++};
            tree_node_stack.push(insert(&(tree_node_stack.top()), &data));
            break;
        }
        case 2:
        {
            data = {HSPLIT_CHILD, child_count++};

            tree_node_stack.push(insert(&(tree_node_stack.top()), &data));
            break;
        }
        }

        // removing terminal and fully filled node from stack
        while (!tree_node_stack.empty())
        {
            if (tree_node_stack.top()->left_child != NULL && tree_node_stack.top()->right_child != NULL)
                tree_node_stack.pop();
            else if (tree_node_stack.top()->data.type == TERMINAL)
                tree_node_stack.pop();
            else
                break;
        }
    }

    char space[] = "      ";

    printf("  [[roscli]]\n    [[[window0]]]\n%sparent = \"\"\n%stype = Window\n", space, space);
    printf("    [[[%s]]]\n%sparent = %s\n%stype = %s\n", get_name(root).c_str(), space, "window0", space, get_type(root).c_str());

    // remaining layout
    iterate(root);

    // identifier text for bash script
    printf("%sidentifier = roscli\n", space);
}

void iterate(struct treenode *root)
{
    if (root != NULL)
    {
        if (root->left_child == NULL)
        {
            return;
        }
        char space[] = "      ";
        printf("    [[[%s]]]\n%sorder = 0\n%sparent = %s\n%stype = %s\n", get_name(root->left_child).c_str(), space, space, get_name(root).c_str(), space, get_type(root->left_child).c_str());
        iterate(root->left_child);
        printf("    [[[%s]]]\n%sorder = 1\n%sparent = %s\n%stype = %s\n", get_name(root->right_child).c_str(), space, space, get_name(root).c_str(), space, get_type(root->right_child).c_str());
        iterate(root->right_child);
    }
}

std::string get_name(struct treenode *root)
{
    std::string child_name;
    if (root->data.type == TERMINAL)
        child_name = "terminal";
    else if (root->data.type == VSPLIT_CHILD || root->data.type == HSPLIT_CHILD)
        child_name = "child";
    else if (root->data.type == WINDOW)
        child_name = "window";

    child_name.append(std::to_string(root->data.value));
    return child_name;
}
std::string get_type(struct treenode *root)
{
    std::string child_name;
    if (root->data.type == TERMINAL)
        child_name = "Terminal";
    else if (root->data.type == VSPLIT_CHILD)
        child_name = "HPaned";
    else if (root->data.type == HSPLIT_CHILD)
        child_name = "VPaned";
    else if (root->data.type == WINDOW)
        child_name = "Window";
    return child_name;
}

struct treenode *insert(struct treenode **node_d_ptr, struct datanode *data_ptr)
{
    if (!(*node_d_ptr))
    {
        (*node_d_ptr) = new struct treenode;
        (*node_d_ptr)->data.type = data_ptr->type;
        (*node_d_ptr)->data.value = data_ptr->value;
        (*node_d_ptr)->left_child = NULL;
        (*node_d_ptr)->right_child = NULL;
        return *node_d_ptr;
    }
    else if (!(*node_d_ptr)->left_child)
    {
        auto tmp_ptr = new struct treenode;
        tmp_ptr->data.type = data_ptr->type;
        tmp_ptr->data.value = data_ptr->value;
        (*node_d_ptr)->left_child = tmp_ptr;
        return (*node_d_ptr)->left_child;
    }
    else if (!(*node_d_ptr)->right_child)
    {
        auto tmp_ptr = new struct treenode;
        tmp_ptr->data.type = data_ptr->type;
        tmp_ptr->data.value = data_ptr->value;
        (*node_d_ptr)->right_child = tmp_ptr;
        return (*node_d_ptr)->right_child;
    }
    return NULL;
}

void print_n(int num)
{
    for (int i = 0; i < num; i++)
    {
        printf("  ");
    }
}
