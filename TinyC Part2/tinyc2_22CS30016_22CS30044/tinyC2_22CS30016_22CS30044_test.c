static int global_static_variable; // Declaring a static global variable
volatile int global_volatile_variable; // Declaring a volatile global variable
inline int subtract_function(); // Declaration of an inline function

void main()
{
    /* 
        Multi-line comment at the start
        Second line of this
    */
    // Single line comment
    
    long long int very_large_number = 1E18; // Initializing a long long int with a large value
    float decimal_number = 34.5678; // Declaring and initializing a float variable
    extern int yyparse(); // External declaration of yyparse function
    char single_character = 'x'; // Declaring and initializing a char variable
    register int register_variable; // Declaring a register int variable
    short int small_integer = 5; // Declaring and initializing a short int
    unsigned int positive_number = 7; // Declaring and initializing an unsigned int
    int loop_count = 6, index = 0; // Declaring and initializing two int variables
    int integer_array1[150]; // Declaring an integer array
    int integer_array2[150]; // Declaring another integer array
    char string_array[150] = "Sample string with escape chars\n\t"; // Initializing a char array with a string
    _Bool boolean_flag = 0; // Declaring and initializing a boolean variable
    double _Complex complex_number = 2; // Declaring and initializing a complex number

    while (index < loop_count || index < 60 && index > 40) { // Start of a while loop with compound condition
        index++; // Incrementing the index
    }

    for (auto k = 0; k < loop_count; k++) // Start of a for loop with auto-typed iterator
    {
        k += 3; // Incrementing k by 3
        if (k > 5) break; // Conditional break statement
        continue; // Continue statement
    }
    
    index--; // Decrementing index

    int size_of_long = sizeof(long); // Getting the size of long data type
    
    if (positive_number < small_integer) { // Start of if condition
        small_integer = positive_number; // Assigning value of positive_number to small_integer
    }
    else if (small_integer > positive_number) { // Start of else-if condition
        loop_count = small_integer | positive_number; // Bitwise OR operation
        loop_count = loop_count / 2; // Division operation
        loop_count = loop_count | (~2); // Bitwise OR with complement of 2
        if (loop_count >= positive_number); // Empty if statement
        if (positive_number <= loop_count); // Another empty if statement
        loop_count ^= positive_number; // Bitwise XOR operation
        loop_count += positive_number; // Addition assignment
        loop_count -= positive_number; // Subtraction assignment
        loop_count &= positive_number; // Bitwise AND assignment
        loop_count *= positive_number; // Multiplication assignment
        loop_count <<= positive_number; // Left shift assignment
        loop_count >>= positive_number; // Right shift assignment
        loop_count %= 3; // Modulus assignment
        loop_count /= 23; // Division assignment
        loop_count = (loop_count < positive_number) ? (positive_number) : (loop_count); // Ternary operation
    } 
    else { 
        loop_count = 15; // Assigning a value to loop_count
    }
}