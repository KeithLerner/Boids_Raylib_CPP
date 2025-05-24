#include "Boid.h"

float Boid::maxSpeed = 16.0f;
float Boid::alignmentWeight = 6.0f;
float Boid::cohesionWeight= 1.0f;
float Boid::separationWeight= 3.0f;
float Boid::avoidEdgesWeight = 12.0f;
float Boid::separationDistance = 12.0f;
float Boid::senseDistance = 64.0f;