#include "Boid.h"

float Boid::maxSpeed = 6.0f;
float Boid::alignmentWeight = 3.0f;
float Boid::cohesionWeight= 1.0f;
float Boid::separationWeight= 2.0f;
float Boid::avoidEdgesWeight = 4.0f;
float Boid::separationDistance = 12.0f;
float Boid::senseDistance = 24.0f;