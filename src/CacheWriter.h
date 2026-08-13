#pragma once
#include "SPHSolver.h"
#include <filesystem>
bool writePlyCache(const std::filesystem::path& directory,int frame,const std::vector<Particle>& particles);

