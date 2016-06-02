//
//  TestSet.hpp
//  BabyM
//
//  Created by Kris Meeusen on 31/05/16.
//
//

#ifndef TestSet_hpp
#define TestSet_hpp

#include <stdio.h>
#include <vector>

#include "Emitter.hpp"
#include "Dot.hpp"
#include "EmmiterData.hpp"

class TestSet{

public:
    //std::vector<std::vector<float> > mData;
    EmitterData emmitterData;
    
    bool isRunning;
    bool isHitTarget;
    
    std::vector<Emitter> emmitters;
    std::vector<Dot> dots;

    void randomize(int emmitterAmount,int frames);
    void setup();
    void update(cinder::vec2& gravity,cinder::vec2& target);
    void draw(int textOffset);
    
    void setNewData(EmitterData e);
    void applyForces(Dot& d,int dataIndex);
    void checkBounderies(Dot& d);
    bool checkTarget(Dot& d);
    void limitSpeed(Dot& d);
    
    void start();
    void stop();
    
    int lifeTime;
    float fitness = 0;
   // float recordDistance = 1000;
    
    float calcuclateFitnessScore();
    
    
};

#endif /* TestSet_hpp */
