#include "npc.h"
#include <iostream>

// Jeremy Cuthbert CS302 Section 02
// npc.cpp

// Base NPC constructor
NPC::NPC(const std::string& name, const std::string& location)
    : name(name), location(location) {}

std::string NPC::getName() const { return name; }
