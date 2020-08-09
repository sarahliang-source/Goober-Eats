//
//  PointToPointRouter.h
//  GooberEats
//
//  Created by Sarah Liang on 7/26/20.
//  Copyright © 2020 Sarah Liang. All rights reserved.
//

#ifndef PointToPointRouter_h
#define PointToPointRouter_h
#include "provided.h"
#include "StreetMap.h"
#include "ExpandableHashMap.h"
#include <queue>

class PointToPointRouter
{
    public:
        PointToPointRouter(const StreetMap* sm);
        ~PointToPointRouter(){};
        DeliveryResult generatePointToPointRoute(const GeoCoord& start, const GeoCoord& end, std::list<StreetSegment>& route, double& totalDistanceTravelled) const ;
    private:
        const StreetMap* myStreetMap;
        void explore(queue<GeoCoord>& toDo, const GeoCoord &toAdd1, std::vector<StreetSegment> toAdd2, ExpandableHashMap<GeoCoord,GeoCoord> &locPWP, ExpandableHashMap<pair<GeoCoord,GeoCoord>,std::string> &streetnames,ExpandableHashMap<GeoCoord,bool> &visited,const GeoCoord& end) const;
        bool retrievePath(std::list<StreetSegment> &myList,const GeoCoord& start, const GeoCoord& end,double& totalDistanceTravelled,ExpandableHashMap<GeoCoord,GeoCoord> &locPWP, ExpandableHashMap<pair<GeoCoord,GeoCoord>,std::string> &streetnames,ExpandableHashMap<GeoCoord,bool> &visited) const;
        void order(std::vector<StreetSegment>& toAdd, const GeoCoord& depot) const;
};

#endif /* PointToPointRouter_h */
