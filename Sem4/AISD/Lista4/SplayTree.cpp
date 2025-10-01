#include <iostream>
#include <memory>
#include <algorithm>
#include <vector>
#include <string>
#include <random>
#include <fstream>
#include <queue>
class TreeNode {
public:
    int key;
    std::shared_ptr<TreeNode> left;
    std::shared_ptr<TreeNode> right;
    std::weak_ptr<TreeNode> parent; 

    TreeNode(int k) : key(k), left(nullptr), right(nullptr), parent() {}
};
struct OperationStats {
    long long key_comparisons = 0;
    long long pointer_reads = 0;
    long long pointer_writes = 0;
};
class SplayTree {
private:
    std::shared_ptr<TreeNode> root;
    OperationStats* stats;

    void leftRotate(std::shared_ptr<TreeNode> x) {
        if (stats) stats->pointer_reads++;
        auto y = x->right;
        if (stats) stats->pointer_reads++;
        if (!y) return;
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
        if (!x->parent.lock()) {
            root = y;
            if (stats) stats->pointer_writes++;
        }
        else {
            if (stats) stats->pointer_reads++;
            if (x == x->parent.lock()->left) {
                if (stats) stats->pointer_reads++;
                x->parent.lock()->left = y;
                if (stats) stats->pointer_writes++;
            }
            else {
                if (stats) stats->pointer_reads++;
                x->parent.lock()->right = y;
                if (stats) stats->pointer_writes++;
            }
        }
        if (stats) stats->pointer_reads++;
        y->left = x;
        if (stats) stats->pointer_writes++;
        if (stats) stats->pointer_reads++;
        x->parent = y;
        if (stats) stats->pointer_writes++;
    }

    void rightRotate(std::shared_ptr<TreeNode> x) {
        if (stats) stats->pointer_reads++;
        auto y = x->left;
        if (stats) stats->pointer_reads++;
        if (!y) return;
        if (stats) stats->pointer_reads++;
        x->left = y->right;
        if (stats) stats->pointer_writes++;
        if (stats) stats->pointer_reads++;
        if (y->right) {
            if (stats) stats->pointer_reads++;
            y->right->parent = x;
            if (stats) stats->pointer_writes++;
        }
        if (stats) stats->pointer_reads++;
        y->parent = x->parent;
        if (stats) stats->pointer_writes++;
        if (stats) stats->pointer_reads++;
        if (!x->parent.lock()) {
            root = y;
            if (stats) stats->pointer_writes++;
        }
        else {
            if (stats) stats->pointer_reads++;
            if (x == x->parent.lock()->right) {
                if (stats) stats->pointer_reads++;
                x->parent.lock()->right = y;
                if (stats) stats->pointer_writes++;
            }
            else {
                if (stats) stats->pointer_reads++;
                x->parent.lock()->left = y;
                if (stats) stats->pointer_writes++;
            }
        }
        if (stats) stats->pointer_reads++;
        y->right = x;
        if (stats) stats->pointer_writes++;
        if (stats) stats->pointer_reads++;
        x->parent = y;
        if (stats) stats->pointer_writes++;
    }

    void splay(std::shared_ptr<TreeNode> x) {
        while (true) {
            if (stats) stats->pointer_reads++;
            auto p = x->parent.lock();
            if (!p) break;
            if (stats) stats->pointer_reads++;
            auto gp = p->parent.lock();
            if (!gp) {
                if (stats) stats->pointer_reads++;
                if (x == p->left) rightRotate(p);
                else leftRotate(p);
            } else if (
                (stats ? stats->pointer_reads++, false : false), 
                (stats ? stats->pointer_reads++, false : false), 
                x == p->left && p == gp->left
            ) {
                rightRotate(gp);
                rightRotate(p);
            } else if (
                (stats ? stats->pointer_reads++, false : false), 
                (stats ? stats->pointer_reads++, false : false), 
                x == p->right && p == gp->right
            ) {
                leftRotate(gp);
                leftRotate(p);
            } else if (
                (stats ? stats->pointer_reads++, false : false), 
                (stats ? stats->pointer_reads++, false : false), 
                x == p->right && p == gp->left
            ) {
                leftRotate(p);
                rightRotate(gp);
            } else {
                if (stats) stats->pointer_reads += 2; 
                rightRotate(p);
                leftRotate(gp);
            }
        }
        root = x;
        if (stats) stats->pointer_writes++;
    }

    std::shared_ptr<TreeNode> searchNode(int k) {
        if (stats) stats->pointer_reads++;
        auto node = root;
        std::shared_ptr<TreeNode> last = nullptr;
        while (node) {
            last = node;
            if (stats) stats->key_comparisons++;
            if (k == node->key) break;
            if (stats) stats->pointer_reads++;
            if (k < node->key) {
                if (stats) stats->pointer_reads++;
                node = node->left;
            }
            else {
                if (stats) stats->pointer_reads++;
                node = node->right;
            }
        }
        if (last) splay(last);
        return node;
    }

public:
    SplayTree(OperationStats* stats_ptr = nullptr) : root(nullptr), stats(stats_ptr) {}

    void insert(int k) {
        std::shared_ptr<TreeNode> parent = nullptr;
        if (stats) stats->pointer_reads++;
        auto node = root;
        while (node) {
            parent = node;
            if (stats) stats->key_comparisons++;
            if (k < node->key) {
                if (stats) stats->pointer_reads++;
                node = node->left;
            }
            else {
                if (stats) stats->pointer_reads++;
                node = node->right;
            }
        }
        auto newNode = std::make_shared<TreeNode>(k);
        newNode->parent = parent;
        if (stats) stats->pointer_writes++;
        if (!parent) {
            root = newNode;
            if (stats) stats->pointer_writes++;
        }
        else if (k < parent->key) {
            if (stats) stats->pointer_reads++;
            parent->left = newNode;
            if (stats) stats->pointer_writes++;
        }
        else {
            if (stats) stats->pointer_reads++;
            parent->right = newNode;
            if (stats) stats->pointer_writes++;
        }
        splay(newNode);
    }

    void deleteKey(int k) {
        auto node = searchNode(k);
        if (stats) stats->pointer_reads++;
        if (!node || node->key != k) return;
        splay(node);
        if (stats) stats->pointer_reads++;
        if (!node->left) {
            if (stats) stats->pointer_reads++;
            root = node->right;
            if (stats) stats->pointer_writes++;
            if (stats) stats->pointer_reads++;
            if (root) {
                root->parent.reset();
                if (stats) stats->pointer_writes++;
            }
        }
        else {
            if (stats) stats->pointer_reads++;
            auto leftSub = node->left;
            leftSub->parent.reset();
            if (stats) stats->pointer_writes++;
            auto maxLeft = leftSub;
            if (stats) stats->pointer_reads++;
            while (maxLeft->right) {
                if (stats) stats->pointer_reads++;
                maxLeft = maxLeft->right;
            }
            splay(maxLeft);
            if (stats) stats->pointer_reads++;
            maxLeft->right = node->right;
            if (stats) stats->pointer_writes++;
            if (stats) stats->pointer_reads++;
            if (node->right) {
                node->right->parent = maxLeft;
                if (stats) stats->pointer_writes++;
            }
            root = maxLeft;
            if (stats) stats->pointer_writes++;
            if (stats) stats->pointer_reads++;
            root->parent.reset();
            if (stats) stats->pointer_writes++;
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

 void printSplay() {
    std::vector<char> left_trace(100, ' ');
    std::vector<char> right_trace(100, ' ');
    printSplayRec(root, 0, '-', left_trace, right_trace);
}

void printSplayRec(std::shared_ptr<TreeNode> node, int depth, char prefix,
                 std::vector<char>& left_trace, std::vector<char>& right_trace) {
    if (!node) return;
    if (node->left)
        printSplayRec(node->left, depth + 1, '/', left_trace, right_trace);
if (depth >= 1) {
    if (prefix == '/')
        left_trace[depth - 1] = '|';
    if (prefix == '\\')
        right_trace[depth - 1] = ' ';
}
    if (depth == 0) std::cout << "-";
    if (depth > 0) std::cout << " ";
    for (int i = 0; i < depth - 1; i++)
        if (left_trace[i] == '|' || right_trace[i] == '|')
            std::cout << "| ";
        else
            std::cout << "  ";
    if (depth > 0) std::cout << prefix << "-";
    std::cout << "[" << node->key << "]" << std::endl;

    left_trace[depth] = ' ';
    if (node->right) {
        right_trace[depth] = '|';
        printSplayRec(node->right, depth + 1, '\\', left_trace, right_trace);
    }
}
};

int main() {
    const int n = 30;
    std::vector<int> keys(n);
    for (int i = 0; i < n; ++i) keys[i] = i + 1;

    // PRZYPADEK 1: wstawianie rosnącego ciągu, usuwanie losowej permutacji
    std::cout << "PRZYPADEK 1: wstawianie rosnącego ciągu, usuwanie losowej permutacji\n";
    SplayTree tree1;
    for (int k : keys) {
        std::cout << "insert " << k << std::endl;
        tree1.insert(k);
        tree1.printSplay();
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
        tree1.printSplay();
        std::cout << std::endl;
    }

    // PRZYPADEK 2: wstawianie losowej permutacji, usuwanie losowej permutacji
    std::cout << "PRZYPADEK 2: wstawianie losowej permutacji, usuwanie losowej permutacji\n";
    SplayTree tree2;
    std::vector<int> perm2 = keys;
    std::shuffle(perm2.begin(), perm2.end(), g);

    for (int k : perm2) {
        std::cout << "insert " << k << std::endl;
        tree2.insert(k);
        tree2.printSplay();
        std::cout << std::endl;
    }
    std::vector<int> perm3 = keys;
    std::shuffle(perm3.begin(), perm3.end(), g);

    for (int k : perm3) {
        std::cout << "delete " << k << std::endl;
        tree2.deleteKey(k);
        tree2.printSplay();
        std::cout << std::endl;
    }

// std::ofstream out("splay_stats_combined.txt");
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
//             SplayTree tree(&stats);

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