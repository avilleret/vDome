#include "plane.h"
namespace vd {

extern float projCount;
extern float projWidth;
extern float projHeight;

/******************************************

 CONSTRUCTOR

 ********************************************/

Plane::Plane(){
    cornerpinValues.push_back(ofPoint(0,0));
    cornerpinValues.push_back(ofPoint(1,0));
    cornerpinValues.push_back(ofPoint(0,1));
    cornerpinValues.push_back(ofPoint(1,1));
    cornerpinPoints.push_back(ofPoint(0,0));
    cornerpinPoints.push_back(ofPoint(1,0));
    cornerpinPoints.push_back(ofPoint(0,1));
    cornerpinPoints.push_back(ofPoint(1,1));

    group = false;
    drawBox = false;
    xRes = 10;
    yRes = 10;
    pointIndex = -1;
	value = 1;
    width = projWidth;
    height = projHeight;
    
}
    
/******************************************

 SETUP

 ********************************************/

void Plane::setup(int i){
    index = i;

    int w = width;
    int h = height;

    int x = index*w;
    int y = 0;

    value = 1;

    position.clear();
    position.push_back(x);
    position.push_back(y);
    
    ofPoint tl(cornerpinValues[0].x, cornerpinValues[0].y);
    ofPoint tr(cornerpinValues[1].x, cornerpinValues[1].y);
    ofPoint bl(cornerpinValues[2].x, cornerpinValues[2].y);
    ofPoint br(cornerpinValues[3].x, cornerpinValues[3].y);

    cornerpin.setAnchorSize(w/2, h/2);
    cornerpin.setSourceRect(ofRectangle(0,0,w,h));
    cornerpin.setTopLeftCornerPosition(tl);
    cornerpin.setTopRightCornerPosition(tr);
    cornerpin.setBottomLeftCornerPosition(bl);
    cornerpin.setBottomRightCornerPosition(br);
    cornerpin.setup();

    grid.setup(width, height, 5, 20);
    
    vector<ofVec3f> v = grid.getVertices();
    for (int i=0; i<v.size(); i++) {
        v[i] = cornerpin.getMatrix().preMult(v[i]);
    }
    grid.setVertices(v);
}

/******************************************

 RESET

 ********************************************/

void Plane::resetCornerpin(){
    cornerpin.reset();
}

void Plane::resetGrid(){
    grid.reset();
}

/******************************************

 DRAW

 ********************************************/

void Plane::draw(){
	if (cornerpinActive || gridActive) {
        vector<ofVec3f> c = grid.getControlPnts();
        
        ofMatrix4x4 m = cornerpin.getMatrix();

        if (bfirst) {
            lm.makeIdentityMatrix();
            bfirst = false;
        }
        else lm = lm.getInverse();
        
        for (int i=0; i<c.size(); i++) {
            c[i] = lm.preMult(c[i]);
		}
        for (int i=0; i<c.size(); i++) {
            c[i] = m.preMult(c[i]);
		}
        
        grid.setControlPnts(c);
        
        lm = m;
	}
    grid.draw();
}

void Plane::drawConfig(){    
    if (gridActive) {
        ofPushMatrix();
        ofTranslate(position[0], position[1]);
            grid.drawControls();
        ofPopMatrix();
    }
}

/******************************************

 KEYBOARD

 ********************************************/

void Plane::keyPressed(int key){
    if (gridActive)
        grid.keyPressed(key);
}

void Plane::keyReleased(int key){
    if (gridActive)
        grid.keyReleased(key);
}

/******************************************

 MOUSE

 ********************************************/

void Plane::onMouseDragged(ofMouseEventArgs& mouseArgs){
    mouseArgs.x -= position[0];
    mouseArgs.y -= position[1];
    if (cornerpinActive)
        cornerpin.onMouseDragged(mouseArgs);
    else if (gridActive) {
        grid.mouseDragged(mouseArgs);
    }
}

void Plane::onMousePressed(ofMouseEventArgs& mouseArgs){
    mouseArgs.x -= position[0];
    mouseArgs.y -= position[1];
    if (cornerpinActive)
        cornerpin.onMousePressed(mouseArgs);
    if (gridActive){
        grid.mousePressed(mouseArgs);
    }
}

void Plane::onMouseReleased(ofMouseEventArgs& mouseArgs){
    if (gridActive){
        mouseArgs.x -= position[0];
        mouseArgs.y -= position[1];
        grid.mouseReleased(mouseArgs);
    }
}

/******************************************

 SETTINGS

 ********************************************/

void Plane::load(ofXml &xml) {
    
    ofXml warpXML;
    warpXML.load("settings/warp/warp-"+ofToString(index+1)+".xml");

    if (warpXML.exists("cornerpin[@points]")) {
        string str = warpXML.getAttribute("cornerpin[@points]");
        cornerpinValues[0].x = ofToFloat(ofSplitString(str, ",")[0]);
        cornerpinValues[0].y = ofToFloat(ofSplitString(str, ",")[1]);
        cornerpinValues[1].x = ofToFloat(ofSplitString(str, ",")[2]);
        cornerpinValues[1].y = ofToFloat(ofSplitString(str, ",")[3]);
        cornerpinValues[2].x = ofToFloat(ofSplitString(str, ",")[4]);
        cornerpinValues[2].y = ofToFloat(ofSplitString(str, ",")[5]);
        cornerpinValues[3].x = ofToFloat(ofSplitString(str, ",")[6]);
        cornerpinValues[3].y = ofToFloat(ofSplitString(str, ",")[7]);
    }
    
    warpXML.setTo("bezier");
    string str;
    vector<ofVec3f> vec;
    for (int i = 0; i<warpXML.getNumChildren(); i++) {
        if (warpXML.exists("point["+ofToString(i)+"][@xyz]")) {
            str = warpXML.getAttribute("point["+ofToString(i)+"][@xyz]");
            int x = ofToInt(ofSplitString(str, ",")[0]);
            int y = ofToInt(ofSplitString(str, ",")[1]);
            int z = ofToInt(ofSplitString(str, ",")[2]);
            vec.push_back(ofVec3f(x,y,z));
        }
    }
    //setup(index);
    
    
    int w = width;
    int h = height;
    
    int x = index*w;
    int y = 0;
    
    ofPoint tl(cornerpinValues[0].x*w, cornerpinValues[0].y*h);
    ofPoint tr(cornerpinValues[1].x*w, cornerpinValues[1].y*h);
    ofPoint bl(cornerpinValues[2].x*w, cornerpinValues[2].y*h);
    ofPoint br(cornerpinValues[3].x*w, cornerpinValues[3].y*h);
    
    cornerpin.setAnchorSize(w/2, h/2);
    cornerpin.setSourceRect(ofRectangle(0,0,w,h));
    cornerpin.setTopLeftCornerPosition(tl);
    cornerpin.setTopRightCornerPosition(tr);
    cornerpin.setBottomLeftCornerPosition(bl);
    cornerpin.setBottomRightCornerPosition(br);
    
    gridActive = true;
    draw();
    gridActive = false;

    if (vec.size() > 0) {
        grid.setControlPnts(vec);
    }
}

void Plane::save(ofXml &xml) {
    int w = projWidth;
    int h = projHeight;
    int x = index*w;
    int y = 0;

    ofXml warpXML;
    warpXML.addChild("projector");
    warpXML.setTo("projector");
    
    
    warpXML.addChild("cornerpin");
    warpXML.setTo("cornerpin");
    
    warpXML.setAttribute("points",
                         ofToString((cornerpin.dstPoints[0].x)/w) +  "," +
                         ofToString((cornerpin.dstPoints[0].y)/h) +  "," +
                         ofToString((cornerpin.dstPoints[1].x)/w) +  "," +
                         ofToString((cornerpin.dstPoints[1].y)/h) +  "," +
                         ofToString((cornerpin.dstPoints[3].x)/w) +  "," +
                         ofToString((cornerpin.dstPoints[3].y)/h) +  "," +
                         ofToString((cornerpin.dstPoints[2].x)/w) +  "," +
                         ofToString((cornerpin.dstPoints[2].y)/h) );
    
    
    warpXML.setToParent();
    warpXML.addChild("bezier");
    warpXML.setTo("bezier");
    
    vector<ofVec3f> vec = grid.getControlPnts();
    for (int i = 0; i<vec.size(); i++) {
        warpXML.addChild("point");
        warpXML.setToChild(i);
        warpXML.setAttribute("xyz", ofToString(vec[i].x) + "," + ofToString(vec[i].y) + "," + ofToString(vec[i].z));
        warpXML.setToParent();
    }

    
    warpXML.save("settings/warp/warp-"+ofToString(index+1)+".xml");
}

vector<ofPoint> Plane::getCornerpinPoints() {
    for (int i=0; i<4; i++) {
        cornerpinPoints[i] = cornerpin.dstPoints[i];
    }
    return cornerpinPoints;
}
void Plane::setCornerpinPoints(vector<ofPoint> pts){
    for (int i=0; i<4; i++) {
        cornerpinPoints[i] = pts[i];
    }
    cornerpin.setTopLeftCornerPosition(cornerpinPoints[0]);
    cornerpin.setTopRightCornerPosition(cornerpinPoints[1]);
    cornerpin.setBottomLeftCornerPosition(cornerpinPoints[3]);
    cornerpin.setBottomRightCornerPosition(cornerpinPoints[2]);
}

vector<ofVec3f> Plane::getGridPoints() {
    return grid.getVertices();
}
void Plane::setGridPoints(vector<ofVec3f> v) {
    grid.setVertices(v);
}

}
