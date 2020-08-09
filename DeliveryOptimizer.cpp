//
//  DeliveryOptimizer.cpp
//  GooberEats
//
//  Created by Sarah Liang on 3/5/20.
//  Copyright © 2020 Sarah Liang. All rights reserved.
//

#include "DeliveryOptimizer.h"
#include <vector>
using namespace std;

void DeliveryOptimizer::optimizeDeliveryOrder(const GeoCoord &depot, vector<DeliveryRequest> &deliveries, double &oldCrowDistance, double &newCrowDistance) const {
    // calculate old crow distance with the parameter values
    oldCrowDistance = getCrowDistance(depot, deliveries);
    // declare vector to store reordered requests in optimal order
    vector<DeliveryRequest> allRequests;
    const GeoCoord* start = &depot;
    int numDeliveries = deliveries.size();
    // create a delivery request of the next closest request and add it to allRequests
    for(int i =0; i<numDeliveries; i++){
        DeliveryRequest closestRequest = findClosestRequest(*start, deliveries);
        start = &closestRequest.location;
        allRequests.push_back(closestRequest);
    }
    numDeliveries = allRequests.size();
    // set deliveries to hold the optimized order of requests (allRequests)
    for(int i =0; i<numDeliveries; i++){
        deliveries.push_back(allRequests[i]);
    }
    // calculate old crow distance with the newly reordered deliveries
    newCrowDistance = getCrowDistance(depot, deliveries);
}

// returns total crow distance from depot to each delivery location
double DeliveryOptimizer::getCrowDistance(const GeoCoord &depot, vector<DeliveryRequest> &deliveries) const {
    double crowDistance = 0;
    const GeoCoord* previousLocation = &depot;
    // for each delivery, add the distance from the current location to the next delivery to crowDistance
    for(int i  =0; i<deliveries.size(); i++){
        GeoCoord* deliveryLocation = &deliveries[i].location;
        crowDistance += distanceEarthMiles(*previousLocation, *deliveryLocation);
        previousLocation = deliveryLocation;
    }
    // add distance it takes to return to the depot
    crowDistance += distanceEarthMiles(*previousLocation, depot);
    return crowDistance;
}

// to find where to deliver next, choose the closest location from your current one
DeliveryRequest DeliveryOptimizer:: findClosestRequest(const GeoCoord &start, vector<DeliveryRequest> &deliveries) const {
    double minDistance = distanceEarthMiles(start, deliveries[0].location);
    DeliveryRequest* closestRequest = &deliveries[0];
    int minIndex = 0;
    double currentDistance;
    //loop through to find closest delivery
    for(int i=1; i<deliveries.size(); i++) {
        currentDistance = distanceEarthMiles(start, deliveries[i].location);
        if(currentDistance < minDistance) {
            minDistance = currentDistance;
            closestRequest = &deliveries[i];
            minIndex = i;
        }
    }
    // make a delivery request of closest one, erase it from deliveries and return the copy
    DeliveryRequest request(closestRequest->item, closestRequest->location);
    deliveries.erase(deliveries.begin() + minIndex);
    return request;
}
