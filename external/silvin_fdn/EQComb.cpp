/*
  ==============================================================================

    EQComb.cpp
    Created: 13 Nov 2019 9:22:41am
    Author:  Silvin Willemsen

  ==============================================================================
*/

#include "EQComb.h"
#include "Global.h"

//==============================================================================
EQComb::EQComb(int dLen)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    delayLine.resize(dLen, 0);

    for (int i = 0; i < Global::numOctaveBands + 1; ++i)
    {
        eqFilters.push_back(new EQFilter());
    }
}

EQComb::~EQComb()
{
    for (auto i = 0; i < eqFilters.size(); ++i)
    {
        delete eqFilters[i];
        eqFilters[i] = nullptr;
    }
    eqFilters.clear();
}

float EQComb::filter(float x)
{
    delayLine[writeLoc] += x;

    y = delayLine[readLoc];
    for (int i = 0; i < Global::numOctaveBands + 1; ++i)
    {
        y = eqFilters[i]->filter(y);
    }
    return y;
}

void EQComb::zeroCoefficients()
{
    for (auto filter : eqFilters)
        filter->zeroCoefficients();

    for (int i = 0; i < delayLine.size(); ++i)
        delayLine[i] = 0;
}