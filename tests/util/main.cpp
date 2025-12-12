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
  util::SettingsHolder settings;

  settings["valueA"] = std::string("Hello");
  settings["valueB"] = "World";
  settings["valueC"] = 42;
  settings["valueD"] = -42;
  settings["valueE"] = (unsigned int)42;
  settings["valueF"] = 0.123;
  settings["Section1"]["valueG"] = 1234;
  settings["Section1"]["array"].resize(5);
  settings["Section1"]["array"][0] = 1;
  settings["Section1"]["array"][1] = 2;
  settings["Section1"]["array"][2] = 3;
  settings["Section1"]["array"][3] = 4;
  settings["Section1"]["array"][4] = 5;

  std::cout << "valueA: " << settings["valueA"].as<std::string>() << std::endl;
  std::cout << "valueB: " << settings["valueB"].as<const char*>() << std::endl;
  std::cout << "valueC: " << settings["valueC"].as<int>() << std::endl;
  std::cout << "valueD: " << settings["valueD"].as<int>() << std::endl;
  std::cout << "valueE: " << settings["valueE"].as<unsigned int>() << std::endl;
  std::cout << "valueF: " << settings["valueF"].as<float>() << std::endl;
  std::cout << "valueG: " << settings["Section1"]["valueG"].as<int>() << std::endl;
  settings["Section1"]["array"].forEach(
    [](auto &v) {
      using T = std::decay_t<decltype(v)>;
      if constexpr (util::is_pair_v<T>) {
        util::SettingsValue &val = v.second;
        std::cout << "array[" << v.first << "]: " << (val.as<int>()) << std::endl;
      }
    });
}
