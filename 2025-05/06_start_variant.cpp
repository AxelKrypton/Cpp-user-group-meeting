/*
 *===================================================
 *
 *    Copyright (c) 2025
 *      Alessandro Sciarra
 *
 *    GNU General Public License (GPLv3 or later)
 *
 *===================================================
 */

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string_view>
#include <variant>
#include <vector>

// Taken from https://stackoverflow.com/a/56766138/14967071
template <typename T>
constexpr auto type_name() {
  std::string_view name, prefix, suffix;
#ifdef __clang__
  name = __PRETTY_FUNCTION__;
  prefix = "auto type_name() [T = ";
  suffix = "]";
#elif defined(__GNUC__)
  name = __PRETTY_FUNCTION__;
  prefix = "constexpr auto type_name() [with T = ";
  suffix = "]";
#elif defined(_MSC_VER)
  name = __FUNCSIG__;
  prefix = "auto __cdecl type_name<";
  suffix = ">(void)";
#endif
  name.remove_prefix(prefix.size());
  name.remove_suffix(suffix.size());
  return name;
}


class ScatterAction;
class FluidizationAction;
using Particles = std::vector<int>;
using Action = std::variant<ScatterAction,FluidizationAction>;
using Actions = std::vector<Action>;

class ScatterAction {
  public:
    ScatterAction(Particles p) : particles_{std::move(p)} {}
    void perform() const {
      if(particles_.size() > 1){
        std::cout << "Scattering between " << particles_[0] << " and " << particles_[1] << ".\n";
      }
    }

  private:
    Particles particles_;
};

class FluidizationAction {
  public:
    FluidizationAction(Particles p) : particles_{std::move(p)} {}
    void perform() const {
      if(particles_.size() > 0)
      {
        std::cout << "Particle " << particles_.back() << " will be melt.\n";
      }
    }

  private:
    Particles particles_;
};


void perform_all_actions(const Actions& actions)
{
  for (auto& action : actions)
  {
    std::visit([](const auto& arg){arg.perform();}, action);
  }
}

int main() {
  // Creating actions
  Particles p1 = {1, 11, 111}, p2 = {2, 22, 222};
  Actions actions{};
  actions.emplace_back(ScatterAction{std::move(p1)});
  actions.emplace_back(FluidizationAction{std::move(p2)});

  // Performing actions
  std::cout << "PERFORM:\n";
  perform_all_actions(actions);
}
