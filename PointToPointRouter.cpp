//
//  PointToPointRouter.cpp
//  GooberEats
//
//  Created by Sarah Liang on 3/5/20.
//  Copyright © 2020 Sarah Liang. All rights reserved.
//

#include "PointToPointRouter.h"
#include <deque>
#include <vector>
#include <queue>
#include <set>
#include <list>
#include <map>

// constructor initializes myStreetMap pointer
PointToPointRouter:: PointToPointRouter(const StreetMap* sm) {
    myStreetMap = sm;
}

//orders the delivery by choosing which of of the remaining delivery locations is closest to the depot
void PointToPointRouter:: order(std::vector<StreetSegment>& toAdd, const GeoCoord& depot) const {
    // use a map where the key is a double for the distance from geocoord to depot, and the value is the street segment
    map<double,StreetSegment> orderedSegs;
    int numOrders = toAdd.size();
    double distance;
    for(int i=0; i<numOrders; i++) {
        distance = distanceEarthKM(toAdd[i].end, depot);
        //inserting them in map automatically puts them in ascending order
        orderedSegs.insert(std::pair<double,StreetSegment>(distance,toAdd[i]));
    }
    //clear toAdd vector
    toAdd.clear();
    //iterate through map to add to vector
    map<double,StreetSegment>:: iterator it = orderedSegs.begin();
    while(it!=orderedSegs.end()) {
        StreetSegment temp2= it->second;
        toAdd.push_back(temp2);
        it++;
    }
}

void PointToPointRouter:: explore(queue<GeoCoord>& toDo, const GeoCoord &mainGeoCoord, std::vector<StreetSegment> toAdd2, ExpandableHashMap<GeoCoord,GeoCoord> &locPWP, ExpandableHashMap<std::pair<GeoCoord,GeoCoord>,std::string> &streetnames,ExpandableHashMap<GeoCoord,bool> &visited, const GeoCoord& end) const {
    //order toAdd2 so that the closest delivery is always next
    order(toAdd2, end);
    for(int i=0; i<toAdd2.size(); i++) {
        //if segment's ending geocoord hasn't been explored yet, proceed
        if(!visited.find(toAdd2[i].end)) {
            //all start at same coord (toAdd1) and end at different ones, store all end coords in toDo queue
            toDo.push(toAdd2[i].end);
            //for retrieving path at the end, associate the segment end geocoord to the first
            locPWP.associate(toAdd2[i].end, mainGeoCoord);
            //to retrieve street name of this segment later, store name in hashmap
            streetnames.associate(make_pair(mainGeoCoord, toAdd2[i].end),toAdd2[i].name);
            // add ending geocoord to visited hashmap
            visited.associate(toAdd2[i].end, false);
        }
    }
}

DeliveryResult PointToPointRouter:: generatePointToPointRoute(const GeoCoord& start, const GeoCoord& end, std::list<StreetSegment>& route, double& totalDistanceTravelled) const {
    //if starting and ending geocoords are the same, you are done
    if(start==end) {
        route.clear();
        totalDistanceTravelled=0;
        return DELIVERY_SUCCESS;
    }
    //create three hashmaps, first for retracing steps, second for finding streetnames, and third for checking if a geocoord has been explored
    ExpandableHashMap<GeoCoord,GeoCoord> locationOfPreviousWayPoint;
    ExpandableHashMap<pair<GeoCoord,GeoCoord>,std::string> streetNames;
    ExpandableHashMap<GeoCoord,bool> visitCheckMap;
    // vector of street segments that are associated with current geocoord being explored. Used for getSegmentsThatStartWith method
    std::vector<StreetSegment> streetSegs;
    //if there are no segments associated with starting or ending geocoord, do nothing and return BAD_COORD
    if (!myStreetMap->getSegmentsThatStartWith(start, streetSegs) || !myStreetMap->getSegmentsThatStartWith(end, streetSegs)) {
        return BAD_COORD;
    }
    //create queue to store geocoords for possible pathways to the destination
    queue<GeoCoord> possiblePathways;
    //start exploring from start geocoord
    myStreetMap->getSegmentsThatStartWith(start, streetSegs);
    explore(possiblePathways, start, streetSegs, locationOfPreviousWayPoint, streetNames, visitCheckMap, end);

    while (!possiblePathways.empty()) {
        //explore front element of queue, pop it off
        const GeoCoord curr = possiblePathways.front();
        possiblePathways.pop();
        //if it equals the end, you're done! retrieve path and return DELIVERY_SUCCESSS
        if (curr == end) {
            retrievePath(route, start, end, totalDistanceTravelled, locationOfPreviousWayPoint, streetNames, visitCheckMap);
            return DELIVERY_SUCCESS;
        }
        //or else keep exploring the next street segments
        myStreetMap->getSegmentsThatStartWith(curr, streetSegs);
        explore(possiblePathways, curr, streetSegs,locationOfPreviousWayPoint, streetNames, visitCheckMap,end);
    }
    //if the queue becomes empty, there is no path
    return NO_ROUTE;
}

bool PointToPointRouter:: retrievePath(std::list<StreetSegment> &fullPath, const GeoCoord& start, const GeoCoord& end,double& totalDistanceTravelled,ExpandableHashMap<GeoCoord,GeoCoord> &locPWP, ExpandableHashMap<pair<GeoCoord,GeoCoord>,std::string> &streetnames,ExpandableHashMap<GeoCoord,bool> &visited) const {
    //clear path, set distance to zero
    fullPath.clear();
    totalDistanceTravelled=0;
    // tracing the path from end to start, traceBack1 will always be ahead of traceBack2 (as in one geocoord closer to the start)
    const GeoCoord* traceBack2 = &end;
    const GeoCoord* traceBack1 = locPWP.find(end);
    int segsRemaining = locPWP.size();
    std::string streetName;
    const std::string *temp;
    while(segsRemaining>0) {
        // recreate the street segment of traceBack1 and 2 to find their name in streetnames hashmap
        GeoCoord enRoute1(traceBack1->latitudeText, traceBack1->longitudeText);
        GeoCoord enRoute2(traceBack2->latitudeText, traceBack2->longitudeText);
        temp = streetnames.find(make_pair(enRoute1,enRoute2));
        streetName = *temp;
        //create final route street segment with corresponding street name
        StreetSegment pathSeg(enRoute1, enRoute2, streetName);
        // add it's length to the total length travelled
        totalDistanceTravelled += distanceEarthMiles(enRoute1, enRoute2);
        // add the street segment to the final route
        fullPath.push_front(pathSeg);
        // move traceBack2 geocoord pointer up to the next geocoord in final path (traceBack1)
        traceBack2 = traceBack1;
        // move traceBack1 geocoord pointer up to the next geocoord in final path (locPWP hash map where we stored the path in generatePointToPointRoute)
        traceBack1 = locPWP.find(*traceBack1);
        // if traceBack2 is the start geocoord, we are done!
        if(*traceBack2 == start)
            return true;
        segsRemaining--;
    }
    return false;
}
