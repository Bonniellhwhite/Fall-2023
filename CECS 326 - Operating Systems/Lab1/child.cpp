#include <iostream>

int main(int argc, char* argv[]) {

        // Confirm number of arguments is correct
        // If not the correct number of arguments, error
        if (argc != 3) {
                std::cerr << "Usage " << argv[0] << " <child_number> <child_name>";
                return 1;
        }

        // Print child number and name
        std::cout << "I am child number " << argv[1] << ", and my name is " << argv[2] << "\n";

        return 0;
}