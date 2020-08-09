//
//  DeliveryOptimizer.h
//  GooberEats
//
//  Created by Sarah Liang on 7/26/20.
//  Copyright © 2020 Sarah Liang. All rights reserved.
//

#ifndef DeliveryOptimizer_h
#define DeliveryOptimizer_h
#include "provided.h"
#include "StreetMap.h"

class DeliveryOptimizer
{
    public:
        DeliveryOptimizer(const StreetMap* sm){};
        ~DeliveryOptimizer(){};
        void optimizeDeliveryOrder(const GeoCoord& depot, vector<DeliveryRequest>& deliveries, double& oldCrowDistance, double& newCrowDistance) const;
    private:
        double getCrowDistance(const GeoCoord& depot,vector<DeliveryRequest>& deliveries) const;
        DeliveryRequest findClosestRequest(const GeoCoord& start,vector<DeliveryRequest>& deliveries) const;
};

#endif /* DeliveryOptimizer_h */
