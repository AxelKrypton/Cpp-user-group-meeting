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

class AbstractActionVisitor {
  public:
    virtual ~AbstractActionVisitor() = default;
};

template<typename T>
class ActionVisitor {
  public:
    virtual ~ActionVisitor() = default;
    virtual void visit(const T&) const = 0;
};

class Action {
  public:
    // Rule of 5: Action cannot be copied or moved
    Action(Particles p) : particles_{std::move(p)} {};
    Action(const Action &) = delete;
    Action& operator=(const Action &) = delete;
    Action(Action &&) = delete;
    Action& operator=(Action &&) = delete;
    // Virtual destructor for polymorphism
    virtual ~Action() = default;

    // External read-access to particles
    const Particles& particles() const { return particles_; }

    // Abstract accept Visitor method
    virtual void accept(const AbstractActionVisitor&) const = 0;

  private:
    Particles particles_;
};

class ScatterAction : public Action {
  public:
    ScatterAction(Particles p) : Action{std::move(p)} {}

    void accept(const AbstractActionVisitor& visitor) const override {
      if (auto concrete_visitor = dynamic_cast<const ActionVisitor<ScatterAction>*>(&visitor)){
        concrete_visitor->visit(*this);
      } else {
        std::cout << "ScatterAction: I cannot be visited.\n";
      }
    }
};

class FluidizationAction : public Action {
  public:
    FluidizationAction(Particles p) : Action{std::move(p)} {}

    void accept(const AbstractActionVisitor& visitor) const override {
      if(auto concrete_visitor = dynamic_cast<const ActionVisitor<FluidizationAction>*>(&visitor)){
        concrete_visitor->visit(*this);
      } else {
        std::cout << "FluidizationAction: I cannot be visited.\n";
      }
    }
};

class DecayAction : public Action {
  public:
    DecayAction(Particles p) : Action{std::move(p)} {}

    void accept(const AbstractActionVisitor& visitor) const override {
      if(auto concrete_visitor = dynamic_cast<const ActionVisitor<DecayAction>*>(&visitor)){
        concrete_visitor->visit(*this);
      } else {
        std::cout << "DecayAction: I cannot be visited.\n";
      }
    }
};

class Performer : public AbstractActionVisitor,
                  public ActionVisitor<ScatterAction>,
                  public ActionVisitor<FluidizationAction> {
  public:
    void visit(const ScatterAction& action) const override {
      if(auto particles = action.particles(); particles.size() > 1){
        std::cout << "Scattering between " << particles[0] << " and " << particles[1] << ".\n";
      }
    }
    void visit(const FluidizationAction& action) const override {
      if(auto particles = action.particles(); particles.size() > 0)
      {
        std::cout << "Particle " << particles.back() << " will be melt.\n";
      }
    }
};

// Let's add a new operation for FluidizationAction only
class Remover : public AbstractActionVisitor,
                public ActionVisitor<FluidizationAction> {
  public:
    void visit(const FluidizationAction& action) const override {
      if(auto particles = action.particles(); particles.size() > 0)
      {
        std::cout << "Particle " << particles[0] << " will be removed.\n";
        particles.erase(particles.begin());
      }
    }
};

// Let's add another new operation for DecayAction only
class Decayer : public AbstractActionVisitor,
                public ActionVisitor<DecayAction> {
  public:
    void visit(const DecayAction& action) const override {
      std::cout << "Particle(s) ";
      for(auto p : action.particles())
      {
        std::cout << p << " ";
      }
      std::cout << "will be decayed.\n";
    }
};

template<typename OPERATION>
void do_on_all_actions(const Actions& actions)
{
  for (const auto& action : actions)
  {
    action->accept( OPERATION{} );
  }
}

int main() {
  // Creating actions
  Particles p1 = {1, 11, 111}, p2 = {42, 666, 13}, p3 = {66, 77};
  Actions actions{};
  actions.emplace_back(std::make_unique<ScatterAction>(std::move(p1)));
  actions.emplace_back(std::make_unique<FluidizationAction>(std::move(p2)));
  actions.emplace_back(std::make_unique<DecayAction>(std::move(p3)));

  // Performing actions
  std::cout << "PERFORM:\n";
  do_on_all_actions<Performer>(actions);
  std::cout << "REMOVAL:\n";
  do_on_all_actions<Remover>(actions);
  std::cout << "DECAY:\n";
  do_on_all_actions<Decayer>(actions);
}
