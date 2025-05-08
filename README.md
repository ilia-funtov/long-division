# long-division
This project implements a variant of a division algorithm that can be used with CPUs that don't have native support for division operation.

It performs division in iterative way using only bit shifts (for multiplication and division by 2) and addition and subtraction operations.

Long Division Algorithm Description
The long_division function implements a division algorithm that can be used on CPUs without native division support. It performs division using only bit shifts, addition, and subtraction operations.

Algorithm Overview
The algorithm works by implementing a binary long division approach that:

1. Handles both unsigned and signed integer types
2. Returns both quotient and remainder as a pair
3. Avoids using hardware division instructions


How the Algorithm Works
For Unsigned Types:
1. Initialization:

   * Set quotient = 0
   * Set current_divisor = divisor
   * Set quotient_add_term = 1

2. Main Loop:

   * While dividend >= current_divisor:
     - Subtract current_divisor from dividend
     - Add quotient_add_term to quotient
     - Try to double current_divisor (shift left by 1)
     - If doubling doesn't cause overflow, also double quotient_add_term
   * If dividend < divisor, exit the loop
   * Otherwise, reset current_divisor = divisor and quotient_add_term = 1 and continue

3. Return:

   * Return the pair (quotient, dividend) where dividend is now the remainder

For Signed Types:
1. Sign Handling:

   * Determine if the result should be negative (when signs of operands differ)
   * Convert both operands to unsigned types
   * Check for potential truncation issues when converting to unsigned
2. Delegation:

   * Call the unsigned version of the algorithm
3. Result Adjustment:

   * For negative results with non-zero remainder, adjust according to mathematical rules
   * Return the properly signed quotient and remainder


Key Features
* Handles edge cases like division by zero
* Properly manages signed division according to mathematical rules
* Avoids overflow by checking before doubling values
* Uses only basic operations (shifts, addition, subtraction)
* Maintains the invariant that quotient * divisor + remainder = dividend


This algorithm is particularly useful for embedded systems or architectures where division operations are expensive or unavailable.
