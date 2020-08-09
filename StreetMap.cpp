//
//  StreetMap.cpp
//  GooberEats
//
//  Created by Sarah Liang on 3/5/20.
//  Copyright © 2020 Sarah Liang. All rights reserved.
//

#include "StreetMap.h"
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <functional>
using namespace std;

//hash functions for single or pair of geocoords
unsigned int hasher(const GeoCoord& g) {
    return hash<string>()(g.latitudeText + g.longitudeText);
}

unsigned int hasher(const pair<GeoCoord, GeoCoord> & g) {
    return hash<string>()(g.first.latitudeText + g.first.longitudeText);
}

// geocoord g has a new street seg associated with it (allSegs has that one seg), so we must add
// its previous associations to allSegs and associate that vector with g in the last line
void StreetMap::associateWithPrevious(const GeoCoord& g, std::vector<StreetSegment> *previousSegs, std::vector<StreetSegment> &allSegs) {
    if(previousSegs != nullptr){
        //loop through segments already associated with g
        for(int j =0; j<previousSegs->size(); j++){
            allSegs.push_back((*previousSegs)[j]);
        }
    }
    myHashMap.associate(g,allSegs);
}

// load all of the data from an Open Maps data file and map each GeoCoord to all of the street segment(s) that start with that GeoCoord so that all segments associated with a geo-coordinate can be quickly located
// must map the forward and backward direction of each street segment
bool StreetMap:: load(std::string mapData) {
    // create ifstream to read map data file
    ifstream infile(mapData);
    // if unable to access, return false
    if(!infile) {
        return false;
    }
    // declare while loop variables
    std::string street;
    std::string latitude, longitude;
    std::vector<StreetSegment> allSegs;
    std::vector<StreetSegment> *previousSegs;
    StreetSegment seg;
    int numSegs;
    getline(infile, street);
    // while there is another street, load it's data
    while(!street.empty()) {
        // get size of how many mappings -1
        infile>>numSegs;
        for(int i =0; i<numSegs; i++){
            // take in the first and second geocoords that make up a street segment
            infile>>latitude>>longitude;
            GeoCoord firstGeoCoord(latitude,longitude);
            infile>>latitude>>longitude;
            GeoCoord secondGeoCoord(latitude,longitude);
            
            // create 2 street segments using first and the reverse
            StreetSegment myStreetSeg(firstGeoCoord, secondGeoCoord, street);
            StreetSegment myStreetSegFlipped(secondGeoCoord, firstGeoCoord, street);
            
            // associate the forward street seg with the first geocoord
            allSegs.push_back(myStreetSeg);
            previousSegs  = myHashMap.find(firstGeoCoord);
            associateWithPrevious(firstGeoCoord, previousSegs, allSegs);
            allSegs.clear();
            
            // associate the reverse seg with the second geocoord
            allSegs.push_back(myStreetSegFlipped);
            previousSegs  = myHashMap.find(secondGeoCoord);
            associateWithPrevious(secondGeoCoord, previousSegs, allSegs);
            allSegs.clear();
        }
        // retrieve the next street name
        getline(infile, street);
        getline(infile, street);
    }
    return true;
}

bool StreetMap:: getSegmentsThatStartWith(const GeoCoord& geoCoord, std::vector<StreetSegment>& streetSegs) const {
    // get a pointer to the vector of street segments associated with the geocoord
    const std::vector<StreetSegment> *gcStreetSegs = myHashMap.find(geoCoord);
    // iterate through to recreate this vector in parameter streetSegs
    if(gcStreetSegs != nullptr){
        streetSegs.clear();
        auto it = gcStreetSegs->begin();
        while(it != gcStreetSegs->end()){
            StreetSegment newSS = *it;
            streetSegs.push_back(newSS);
            it++;
        }
        return true;
    }
    //if no vector associated with geocoord, return false
    return false;
}
