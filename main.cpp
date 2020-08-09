//
//  main.cpp
//  GooberEats
//
//  Created by Sarah Liang on 3/5/20.
//  Copyright © 2020 Sarah Liang. All rights reserved.
//

#include "provided.h"
#include "ExpandableHashMap.h"
#include "StreetMap.h"
#include "PointToPointRouter.h"
#include "DeliveryPlanner.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cassert>
#include <unistd.h>
#include <stdio.h>
#include <limits.h>
using namespace std;

int main() {
    // declare all necessary variables
    StreetMap myMap;
    DeliveryPlanner currentPlanner(&myMap);
    vector<DeliveryRequest> allDeliveries;
    vector<DeliveryCommand> allCommands;
    double totalMiles;
    // load map data and initialize all dlivery locations and requests
    assert(myMap.load("mapdata.txt"));
    GeoCoord depot("34.0625329", "-118.4470263");
    const GeoCoord ChickenTenders("34.0712323", "-118.4505969");
    const GeoCoord Salmon("34.0687443", "-118.4449195");
    const GeoCoord Beer("34.0685657", "-118.4489289");
    DeliveryRequest d1("Chicken tenders", ChickenTenders);
    DeliveryRequest d2("Blue Ribbon Beer", Beer);
    DeliveryRequest d3("B-plate Salmon", Salmon);
    // add each delivery request to allDeliveries
    allDeliveries.push_back(d1);
    allDeliveries.push_back(d2);
    allDeliveries.push_back(d3);
    // create delivery plan
    DeliveryResult result = currentPlanner.generateDeliveryPlan(depot, allDeliveries, allCommands, totalMiles);
    if (result == BAD_COORD) {
        cout << "One or more depot or delivery coordinates are invalid." << endl;
    } else if (result == NO_ROUTE) {
        cout << "No route can be found to deliver all items." << endl;
    } else { // successfully generated delivery plan, print all commands and total miles travelled
        cout << "Starting at the depot...\n";
        for (const auto& command : allCommands)
            cout << command.description() << endl;
        cout << "You are back at the depot and your deliveries are done!\n";
        cout.setf(ios::fixed);
        cout.precision(2);
        cout << totalMiles << " miles travelled for all deliveries." << endl;
    }
}
