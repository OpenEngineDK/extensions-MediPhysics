#pragma once

class mediManipulator
{
public:
	mediManipulator(void);
	virtual ~mediManipulator(void);

	virtual void apply()=0;
};
