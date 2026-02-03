/*
  ==============================================================================

    EQComb.h
    Created: 13 Nov 2019 9:22:41am
    Author:  Silvin Willemsen

  ==============================================================================
*/

#pragma once

#include "EQFilter.h"

#include <cassert>
#include <vector>

//==============================================================================
/*
 */

class EQComb
{
  public:
    EQComb(int delayLength);
    ~EQComb();

    EQComb(const EQComb&) = delete;
    EQComb& operator=(const EQComb&) = delete;

    EQComb(const EQComb&&) = delete;
    EQComb& operator=(const EQComb&&) = delete;

    void setFilter(int i, std::vector<double> coeffs)
    {
        assert(i < eqFilters.size());
        eqFilters[i]->setCoeffs(coeffs);
    };

    float filter(float x);

    void increment()
    {
        readLoc = (readLoc + 1) % delayLine.size();
        writeLoc = (writeLoc + 1) % delayLine.size();
    };

    EQFilter* getFilter(int idx)
    {
        assert(idx < eqFilters.size());
        assert(eqFilters.at(idx) != nullptr);
        return eqFilters[idx];
    };
    int getDelayLineLength()
    {
        return static_cast<int>(delayLine.size());
    };

    void debug()
    {
        debugFlag = true;
    };

    void addScatOutput(float val)
    {
        delayLine[writeLoc] += val;
    };
    void zeroWritePointer()
    {
        delayLine[writeLoc] = 0;
    };

    void zeroCoefficients();

    std::vector<double> getCoefficientsOfFilter(int filterIdx)
    {
        return eqFilters[filterIdx]->getCoefficients();
    }

  private:
    std::vector<EQFilter*> eqFilters;
    std::vector<double> delayLine;

    int readLoc = 1;
    int writeLoc = 0;

    bool debugFlag = false;

    double y;
};