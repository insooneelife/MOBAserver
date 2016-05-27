#ifndef FEATURE_H
#define FEATURE_H
//-----------------------------------------------------------------------------
//
//  Name:   Feature.h
//
//  Author: Mat Buckland (ai-junkie.com)
//
//  Desc:   class that implements methods to extract feature specific
//          information from the Raven game world and present it as 
//          a value in the range 0 to 1
//
//-----------------------------------------------------------------------------
class AbstCharacter;

class Feature
{
public:

	//returns a value between 0 and 1 based on the bot's health. The better
	//the health, the higher the rating
	static double healthProp(AbstCharacter* const bot);

};



#endif