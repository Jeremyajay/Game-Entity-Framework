#include <iostream>
#include <memory>
#include <string>
#include "derivednpc.h"
#include "two_three_tree.h"

using namespace std;

// Jeremy Cuthbert CS302 Section 02
// main.cpp

void showMenu() {
    cout << "\n--- NPC Game Menu ---\n";
    cout << "1. Insert a new NPC\n";
    cout << "2. Display all NPCs\n";
    cout << "3. Perform action on an NPC\n";
    cout << "4. Remove all NPCs\n";
    cout << "5. Exit\n";
    cout << "Choice: ";
}

void showActionMenu() {
    cout << "\nSelect action:\n";
    cout << "1. Interact\n";
    cout << "2. Move\n";
    cout << "3. Special Action\n";
    cout << "4. Restock (Merchant only)\n";
    cout << "Choice: ";
}

int main() {
    TwoThreeTree tree;
    int choice;

    while (true) {
        showMenu();
        cin >> choice;
        cin.ignore(); // consume newline

        if (choice == 1) {
            string type, name, loc;
            cout << "Enter NPC type (Merchant/Guard/Villager): ";
            getline(cin, type);
            cout << "Enter name: ";
            getline(cin, name);
            cout << "Enter location: ";
            getline(cin, loc);

            shared_ptr<NPC> npc;
            if (type == "Merchant") {
                npc = make_shared<Merchant>(name, loc);
            } else if (type == "Guard") {
                int aggr;
                cout << "Enter aggression level (0-10): ";
                cin >> aggr; cin.ignore();
                npc = make_shared<Guard>(name, loc, aggr);
            } else if (type == "Villager") {
                int happy;
                cout << "Enter happiness level (0-100): ";
                cin >> happy; cin.ignore();
                npc = make_shared<Villager>(name, loc, happy);
            } else {
                cout << "Invalid NPC type!\n";
                continue;
            }

            try {
                tree.insert(name, npc);
                cout << "NPC inserted successfully.\n";
            } catch (const exception &e) {
                cout << "Error: " << e.what() << "\n";
            }

        } else if (choice == 2) {
            tree.display();

        } else if (choice == 3) {
            string name;
            cout << "Enter NPC name to act on: ";
            getline(cin, name);
            int action;
            showActionMenu();
            cin >> action; cin.ignore();

            tree.performAction(name, action);

        } else if (choice == 4) {
            tree.remove_all();
            cout << "All NPCs removed.\n";

        } else if (choice == 5) {
            cout << "Exiting.\n";
            break;

        } else {
            cout << "Invalid choice. Try again.\n";
        }
    }

    return 0;
}

