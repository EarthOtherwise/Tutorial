#include "Particles.h"
#include <random>
#include <iostream>

namespace Otherwise
{


ParticleEmitterCone::ParticleEmitterCone()
{
}


ParticleEmitterCone::~ParticleEmitterCone()
{
}

void ParticleEmitterCone::init(glm::vec3 position, float emitterSize, 
	glm::vec3 travelVec, float maxDisplacement, int particlesInWorld, 
	unsigned int particleLife)
{
	mPosition = position;
	mEmitterSize = emitterSize;
	mTravelVec = travelVec;
	mMaxDisplacement = maxDisplacement;
	mParticlesInWorld = particlesInWorld;
	mParticleLife = particleLife;
	mParticleLives.resize(mParticlesInWorld);
	mTravelDirs.resize(mParticlesInWorld);
	mCurrentPositions.resize(mParticlesInWorld);
	mParticleTime = 0;
	mParticlesPerTimeframe = static_cast<float>(mParticlesInWorld) /
		static_cast<float>(mParticleLife);
	std::cout << mParticlesPerTimeframe << " particles can appear each frame." <<
		'\n';
}

void ParticleEmitterCone::loopParticles(unsigned int msDeltaTime)
{
	float newParticles = 0.9;
	float particlesWeCanMakeThisFrame = 0.0f;

	mParticleTime += msDeltaTime;

	if (particlesWeCanMakeThisFrame = static_cast<float>(mParticleTime) *
		mParticlesPerTimeframe > 1.0f)
	{
		mParticleTime = 0;
	}

	for (unsigned int i = 0; i < mParticlesInWorld; i++)
	{
		mParticleLives[i] -= msDeltaTime;
		if (mParticleLives[i] < 1 && newParticles < particlesWeCanMakeThisFrame)
		{
			std::random_device rd;
			std::mt19937 mt(rd());
			std::uniform_real_distribution<float> displace(-mMaxDisplacement,
				mMaxDisplacement);
			std::uniform_real_distribution<float> posDisplace(-mEmitterSize,
				mEmitterSize);

			mParticleLives[i] = static_cast<int>(mParticleLife);
			mCurrentPositions[i] = glm::vec3(mPosition.x + posDisplace(mt),
				mPosition.y + posDisplace(mt), mPosition.z + posDisplace(mt));
			mTravelDirs[i] = glm::vec3(mTravelVec.x + displace(mt),
				mTravelVec.y + displace(mt), mTravelVec.z + displace(mt));
			newParticles += 1.0f;
		}
		if (mParticleLives[i] > 1)
		{
			mCurrentPositions[i] += mTravelDirs[i] * static_cast<float>(msDeltaTime);
		}
	}
}

std::vector<glm::vec3> ParticleEmitterCone::getParticlePositions()
{
	std::vector<glm::vec3> livingParticles;

	for (unsigned int i = 0; i < mCurrentPositions.size(); i++)
	{
		if (mParticleLives[i] > 0)
		{
			livingParticles.push_back(mCurrentPositions[i]);
		}
	}
	return livingParticles;
}

}