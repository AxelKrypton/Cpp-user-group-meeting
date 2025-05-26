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

class Action;

using Particles = std::vector<int>;
using Actions = std::vector<std::unique_ptr<Action>>;

class Action {
  public:
    // Rule of 5: Action cannot be copied or moved
    explicit Action(Particles p) : particles_{std::move(p)} {};
    Action(const Action &) = delete;
    Action& operator=(const Action &) = delete;
    Action(Action &&) = delete;
    Action& operator=(Action &&) = delete;
    // Virtual destructor for polymorphism
    virtual ~Action() = default;

    // External read-access to particles
    const Particles& particles() const { return particles_; }

    // Operations
    virtual void perform() const = 0;

  private:
    Particles particles_;
};

class ScatterAction : public Action {
  public:
    explicit ScatterAction(Particles p) : Action{std::move(p)} {}
    void perform() const override {
      if(auto p = particles(); p.size() > 1){
        std::cout << "Scattering between " << p[0] << " and " << p[1] << ".\n";
      }
    }
};

class FluidizationAction : public Action {
  public:
    explicit FluidizationAction(Particles p) : Action{std::move(p)} {}
    void perform() const override {
      if(auto p = particles(); p.size() > 0)
      {
        std::cout << "Particle " << p.back() << " will be melt.\n";
      }
    }
};


void perform_all_actions(const Actions& actions)
{
  for (const auto& action : actions)
  {
    action->perform();
  }
}

int main() {
  // Creating actions
  Particles p1 = {1, 11, 111}, p2 = {2, 22, 222};
  Actions actions{};
  actions.emplace_back(std::make_unique<ScatterAction>(std::move(p1)));
  actions.emplace_back(std::make_unique<FluidizationAction>(std::move(p2)));

  // Performing actions
  std::cout << "PERFORM:\n";
  perform_all_actions(actions);
}
