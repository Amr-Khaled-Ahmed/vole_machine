// Header
string decimal_to_binary(int num);
string binary_ones_comp(string num);
string binary_two_comp(string num);
string binary_addition(string num1, string num2);
// Implementation
string binary_ones_comp(string num) {
    string complement;
    for (char digit : num) {
        if(digit=='0'){complement+='1';}
        else if(digit=='1'){complement+='0';}
    }
    return complement;
}
string binary_twos_comp(string num) {
    string ones_complement_result = binary_ones_comp(num);
    if (all_of(num.begin(), num.end(), [](char digit) { return digit == '0'; })) {
        return num;
    }
    string twos_complement_result;
    int carry = 1;
    // Add 1 to the one's complement to get the two's complement
    for (auto it = ones_complement_result.rbegin(); it != ones_complement_result.rend(); ++it) {
        int sum_digit = (*it - '0' + carry) % 2;       // Calculate the new bit
        carry = (*it - '0' + carry) / 2;               // Update carry for the next position
        twos_complement_result = to_string(sum_digit) + twos_complement_result;
    }

    // If there's a remaining carry, append '1' at the start
    if (carry == 1) {
        twos_complement_result = '1' + twos_complement_result;
    }
    return twos_complement_result;
}
string binary_addition(string num1, string num2) {
    int len_num1 = num1.length();
    int len_num2 = num2.length();
    // Make both numbers the same length by padding with leading zeros
    if (len_num1 > len_num2) {
        num2.insert(0, len_num1 - len_num2, '0');  // Pad num2 with leading zeros
    }
    else {
        num1.insert(0, len_num2 - len_num1, '0');  // Pad num1 with leading zeros
    }
    int carry = 0;
    string result;
    // Start from the rightmost bit and move to the left
    for (int index = num1.length() - 1; index >= 0; --index) {
        int bit1 = num1[index] - '0';  // Convert char to int
        int bit2 = num2[index] - '0';  // Convert char to int
        int sum_bits = (bit1 + bit2 + carry) % 2;  // Calculate the sum bit
        carry = (bit1 + bit2 + carry) / 2;         // Update the carry
        result += std::to_string(sum_bits);        // Append sum bit to result
    }
    // If there's a carry left over, append it
    if (carry > 0) {
        result += std::to_string(carry);
    }
    // Reverse the result to get the correct order
    reverse(result.begin(), result.end());
    return result;
}
