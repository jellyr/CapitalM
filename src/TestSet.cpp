//
//  TestSet.cpp
//  BabyM
//
//  Created by Kris Meeusen on 31/05/16.
//
//

#include "TestSet.hpp"
#include "cinder/Rand.h"
#include "cinder/Perlin.h"
#include "cinder/app/App.h"
#include "cinder/gl/gl.h"

using namespace std;
using namespace ci;
using namespace ci::app;



void TestSet::setup(){
    
    float f = 1;

    float const offset = 324;
    for(int i=0; i < 3; ++i){
        emmitters.push_back(Emitter(vec2(38,offset + 260 * i),f));
        emmitters.push_back(Emitter(vec2(451,offset + 260 * i),f));
        emmitters.push_back(Emitter(vec2(848,offset + 260 * i),f));

    }

    
    Dot d19;
    d19.setup(vec2(304, 500), vec2(354, 583), ci::Color(1.0, .8, 0.9));
    dots.push_back(d19);


    Dot d20;
    d20.setup(vec2(384, 200), vec2(384, 617), ci::Color(1.0, .8, 0.9));
    dots.push_back(d20);

    
    Dot d21;
    d21.setup(vec2(324, 600), vec2(373, 748), ci::Color(.4, .1, 0.9));
    dots.push_back(d21);

    // left under
    Dot d22;
    d22.setup(vec2(124, 869), vec2(354, 869), ci::Color(.8, .8, 0.1));
    dots.push_back(d22);

    
    // left under
    Dot d4;
    d4.setup(vec2(184, 704), vec2(206, 868), ci::Color(1.0, 0.2, 0));
    dots.push_back(d4);

    // top left
    Dot d1;
    d1.setup(vec2(304,704), vec2(208,238), ci::Color(0, 1, 1));
    dots.push_back(d1);
    
    // middle
    Dot d3;
    d3.setup(vec2(420,704), vec2(446, 478), ci::Color(0.5, 1, 0));
    dots.push_back(d3);
    
    //top right
	Dot d2;
	d2.setup(vec2(537, 704), vec2(686, 240), ci::Color(0, 0, 1));
	dots.push_back(d2);
//
    // bottom right
	Dot d5;
	d5.setup(vec2(520, 704), vec2(691, 872), ci::Color(1.0, 0.5, 0.3));
	dots.push_back(d5);

    
    

}


void TestSet::setNewData(EmitterData e){
    emmitterData = e;
    

    
}


vec2 vLerp(vec2 p1,vec2 p2,float v){
    
    vec2 r;
    r.x = lerp(p1.x,p2.x,v);
    r.y = lerp(p1.y,p2.y,v);
    
    return r;
    
}



void TestSet::start(){
    
    isRunning = true;
    isHitTarget = false;
    lifeTime = 0;
	recordDistance = 10000;
    for (auto& d : dots){
        d.resetForces();
        d.resetPosition();
    }
    
    for(auto&e : emmitters){
        e.reset();
    }
         
         

}



void TestSet::stop(){
    isRunning = false;
    calcuclateFitnessScore();
}




void TestSet::update(vec2& gravity){
    
    
    if(!isRunning) return;
    
    ++lifeTime;
    
    
    isHitTarget = true;
	float combinedDistance = 0;

    
    for(int i = 0; i < emmitters.size();++i){
        
        Emitter* e = &emmitters[i];
        int dataIndex = getElapsedFrames() % emmitterData.data.size();
        e->mTargetForce = emmitterData.data[dataIndex][i];
        e->update();
    }
    

    for(auto& d : dots){
        checkBounderies(d);
        applyForces(d,0);


		float distance = glm::distance(d.mPosition, d.mTargetPosition);
		combinedDistance += distance;


        isHitTarget *= distance < 10;
        limitSpeed(d);
        
        d.mVelocity = vLerp(d.mVelocity, vec2(0,0), 0.09);
        d.mDirection = vLerp(d.mDirection, gravity, 0.1);

        d.update();
    }
    
	if (combinedDistance < recordDistance) recordDistance = combinedDistance;

    if(isHitTarget) stop();
    
}



void TestSet::limitSpeed(Dot& dot){
    if(length(dot.mVelocity) > 20){
        vec2 n = normalize(dot.mVelocity);
        dot.mVelocity = n * 20.0f;
    }
}


bool TestSet::checkTarget(Dot& dot){

	/*  float distance = glm::distance(dot.mPosition, dot.mTargetPosition);

	  if(distance < dot.recordDistance){
	  dot.recordDistance = distance;
	  }


	  return (distance < 20);    */
	return 0;
}


void TestSet::checkBounderies(Dot& dot){
    
    // hit the floor
    if(dot.mPosition.y > GS()->mScreen.getHeight() || dot.mPosition.y < 0){
        dot.mDirection.y = -(dot.mDirection.y);
        dot.mVelocity.y = -(dot.mVelocity.y);
    }
    
    
    // moved outside on the left or right
    if(dot.mPosition.x > GS()->mScreen.getWidth() || dot.mPosition.x < 0){
        // stop();
        dot.mDirection.x = -dot.mDirection.x;
        dot.mVelocity.x = -dot.mVelocity.x;
    }
}




void TestSet::applyForces(Dot& dot,int dataIndex){
    
    for(int i = 0; i < emmitters.size();++i){
        
        Emitter* e = &emmitters[i];
       // e->mForce = emmitterData.data[dataIndex][i];
        
        vec2 distance(e->mPosition - dot.mPosition);
        float length  = glm::length(distance);
        
        
        float forceFactor = lmap<float>(length, 0, e->mForce, 2.0, 1);
        forceFactor = fmax(forceFactor,0);
        
        forceFactor = pow(forceFactor, 2);
        //  forceFactor *= 1.6;
        // cout << forceFactor << endl;
        
        dot.mDirection -= normalize(distance) * forceFactor;
    }

}



void TestSet::draw(int textOffset,bool background){

    
    //lmap<float>(recordDistance, 400, 0, 0.1, 3);
	
	//if (lineWidth < 0) lineWidth = 0.1;


	//float alpha = lmap<float>(recordDistance, 2000, 0, 0.1, 1);
	//gl::lineWidth(14);
    
    if(GS()->isBackgroundDrawingOff && background) return;

    if(!background ){

        for(auto& e : emmitters){
            gl::color(1, 1, 1, 0.2);

            gl::drawSolidCircle(e.mPosition, fmax(e.mForce,4));
            
            gl::color(1, 1, 1, 0.6);
            gl::drawStrokedCircle(e.mPosition, fmax(e.mForce,4),8,28);
        }
    }
    
    for(int i = 0; i < dots.size(); ++i){
        
        if(i > 0 ){
            gl::color(1, 1, 1, background ? 0.4 : 1);
            gl::drawLine(dots[i-1].mPosition, dots[i].mPosition);
        }
        
		dots[i].draw(!background);
    }


    
	if (recordDistance< 250) gl::drawString("l:" + to_string((int) recordDistance), vec2(800, 10 + textOffset));
    
    int f = fitness*10000000000000;
    if(f>0) gl::drawString(to_string(f), vec2(900,10 + textOffset));
    
    
}




void TestSet::randomize(int frames){
    
    
    ci::Perlin perlin = Perlin( 4,  clock() & 65535 );
;
    int const eSize = emmitters.size();// emmitters.size();
    
    for (float i=0; i< frames; ++i) {
        
        vector<float> emitterForces;
        emitterForces.reserve(eSize);
        
        for(float j=0; j< eSize;++j){
            float n = fabs(perlin.noise(i * 0.01f, j ));
            
            emitterForces.push_back(fabs(n) * 320.0f);
        }
        
        emmitterData.data.push_back(emitterForces);
    }
}




long double TestSet::calcuclateFitnessScore(){
    fitness= 0;
   // for(auto&d :dots){
    //    if (d.recordDistance < 1) d.recordDistance = 1;
        
        // Reward finishing faster and getting close
		//lifeTime *= 0.1;
        fitness = (1.0f/(recordDistance*lifeTime));
        
        
        //if (hitObstacle) fitness *= 0.1; // lose 90% of fitness hitting an obstacle
  //  }

    // Make the function exponential
    fitness = pow(fitness, 4);

	//fitness *= 0.01;

    if (isHitTarget) fitness *= 2.0; // twice the fitness for finishing!

    return fitness;
}


void TestSet::readData(std::string fileName){
    ifstream dataFile;
    dataFile.open (getAssetPath("").string() + "/" + fileName);
    
    string line;
    int const eSize = emmitters.size();// emmitters.size();

    if (dataFile.is_open())
    {
        while ( getline (dataFile,line) )
        {
            
            vector<float> emitterForces;
            emitterForces.reserve(eSize);
            
            vector<string> splitEmitters = split(line, ';');
            for(auto& s : splitEmitters){
                if(s!= "") emitterForces.push_back(stof(s));
            }

            emmitterData.data.push_back(emitterForces);


        }
        dataFile.close();
    }
    
    


    
}


void TestSet::dumpData(std::string fileName){
    ofstream dataFile;
    dataFile.open (getAssetPath("").string() + "/" + fileName);

    for(auto& timeFrame : emmitterData.data){
        string row;
        for(float emmiterForce : timeFrame){
            row += toString(emmiterForce) + ";";
        }
        
        dataFile << row << "\n";

    }
    
    dataFile.close();
}


