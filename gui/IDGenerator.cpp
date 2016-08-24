#include "IDGenerator.h"

#include "leaker.h"

int CIDGenerator::currentId = 3000;

/**
 * Constructor
 */
CIDGenerator::CIDGenerator(void)
{
}

/**
 * Destructor
 */
CIDGenerator::~CIDGenerator(void)
{
}

/**
 * Returns the next id
 */
int CIDGenerator::getNextID()
{
	return  currentId ++;
}