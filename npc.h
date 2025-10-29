#ifndef NPC_H
#define NPC_H

#include <string>
#include <memory>

// Jeremy Cuthbert CS302 Section 02
// NPC.h
/* Abstract Base Class for all NPCs in the game.
 * All NPCs share a name and location, and must support interaction,
 * movement, and a special action. 
 * This class enforces a consistent interface via virtual functions.
 */

class NPC {
public:
    NPC(const std::string &n, const std::string &loc);
    virtual ~NPC() = default; // Always virtual in base class

    // Common dynamic-binding functions
    virtual void interact() = 0;       // polymorphic interaction
    virtual void move() = 0;           // polymorphic movement
    virtual void specialAction() = 0;  // pure virtual, no default behavior
    virtual void display() const = 0;  // polymorphic display

    std::string getName() const;

protected:
    std::string name;
    std::string location;
};

#endif // NPC_H

