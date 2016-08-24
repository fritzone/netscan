#pragma once

/**
 * This class will create an id for items
 */
class CIDGenerator
{
public:

	/**
	 * Returns the next ID
	 */
	static int getNextID();

	/**
	 * Destructor
	 */
	~CIDGenerator(void);

private:

	/**
	 * Constructor
	 */
	CIDGenerator(void);
	
private:

	// the current ID
	static int currentId;
	
};
