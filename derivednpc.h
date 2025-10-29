#ifndef DERIVED_NPCS_H
#define DERIVED_NPCS_H

#include "npc.h"
#include <vector>
#include <memory>
#include <string>

// Jeremy Cuthbert CS302 Section 02
// derivednpc.h
/* Defines specific types of NPCs that inherit from the NPC base class.
 * Each derived class has at least two unique attributes and provides
 * different implementations of the dynamic-binding functions.
 */

/* ----------------------- Merchant NPC -----------------------
 * Merchants sell goods to the player.
 * Unique attributes: inventory of items and corresponding prices.
 * Provides a non-virtual "restock" method to demonstrate RTTI.
 */
class Merchant : public NPC {
public:
    Merchant(const std::string &n, const std::string &loc,
             const std::vector<std::string> &inv = {},
             const std::vector<int> &pr = {});

    // Overrides
    void interact() override;
    void move() override;
    void specialAction() override;
    void display() const override;

    // RTTI method - not virtual, unique to Merchant
    void restock();

private:
    std::vector<std::string> inventory;
    std::vector<int> prices;

    // Use smart pointer example: maybe later manage Item objects
    std::shared_ptr<std::string> featuredItem; 
};

/* ----------------------- Guard NPC -----------------------
 * Guards protect areas and may warn or attack players.
 * Unique attributes: aggression level and patrol area.
 */
class Guard : public NPC {
public:
    Guard(const std::string &n, const std::string &loc,
          int aggr = 1, const std::string &area = "Gate");

    // Overrides
    void interact() override;
    void move() override;
    void specialAction() override;
    void display() const override;

private:
    int aggressionLevel;
    std::string patrolArea;
};

/* ----------------------- Villager NPC -----------------------
 * Villagers live in a settlement, gossip, and have moods.
 * Unique attributes: happiness level and gossip message.
 */
class Villager : public NPC {
public:
    Villager(const std::string &n, const std::string &loc,
             int happy = 50, const std::string &g = "None");

    // Overrides
    void interact() override;
    void move() override;
    void specialAction() override;
    void display() const override;

private:
    int happinessLevel;
    std::string gossip;

    // Use smart pointer example: share gossip reference
    std::shared_ptr<std::string> rumorPtr;
};

#endif // DERIVED_NPCS_H

