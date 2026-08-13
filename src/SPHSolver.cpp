#include "SPHSolver.h"
#include <algorithm>
#include <cmath>
#include <numbers>

SPHSolver::SPHSolver(SimulationSettings settings):settings_(settings){reset();}
std::size_t SPHSolver::CellHash::operator()(const Cell& c) const noexcept {
    return (static_cast<std::size_t>(c.x)*73856093u) ^ (static_cast<std::size_t>(c.y)*19349663u) ^ (static_cast<std::size_t>(c.z)*83492791u);
}
SPHSolver::Cell SPHSolver::cellOf(const Vec3& p) const {
    const float h=settings_.smoothingRadius;
    return {(int)std::floor(p.x/h),(int)std::floor(p.y/h),(int)std::floor(p.z/h)};
}
void SPHSolver::reset(){particles_.clear();emitBlock({-2.2f,0.25f,-1.45f},{-0.25f,3.0f,1.45f},0.21f);}
void SPHSolver::emitBlock(Vec3 mn,Vec3 mx,float spacing,Vec3 velocity){
    for(float y=mn.y;y<=mx.y;y+=spacing) for(float z=mn.z;z<=mx.z;z+=spacing) for(float x=mn.x;x<=mx.x;x+=spacing)
        particles_.push_back({{x,y,z},velocity,{}});
}
void SPHSolver::emitJet(Vec3 o,Vec3 v,int width){
    const float s=0.19f;
    for(int y=0;y<width;++y) for(int z=0;z<width;++z)
        particles_.push_back({o+Vec3{0,(y-(width-1)*.5f)*s,(z-(width-1)*.5f)*s},v,{}});
}
void SPHSolver::buildGrid(){grid_.clear();grid_.reserve(particles_.size());for(std::size_t i=0;i<particles_.size();++i)grid_[cellOf(particles_[i].position)].push_back(i);}
template<class F> void SPHSolver::forNeighbors(const Particle& p,F&& fn) const {
    Cell c=cellOf(p.position);
    for(int z=-1;z<=1;++z)for(int y=-1;y<=1;++y)for(int x=-1;x<=1;++x){
        auto it=grid_.find({c.x+x,c.y+y,c.z+z});if(it!=grid_.end())for(auto i:it->second)fn(particles_[i]);
    }
}
void SPHSolver::computeDensityPressure(){
    const float h=settings_.smoothingRadius,h2=h*h;
    const float poly6=315.0f/(64.0f*std::numbers::pi_v<float>*std::pow(h,9));
    for(auto& p:particles_){float density=0;forNeighbors(p,[&](const Particle& q){float r2=lengthSq(p.position-q.position);if(r2<h2)density+=settings_.particleMass*poly6*std::pow(h2-r2,3);});p.density=std::max(density,1.0f);p.pressure=settings_.gasConstant*(p.density-settings_.restDensity);}
}
void SPHSolver::computeForces(){
    const float h=settings_.smoothingRadius;
    const float spiky=-45.0f/(std::numbers::pi_v<float>*std::pow(h,6));
    const float visc=45.0f/(std::numbers::pi_v<float>*std::pow(h,6));
    for(auto& p:particles_){Vec3 pressure{},viscosity{};forNeighbors(p,[&](const Particle& q){
        Vec3 d=p.position-q.position;float r=length(d);if(r>0.0001f&&r<h){Vec3 n=d/r;pressure+=n*(-settings_.particleMass*(p.pressure+q.pressure)/(2*q.density)*spiky*(h-r)*(h-r));viscosity+=(q.velocity-p.velocity)*(settings_.viscosity*settings_.particleMass/q.density*visc*(h-r));}});p.force=pressure+viscosity+settings_.gravity*p.density;}
}
void SPHSolver::integrate(float dt){
    const float radius=.07f;for(auto& p:particles_){p.velocity+=p.force*(dt/p.density);p.position+=p.velocity*dt;
        auto collide=[&](float& pos,float& vel,float lo,float hi){if(pos<lo+radius){pos=lo+radius;vel=std::abs(vel)*settings_.boundaryDamping;}if(pos>hi-radius){pos=hi-radius;vel=-std::abs(vel)*settings_.boundaryDamping;}};
        collide(p.position.x,p.velocity.x,settings_.boundsMin.x,settings_.boundsMax.x);collide(p.position.y,p.velocity.y,settings_.boundsMin.y,settings_.boundsMax.y);collide(p.position.z,p.velocity.z,settings_.boundsMin.z,settings_.boundsMax.z);
    }
}
void SPHSolver::step(float dt){buildGrid();computeDensityPressure();computeForces();integrate(std::min(dt,0.008f));}

