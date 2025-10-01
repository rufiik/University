#include <iostream>
#include <memory>
#include <vector>
#include <algorithm>
#include <random>
#include <fstream>
#include <string>
#include <queue>

enum Color { RED, BLACK };

class TreeNode {
public:
    int key;
    Color color;
    std::shared_ptr<TreeNode> left, right, parent;

    TreeNode(int k)
        : key(k), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
};
struct OperationStats {
    long long key_comparisons = 0;
    long long pointer_reads = 0;
    long long pointer_writes = 0;
};
class RedBlackTree {
private:
    std::shared_ptr<TreeNode> root;
    OperationStats* stats;

void leftRotate(std::shared_ptr<TreeNode> x) {
    if (stats) stats->pointer_reads++;
    auto y = x->right;
    if (stats) stats->pointer_reads++;
    x->right = y->left;
    if (stats) stats->pointer_writes++;
    if (stats) stats->pointer_reads++;
    if (y->left) {
        if (stats) stats->pointer_reads++;
        y->left->parent = x;
        if (stats) stats->pointer_writes++;
    }

    if (stats) stats->pointer_reads++;
    y->parent = x->parent;
    if (stats) stats->pointer_writes++;
    if (stats) stats->pointer_reads++;
    if (!x->parent) {
        root = y;
        if (stats) stats->pointer_writes++;
    }
    else {
        if (stats) stats->pointer_reads++;
        if (x == x->parent->left) {
            if (stats) stats->pointer_reads++;
            x->parent->left = y;
            if (stats) stats->pointer_writes++;
        }
        else {
            if (stats) stats->pointer_reads++;
            x->parent->right = y;
            if (stats) stats->pointer_writes++;
        }
    }

    y->left = x;
    if (stats) stats->pointer_writes++;
    x->parent = y;
    if (stats) stats->pointer_writes++;
}

void rightRotate(std::shared_ptr<TreeNode> y) {
    if (stats) stats->pointer_reads++;
    auto x = y->left;
    if (stats) stats->pointer_reads++;
    y->left = x->right;
    if (stats) stats->pointer_writes++;
    if (stats) stats->pointer_reads++;
    if (x->right) {
        if (stats) stats->pointer_reads++;
        x->right->parent = y;
        if (stats) stats->pointer_writes++;
    }

    if (stats) stats->pointer_reads++;
    x->parent = y->parent;
    if (stats) stats->pointer_writes++;
    if (stats) stats->pointer_reads++;
    if (!y->parent) {
        root = x;
        if (stats) stats->pointer_writes++;
    }
    else {
        if (stats) stats->pointer_reads++;
        if (y == y->parent->right) {
            if (stats) stats->pointer_reads++;
            y->parent->right = x;
            if (stats) stats->pointer_writes++;
        }
        else {
            if (stats) stats->pointer_reads++;
            y->parent->left = x;
            if (stats) stats->pointer_writes++;
        }
    }

    x->right = y;
    if (stats) stats->pointer_writes++;
    y->parent = x;
    if (stats) stats->pointer_writes++;
}

void fixInsert(std::shared_ptr<TreeNode> z) {
    while (true) {
        if (stats) stats->pointer_reads++;
        if (!z->parent) break;
        if (stats) stats->pointer_reads++;
        if (z->parent->color != RED) break;
        if (stats) stats->pointer_reads++;
        auto gp = z->parent->parent;
        if (stats) stats->pointer_reads++;
        if (z->parent == gp->left) {
            if (stats) stats->pointer_reads++;
            auto y = gp->right;
            if (stats) stats->pointer_reads++;
            if (y && y->color == RED) {
                if (stats) stats->pointer_reads++;
                z->parent->color = BLACK;
                if (stats) stats->pointer_writes++;
                y->color = BLACK;
                if (stats) stats->pointer_writes++;
                gp->color = RED;
                if (stats) stats->pointer_writes++;
                z = gp;
                if (stats) stats->pointer_reads++;
            } else {
                if (stats) stats->pointer_reads++;
                if (z == z->parent->right) {
                    if (stats) stats->pointer_reads++;
                    z = z->parent;
                    if (stats) stats->pointer_reads++;
                    leftRotate(z);
                }
                z->parent->color = BLACK;
                if (stats) stats->pointer_writes++;
                gp->color = RED;
                if (stats) stats->pointer_writes++;
                rightRotate(gp);
            }
        } else {
            if (stats) stats->pointer_reads++;
            auto y = gp->left;
            if (stats) stats->pointer_reads++;
            if (y && y->color == RED) {
                if (stats) stats->pointer_reads++;
                z->parent->color = BLACK;
                if (stats) stats->pointer_writes++;
                y->color = BLACK;
                if (stats) stats->pointer_writes++;
                gp->color = RED;
                if (stats) stats->pointer_writes++;
                z = gp;
                if (stats) stats->pointer_reads++;
            } else {
                if (stats) stats->pointer_reads++;
                if (z == z->parent->left) {
                    if (stats) stats->pointer_reads++;
                    z = z->parent;
                    if (stats) stats->pointer_reads++;
                    rightRotate(z);
                }
                z->parent->color = BLACK;
                if (stats) stats->pointer_writes++;
                gp->color = RED;
                if (stats) stats->pointer_writes++;
                leftRotate(gp);
            }
        }
    }
    root->color = BLACK;
    if (stats) stats->pointer_writes++;
}

void transplant(std::shared_ptr<TreeNode> u, std::shared_ptr<TreeNode> v) {
    if (stats) stats->pointer_reads++;
    if (!u->parent) {
        root = v;
        if (stats) stats->pointer_writes++;
    } else {
        if (stats) stats->pointer_reads++;
        if (u == u->parent->left) {
            if (stats) stats->pointer_reads++;
            u->parent->left = v;
            if (stats) stats->pointer_writes++;
        } else {
            if (stats) stats->pointer_reads++;
            u->parent->right = v;
            if (stats) stats->pointer_writes++;
        }
    }
    if (v) {
        if (stats) stats->pointer_reads++;
        v->parent = u->parent;
        if (stats) stats->pointer_writes++;
    }
}

std::shared_ptr<TreeNode> minNode(std::shared_ptr<TreeNode> node) {
    while (true) {
        if (stats) stats->pointer_reads++;
        if (!node->left) break;
        node = node->left;
    }
    return node;
}

void fixDelete(std::shared_ptr<TreeNode> x, std::shared_ptr<TreeNode> parent) {
    while (true) {
        if (stats) stats->pointer_reads++;
        if (x == root) break;
        if (stats) stats->pointer_reads++;
        if (x && x->color != BLACK) break;
        std::shared_ptr<TreeNode> w;
        if (stats) stats->pointer_reads++;
        if (x == (parent ? parent->left : nullptr)) {
            if (stats) stats->pointer_reads++;
            w = parent->right;
            if (stats) stats->pointer_reads++;
            if (w && w->color == RED) {
                w->color = BLACK;
                if (stats) stats->pointer_writes++;
                parent->color = RED;
                if (stats) stats->pointer_writes++;
                leftRotate(parent);
                if (stats) stats->pointer_reads++;
                w = parent->right;
            }
            if (stats) stats->pointer_reads += 2;
            if ((!w->left || w->left->color == BLACK) && (!w->right || w->right->color == BLACK)) {
                w->color = RED;
                if (stats) stats->pointer_writes++;
                x = parent;
                if (stats) stats->pointer_reads++;
                parent = x ? x->parent : nullptr;
            } else {
                if (stats) stats->pointer_reads++;
                if (!w->right || w->right->color == BLACK) {
                    if (stats) stats->pointer_reads++;
                    if (w->left) {
                        w->left->color = BLACK;
                        if (stats) stats->pointer_writes++;
                    }
                    w->color = RED;
                    if (stats) stats->pointer_writes++;
                    rightRotate(w);
                    if (stats) stats->pointer_reads++;
                    w = parent->right;
                }
                w->color = parent->color;
                if (stats) stats->pointer_writes++;
                parent->color = BLACK;
                if (stats) stats->pointer_writes++;
                if (stats) stats->pointer_reads++;
                if (w->right) {
                    w->right->color = BLACK;
                    if (stats) stats->pointer_writes++;
                }
                leftRotate(parent);
                x = root;
            }
        } else {
            if (stats) stats->pointer_reads++;
            w = parent->left;
            if (stats) stats->pointer_reads++;
            if (w && w->color == RED) {
                w->color = BLACK;
                if (stats) stats->pointer_writes++;
                parent->color = RED;
                if (stats) stats->pointer_writes++;
                rightRotate(parent);
                if (stats) stats->pointer_reads++;
                w = parent->left;
            }
            if (stats) stats->pointer_reads += 2;
            if ((!w->right || w->right->color == BLACK) && (!w->left || w->left->color == BLACK)) {
                w->color = RED;
                if (stats) stats->pointer_writes++;
                x = parent;
                if (stats) stats->pointer_reads++;
                parent = x ? x->parent : nullptr;
            } else {
                if (stats) stats->pointer_reads++;
                if (!w->left || w->left->color == BLACK) {
                    if (stats) stats->pointer_reads++;
                    if (w->right) {
                        w->right->color = BLACK;
                        if (stats) stats->pointer_writes++;
                    }
                    w->color = RED;
                    if (stats) stats->pointer_writes++;
                    leftRotate(w);
                    if (stats) stats->pointer_reads++;
                    w = parent->left;
                }
                w->color = parent->color;
                if (stats) stats->pointer_writes++;
                parent->color = BLACK;
                if (stats) stats->pointer_writes++;
                if (stats) stats->pointer_reads++;
                if (w->left) {
                    w->left->color = BLACK;
                    if (stats) stats->pointer_writes++;
                }
                rightRotate(parent);
                x = root;
            }
        }
    }
    if (x) {
        if (stats) stats->pointer_reads++;
        x->color = BLACK;
        if (stats) stats->pointer_writes++;
    }
}
public:
    RedBlackTree(OperationStats* stats_ptr = nullptr) : root(nullptr), stats(stats_ptr) {}
    
void insert(int key) {
    auto z = std::make_shared<TreeNode>(key);
    std::shared_ptr<TreeNode> y = nullptr;
    auto x = root;
    if (stats) stats->pointer_reads++;
    while (x) {
        y = x;
        if (stats) stats->pointer_reads++;
        if (stats) stats->key_comparisons++;
        if (key < x->key) {
            if (stats) stats->pointer_reads++;
            x = x->left;
        } else {
            if (stats) stats->pointer_reads++;
            x = x->right;
        }
    }

    z->parent = y;
    if (stats) stats->pointer_writes++;
    if (stats) stats->pointer_reads++;
    if (!y) {
        root = z;
        if (stats) stats->pointer_writes++;
    }
    else {
        if (stats) stats->pointer_reads++;
        if (key < y->key) {
            y->left = z;
            if (stats) stats->pointer_writes++;
        }
        else {
            y->right = z;
            if (stats) stats->pointer_writes++;
        }
    }

    fixInsert(z);
}

void deleteKey(int key) {
    auto z = root;
    if (stats) stats->pointer_reads++;
    while (z && z->key != key) {
        if (stats) stats->pointer_reads++;
        if (stats) stats->key_comparisons++;
        if (key < z->key) {
            if (stats) stats->pointer_reads++;
            z = z->left;
        } else {
            if (stats) stats->pointer_reads++;
            z = z->right;
        }
    }
    if (stats) stats->pointer_reads++;
    if (!z) return; 

    auto y = z;
    if (stats) stats->pointer_reads++;
    Color y_original_color = y->color;
    std::shared_ptr<TreeNode> x;
    std::shared_ptr<TreeNode> x_parent; 

    if (stats) stats->pointer_reads++;
    if (!z->left) {
        if (stats) stats->pointer_reads++;
        x = z->right;
        if (stats) stats->pointer_reads++;
        x_parent = z->parent;
        transplant(z, z->right);
    } else if (!z->right) {
        if (stats) stats->pointer_reads++;
        x = z->left;
        if (stats) stats->pointer_reads++;
        x_parent = z->parent;
        transplant(z, z->left);
    } else {
        if (stats) stats->pointer_reads++;
        y = minNode(z->right);
        if (stats) stats->pointer_reads++;
        y_original_color = y->color;
        if (stats) stats->pointer_reads++;
        x = y->right;
        if (stats) stats->pointer_reads++;
        if (y->parent == z) {
            x_parent = y;
            if (stats) stats->pointer_reads++;
            if (x) {
                if (stats) stats->pointer_reads++;
                x->parent = y;
                if (stats) stats->pointer_writes++;
            }
        } else {
            if (stats) stats->pointer_reads++;
            x_parent = y->parent;
            transplant(y, y->right);
            if (stats) stats->pointer_reads++;
            y->right = z->right;
            if (stats) stats->pointer_writes++;
            if (stats) stats->pointer_reads++;
            if (y->right) {
                if (stats) stats->pointer_reads++;
                y->right->parent = y;
                if (stats) stats->pointer_writes++;
            }
        }
        transplant(z, y);
        if (stats) stats->pointer_reads++;
        y->left = z->left;
        if (stats) stats->pointer_writes++;
        if (stats) stats->pointer_reads++;
        if (y->left) {
            if (stats) stats->pointer_reads++;
            y->left->parent = y;
            if (stats) stats->pointer_writes++;
        }
        if (stats) stats->pointer_reads++;
        y->color = z->color;
        if (stats) stats->pointer_writes++;
    }
    if (stats) stats->pointer_reads++;
    if (y_original_color == BLACK) {
        fixDelete(x, x_parent);
    }
}
int height() {
    if (!root) return -1;
    std::queue<std::shared_ptr<TreeNode>> q;
    q.push(root);
    int h = -1;
    while (!q.empty()) {
        int level_size = q.size();
        h++;
        for (int i = 0; i < level_size; ++i) {
            auto node = q.front(); q.pop();
            if (node->left) q.push(node->left);
            if (node->right) q.push(node->right);
        }
    }
    return h;
}
    void inorder() {
        inorderRec(root);
        std::cout << std::endl;
    }

    void inorderRec(std::shared_ptr<TreeNode> node) {
        if (!node) return;
        inorderRec(node->left);
        std::cout << node->key << (node->color == RED ? "R " : "B ");
        inorderRec(node->right);
    }
    void printRBTree() {
    std::vector<char> left_trace(100, ' ');
    std::vector<char> right_trace(100, ' ');
    printRBTreeRec(root, 0, '-', left_trace, right_trace);
}
void printRBTreeRec(std::shared_ptr<TreeNode> node, int depth, char prefix,
                 std::vector<char>& left_trace, std::vector<char>& right_trace) {
    if (!node) return;
    if (node->left)
        printRBTreeRec(node->left, depth + 1, '/', left_trace, right_trace);

    if (prefix == '/')
        left_trace[depth - 1] = '|';
    if (prefix == '\\')
        right_trace[depth - 1] = ' ';

    if (depth == 0) std::cout << "-";
    if (depth > 0) std::cout << " ";
    for (int i = 0; i < depth - 1; i++)
        if (left_trace[i] == '|' || right_trace[i] == '|')
            std::cout << "| ";
        else
            std::cout << "  ";
    if (depth > 0) std::cout << prefix << "-";
    std::cout << "[" << node->key << (node->color == RED ? "R" : "B") << "]" << std::endl;

    left_trace[depth] = ' ';
    if (node->right) {
        right_trace[depth] = '|';
        printRBTreeRec(node->right, depth + 1, '\\', left_trace, right_trace);
    }
}

};

int main() {
     const int n = 30;
    std::vector<int> keys(n);
    for (int i = 0; i < n; ++i) keys[i] = i + 1;

    // PRZYPADEK 1: wstawianie rosnącego ciągu, usuwanie losowej permutacji
    std::cout << "PRZYPADEK 1: wstawianie rosnącego ciągu, usuwanie losowej permutacji\n";
    RedBlackTree tree1;
    for (int k : keys) {
        std::cout << "insert " << k << std::endl;
        tree1.insert(k);
        tree1.printRBTree();
        std::cout << std::endl;
    }
    // permutacja do usuwania
    std::random_device rd;
    std::mt19937 g(rd());
    std::vector<int> perm1 = keys;
    std::shuffle(perm1.begin(), perm1.end(), g);

    for (int k : perm1) {
        std::cout << "delete " << k << std::endl;
        tree1.deleteKey(k);
        tree1.printRBTree();
        std::cout << std::endl;
    }

    // PRZYPADEK 2: wstawianie losowej permutacji, usuwanie losowej permutacji
    std::cout << "PRZYPADEK 2: wstawianie losowej permutacji, usuwanie losowej permutacji\n";
    RedBlackTree tree2;
    std::vector<int> perm2 = keys;
    std::shuffle(perm2.begin(), perm2.end(), g);

    for (int k : perm2) {
        std::cout << "insert " << k << std::endl;
        tree2.insert(k);
        tree2.printRBTree();
        std::cout << std::endl;
    }
    std::vector<int> perm3 = keys;
    std::shuffle(perm3.begin(), perm3.end(), g);

    for (int k : perm3) {
        std::cout << "delete " << k << std::endl;
        tree2.deleteKey(k);
        tree2.printRBTree();
        std::cout << std::endl;
    }

// std::ofstream out("rb_stats_combined.txt");
// out << "n,scenario,avg_cmp,max_cmp,avg_ptr,max_ptr,avg_height,max_height\n";

// std::vector<int> ns = {10000, 20000, 30000, 40000, 50000, 60000, 70000, 80000, 90000, 100000};
// int tests = 20;

// for (int n : ns) {
//     for (int scenario = 1; scenario <= 2; ++scenario) {
//         for (int t = 0; t < tests; ++t) {
//             long long sum_cmp = 0, max_cmp = 0;
//             long long sum_ptr = 0, max_ptr = 0;
//             long long sum_height = 0, max_height = 0;

//             OperationStats stats;
//             RedBlackTree tree(&stats);

//             std::vector<int> keys(n);
//             for (int i = 0; i < n; ++i) keys[i] = i + 1;

//             std::vector<int> insert_seq = keys;
//             std::vector<int> delete_seq = keys;

//             if (scenario == 2) {
//                 std::random_device rd;
//                 std::mt19937 g(rd());
//                 std::shuffle(insert_seq.begin(), insert_seq.end(), g);
//             }
//             std::random_device rd;
//             std::mt19937 g(rd());
//             std::shuffle(delete_seq.begin(), delete_seq.end(), g);

//             // INSERT
//             for (int k : insert_seq) {
//                 stats.key_comparisons = 0;
//                 stats.pointer_reads = 0;
//                 stats.pointer_writes = 0;

//                 tree.insert(k);
//                 int h = tree.height();

//                 sum_cmp += stats.key_comparisons;
//                 sum_ptr += stats.pointer_reads + stats.pointer_writes;
//                 sum_height += h;

//                 max_cmp = std::max(max_cmp, (long long)stats.key_comparisons);
//                 max_ptr = std::max(max_ptr, (long long)(stats.pointer_reads + stats.pointer_writes));
//                 max_height = std::max(max_height, (long long)h);
//             }

//             // DELETE
//             for (int k : delete_seq) {
//                 stats.key_comparisons = 0;
//                 stats.pointer_reads = 0;
//                 stats.pointer_writes = 0;

//                 tree.deleteKey(k);
//                 int h = tree.height();

//                 sum_cmp += stats.key_comparisons;
//                 sum_ptr += stats.pointer_reads + stats.pointer_writes;
//                 sum_height += h;

//                 max_cmp = std::max(max_cmp, (long long)stats.key_comparisons);
//                 max_ptr = std::max(max_ptr, (long long)(stats.pointer_reads + stats.pointer_writes));
//                 max_height = std::max(max_height, (long long)h);
//             }

//             int total_ops = 2 * n; // insert + delete
//             out << n << "," << scenario << ","
//                 << (double)sum_cmp / total_ops << "," << max_cmp << ","
//                 << (double)sum_ptr / total_ops << "," << max_ptr << ","
//                 << (double)sum_height / total_ops << "," << max_height << "\n";
//         }
//     }
// }
// out.close();

    return 0;
}
