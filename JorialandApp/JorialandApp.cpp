#include <iostream>
#include <algorithm>
#include <string>
#include <chrono>
#include <thread>
#include <locale>
#include <sstream>
#include <string_view>
#include <Poco/JSON/JSON.h>
#include <Poco/JSON/Object.h>
#include <Poco/JSON/Stringifier.h>
#include <Poco/JSON/Parser.h>
#include <Poco/Dynamic/Var.h>
#include <mongocxx/instance.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/uri.hpp>
#include <bsoncxx/json.hpp>

#include "JorialandApp.h"

using namespace std::chrono_literals;

const inline static std::string sInvalidInputMsg = "Opción no válida. Inténtelo de nuevo.";
const inline static std::string sNotImplemented = "Opción no implementada.";

namespace Utilities
{
	bool strToBool(std::string_view s)
	{
		return 0 != std::stoi(s.data());
	}
}

namespace Enums
{
	enum class eMenuOption
	{
		ADD_FOOD = 1,
		VIEW_FOOD,
		EXIT
	};

	enum class eEventType
	{
		EVENT = 1,
		FOOD
	};

	enum class eSatisfaction
	{
		LOW = 1,
		MEDIUM,
		HIGH
	};

	eSatisfaction strToEnum(std::string_view s)
	{
		if (s == "1") return Enums::eSatisfaction::LOW;
		else if (s == "2") return Enums::eSatisfaction::MEDIUM;
		else if (s == "3") return Enums::eSatisfaction::HIGH;
		else throw std::runtime_error(s.data());
	}
}

struct Event
{
	std::string sTitle;
	std::string sDescription;
	std::string sDate;
	virtual Enums::eEventType GetEventType() = 0;
	Event() = default;
	Event(const std::string& sTitle,
				const std::string& sDescription,
				const std::string& sDate) : sTitle{ sTitle }, sDescription{ sDescription }, sDate{ sDate }
	{}
};
struct Food : Event
{
	std::string sImage;
	bool bHungry{ false };
	Enums::eSatisfaction eSatisfaction{ Enums::eSatisfaction::MEDIUM };
	bool bBadFood{ false };
	Enums::eEventType GetEventType() override
	{
		return Enums::eEventType::FOOD;
	}
	Food() = default;
	Food(const std::string& sTitle,
			 const std::string& sDescription,
			 const std::string& sDate,
			 const std::string& sImage,
			 bool bHungry,
			 Enums::eSatisfaction eSatisfaction,
			 bool bBadFood) : Event(sTitle, sDescription, sDate), sImage{ sImage }, bHungry{ bHungry }, eSatisfaction{ eSatisfaction }, bBadFood{ bBadFood }
	{}

	static void Serialize(const Food& food, std::string& json)
	{
		Poco::JSON::Object jsonObj;
		jsonObj.set("sTitle", food.sTitle);
		jsonObj.set("sDescription", food.sDescription);
		jsonObj.set("sDate", food.sDate);
		jsonObj.set("sImage", food.sImage);
		jsonObj.set("bHungry", food.bHungry);
		jsonObj.set("eSatisfaction", static_cast<int>(food.eSatisfaction));
		jsonObj.set("bBadFood", food.bBadFood);

		std::ostringstream oss;
		Poco::JSON::Stringifier::stringify(jsonObj, oss);
		json = oss.str();
	}
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

void WaitAnyInputFromUser()
{
	std::string input;
	std::getline(std::cin, input);
}

Food GetFoodFromUser()
{
	std::string input, sTitle, sDate, sImage, sDescription, bHungry, eSatisfaction, bBadFood;
	std::istringstream iss;
	std::cout << "Introduzca los datos separados por comas (Nombre,Fecha,Imagen,Descripción,Hambre,Satisfacción,Malestar): " << std::endl;
	std::getline(std::cin, input);
	iss.str(input);

	std::getline(iss, sTitle, ',');
	std::getline(iss, sDate, ',');
	std::getline(iss, sImage, ',');
	std::getline(iss, sDescription, ',');
	std::getline(iss, bHungry, ',');
	std::getline(iss, eSatisfaction, ',');
	std::getline(iss, bBadFood, ',');

	std::cout << std::endl << "---" << std::endl; 
	//// Print the parsed values to the console
	std::cout << "Nombre: " << sTitle << std::endl;
	std::cout << "Fecha: " << sDate << std::endl;
	std::cout << "Imagen: " << sImage << std::endl;
	std::cout << "Descripción: " << sDescription << std::endl;
	std::cout << "Hambre: " << bHungry << std::endl;
	std::cout << "Satisfacción: " << eSatisfaction << std::endl;
	std::cout << "Malestar: " << bBadFood << std::endl;

	return Food(sTitle, sDescription, sDate, sImage,
							Utilities::strToBool(bHungry),
							Enums::strToEnum(eSatisfaction),
							Utilities::strToBool(bBadFood));
}

bool InsertInDatabase(const Food& food)
{
	std::cout << std::endl << "---" << std::endl;
	std::cout << "Comida " << food.sTitle << " añadida." << std::endl;
	std::string foodJson;
	Food::Serialize(food, foodJson);
	std::cout << "Json:" << std::endl << foodJson << std::endl;

	mongocxx::instance instance{};
	mongocxx::client client{mongocxx::uri{"mongodb://localhost:27017"}};
	mongocxx::database db = client["foodApp"];
	mongocxx::collection collection = db["foods"];
	auto bsonDoc = bsoncxx::from_json(foodJson);
	collection.insert_one(bsonDoc.view());

	return true;
}

int main()
{
	//const auto locale = std::locale();
	std::locale::global(std::locale("")/*locale*/); // Set the global locale for the console
	std::cout.imbue(std::locale("")/*locale*/);       // Set the locale for cout

	std::string input;
	Enums::eMenuOption option;

	while (true)
	{
		DisplayMenu();

		std::getline(std::cin, input);
		try
		{
			Food food;

			option = static_cast<Enums::eMenuOption>(std::stoi(input));
			switch (option)
			{
				case Enums::eMenuOption::ADD_FOOD:
				//std::cout << sNotImplemented << std::endl;
				CleanScreen();
				InsertInDatabase(GetFoodFromUser());
				WaitAnyInputFromUser();
				break;
				case Enums::eMenuOption::VIEW_FOOD:
				std::cout << sNotImplemented << std::endl;
				WaitAnyInputFromUser();
				break;
				case Enums::eMenuOption::EXIT:
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

        //{
        //    "name": "mongo-cxx-driver"
        //}