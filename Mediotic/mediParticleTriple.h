#pragma once
class mediParticle; //include mediParticle.h

class mediParticleTriple
{
private:
	mediParticle* particles[3];

public:
	mediParticleTriple(void);
	~mediParticleTriple(void);
};
