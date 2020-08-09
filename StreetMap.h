//
//  StreetMap.h
//  GooberEats
//
//  Created by Sarah Liang on 7/26/20.
//  Copyright © 2020 Sarah Liang. All rights reserved.
//

#ifndef StreetMap_h
#define StreetMap_h
#include "ExpandableHashMap.h"
#include "provided.h"

unsigned int hasher(const GeoCoord& g);
unsigned int hasher(const pair<GeoCoord, GeoCoord> & g);

class StreetMap
{
    public:
        StreetMap(){};
        ~StreetMap(){};
        bool load(std::string mapFile);
        bool getSegmentsThatStartWith(const GeoCoord& geoCoord, std::vector<StreetSegment>& streetSegs) const;
    
    private:
        ExpandableHashMap<GeoCoord, std::vector<StreetSegment>> myHashMap;
        void associateWithPrevious(const GeoCoord& g, std::vector<StreetSegment> *previousSegs, std::vector<StreetSegment> &allSegs);
};


#endif /* StreetMap_h */
