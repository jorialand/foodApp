#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <locale>
#include <sstream>

using namespace std::chrono_literals;

const inline static std::string sInvalidInputMsg = "Opción no válida. Inténtelo de nuevo.";
const inline static std::string sNotImplemented = "Opción no implementada.";

enum class MenuOption
{
	ADD_FOOD = 1,
	VIEW_FOOD,
	EXIT
};

void CleanScreen()
{
	system("cls");
}

void DisplayMenu()
{
	CleanScreen();
	std::cout << "Elija una opción:\n";
	std::cout << "1) Añadir comida.\n";
	std::cout << "2) Ver comidas.\n";
	std::cout << "3) Salir del programa.\n";
}

void AddFood()
{}

int main()
{
	std::locale::global(std::locale("")); // Set the global locale for the console
	std::cout.imbue(std::locale());       // Set the locale for cout

	std::string input;
	MenuOption option;

	while (true)
	{
		DisplayMenu();

		std::getline(std::cin, input);
		try
		{
			// For adding food
			std::istringstream iss;
			std::string sTitle, sDate, sPhoto, sDescription, bHungry, eSatisfaction, bBadFood;

			option = static_cast<MenuOption>(std::stoi(input));
			switch (option)
			{
				case MenuOption::ADD_FOOD:
				//std::cout << sNotImplemented << std::endl;
				CleanScreen();
				std::cout << "Introduzca los datos separados por comas (Title,Date,Photo,Description,Hungry,Satisfaction,BadFood): ";
				std::getline(std::cin, input);
				iss.str(input);

				std::getline(iss, sTitle, ',');
				std::getline(iss, sDate, ',');
				std::getline(iss, sPhoto, ',');
				std::getline(iss, sDescription, ',');
				std::getline(iss, bHungry, ',');
				std::getline(iss, eSatisfaction, ',');
				std::getline(iss, bBadFood, ',');

				//// Print the parsed values to the console
				std::cout << "Title: " << sTitle << std::endl;
				std::cout << "Date: " << sDate << std::endl;
				std::cout << "Photo: " << sPhoto << std::endl;
				std::cout << "Description: " << sDescription << std::endl;
				std::cout << "Hungry: " << bHungry << std::endl;
				std::cout << "Satisfaction: " << eSatisfaction << std::endl;
				std::cout << "BadFood: " << bBadFood << std::endl;
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
