#include "derivednpc.h"
#include <iostream>
#include <stdexcept>
#include <algorithm> // for std::min
#include <cstdlib>   // for rand() if used (not required)
using namespace std;

// Jeremy Cuthbert CS302 Section 02
// derived_npcs.cpp
// Implementation of Merchant, Guard, and Villager classes
//
// This file implements the derived NPC classes declared in derived_npcs.h.
// All methods match the signatures in the headers exactly and include
// exception handling and inline documentation.

/* ===========================
   Merchant
   =========================== */

/**
 * @brief Construct a new Merchant object.
 * @param n Merchant name
 * @param loc Merchant initial location
 * @param inv Optional inventory list
 * @param pr Optional prices list (must match inv size)
 * @throws invalid_argument if inventory and prices sizes differ
 */
Merchant::Merchant(const string &n, const string &loc,
                   const vector<string> &inv, const vector<int> &pr)
    : NPC(n, loc), inventory(inv), prices(pr)
{
    // Validate that inventory and prices align
    if (inventory.size() != prices.size()) {
        throw invalid_argument("Merchant constructor: inventory and prices size mismatch.");
    }

    // Initialize featuredItem smart pointer to first item if available
    if (!inventory.empty()) {
        featuredItem = make_shared<string>(inventory.front());
    } else {
        featuredItem = make_shared<string>(""); // empty placeholder
    }
}

/**
 * @brief Merchant-specific interaction (shows wares).
 */
void Merchant::interact() {
    cout << name << " (Merchant) says: Welcome to my shop in " << location << "!\n";
    if (inventory.empty()) {
        cout << "  Sorry, " << name << " is sold out.\n";
    } else {
        cout << "  Available items:\n";
        for (size_t i = 0; i < inventory.size(); ++i) {
            cout << "   - " << inventory[i] << " (" << prices[i] << " gold)\n";
        }
    }
}

/**
 * @brief Merchant movement behaviour (demo: moves stall to Market).
 *
 * Note: move() takes no parameters (matches header). We implement a
 * simple, deterministic move to a named location to demonstrate polymorphism.
 */
void Merchant::move() {
    string old = location;
    location = "Market Square"; // example movement
    cout << name << " moves their stall from " << old << " to " << location << ".\n";
}

/**
 * @brief Merchant special action: sell first item (if any).
 * @throws runtime_error if inventory empty
 */
void Merchant::specialAction() {
    if (inventory.empty()) {
        throw runtime_error("Merchant::specialAction - no items to sell.");
    }

    cout << name << " sells " << inventory.front() << " for " << prices.front() << " gold.\n";

    // Remove sold item from inventory and price list
    inventory.erase(inventory.begin());
    prices.erase(prices.begin());

    // Update featuredItem smart pointer
    if (!inventory.empty()) {
        *featuredItem = inventory.front();
    } else {
        *featuredItem = "";
    }
}

/**
 * @brief Display Merchant details (overrides base display).
 */
void Merchant::display() const {
    cout << "[Merchant] " << name << " | Location: " << location << "\n";
    if (inventory.empty()) {
        cout << "  Inventory: (empty)\n";
    } else {
        cout << "  Inventory:\n";
        for (size_t i = 0; i < inventory.size(); ++i) {
            cout << "   - " << inventory[i] << " (" << prices[i] << "g)\n";
        }
    }
    // Show featured item if present
    if (featuredItem && !featuredItem->empty()) {
        cout << "  Featured: " << *featuredItem << "\n";
    }
}

/**
 * @brief Non-virtual Merchant-only function used for RTTI demonstration.
 *
 * Adds a sensible restock item to inventory and updates featuredItem.
 * This method is intentionally NOT virtual per assignment requirements.
 *
 * @throws invalid_argument if item is empty or price <= 0
 */
void Merchant::restock() {
    // For simplicity, add a fixed restock item. In a fuller program this
    // could read from a file or other data source.
    const string item = "Restock Item";
    const int price = 25;

    if (item.empty() || price <= 0) {
        throw invalid_argument("Merchant::restock - invalid item/price.");
    }

    inventory.push_back(item);
    prices.push_back(price);

    // update featured item smart pointer
    featuredItem = make_shared<string>(item);

    cout << name << " restocked with " << item << " (" << price << "g).\n";
}

/* ===========================
   Guard
   =========================== */

/**
 * @brief Construct a new Guard object.
 * @param n Guard name
 * @param loc Guard location
 * @param aggr Aggression level (expected 0-10; validated)
 * @param area Patrol area (optional)
 * @throws out_of_range if aggression is outside an expected range
 */
Guard::Guard(const string &n, const string &loc, int aggr, const string &area)
    : NPC(n, loc), aggressionLevel(aggr), patrolArea(area)
{
    if (aggressionLevel < 0 || aggressionLevel > 10) {
        throw out_of_range("Guard constructor: aggressionLevel must be 0..10.");
    }
}

/**
 * @brief Guard interaction: warns or attacks depending on aggression.
 */
void Guard::interact() {
    if (aggressionLevel > 5) {
        cout << name << " (Guard) growls: Intruder! I will attack near " << patrolArea << "!\n";
    } else {
        cout << name << " (Guard) says: Keep to the paths around " << patrolArea << ".\n";
    }
}

/**
 * @brief Guard movement: moves to patrol area (no args).
 * @throws runtime_error if patrolArea is empty
 */
void Guard::move() {
    if (patrolArea.empty()) {
        throw runtime_error("Guard::move - empty patrol area.");
    }
    location = patrolArea;
    cout << name << " moves to patrol " << patrolArea << ".\n";
}

/**
 * @brief Guard special action: inspect or attack depending on aggression.
 */
void Guard::specialAction() {
    if (aggressionLevel > 7) {
        cout << name << " sounds the alarm and attacks!\n";
    } else {
        cout << name << " inspects travelers carefully.\n";
    }
}

/**
 * @brief Display Guard details.
 */
void Guard::display() const {
    cout << "[Guard] " << name << " | Location: " << location
         << " | Patrol: " << patrolArea
         << " | Aggression: " << aggressionLevel << "\n";
}

/* ===========================
   Villager
   =========================== */

/**
 * @brief Construct a new Villager object.
 * @param n Villager name
 * @param loc Villager location
 * @param happy Happiness level (0-100; validated)
 * @param g Gossip string (optional)
 * @throws out_of_range if happiness is out of [0,100]
 */
Villager::Villager(const string &n, const string &loc, int happy, const string &g)
    : NPC(n, loc), happinessLevel(happy), gossip(g)
{
    if (happinessLevel < 0 || happinessLevel > 100) {
        throw out_of_range("Villager constructor: happinessLevel must be 0..100.");
    }
    // Initialize rumorPtr to share the gossip string
    rumorPtr = make_shared<string>(gossip);
}

/**
 * @brief Villager interaction: greets or shares gossip.
 */
void Villager::interact() {
    if (!gossip.empty() && gossip != "None") {
        cout << name << " (Villager) whispers: \"" << gossip << "\"\n";
    } else {
        cout << name << " (Villager) greets you warmly.\n";
    }
}

/**
 * @brief Villager movement: stroll to a village center (no args).
 */
void Villager::move() {
    string old = location;
    location = "Village Center"; // demonstration movement
    cout << name << " wanders from " << old << " to " << location << ".\n";
}

/**
 * @brief Villager special action: boost or change happiness and optionally echo the rumor.
 */
void Villager::specialAction() {
    // Increase happiness slightly (cap at 100)
    happinessLevel = min(100, happinessLevel + 5);
    cout << name << " organizes a small festival, happiness is now " << happinessLevel << ".\n";

    // If there's a rumor pointer, mention it (demonstrates smart pointer usage)
    if (rumorPtr && !rumorPtr->empty() && *rumorPtr != "None") {
        cout << name << " shares a rumor: " << *rumorPtr << "\n";
    }
}

/**
 * @brief Display Villager details.
 */
void Villager::display() const {
    cout << "[Villager] " << name << " | Location: " << location
         << " | Happiness: " << happinessLevel
         << " | Gossip: " << gossip << "\n";
}

