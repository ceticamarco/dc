# Technical Documentation
Welcome to the documentation of the [DC RPN calculator and programming language](https://git.marcocetica.com/marco/dc). 
This document serves as a comprehensive guide to the source of the DC RPN calculator and its associated programming language. For
instructions on how to use this program, please consult the [online manual](https://git.marcocetica.com/marco/dc/src/branch/master/man.md)
or the UNIX man page that comes with the source code and the binary of the program. This guide is intended specifically for programmers and maintainers who wish to enhance the codebase and introduce new features to the program.

### Expanding DC
To add new functionalities to the codebase, you can either:

- Add a new method to an existing class.
- Implement a new instance of the IOperation protocol.

The first approach is recommended when you want to expand the capabilities of an existing class. For example, if you want to implement a new arithmetic function, 
it is better to add it as a method of the Mathematics class rather than creating a new class from scratch.

The second approach is recommended when you need to introduce a new set of features that do not align with any existing class. For instance, if you are about to add
multithreading support to the program, none of the existing classes may be suitable for this kind of feature. Therefore, the correct approach is to implement a new instance
of the IOperation interface by defining a new `Multithreading` class and, from there, adding all the required features.

Each new command must be tested using the testing framework.

### Adding features to existing class
To add new features to an existing class, you can follow this procedure:
1. Add a new _operation type_ to the **OPType** enumeration(`src/operation.h`);  
2. Add a new **private** method to an existing class with the return type of `std::optional<std::string>`;  
3. Modify the `exec` method of the class by adding a case for the new _operation type_ on the switch statement;
4. Modify the Evaluate::init_environment method by adding a new entry to the environment map.

Below, there is a step-by-step example.  
Suppose that you would like to add a new function - `double_factorial` - to the Mathematics class.
Let's also suppose that the DC command to invoke this new function is the character **X**. To do this, start by adding a new _operation type_ to the **OPType** enumeration:

```cpp
enum class OPType {
    // Numerical operations
    ADD, SUB, MUL, DIV, MOD, DIV_MOD, MOD_EXP, EXP,
    SQRT, SIN, COS, TAN, ASIN, ACOS, ATAN, FACT, PI, 
    E, RND, INT, TO_CMPLX, GET_RE, GET_IM, LOG, D_FACT // <-- New operation
};
```

Next, implement the double factorial function by defining a new private method inside
the Mathematics class(`src/mathematics.h`) and then implementing it on `src/mathematics.cpp`.

```cpp
class Mathematics : public IOperation {
public:
    explicit Mathematics(const OPType op_t) : op_type(op_t) {}
    std::optional<std::string> exec(dc::Stack<std::string> &stack, dc::Parameters &parameters, std::unordered_map<char, dc::Register> &regs) override;

private:
    // other methods
    std::optional<std::string> double_factorial(dc::Stack<std::string> &stack, const dc::Parameters &parameters);
};
```

The, modify the Mathematics::exec method(`src/mathematics.cpp`) by adding a new case to the switch:

```cpp
std::optional<std::string> Mathematics::exec(dc::Stack<std::string> &stack, dc::Parameters &parameters, __attribute__((unused))  std::unordered_map<char, dc::Register> &regs) {
    std::optional<std::string> err = std::nullopt;

    switch(this->op_type) {
        // Other cases
        case OPType::D_FACT: err = double_factorial(stack, parameters); break;
        default: break;
    }

    return err;
}
```

Finally, register this new function on the environment map by editing the Evaluate::init_environment
method(`src/eval.cpp`):

```cpp
void Evaluate::init_environment() {
    // Numerical operations
    this->op_factory.emplace("X", MAKE_UNIQUE_PTR(Mathematics, OPType::D_FACT));
}
```

### Adding features to a new class
If you feel that existing classes are not suitable for your new feature, follow these steps:

1. Create a new set of files - `foo.h` and `foo.cpp` - inside `src/` folder;  
2. Notify CMake about new source files by updating the `src/CMakeLists.txt` file;  
3. Inside `src/foo.h` define a new class `Foo` that implements the IOperation protocol;  
4. Add a new _operation type_ to the **OPType** enumeration;  
5. Implement the methods of your new class as needed;  
6. Include your new class header file inside `src/eval.cpp` and then update the program's environment by modifying the Evaluate::init_environment method.  

Below, there is a step-by-step example.

Suppose that you want to add a new class - `Multithreading` - to support parallel computing.
Let us also suppose that this class defines three new commands: **X**, **Y** and **Z**.

After creating the two new files - `src/multithreading.h` and `src/multithreading.cpp` - update `src/CMakeLists.txt` to inform Cmake about the new sources:

```cmake
project(src)

set(HEADER_FILES
    # ...
    multithreading.h
)

set(SOURCE_FILES
    # ...
    multithreading.cpp
)

add_library(src STATIC ${SOURCE_FILES} ${HEADER_FILES})
```

Next, create a new class that implements the IOperation protocol:

```cpp
// src/multithreading.h
class Multithreading : public IOperation {
public:
    explicit Multithreading(const OPType op_t) : op_type(op_t) {}
    std::optional<std::string> exec(dc::Stack<std::string> &stack, dc::Parameters &parameters, std::unordered_map<char, dc::Register> &regs) override;


private:
    OPType op_type;
};
```

Then add a new _operation type_ to the **OPType** enumeration:

```cpp
enum class OPType {
    // Numerical operations
    ADD, SUB, MUL, DIV, MOD, DIV_MOD, MOD_EXP, EXP,
    SQRT, SIN, COS, TAN, ASIN, ACOS, ATAN, FACT, PI, 
    E, RND, INT, TO_CMPLX, GET_RE, GET_IM, LOG, 
    M_X, M_Y, M_Z // <-- New operations
};
```

At this point, you can implement the business logic of your class by editing the
`src/multithreading.cpp` file. 

The suggested approach is to implement each feature
in a separate private method and then let the `exec` function call them by discriminating based on
the `op_type` attribute.

Finally, update the `src/eval.cpp` file by including the header of your class and by
updating the Evaluate::init_environment method to register the new operations:

```cpp
// src/eval.cpp
#include "multithreading.h"

// ...
void Evaluate::init_environment() {
    // ...
    // Multithreading operations
    this->op_factory.emplace("X", MAKE_UNIQUE_PTR(Multithreading, OPType::M_X));
    this->op_factory.emplace("Y", MAKE_UNIQUE_PTR(Multithreading, OPType::M_Y));
    this->op_factory.emplace("Z", MAKE_UNIQUE_PTR(Multithreading, OPType::M_Z));
}
```

### Testing
Testing is done using the `utest` shell framework, which allows you to write tests using
shell scripts. You can find some examples in the `tests/` directory. 

To write a new test, create a new text file called `test_<NAME>` inside `tests/`. Then
add the following template:

```bash
#!/bin/sh

utest() {
    PROGRAM="$PWD/build/dc"
    EXPECTED="10" # The expected result
    ACTUAL=$("$PROGRAM" -e '5 5 + p') # The actual result

    assert_eq "$EXPECTED" "$ACTUAL"
}
# vim: ts=4 sw=4 softtabstop=4 expandtab:
```

You can add as many assert statements as you want(check out the `utest.sh` file for a complete list of 
supported assert functions) and as many shell functions as you need. Be sure to define the `utest`
function, which will serves as the entry point for the test.

You can also define _"failing"_ tests, that is a test that generates an error, by using the following
template:

```bash
#!/bin/sh

utest() {
    PROGRAM="$PWD/build/dc"

    # Test division by zero
    EXPECTED="Cannot divide by zero"
    ACTUAL=$("$PROGRAM" -e '5 0 /' 2>&1) || true
    assert_eq "$EXPECTED" "$ACTUAL"
}
# vim: ts=4 sw=4 softtabstop=4 expandtab:
```

In this test, we ensure that dividing a number by zero results in an error message. To intercept
the error we redirect the error stream(file descriptor 2) to the output stream(file descriptor 1).