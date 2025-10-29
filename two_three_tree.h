#ifndef TWOTHREETREE_H
#define TWOTHREETREE_H

#include <memory>
#include <string>
#include "npc.h"
#include "derivednpc.h" // for dynamic_cast to Merchant

// Jeremy Cuthbert CS302 Section 02
// two_three_tree.h

class Node {
public:
    Node() = default;
    Node(const std::string & key, std::shared_ptr<NPC> npc);

    bool is_leaf() const;
    bool is_full() const;

    std::string keys[2];
    std::shared_ptr<NPC> npcs[2];
    std::unique_ptr<Node> children[3];

    int num_keys {0};
};

class TwoThreeTree {
public:
    TwoThreeTree() = default;
    ~TwoThreeTree() = default;

    TwoThreeTree(const TwoThreeTree &) = delete;
    TwoThreeTree & operator=(const TwoThreeTree &) = delete;

    void insert(const std::string & key, std::shared_ptr<NPC> npc);
    void display() const;
    void remove_all();

    // user-interaction helper
    void performAction(const std::string &name, int actionChoice);
    std::shared_ptr<NPC> search(const std::string & key) const;

private:
    std::unique_ptr<Node> root;

    void display_recursive(const Node * current, int depth) const;
    void remove_all_recursive(std::unique_ptr<Node> & current);

    std::unique_ptr<Node> insert_recursive(std::unique_ptr<Node> & current,
                                           const std::string & key,
                                           std::shared_ptr<NPC> npc,
                                           std::string & promoted_key,
                                           std::unique_ptr<Node> & promoted_child);
    std::shared_ptr<NPC> search_recursive(const Node * current, const std::string & key) const;
};

#endif // TWOTHREETREE_H

