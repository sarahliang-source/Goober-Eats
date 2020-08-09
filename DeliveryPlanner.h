//
//  DeliveryPlanner.h
//  GooberEats
//
//  Created by Sarah Liang on 7/26/20.
//  Copyright © 2020 Sarah Liang. All rights reserved.
//

#ifndef DeliveryPlanner_h
#define DeliveryPlanner_h
#include "provided.h"
#include "StreetMap.h"
#include "PointToPointRouter.h"
#include "DeliveryOptimizer.h"

class DeliveryPlanner
{
    public:
        DeliveryPlanner(const StreetMap* Map): Router(Map), DeliveryOpt(Map){};
        ~DeliveryPlanner(){};
        DeliveryResult generateDeliveryPlan(const GeoCoord& depot, const vector<DeliveryRequest>& deliveries, vector<DeliveryCommand>& commands, double& totalDistanceTravelled) const;
    private:
        PointToPointRouter Router;
        DeliveryOptimizer DeliveryOpt;
        std:: string  getDirection(double angle) const;
};

#endif /* DeliveryPlanner_h */
