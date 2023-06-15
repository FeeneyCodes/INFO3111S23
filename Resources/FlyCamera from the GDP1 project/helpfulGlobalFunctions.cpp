#include "helpfulGlobalFunctions.h"

#include <iostream>


void g_coutAVec3(glm::vec3 theVec3, bool bNewLine /*=false*/)
{
	std::cout << "  " << theVec3.x << ", " << theVec3.y << ", " << theVec3.z << "   ";
	if (bNewLine)
	{
		std::cout << std::endl;
	}
	return;
}

unsigned int g_NextCrappyRandNumberIndex = 0;
bool g_isRandomNumberTableLoaded = false;
const unsigned int NUM_OF_CRAPPY_NUMBERS_IN_TABLE = 10'000;
float g_CrappyRandomNumberTable[NUM_OF_CRAPPY_NUMBERS_IN_TABLE];

// Part of explaining the thread locking
float getRandCrappyVersion(void)
{
	if ( ! ::g_isRandomNumberTableLoaded )
	{
		for (unsigned int index = 0; index != NUM_OF_CRAPPY_NUMBERS_IN_TABLE; index++)
		{
			::g_CrappyRandomNumberTable[index] = gGetRandBetween(-1.0f, 1.0f);
		}
		::g_isRandomNumberTableLoaded = true;
	}


	float randNumber = ::g_CrappyRandomNumberTable[::g_NextCrappyRandNumberIndex];

	::g_NextCrappyRandNumberIndex++;

	if (::g_NextCrappyRandNumberIndex >= NUM_OF_CRAPPY_NUMBERS_IN_TABLE)
	{
		// Reset the index
		::g_NextCrappyRandNumberIndex = 0;
	}

	return randNumber;
}