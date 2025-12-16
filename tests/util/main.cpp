/***************************************************
 * main.cpp
 * Created on Fri, 12 Dec 2025 07:08:37 +0000 by vladimir
 *
 * $Author$
 * $Rev$
 * $Date$
 ***************************************************/

#include <iostream>
#include <tuple>
#include <util/Settings.hpp>
#include <util/type_traits.hpp>

enum class Colors : short
{
  Red = 0,
  Green = 4,
  Blue = 7
};

enum State
{
  NOSTATE = -1,
  START = 0,
  TRANSFER_STARTED,
  PROGRESS,
  RINGING,
  END
};

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
  std::cout << "valueG: " << settings["Section1"]["valueG"].as<int>()
            << std::endl;
  settings["Section1"]["array"].forEach(
    [](auto& v)
    {
      using T = std::decay_t<decltype(v)>;
      if constexpr (util::is_pair_v<T>) {
        util::SettingsValue& val = v.second;
        std::cout << "array[" << v.first << "]: " << (val.as<int>())
                  << std::endl;
      }
    });
  using colors_traits = util::enum_traits<Colors>;
  for (auto c = colors_traits::begin(); c != colors_traits::end(); ++c) {
    std::cout << "C: " << (int)*c << ": " << colors_traits::get_name(*c)
              << std::endl;
  }
  std::cout << "MIN: " << (int)colors_traits::min() << ": "
            << colors_traits::get_name(colors_traits::min()) << std::endl;
  std::cout << "MAX: " << (int)colors_traits::max() << ": "
            << colors_traits::get_name(colors_traits::max()) << std::endl;
  std::cout << (int)colors_traits::get_value(
                 colors_traits::get_name(colors_traits::min()))
            << std::endl;
  std::cout << colors_traits::has_value(10) << std::endl;
  typedef std::tuple<Colors, State> MyType;
  std::cout << util::type_reflection<MyType>::name << std::endl;
}
