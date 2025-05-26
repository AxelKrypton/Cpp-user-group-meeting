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

class ScatterAction;
class FluidizationAction;
class DecayAction;
using Particles = std::vector<int>;
using Action = std::variant<ScatterAction,FluidizationAction,DecayAction>;
using Actions = std::vector<Action>;

class ScatterAction {
  public:
    ScatterAction(Particles p) : particles_{std::move(p)} {}

    // External read-access to particles
    const Particles& particles() const { return particles_; }

  private:
    Particles particles_;
};

class FluidizationAction {
  public:
    FluidizationAction(Particles p) : particles_{std::move(p)} {}

    // External read-access to particles
    const Particles& particles() const { return particles_; }

  private:
    Particles particles_;
};

class DecayAction {
  public:
    DecayAction(Particles p) : particles_{std::move(p)} {}

    // External read-access to particles
    const Particles& particles() const { return particles_; }

  private:
    Particles particles_;
};

class Performer {
  public:
    void operator()(const ScatterAction& action) const {
      if(auto particles = action.particles(); particles.size() > 1){
        std::cout << "Scattering between " << particles[0] << " and " << particles[1] << ".\n";
      }
    }
    void operator()(const FluidizationAction& action) const {
      if(auto particles = action.particles(); particles.size() > 0)
      {
        std::cout << "Particle " << particles.back() << " will be melt.\n";
      }
    }
    template<typename T>
    void operator()(const T& action) const {
        std::cout << "Performer not possible for " << type_name<T>() << " type.\n";
    }
};

// Let's add a new operation for FluidizationAction only
class Remover {
  public:
    void operator()(const FluidizationAction& action) const {
      if(auto particles = action.particles(); particles.size() > 0)
      {
        std::cout << "Particle " << particles[0] << " will be removed.\n";
        particles.erase(particles.begin());
      }
    }
    template<typename T>
    void operator()(const T& action) const {
        std::cout << "Remover not possible for " << type_name<T>() << " type.\n";
    }
};

class Decayer {
  public:
    void operator()(const DecayAction& action) const {
      std::cout << "Particle(s) ";
      for(auto p : action.particles())
      {
        std::cout << p << " ";
      }
      std::cout << "will be decayed.\n";
    }
    template<typename T>
    void operator()(const T& action) const {
        std::cout << "Decayer not possible for " << type_name<T>() << " type.\n";
    }
};

template<typename OPERATION>
void do_on_all_actions(const Actions& actions)
{
  for (auto& action : actions)
  {
    std::visit( OPERATION{}, action );
  }
}

int main() {
  // Creating actions
  Particles p1 = {1, 11, 111}, p2 = {42, 666, 13}, p3 = {66, 77};
  Actions actions{};
  actions.emplace_back(ScatterAction{std::move(p1)});
  actions.emplace_back(FluidizationAction{std::move(p2)});
  actions.emplace_back(DecayAction{std::move(p3)});

  // Performing actions
  std::cout << "PERFORM:\n";
  do_on_all_actions<Performer>(actions);
  std::cout << "REMOVAL:\n";
  do_on_all_actions<Remover>(actions);
  std::cout << "DECAY:\n";
  do_on_all_actions<Decayer>(actions);
}
