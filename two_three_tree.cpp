#include "two_three_tree.h"
#include <iostream>
#include <algorithm>
#include <functional>

using namespace std;

/* ================= NODE ================= */

Node::Node(const string & key, shared_ptr<NPC> npc) {
    keys[0] = key;
    npcs[0] = npc;
    num_keys = 1;
}

bool Node::is_leaf() const {
    return !children[0];
}

bool Node::is_full() const {
    return num_keys == 2;
}

/* ================= TWO-THREE TREE ================= */

void TwoThreeTree::insert(const string & key, shared_ptr<NPC> npc) {
    string promoted_key;
    unique_ptr<Node> promoted_child;
    auto new_root = insert_recursive(root, key, npc, promoted_key, promoted_child);
    if (new_root) {
        // Root split, create new root
        auto old_root = move(root);
        root = make_unique<Node>();
        root->keys[0] = promoted_key;
        root->npcs[0] = nullptr; // internal node doesn't store NPC
        root->num_keys = 1;
        root->children[0] = move(old_root);
        root->children[1] = move(promoted_child);
    }
}

// Recursive insertion helper
unique_ptr<Node> TwoThreeTree::insert_recursive(unique_ptr<Node> & current,
                                                const string & key,
                                                shared_ptr<NPC> npc,
                                                string & promoted_key,
                                                unique_ptr<Node> & promoted_child) {
    if (!current) {
        current = make_unique<Node>(key, npc);
        return nullptr;
    }

    // Check for duplicates
    for (int i = 0; i < current->num_keys; ++i) {
        if (current->keys[i] == key) {
            throw runtime_error("Duplicate key: " + key);
        }
    }

    if (current->is_leaf()) {
        // Leaf node insertion
        if (!current->is_full()) {
            // Insert into leaf
            if (key < current->keys[0]) {
                current->keys[1] = current->keys[0];
                current->npcs[1] = current->npcs[0];
                current->keys[0] = key;
                current->npcs[0] = npc;
            } else {
                current->keys[1] = key;
                current->npcs[1] = npc;
            }
            current->num_keys = 2;
            return nullptr;
        } else {
            // Split leaf
            unique_ptr<Node> right = make_unique<Node>();
            if (key < current->keys[0]) {
                right->keys[0] = current->keys[1];
                right->npcs[0] = current->npcs[1];
                current->keys[1] = "";
                current->npcs[1] = nullptr;
                current->num_keys = 1;
                promoted_key = current->keys[0];
                current->keys[0] = key;
                current->npcs[0] = npc;
            } else if (key < current->keys[1]) {
                right->keys[0] = current->keys[1];
                right->npcs[0] = current->npcs[1];
                current->keys[1] = "";
                current->npcs[1] = nullptr;
                current->num_keys = 1;
                promoted_key = key;
                right->num_keys = 1;
            } else {
                right->keys[0] = key;
                right->npcs[0] = npc;
                right->num_keys = 1;
                promoted_key = current->keys[1];
                current->keys[1] = "";
                current->npcs[1] = nullptr;
                current->num_keys = 1;
            }
            promoted_child = move(right);
            return make_unique<Node>(); // signal split
        }
    } else {
        // Internal node
        int idx = (key < current->keys[0]) ? 0 : (current->num_keys == 1 || key < current->keys[1]) ? 1 : 2;
        string new_promoted;
        unique_ptr<Node> new_child;
        auto res = insert_recursive(current->children[idx], key, npc, new_promoted, new_child);
        if (!res) return nullptr;

        // Node split handling for internal nodes
        if (!current->is_full()) {
            if (idx == 0) {
                current->keys[1] = current->keys[0];
                current->keys[0] = new_promoted;
                current->children[2] = move(current->children[1]);
                current->children[1] = move(new_child);
            } else if (idx == 1) {
                current->keys[1] = new_promoted;
                current->children[2] = move(new_child);
            }
            current->num_keys = 2;
            return nullptr;
        } else {
            // Full node split
            unique_ptr<Node> right = make_unique<Node>();
            if (idx == 0) {
                right->keys[0] = current->keys[1];
                right->children[0] = move(current->children[1]);
                right->children[1] = move(current->children[2]);
                right->num_keys = 1;

                promoted_key = current->keys[0];
                current->keys[0] = new_promoted;
                current->children[1] = move(new_child);
                current->children[2] = nullptr;
                current->num_keys = 1;
            } else if (idx == 1) {
                right->keys[0] = current->keys[1];
                right->children[0] = move(new_child);
                right->children[1] = move(current->children[2]);
                right->num_keys = 1;
                promoted_key = current->keys[1];
                current->children[2] = nullptr;
                current->num_keys = 1;
            } else {
                right->keys[0] = new_promoted;
                right->children[0] = move(current->children[2]);
                right->children[1] = move(new_child);
                right->num_keys = 1;
                promoted_key = current->keys[1];
                current->children[2] = nullptr;
                current->num_keys = 1;
            }
            promoted_child = move(right);
            return make_unique<Node>();
        }
    }
}

void TwoThreeTree::display() const {
    display_recursive(root.get(), 0);
}

void TwoThreeTree::display_recursive(const Node * current, int depth) const {
    if (!current) return;
    if (!current->is_leaf()) display_recursive(current->children[0].get(), depth + 1);
    if (current->num_keys > 0) cout << string(depth*2, ' ') << current->keys[0] << "\n";
    if (!current->is_leaf() && current->num_keys > 1) display_recursive(current->children[1].get(), depth + 1);
    if (current->num_keys > 1) cout << string(depth*2, ' ') << current->keys[1] << "\n";
    if (!current->is_leaf()) display_recursive(current->children[2].get(), depth + 1);
}

void TwoThreeTree::remove_all() {
    remove_all_recursive(root);
    root.reset();
}

void TwoThreeTree::remove_all_recursive(unique_ptr<Node> & current) {
    if (!current) return;
    for (int i = 0; i < 3; ++i) {
        remove_all_recursive(current->children[i]);
    }
    current->num_keys = 0;
    for (int i = 0; i < 2; ++i) current->npcs[i].reset();
}

shared_ptr<NPC> TwoThreeTree::search(const string & key) const {
    return search_recursive(root.get(), key);
}

shared_ptr<NPC> TwoThreeTree::search_recursive(const Node * current, const string & key) const {
    if (!current) return nullptr;
    for (int i = 0; i < current->num_keys; ++i) {
        if (current->keys[i] == key) return current->npcs[i];
        if (key < current->keys[i] && current->children[i]) return search_recursive(current->children[i].get(), key);
    }
    if (current->num_keys < 2 && current->children[current->num_keys]) 
        return search_recursive(current->children[current->num_keys].get(), key);
    return nullptr;
}

void TwoThreeTree::performAction(const string &name, int actionChoice) {
    function<void(const Node*)> helper = [&](const Node *current) {
        if (!current) return;
        for (int i = 0; i < current->num_keys; ++i) {
            if (current->keys[i] == name) {
                auto npc = current->npcs[i];
                switch (actionChoice) {
                    case 1: npc->interact(); break;
                    case 2: npc->move(); break;
                    case 3: npc->specialAction(); break;
                    case 4: {
                        if (auto merchant = dynamic_pointer_cast<Merchant>(npc)) {
                            merchant->restock();
                        } else {
                            cout << "Not a merchant!\n";
                        }
                        break;
                    }
                    default: cout << "Invalid choice.\n";
                }
                return;
            }
        }
        for (int i = 0; i <= current->num_keys; ++i)
            helper(current->children[i].get());
    };
    helper(root.get());
}

