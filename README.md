# CR: Compile-Time Reflection for C++17

CR is a library which offers various meta functions which enable compile time reflection in C++17.

## Supported Functionality

- Datastructure Generation
- Inheritance of Datastructures (1 Deep inheritance)
- Naming of Members

## Planned Functionality

- Function Generation
- Member Function Generation
- Exporting Of Datastructures
- Exporting Of Functions
- Member Accessibility Levels

## Why use CR?

CR is a true Compile-Time Reflection Library which focuses on minimal performance penalty.
The desire is to have meta-functions that have near identical performance as manually constructed types.
If possible CR constructed types compile to identical types when using O2/O3 optimization levels.

Comparing to other "Reflection" libraries, this library does not impose runtime performance hits.
Thus making it possible to utilize it in most fields.

## Example

```C++
#include "cr.h"
#include <iostream>

// static constexpr may be omitted, however, that comes with slight performance penalty
cr::str x{"x"};
cr::str y{"y"};
cr::str z{"z"};

// Constructs a struct, with members:
// [int: x, int: y, int: z]
using coordinate = cr::gen
    ::stuc<int, int, int>
    ::names<x, y, z>
    ::type;    

int main()
{
    coordinate a;
    a.get<x>() = 1;
    a.get<y>() = 2;
    a.get<z>() = 3;

    // Prints:
    // 1
    // 2
    // 3
    std::cout << a.get<x>() << "\n";
    std::cout << a.get<y>() << "\n";
    std::cout << a.get<z>() << "\n";
    return 0;
}
```

CR is able to name members without any performance penalties.

## Current Status

Very WIP, however, it seems to reliably work for creating datastructures.