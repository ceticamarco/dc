# dc ![](https://github.com/ice-bit/dc/actions/workflows/dc.yml/badge.svg)

**dc** is an advanced, scientific and programmable RPN desktop calculator with macro support (re)written in C++. 
By default, dc supports a wide range of arithmetical, trigonometrical and numerical functions. 
Its capabilities can be further extended by writing user-defined programs using the embedded, turing-complete, macro system.

**dc** reads from the standard input, but it can also work with text files using the `-f` flag. Futhermore, you can decide to evaluate an expression
without opening the REPL by using the `-e` flag.

Operands are pushed onto the stack following the LIFO policy; operators, on the other hand, pop one or more values
from the stack and push back the result. By default **dc** is very quiet, in order to inquiry the stack you need to use one of the supported
options(see below).

`dc` can be invoked with the following command line options:
```
RPN desktop calculator with macro support. Usage: 
-e, --expression <EXPRESSION> | Evaluate an expression
-f, --file <FILE>             | Evaluate a file
-h, --help                    | Show this helper
-V, --version                 | Show version
```

Some of the supported features are:
- Basic arithmetical operations(`+`, `-`, `*`, `/`, `^`, `%`);  
- Scientific notation support(`5e3` -> `5000`);  
- Trigonometrical functions(`sin`, `cos`, `tan`);  
- Factorial and constants(`!`, `pi`, `e`);  
- Stack operations:
    - Print top element(`p`, `P`);  
    - Clear the stack(`c`); 
    - Remove top element(`R`);  
    - Swap order of top two elements(`r`);  
    - Duplicate top element(`d`);  
    - Dump the whole stack(`f`);  
- Registers:
    - Store top element of the stack on register `X`(`sX` or `SX`);  
    - Load content of register `X` on top of the stack(`lX` or `LX`);  
- Arrays:
    - Store second-to-top of main stack into array `X` indexed by top-of-stack(`:X`);  
    - Pop top-of-stack and use it as an index for array `X`(`;X`);  
- Macros:
    - Define a new macro inside square brackets(`[ ]`);  
    - Executing a macro from the stack(`x`);  
    - Evaluate a macro by comparing top-of-head and second-of-head elements(`>X`, `<X`, `>=X`, `<=X`, `!=` where `X` is a register).

And much more. You can find the complete manual [here](https://github.com/ice-bit/dc/blob/master/man.md).

## Installation
`dc` is written in C++20 without using any additional dependency. In order to build it, issue the following commands: 
```sh
$> make clean all
```

A new binary called `dc` will be created in your local folder.

To generate a man page from the `man.md` document, use the following command(note: needs pandoc):
```sh
$> pandoc man.md -s -t man > dc.1
```

## Usage
dc can be used in three different ways:
1. From the interactive REPL(run it without any argument);  
2. By evaluating an inline expression, i.e.
    ```sh
    $> dc -e "5 5 + p"
    ```
3. By evaluating a text file, i.e.
    ```sh
    $> cat foo
    2 4 - # Evaluate 2 - 4
    2 ^   # Evaluate x^2
    p     # Print the result(4)
    $> dc -f foo
    4
    ```

Below there are more examples.

1. Evaluate
$$\frac{-5 + \sqrt(25 - 16)}{2}$$
```
-5 25 16 - v + 2 / p
```
where `v` is the square root function

2. Evaluate
$$\frac{.5 + .9}{3^4}$$
```
.5 .9 + 3 4 ^ / p
```

3. Evaluate `10 + 5` inline(i.e. without opening the REPL):
```sh
$> dc -e "10 5 +"
```

4. Evaluate an expression from a file:
```sh
$> cat foo
    5 5 + 
    2 d * v 
    f
$> dc -f ./foo
```

5. Evaluate
$$\sin(2\pi) + \cos(2\pi)$$
```
2 pi * sin 2 pi * cos + p
```

6. Swap top two elements using registers(you can also use the `r` command):
```sh
5 4 p # Load some values on the stack(output: 4)
sA sB # Pop values and store them into the registers 'A' and 'B'
lA lB # Push 'A' and 'B' content onto the stack
p     # Print top element(output: 5)
```

7. Print out numbers from 1 through user-defined upper bound:
```sh
[ p 1 + d lN >L ] sL # Print numbers from 1 through 'N'

[ Enter limit: ] P # Ask user for limit 'N'
? 1 + sN # Read from stdin
c 1 lL x # Clear the stack, add lower bound, load and execute macro
```

8. Sum the first 36 natural numbers(😈), i.e.,
$$\sum_{i=1}^{37} i = 666$$
```sh
$> dc -e "36 [ d 1 - d 1 <F + ] d sF x p"
```

5. Prints the first 20 values of `n!`:
```
[ la 1 + d sa * p la 20 >y ] sy
0 sa 1
ly x
```

9. Computes the factorial of a given number:
```
[ ln 1 - sn ln la * sa ln 1 !=f ] sf
[ Enter value: ] P ? sn
ln sa
lf x
la p
```

10. Computes the Greatest Common Divisor(GCD) between two user-defined numbers `A` and `B`:
```
[ Enter A: ] P R ?
[ Enter B: ] P R ?
[ d Sa r La % d 0 <a ] d sa x +
[ GCD(A,B)= ] P R p
```

11. Computes the Least Common Multiple(LCM) between two user-defined numbers `A` and `B`:
```
[ Enter A: ] P R ? d sA
[ Enter B: ] P R ? d SA
[ d Sa r La % d 0 <a ] d sa x +
LA lA * r /
[ LCM(A,B)= ] P R p
```

12. Find the roots of a quadratic equation of the form:
$$ax^2 + bx + c = 0$$

with $$a,b,c \in \mathbb{R}, a \neq 0$$

using the formula
$$x_{1,2} = \frac{-b \pm \sqrt{b^2 - 4ac}}{2a}$$

```sh
#!/usr/local/bin/dc -f
# GIVEN A QUADRATIC EQUATION OF THE FORM
# AX^2 + BX + C = 0
# COMPUTE ITS REAL ROOTS
# THIS PROGRAM DOES NOT WORK WITH CMPLX NUMBERS
# DEVELOPED BY MARCO CETICA 2023
#
[ Enter A: ] P ? sA
[ Enter B: ] P ? sB
[ Enter C: ] P ? sC
lB 2 ^ 4 lA lC * * - v sD
lB -1 * lD - lA # NEGATIVE DELTA
2 * / sS # FIRST SOLUTION
lB -1 * lD + lA # POSITIVE DELTA
2 * / SS # SECOND SOLUTION
[ X: ] P R lS p
[ Y: ] P R LS lS p
```

## License

[GPLv3](https://choosealicense.com/licenses/gpl-3.0/)
