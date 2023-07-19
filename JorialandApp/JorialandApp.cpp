#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <locale>

using namespace std::chrono_literals;

const inline static std::string sInvalidInputMsg = "Opción no válida. Inténtelo de nuevo.";
const inline static std::string sNotImplemented = "Opción no implementada.";

enum class MenuOption
{
	ADD_FOOD = 1,
	VIEW_FOOD,
	EXIT
};

void displayMenu()
{
	system("cls");
	std::cout << "Elija una opción:\n";
	std::cout << "1) Añadir comida.\n";
	std::cout << "2) Ver comidas.\n";
	std::cout << "3) Salir del programa.\n";
}

int main()
{
	std::locale::global(std::locale("")); // Set the global locale for the console
	std::cout.imbue(std::locale());       // Set the locale for cout

	std::string input;
	MenuOption option;

	while (true)
	{
		displayMenu();

		std::getline(std::cin, input);
		try
		{
			option = static_cast<MenuOption>(std::stoi(input));
			switch (option)
			{
				case MenuOption::ADD_FOOD:
					std::cout << sNotImplemented << std::endl;
					std::getline(std::cin, input);
					break;
				case MenuOption::VIEW_FOOD:
					std::cout << sNotImplemented << std::endl;
					std::getline(std::cin, input);
					break;
				case MenuOption::EXIT:
					std::cout << "Saliendo..." << std::endl;
					std::this_thread::sleep_for(1s);
					return 0;
				default:
					std::cout << sInvalidInputMsg << std::endl;
			}
		}
		catch (std::invalid_argument&)
		{
			std::cout << sInvalidInputMsg;
		}
	}

	return 0;
}
