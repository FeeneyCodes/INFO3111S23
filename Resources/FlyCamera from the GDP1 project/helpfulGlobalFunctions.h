#pragma once

#include <stdlib.h>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>

// SUPER quick-n-dirty template example
template <class T>
T addTwoNumbers(T a, T b)
{
    return a + b;
}

// Here's a more useful templated one, based on this one:
//https://stackoverflow.com/questions/686353/random-float-number-generation
template <class T>
T gGetRandBetween(T LO, T HI)
{
    float fLO = static_cast<float>(LO);
    float fHI = static_cast<float>(HI);
    float r3 = fLO + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (fHI - fLO)));
    return r3;
}

//void g_coutAVec3( glm::vec3 theVec3, bool bNewLin
                 
// Part of explaining the thread locking
float getRandCrappyVersion(void);