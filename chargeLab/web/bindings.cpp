#include <emscripten/bind.h>

#include "../src/Simulation.h"
#include "../src/Vector2D.h"

using namespace emscripten;

EMSCRIPTEN_BINDINGS(charge_lab_module) {
    class_<Vector2D>("Vector2D")
        .constructor<>()
        .constructor<double, double>()
        .function("getX", &Vector2D::GetX)
        .function("getY", &Vector2D::GetY)
        .function("magnitude", &Vector2D::Magnitude)
        .function("magnitudeSquared", &Vector2D::Magnitude)
        ;

    class_<Simulation>("Simulation")
        .constructor<>()
        .constructor<double, double>()
        .function("update", &Simulation::Update)
        .function("addParticleAt",
            select_overload<bool(double, double, double)>(&Simulation::AddParticleAt)
        )
        .function("addParticleAtFull",
            select_overload<bool(double, double, double, double, double)>(&Simulation::AddParticleAt)
        )
        .function("clearParticles", &Simulation::ClearParticles)
        .function("setSelectedFixed", &Simulation::SetSelectedFixed)
        .function("getSelectedFixed", &Simulation::GetSelectedFixed)
        .function("getParticleCount", &Simulation::GetParticleCount)
        .function("getMaxParticles", &Simulation::GetMaxParticles)
        .function("getParticleX", &Simulation::GetParticleX)
        .function("getParticleY", &Simulation::GetParticleY)
        .function("getParticleRadius", &Simulation::GetParticleRadius)
        .function("getParticleCharge", &Simulation::GetParticleCharge)
        .function("togglePaused", &Simulation::TogglePaused)
        .function("isPaused", &Simulation::IsPaused)
        .function("selectParticleAt", &Simulation::SelectedParticleAt)
        .function("clearSelected", &Simulation::ClearSelectedParticle)
        .function("hasSelected", &Simulation::HasSelectedParticle)
        .function("getSelectedIndex", &Simulation::GetSelectedParticleIndex)
        .function("moveSelected", &Simulation::MoveSelected)
        .function("setSelectedVelocity", &Simulation::SetSelectedVelocity)
        .function("setSelectedCharge", &Simulation::SetSelectedCharge)
        .function("setSelectedMass", &Simulation::SetSelectedMass)
        .function("setSelectedRadius", &Simulation::SetSelectedRadius)
        .function("getSelectedX", &Simulation::GetSelectedX)
        .function("getSelectedY", &Simulation::GetSelectedY)
        .function("getSelectedVx", &Simulation::GetSelectedVx)
        .function("getSelectedVy", &Simulation::GetSelectedVy)
        .function("getSelectedCharge", &Simulation::GetSelectedCharge)
        .function("getSelectedMass", &Simulation::GetSelectedMass)
        .function("getSelectedRadius", &Simulation::GetSelectedRadius)
        .function("getForceBetweenParticles", &Simulation::GetForceBetweenParticles)
        .function("getTotalForceOnParticle", &Simulation::GetTotalForceOnParticle)
        .function("getElapsedTime", &Simulation::GetElapsedTime)
        .function("resetElapsedTime", &Simulation::ResetElapsedTime)
        .function("getKE", &Simulation::GetKE)
        .function("getPE", &Simulation::GetPE)
        .function("getME", &Simulation::GetME)
        .function("setME0", &Simulation::SetME0)
        .function("getME0", &Simulation::GetME0)
        .function("getMEDrift", &Simulation::GetMEDrift)
        .function("getMEDriftPct", &Simulation::GetMEDriftPct)
        .function("isMEStable", &Simulation::IsMEStable)
        ;
}