#ifndef ASTARNODECOMPARATOR_H
#define ASTARNODECOMPARATOR_H

#include "e2150/AStarNode.h"

class AStarNodeComparator{
	public:
		bool operator()(const AStarNode* lhs, const AStarNode* rhs) const{return lhs->getOverallCost()>rhs->getOverallCost();}
};

#endif // ASTARNODECOMPARATOR_H
