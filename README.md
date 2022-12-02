# CR: Compile-Time Reflection for C++17

CR is a library which offers various meta functions which enable compile time reflection in C++17.

## Supported Functionality

- Datastructure Generation
- Multiple Inheritance of Datastructures
- Naming of Members
- Member Function Generation (While loops function, however, they automatically return upon a false condition)

## Planned Functionality

- Variable Aliases
- Function Generation
- Exporting Of Datastructures
- Exporting Of Functions
- Member Accessibility Levels
- Improved Error Messages
- Code Generation utilities

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

## Tested

It works with the latests version of Clang, and GCC.

## Member Function Generation
```C++
#include "cr.h"

using integer_operations = ::cr:gen
    ::stuc<int, int, int>
    ::names<>
    ::inherit<>
    ::memfun<
        // Add two integers
        ::cr::gen::logic<int,
            ::cr::gen::return_variable<
                ::cr::gen::add<::cr::gen::uvar<0>,
                    ::cr::gen::uvar<1>
                >
            >
        >,

        // Return bigger integer
        ::cr::gen::logic<int,
            ::cr::gen::iff<
                ::cr::gteq<::cr::gen::uvar<0>, ::cr::gen::uvar<1>>,
                ::cr::gen::return_variable<
                    ::cr::gen::uvar<0>
                >
            >,
            ::cr::gen::els<
                ::cr::gen::return_variable<
                    ::cr::gen::uvar<1>
                >
            >
        >
    >
```

The above text will generate a struct called "integer_operations", with 2 member functions:
- Add 2 integers
- Return Bigger Integer

The construction will compile to equivalent code compared to manual constructed code. Given you use optimization of O1/O2/O3. Generally O2 is enough.

In order to construct functions, you need to use the dedicated instructions CR provides. There are a lot of instructions, and higher level instructions.

## Instructions

| Instruction | Arg 1 | Arg 2 | Variadic |
|---|---|---|---|
| scope ||| Yes
| echo | expression
| return_variable | expression
| create_variable | type | expression 
| create_int | integer
| create_bool | boolean
| var | var_id (int)
| tvar | var_id (int)
| uvar | var_id (int)
| iff | conditional | action
| elif | conditional | action
| els | action
| whil | conditional | action
| ass | lhs | rhs
| asc | lhs | rhs
| eq | lhs | rhs
| gt | lhs | rhs
| gteq | lhs | rhs
| lt | lhs | rhs
| lteq | lhs | rhs
| add | lhs | rhs
| min | lhs | rhs
| mul | lhs | rhs
| div | lhs | rhs
| mod | lhs | rhs

All these instructions are further defined at their specific doc.