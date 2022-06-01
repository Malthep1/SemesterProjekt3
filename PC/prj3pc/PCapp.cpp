
#include <iostream>
#include <fstream>
#include <string>

int main()
{
    
    std::string pw = "", check;
    std::ifstream text("password.txt");
    getline(text, check);

    while (pw!=check)
    {
        std::cout << "Enter password: " << std::endl;
        std::cin >> pw;
        if (pw!=check) std::cout << "Incorrect password" << std::endl;
    }

    std::cout << "Welcome [User]" << std::endl;
    text.close();

    return 0;
    
}