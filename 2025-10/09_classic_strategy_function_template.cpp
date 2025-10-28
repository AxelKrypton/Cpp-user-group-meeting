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

#include <iostream>
#include <memory>
#include <vector>

class Action;
class ScatterAction;
class FluidizationAction;
using Particles = std::vector<int>;
using Actions = std::vector<std::unique_ptr<Action>>;

using PerformScatterStrategy = std::function<void(ScatterAction const&)>;
using PerformFluidizationStrategy =
    std::function<void(FluidizationAction const&)>;

struct Performer {
  template <typename T>
  void operator()(const T& action) const {
    perform(action);
  }
};

class Action {
 public:
  // Rule of 5: Action cannot be copied or moved
  explicit Action(Particles p) : particles_{std::move(p)} {};
  Action(const Action&) = delete;
  Action& operator=(const Action&) = delete;
  Action(Action&&) = delete;
  Action& operator=(Action&&) = delete;
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
  explicit ScatterAction(Particles p, PerformScatterStrategy ps = Performer{})
      : Action{std::move(p)}, performer_{std::move(ps)} {}
  void perform() const override { performer_(*this); }

 private:
  PerformScatterStrategy performer_{};
};

class FluidizationAction : public Action {
 public:
  explicit FluidizationAction(Particles p,
                              PerformFluidizationStrategy ps = Performer{})
      : Action{std::move(p)}, performer_{std::move(ps)} {}
  void perform() const override { performer_(*this); }

 private:
  PerformFluidizationStrategy performer_{};
};

void perform(ScatterAction const& action) {
  if (auto p = action.particles(); p.size() > 1) {
    std::cout << "Scattering between " << p[0] << " and " << p[1] << ".\n";
  }
}
void perform(FluidizationAction const& action) {
  if (auto p = action.particles(); p.size() > 0) {
    std::cout << "Particle " << p.back() << " will be melt.\n";
  }
}

void perform_all_actions(const Actions& actions) {
  for (const auto& action : actions) {
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
