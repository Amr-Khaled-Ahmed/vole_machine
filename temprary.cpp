#include <iostream>
#include <bitset>

int addTwosComplement(int a, int b) {
    // Define 4-bit mask to simulate overflow in 4-bit addition
    const int BIT_MASK = 0xF;

    // Perform addition and mask to get 4-bit result
    int sum = (a + b) & BIT_MASK;

    // Check if the result is negative by examining the most significant bit (sign bit)
    if (sum & 0x8) {  // If sign bit is set, the number is negative
        sum = -(~sum + 1 & BIT_MASK);  // Convert back from two's complement to negative integer
    }

    return sum;
}

int main() {
    // Define 4-bit two's complement numbers
    int a = 5;       // 5 in binary is 0101
    int b = -3;      // -3 in binary is 1101 (4-bit two's complement)

    int result = addTwosComplement(a, b);

    // Display result
    std::cout << "Result: " << result << std::endl;

    return 0;
}
