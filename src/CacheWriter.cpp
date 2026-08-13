#include "CacheWriter.h"
#include <fstream>
#include <iomanip>
#include <sstream>
bool writePlyCache(const std::filesystem::path& directory,int frame,const std::vector<Particle>& particles){
    std::error_code ec;std::filesystem::create_directories(directory,ec);if(ec)return false;
    std::ostringstream name;name<<"fluid_"<<std::setw(6)<<std::setfill('0')<<frame<<".ply";
    std::ofstream out(directory/name.str());if(!out)return false;
    out<<"ply\nformat ascii 1.0\nelement vertex "<<particles.size()<<"\nproperty float x\nproperty float y\nproperty float z\nproperty float vx\nproperty float vy\nproperty float vz\nend_header\n";
    for(const auto& p:particles) {
        out<<p.position.x<<' '<<p.position.y<<' '<<p.position.z<<' '
           <<p.velocity.x<<' '<<p.velocity.y<<' '<<p.velocity.z<<'\n';
    }
    return true;
}
