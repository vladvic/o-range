/***************************************************
 * main.cpp
 * Created on Fri, 12 Dec 2025 07:08:37 +0000 by vladimir
 *
 * $Author$
 * $Rev$
 * $Date$
 ***************************************************/

#include <iostream>
#include <util/Settings.hpp>

int main()
{
  util::Settings settings;

  settings.set("valueA", std::string("Hello"));
  settings.set("valueB", "World");
  settings.set("valueC", 42);
  settings.set("valueD", -42);
  settings.set("valueE", (unsigned int)42);
  settings.set("valueF", 0.123);

  std::cout << "valueA: " << settings.get<std::string>("valueA") << std::endl;
  std::cout << "valueB: " << settings.get<const char*>("valueB") << std::endl;
  std::cout << "valueC: " << settings.get<int>("valueC") << std::endl;
  std::cout << "valueD: " << settings.get<int>("valueD") << std::endl;
  std::cout << "valueE: " << settings.get<unsigned int>("valueE") << std::endl;
  std::cout << "valueF: " << settings.get<float>("valueF") << std::endl;
}
