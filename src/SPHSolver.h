#pragma once
#include "Vec3.h"
#include <cstddef>
#include <unordered_map>
#include <vector>

struct Particle {
    Vec3 position;
    Vec3 velocity;
    Vec3 force;
    float density{1000.0f};
    float pressure{};
};

struct SimulationSettings {
    float smoothingRadius{0.32f};
    float particleMass{0.018f};
    float restDensity{1000.0f};
    float gasConstant{1800.0f};
    float viscosity{16.0f};
    float boundaryDamping{0.45f};
    Vec3 gravity{0.0f,-9.81f,0.0f};
    Vec3 boundsMin{-3.0f,0.0f,-2.0f};
    Vec3 boundsMax{3.0f,5.0f,2.0f};
};

class SPHSolver {
public:
    explicit SPHSolver(SimulationSettings settings = {});
    void reset();
    void step(float dt);
    void emitBlock(Vec3 min, Vec3 max, float spacing, Vec3 velocity = {});
    void emitJet(Vec3 origin, Vec3 velocity, int width = 3);
    std::vector<Particle>& particles() { return particles_; }
    const std::vector<Particle>& particles() const { return particles_; }
    SimulationSettings& settings() { return settings_; }
private:
    struct Cell { int x,y,z; bool operator==(const Cell&) const = default; };
    struct CellHash { std::size_t operator()(const Cell& c) const noexcept; };
    Cell cellOf(const Vec3& p) const;
    void buildGrid();
    void computeDensityPressure();
    void computeForces();
    void integrate(float dt);
    template<class F> void forNeighbors(const Particle& p, F&& fn) const;
    SimulationSettings settings_;
    std::vector<Particle> particles_;
    std::unordered_map<Cell,std::vector<std::size_t>,CellHash> grid_;
};

