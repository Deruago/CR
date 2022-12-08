# CR: Compile-Time Reflection for C++17

CR is a library which offers various meta functions which enable True Compile Time Reflection in C++17.

## Supported Functionality

- Datastructure Generation
- Multiple Inheritance of Datastructures
- Naming of Members
- Member Function Generation

## Planned Functionality

- Variable Aliases
- Function Generation
- Exporting Of Datastructures
- Exporting Of Functions
- Member Accessibility Levels
- Improved Error Messages
- Code Generation utilities
- Enumeration Generation
- Function Overloading
- Virtual Functions
- Interfaces
- Template Functions
- Multiple Inheritance also supports Inheriting Member Functions

## Why use CR?

CR is a True Compile-Time Reflection Library which focuses on minimal performance penalty.
The desire is to have meta-functions that have near identical performance as manually constructed types.
If possible CR constructed types compile to identical types when using O2/O3 optimization levels.

Comparing to other "Reflection" libraries, this library does not impose runtime performance hits.
Thus making it possible to utilize it in most fields.

## Why use CR instead of alternative?

Currently any "Reflection" library for C++17 is not an Reflection library, but instead is an introspection library. Introspection allows you to investigate the structs you create. However, introspection is not as Powerful as Reflection. True Reflection allows you to Generate Types, Generate Function, and much more.

CR is a True Compile Time Reflection. CR is currently the only library capable of actually generating types, functions, and more. Introspection is a natural consequence of its power.

## Datastructure Generation

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

CR is capable of generating types and member functions. There are a lot features still planned, as CR allows a more powerful type-system than C++. Due to its design.

## Tested

It works with the latests version of Clang, and GCC.

CR, is mainly developed with Clang. So Clang is generally always supported with latest releases. However, CR tries to also support GCC whenever possible.
If some feature is not yet supported for other compilers this will be displayed in the section "Support".

## Type Inheritance
```C++
#include "cr.h"

cr::str x{"x"};
cr::str y{"y"};

cr::str coord1{"coord1"};
cr::str coord2{"coord2"};
cr::str coord3{"coord3"};
cr::str coord4{"coord4"};

using Shape = ::cr::gen
    ::stuc<>
    ::type;

using Point = ::cr::gen
    ::stuc<int, int>
    ::names<x, y>
    ::type;

using Triangle = ::cr::gen
    ::stuc<Point, Point, Point>
    ::names<coord1, coord2, coord3>
    ::inherit<Shape>
    ::type;

using Square = ::cr::gen
    ::stuc<Point, Point, Point, Point>
    ::names<coord1, coord2, coord3, coord4>
    ::inherit<Shape>
    ::type;

int main()
{
    Triangle triangle;
    Square square;

    Shape* shape = &square;
}

```

## Enumeration Generation
```C++
// Create some unique types, used for type enumeration
using red = struct {};
using green = struct {};
using blue = struct {};

using color = ::cr::gen
    ::enu<
        red,
        green,
        blue
    >::type;

int main()
{
    color some_color = color::cons<green>;
    switch(some_color)
    {
    case color::cons<red>: {
        std::cout << "Red!\n";
        break;
    }
    case color::cons<green>: {
        std::cout << "Green!\n";
        break;
    }
    case color::cons<blue>: {
        std::cout << "Blue!\n";
        break;
    }
    }
    
    return 0;
}
```

Enumeration is simulated, however, the simulated enumeration behave exactly like C++ enumerations.

The CR Enumerations enumerate over types. Thus you need a set of unique types.

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

## Support

- Clang: Fully
- GCC: Fully
- MSVC: Not Tested