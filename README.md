# Program 3 - NPC Hierarchy
Author: Jeremy Cuthbert  
Course: CS302 - Summer 2025  

## Game Name
Cuthbert Valley

## Description
This program implements a hierarchy of NPCs (Non-Playable Characters) using inheritance, polymorphism, and abstract base classes. The base class `NPC` defines common data and behaviors, while the derived classes `Merchant`, `Guard`, and `Villager` each add unique functionality.  

The design demonstrates:
- Abstract base class usage (`NPC` with pure virtual functions).
- Polymorphism through overriding `interact`, `move`, and `specialAction`.
- Proper use of `std::string` (no raw char pointers).
- Smart pointers (`std::shared_ptr`) for memory management.
- STL containers (`std::vector`) for managing collections.
- Exception handling for invalid or unexpected states.

## Files
- `npc.h` – Abstract base class definition  
- `derivednpc.h` – Derived classes: Merchant, Guard, Villager  
- `derivednpc.cpp` – Implementation of derived classes  
- `main.cpp` – Driver program to test hierarchy functionality  
- `Makefile` – Builds the project  
- `README` – Instructions and description  

## Compilation
To compile the program:
```bash
make
