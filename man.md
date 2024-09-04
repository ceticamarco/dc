---
title: dc
section: 1
header: General Commands Manual
footer: Marco Cetica
date: April 22, 2024
---


# NAME
dc − RPN desktop calculator and stack-based programming language

# SYNOPSIS
```
RPN desktop calculator and stack-based programming language. Usage: 
-e, --expression <EXPRESSION> | Evaluate an expression
-f, --file <FILE>             | Evaluate a file
-h, --help                    | Show this helper
-V, --version                 | Show version
```

# DESCRIPTION
**dc** is an advanced, scientific and extensible RPN desktop calculator and a 
stack-based programming language (re)written in C++. 
By default, dc supports a wide range of arithmetical, trigonometrical and numeric functions and 
its capabilities can be further extended by writing user-defined programs using the embedded, turing-complete, macro system.

**dc** uses the reverse polish notation(**RPN**) to parse mathematical expressions. Unlike the infix notation, where operators
are placed _between_ operands, the polish notation(also called prefix notation) places operators _before_ the operands. The **reverse**
polish notation takes this concept even further by placing the operators _after_ the operands. As an example, consider the following
infix expression:

```
    (((5 + 4) * (3 - 2)) / 2)
```

In RPN, this would be:

```
    2 5 4 + 3 2 - * r / p
```

Operands are pushed onto the stack following the LIFO policy; operators, on the other hand, pop one or more values
from the stack and push back the result. By default, **dc** is very quiet, in order to inquiry the stack you need to use one of the supported
options(see below).

**dc** reads from the standard input, but it can also work with text files using the `-f` flag. Furthermore, you can decide to evaluate an expression
without opening the REPL by using the `-e` flag.

# PROGRAMMING IN DC
As a stack-based, concatenative and procedural programming language, **dc** programs follow a *bottom up* approach where the program is built by
starting with the most minimal facts about the problem and then is built up towards the complete solution. Following this programming paradigm means
creating many short and simple routines that are defined either in terms of existing routines or in terms of built-in primitives.

The main strength of this paradigm is that you have full control on what happens at the lower levels of your program. This means that your
programming logic is not abstracted away into generic protocols, everything you write has to be as concrete as possible. 
Another advantage of this approach is the ability to test and debug interactively each definition and each routine as you build up your solution, without having
to rely on external testing framework. The dc programming language is also very extensible: the core language consists on only a few primitives, which are enough for
building new programs and extending the programming language itself. Also, dc - just like LISP-like languages, is homoiconic: there is no distinction between code and data.

On the other hand, the *bottom up* approach is not suitable for building large infrastructures: the ability to abstract away methods, procedures and to shape
real-world objects into templates is essential for many modern programming use cases. In fact, dc excels when used for what it was originally 
developed for: computations and small math-oriented programs.

# ARCHITECTURE
As an advanced scientific calculator, **dc** has a complex architecture defined by the following two data structures:

1. The main stack
2. The register ADT

Which can be graphically represented as follows:
```
+------------------+         +----+----+----+-----
|    MAIN STACK    +-------->|    |    |    |    ... STACK
+------------------+         +----+----+----+-----
|   REGISTER ADT   |
+---------------+--+
                |                                +----+----+----+-----
                |               HASH MAP         |    |    |    |    ...  STACK
                |            +-------+---+------>+----+----+----+-----
                |            | REG A |   |
                +----------->+-------+---+------>+----+----+----+-----
                             |           |       |    |    |    |    ...  ARRAY
                             |           |       +----+----+----+-----
                             |           |         0    1    2    3
                             |    ....   |
                             |           |
                             |           |
                             |           |       +----+----+----+-----
                             |           |       |    |    |    |    ...  STACK
                             +-------+---+------>+----+----+----+-----
                             | REG G |   |
                             +-------+---+------>+----+----+----+-----
                                                 |    |    |    |    ...  ARRAY
                                                 +----+----+----+-----
                                                   0    1    2    3
```


The _main stack_ is the primary form of memory available in this program. Every time you enter a number or execute a command, you are operating
within the main stack. The _main stack_ is virtually infinite and grows as much as needed; the _main stack_ is **public**, i.e. it is
shared between any **dc** command.

The **register** is a hash map-like abstract data type that allows users to operate on an _isolated_ environment formed by a _stack_
and an _array_. Each instance of the register is an ordered pair `(key, value)` where the _key_ is a character representing the name of the 
register and the _value_ is a **private** instance of a stack and a **private** instance of an array. **dc** commands - exception made for registers, macro and array commands -
cannot operate directly on the auxiliary stacks or on the auxiliary arrays. In order to use a value stored on an auxiliary stack, you need to pop it
and push it onto the main stack(see the register section).

Both the _main stack_ and the _auxiliary stack_ implement the same abstract data type, therefore any kind of data type supported by the main
stack - as well as any other property or feature supported by the main stack - is also supported by the register's stack.

_Arrays_ are dynamic, homogeneous and private abstract data type associated with a register.
The underlying data type of dc array is a hashmap where the index is represented by
the map's `key` and the associated value is represented by the map's `value`.

# TYPE SYSTEM
By default, each value of any kind of stack is represented by a string. Each operation is in charge to type convert the value before and after
their invocation. The user can store both numeric and alphanumeric values on the stack. The latter using the _macro_ syntax(see below).

Arrays are homogeneous data structures that implement the same data type of the stack, i.e. the string.


# COMMANDS
Below, there is a list of supported **dc** commands.

## Printing Commands

**p**

Prints the value on the top of the stack, without altering the stack. A newline is printed after the value.

**P**

Pops off the value on top of the stack, without altering the stack.

**f**

Prints the entire contents of the stack without altering anything.

## Mathematics

**+**

Pops two values off the stack, adds them, and pushes the result.

**-**

Pops two values, subtracts the first one popped from the second one popped, and pushes the result.

**\***

Pops two values, multiplies them, and pushes the result.

**/**

Pops two values, divides the second one popped from the first one popped, and pushes the result.

**%**

Pops two values, computes the remainder of the division between the second one popped and the first one popped. Pushes back the result.

**~**

Pops two values, divides the second one popped from the first one popped. The quotient is pushed first, and the remainder is pushed next.

**^**

Pops two values and computes their exponentiates, using the first value popped as the exponent and the second popped as the base.

**|**

Pops three values and computes a modular exponentiation. The first value popped is used as the reduction modulus; this value must be 
a non-zero integer. The second popped is used as the exponent; this value must be a non-negative number. The third value popped is the base
which gets exponentiated, which should also be an integer. This function computes the following modular equivalence: `c ≡ b^e (mod n)`

**v**

Pops one value, computes its square root, and pushes that.

**!**

Pops one non-negative integer, computes its factorial, and pushes that.

**pi**

Pushes pi approximation.

**e**

Pushes e approximation.

**\@**

Pops two values from the stack and generates a random number, using the first value popped as the upper bound and the second popped as the lower bound.
The random value is generated using a 64-bit Mersenne Twister pseudorandom number generator and a real uniform distribution. 

**$**

Pops one value from the stack and convert it to the nearest integer of lesser magnitude.

**y**

Pops one value from the stack and compute the _common_(base-10) logarithm of that number.

## Trigonometrical

**sin**

Pops one value, computes its `sin`, and pushes that.

**cos**

Pops one value, computes its `cos`, and pushes that.

**tan**

Pops one value, computes its `tan`, and pushes that.

**asin**

Pops one value, computes its `asin`, and pushes that.

**acos**

Pops one value, computes its `acos`, and pushes that.

**atan**

Pops one value, computes its `atan`, and pushes that.

## Complex Numbers
**dc** allows you to operate on complex numbers in an easy and straightforward way:
most arithmetical functions that work with real numbers also work with complex numbers. Complex
numbers are stored on the stack just like any other real, rational, integer or natural number. To enter
a new complex number of the form

$$
    z = a + ib
$$

write the _real_ part($a$) into the _second-to-top_ of the stack, the _imaginary_ part($b$) into the _head_ of the stack and then issue the `b` command. For example, to enter $(9 + 4i)$, write:

```
9 4 b p
(9,4)
```

As you can see, complex numbers are represented as a tuple $(Re,Im)$ and can be printed just like
any other `dc` type.

You can then operate on them just like any other number. Consider the following example:

``` 
    (6 + 4i) - (5 - 2i)
    -------------------
         (7 + i)^2
```

This can easily be calculated using the following sequence of commands:

```
10 k
6 4 b 5 -2 b - 7 1 b 2 ^ / p
(0.0528000000,0.1096000000)
```

And if you want to extract only the real or imaginary part of the number, issue one of the 
following command, respectively:

```
re p .x im p
0.0528000000
0.1096000000
```

That is:

**re**

Pops one value, if it is a complex number retrieve its real part and pushes that.

**im**

Pops one value, if it is a complex number retrieve its imaginary part and pushes that.

## Statistics Operations
**dc** supports various common statistics operations, such as permutations, combinations, mean, standard deviation,
summation, sum of squares and linear regression. Most statistics functions are limited to **non-negative integers**.
The accumulating functions(such as the mean, the standard deviation and the linear regression) use the `X` and 
the `Y` registers.

**gP**

Pops two non-negative integers(that is, >=0) and computes `P_{y, x}`, that is the number of possible different arrangements of
`y` different items taken in quantities of `x` items at a time. No item shall occur more than once in an arrangement and different orders of same `x` 
items are counted separately. The `y` parameter correspond to the second one value popped while the `x` parameter correspond to the first one popped.

**gC**

Pops two non-negative integers(that is, >=0) and computes `C_{y, x}`, that is the number of possible sets of
`y` different items taken in quantities of `x` items at a time. No item shall occur more than once in a set and different orders of the same `x`
items are not counted separately. The `y` parameter correspond to the second one value popped while the `x` parameter correspond to the first one popped.

**gs**

Computes Σx of the `X` register's stack and pushes that.

**gS**

Computes Σx^2 of the `X` register's stack and pushes that.

**gM**

Computes x̄(mean) of the `X` register's stack and pushes that.

**gD**

Computes σ(standard deviation) of the `X` register's stack and pushes that.

**gL**

Computes linear regression of the `X` register's stack and the `Y` register's stack. Linear regression is a
simple statistical model to find a relationship between a *dependent variable*(`Y`) and an independent 
variable(`X`). This function will compute the following linear equation:
$$
    y = mx + b
$$

using the following formulae:

```      
        ( n * ∑(x_i * y_i) ) - ( ∑x_i * ∑y_i )
    m = ---------------------------------------
               n * ∑x^2 - (∑x)^2

    
                 ∑y_i - (m * ∑x_i)
    b = ---------------------------------------
                        n
``` 

Where **n** is the number of elements of each set.
The results - the _slope_ **m** and the _y-intercept_ **b** - will be pushed onto the stack in that order.

## Base Conversion

**pb**

Prints the value on the top of the stack in base 2, without altering the stack. A newline is printed after the value.

**po**

Prints the value on the top of the stack in base 8, without altering the stack. A newline is printed after the value.

**ph**

Prints the value on the top of the stack in base 16, without altering the stack. A newline is printed after the value.

## Bitwise Operations
**dc** supports various bitwise operations. These operations support integral types only and are represented using
a 64bit data type.

**{**

Pops two numbers from the stack and computes bitwise *AND* between the second one popped and the first one popped. Pushes the result.

**}**

Pops two numbers from the stack and computes bitwise *OR* between the second one popped and the first one popped. Pushes the result.

**l**

Pops one **signed integer** from the stack and computes bitwise *NOT*(one's complement). Pushes the result.

**L**

Pops two numbers from the stack and computes bitwise *XOR* between the second one popped and the first one popped. Pushes the result.

**m**

Pops two numbers from the stack *left shift* the second one popped by *n* bits, where *n* is the first one popped. Pushes the result.

**M**

Pops two numbers from the stack *right shift* the second one popped by *n* bits, where *n* is the first one popped. Pushes the result.

## Stack Control

**c**

Clears the stack, rendering it empty.

**d**

Duplicates the value on the top of the stack, pushing another copy of it. Thus, `4 d * p` computes 4 squared and prints it.

**r**

Reverses the order of the top two values of the stack. This can also be accomplished with the sequence `Sa Sb La Lb`.

**R**

Pops the top-of-stack without printing it

**.x**

Retrieve the value that was last in the top-of-stack *before execution of any operation*(stack, macro, arithmetical, etc.).
This function can be used to quickly key back numbers when you are recovering from errors, such as typing in the wrong number
or executing the wrong operation.

**.y**

Retrieve the value that was last in the second-to-top of the stack *before execution of any operation*(stack, macro, arithmetical, etc.).

**.z**

Retrieve the value that was last in the third-to-top of the stack *before execution of any operation*(stack, macro, arithmetical, etc.).

## Parameters
**dc** has three parameters that control its operation: the *precision*, the *input radix* and the *output radix*.
The precision specifies the number of fraction digits to keep in the result of most arithmetical operations. The
input radix controls the interpretation of numbers typed in(i.e., the input base). The output radix specifies the numeric
base used to print numbers.

The input and the output radixes are separate parameters which are, by default, equal to 10(i.e., the decimal numeral system); you
can make them unequal using the `i` and `o` options(see below). The input radix must be within the range 2-16. The output radix, instead, 
can either be equal to 2, 8, 10 or 16. The precision parameter must be zero or greater and cannot be a float value.

The input base affect the behavior of the whole program, even the parameters commands. For example, the sequence `8 i` will change
the input base to the octal numeric system and thus every new numeric input after this step will be evaluated in base 8(and not in base 10).
Therefore, to reset the input base from the octal system to the decimal system, you will need to issue the following sequence: `12 i`.

Non-decimal radixes enable a special set of characters(`ABCDEF`) that allows the user to insert into the stack non-decimal numbers.
For example, the sequence `16 i F p` will print the number `15` since it represent the number `0xF` in base 10. Do note that these
characters are case-sensitive(thus `F =/= f`) and cannot be used in the default input base system(thus `10 i F` will yield an error).

**i**

Pops the value off the top of the stack and uses it to set the input radix

**o**

Pops the value off the top of the stack and uses it to set the output radix

**k**

Pops the value off the top of the stack and uses it to set the precision

**I**

Pushes the current input radix on the stack

**O**

Pushes the current output radix on the stack

**K**

Pushes the current precision on the stack

## Register(Stack)
As mentioned before, **dc** supports a hashmap ADT called **register** represented by an ordered pair `(key, value)`. 
A register maps the `key`(represented by a single character) with a `value`(represented by an auxiliary stack and a private array).
At least 256 registers are available. Below, you can see the supported operations on register's stack.

**s**`r`

Pop the value off the top of the (main) stack and store it into top of the stack of register _r_.
This overwrites the top of the stack and does **NOT** follow the LIFO policy.

**l**`r`

Copy the value in top of the stack of register _r_ and push it onto the main stack. 
The value 0 is retrieved if the register is uninitialized. This does not alter the contents of _r_.

The current register value is the top of the register's stack.

**S**`r`

Pop the value off the top of the (main) stack and push it onto the stack of register _r_.
The previous of the register becomes inaccessible, thus it follows the LIFO policy.

**L**`r`

Pop the value off the top of register _r_'s stack and push it onto the main stack. The previous value in register _r_'s stack, if any,
is now accessible via the **l**r command.

**c**`r`

Clear the register `r` from memory.

**z**`r`

Pushes the current register's stack depth: the number of objects on the register's stack before the execution of the **z**`r` command.

## Register(Array)
Arrays support random access through an index. You can store a value in an array and retrieve it later.

**:**`r`

Will pop the top two values off the stack. The second-to-top value will be stored in
the array `r`, indexed by the top-of-stack value.

**;**`r`

Pops the top-of-stack and uses it as an index into array `r`. The selected value
is then pushed onto the stack.

## Strings/Macros

_dc_ has a limited ability to operate on strings as well as on numbers; strings can be printed or executed as a macro, that is as a dc program.
Strings can be nested, i.e. a string can contain another string. For example:

- **\[ 1 \] p**: Prints the string(note: `1` is a string and not a number);  
- **\[ \[ Hello World \] p \] x**: pushes a string containing another string and a command and execute it;  
- **\[ Executing: \] p \[ 2 p 10 p \[ r / p \] p x \] x**: Prints the ongoing operation and execute it using homoiconicity property.

When a string is used as a macro, dc *lazily evaluate* it; that is, the evaluation of subprograms is delayed until the evaluator requires their values.
Avoiding eagerly evaluation allows the programmer to take advantage of DC's homoiconicity and to make macro evaluation more lightweight.

Any kind of stack can hold strings, and _dc_ always knows whether any given object is a string or a number. Some commands such as arithmetic operations demand
numbers as arguments and print errors if given strings. Other commands can accept either a number or a string; for example, the **p** 
command can accept either and prints the object according to its type.

**[ characters ]**

Makes a string containing _characters_ (contained between balanced **\[** and **\]** characters), and pushes it on the stack. For example, **\[ Hello World \] P** prints the string **Hello World** (with no newline).

**x**

Pops a value off the stack and executes it as a macro. Normally it should be a string; if it is a number, it is simply pushed back onto the stack. For example, **\[ 1 p \] x** executes the macro **1 p** which pushes **1** on the stack and prints **1** on a separate line.

Macros are most often stored in register's stacks; **\[ 1 p \] sa** stores a macro to print **1** into register's stack **a**, and **la x** invokes this macro.

**\>**`r`

Pops two values off the stack and compares them assuming they are numbers, executing the contents of register _r_ as a macro if the original top-of-stack is greater. Thus, **1 2>a** will invoke register **a**’s contents and **2 1>a** will not.

**\=>**`r`

Similar but invokes the macro if the original top-of-stack is greater or equal to the second-to-top.

**<**`r`

Similar but invokes the macro if the original top-of-stack is less.

**<=**`r`

Similar but invokes the macro if the original top-of-stack is less or equal to the second-to-top.

**\=**`r`

Similar but invokes the macro if the two numbers popped are equal.

**!=**`r`

Similar but invokes the macro if the two numbers popped are not equal.

## Status Inquiry

**Z**

Pops a value off the stack, calculates the number of digits it has (or number of characters, if it is a string) and pushes that number.

**z**

Pushes the current stack depth: the number of objects on the stack before the execution of the **z** command.

## Miscellaneous

**q**

Exit with return code `0`.

**?**

Reads a line from the terminal and executes it. This command allows a macro to request input from the user.

**'**

Pops a string off the stack, use it as a filepath, read its content and execute it.

# EXAMPLES
Below, there are some practical problems solved using **dc**.

1. Evaluate `(-5 + sqrt(25 - 16)) / 2`:
```
-5 25 16 - v + 2 / p
```

2. Evaluate `sin(2pi)+cos(2pi)`:
```
2 pi * sin 2 pi * cos + p
```

3. Loop from 1 to `n`, where `n` is a user-defined value:
```
[ p 1 + d lN >L ] sL # Print numbers from 1 through 'N'

[ Enter limit: ] P # Ask user for limit 'N'
? 1 + sN # Read from stdin
c 1 lL x # Clear the stack, add lower bound, load and execute macro
```

4. Sum the first `n` natural numbers, where `n` is a user-defined value:
```
[ Enter bound: ] P ?
[ d 1 - d 1 <F + ] d sF x p
```

5. Prints the first 20 values of `n!`:
```
[ la 1 + d sa * p la 20 >y ] sy
0 sa 1
ly x
```

6. Computes the factorial of a given number:
```
[ ln 1 - sn ln la * sa ln 1 !=f ] sf
[ Enter value: ] P ? sn
ln sa
lf x
la p
```

7. Computes the Greatest Common Divisor(GCD) between two user-defined numbers `A` and `B`:
```
[ Enter A: ] P R ?
[ Enter B: ] P R ?
[ d Sa r La % d 0 <a ] d sa x +
[ GCD(A,B)= ] P R p
```

8. Computes the Least Common Multiple(LCM) between two user-defined numbers `A` and `B`:
```
[ Enter A: ] P R ? d sA
[ Enter B: ] P R ? d SA
[ d Sa r La % d 0 <a ] d sa x +
LA lA * r /
[ LCM(A,B)= ] P R p
```

9. Find the roots of a quadratic equation
```
3 k
[ Enter A: ] P ? sA
[ Enter B: ] P ? sB
[ Enter C: ] P ? sC
lB 2 ^ 4 lA lC * * - v sD
lB -1 * lD - lA # NEGATIVE DELTA
2 * / sS # FIRST SOLUTION
lB -1 * lD + lA # POSITIVE DELTA
2 * / SS # SECOND SOLUTION
[ X1: ] P R lS p
[ X2: ] P R LS lS p
```

10. Compute the sum `8AB6F + B783E` in base 16. Print the result in base 10 and in base 2:
```
16 i
8AB6F B783E +
[ Result in base 10: ] P R p
[ Result in base 2: ] P R pb
```

11. Load an external file:
```
$> echo "[ p 1 + d lN >=L ] sL" > loop.dc
$> cat prg.dc
[ loop.dc ] ' # Load loop macro
[ Enter limit: ] P # Ask user for limit 'N'
? 1 + sN # Read from stdin
c 1 lL x # Clear the stack, add lower bound, load and execute macro
```

12. Generate *n* (pseudo)random numbers from user-defined range:
```
5 k
[ lA lB @ p ] sR
[ Enter number of samples: ] P ? sN
[ Enter lower bound: ] P ? sA
[ Enter upper bound: ] P ? sB
[ lR x r 1 + d lN >=L ] sL
0 lL x
```

13. Estimate *pi* using Monte Carlo simulation
```
10 k
[ 0 1 @ sX 0 1 @ sY ] sR
[ lX 2 ^ sX lY 2 ^ sY ] sQ
[ 0 ;A 1 + 0 :A ] sI
[ 1 lX lY + <=I ] sC
[ lR x lQ x lC x 1 + d lN >=L ] sL
0 0 :A
0 6500 sN
lL x 0 ;A lN /
4 * p
```

14. Convert a hex color to RGB:
```
16 i
[ Enter hex value: ] P R ? sV
lV FF { 0 :A # Blue
lV 8 M FF { 1 :A # Green
lV 10 M FF { 2 :A # Red
[ , ] sc
[ [ RGB( ] P 2 ;A P lc p. 1 ;A P lc p. 0 ;A P [ ) ] p. [ = ] p. lV ph ] x
```

15. Find the mean of the following temperatures(Celsius): `[25, 15, 9.5, 10, 20, 16, 20]`:
```
4 k
25 15 9.5 10 20 16 20
SX SX SX  SX SX SX SX
gM p # Prints 16.5000
```

# AUTHORS
The original version of the **dc** command was written by Robert Morris and Lorinda Cherry. 
This version of **dc** is developed by Marco Cetica.

# BUGS

If you encounter any kind of problem, email me at [email@marcocetica.com](mailto:email@marcocetica.com) or open an issue at [https://github.com/ceticamarco/dc](https://github.com/ceticamarco/dc).
