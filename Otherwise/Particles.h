#pragma once
#include "MathLibraryInterface.h"
#include <vector>

namespace Otherwise
{

class ParticleEmitterCone
{
public:
	ParticleEmitterCone();
	~ParticleEmitterCone();

	void init (ovec3 position, float emitterSize, ovec3 travelVec,
		float maxDisplacement, int particlesInWorld, unsigned int particleLife);
	
	void loopParticles(unsigned int msDeltaTime);

	std::vector<ovec3> getParticlePositions();

private:
	float mParticlesPerTimeframe = 0.0f;
	unsigned int mParticleTime = 0;
	unsigned int mParticlesInWorld = 0;
	unsigned int mParticleLife = 0;
	ovec3 mPosition;
	ovec3 mTravelVec;
	float mEmitterSize = 0.0f;
	float mMaxDisplacement = 0.0f;
	std::vector<int> mParticleLives;
	std::vector<ovec3> mTravelDirs;
	std::vector<ovec3> mCurrentPositions;
};

}