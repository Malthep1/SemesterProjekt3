#pragma once

/*
Welcome PCCOM programme

Elisabeth Lennert
*/

#include <iostream>
#include <string>
#include <istream>

using namespace std;

int main(void)
{

	int password = 12345;
	string username = 'food';

	do
	{
		cout << "Enter username:" << endl;
		getline(cin, username);

		if (username == "foodgiver")
		{
			cout << " Enter Password:" << endl;
			getline(cin, password);

			if (password != password)
			{
				cout << "invalid password, please try again." << endl;
			}
			while (password == password)
			{
				cout << "Access granted" << endl;

				int nsize = 35;

				vector<char> somedata(nsize);

				ifstream "SSH";
				SSH.open("C:\Users\Elisa\OneDrive\Skrivebord\ssh.rpi.bat");
				SSH.read( nsize);
				SSH.close();

			}
		}
	}
	return 0;
}
