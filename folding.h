#ifndef FOLDING_H
#define FOLDING_H

#include <vector>
#include <ostream>
#include <string>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#include "element.h"

class Folding
{
public:
	Folding(std::string &sequence);
	~Folding();

	int getFitness(void);
    Element& getElement(unsigned int index);
    unsigned long getSize(void);
    int getOverlaps(void);

	void browse(std::ostream &outputStream);
	void calculateFitness(void);		// energy /= anzahlüberlappungen +1 noch machen
	void calculatePositionsAndDirections(void);
	void calculateOverlaps(void);
	void draw(int offsetX, int offsetY);

private:
	std::vector<Element> m_Elements;
	int m_Fitness;
	int m_Overlaps;
};

#endif