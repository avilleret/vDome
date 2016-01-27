#pragma once
#include "ofMain.h"
#include "plane.h"
#include "mask.h"
#include "curves.h"

namespace vd {
class Command;
class Projector {

public:
    void    init(int i, int pStartingIndex);
    void    setup();
    void    begin();
    void    end();
    void    bind();
    void    unbind();
    void    draw();
   
    void    drawPlaneConfig();
    void    drawKeystone();
	void    drawCurves(int x, int y);
    
    void    mousePressed(ofMouseEventArgs& mouseArgs);
    void    mouseDragged(ofMouseEventArgs& mouseArgs);
    void    mouseReleased(ofMouseEventArgs& mouseArgs);
    
    void    keyPressed(int key);
    void    keyReleased(int key);
    
    void    loadXML(ofXml &xml);
    void    loadXML2(ofXml &xml);
    void    saveXML(ofXml &xml);

	Command*    execute(float v);
	Command*    executeBrush();
	Command*    reset();
    
    // plane
    ofVec2f getPlanePosition();
    void    setPlanePosition(float x, float y);
    
    ofVec2f getPlaneDimensions();
    void    setPlaneDimensions(float w, float h);
    
    // keystone
    bool getKeystoneActive();
    void setKeystoneActive(bool v);
    
    vector<ofPoint> getKeystonePoints();
    void            setKeystonePoints(vector<ofPoint> pts);
    
    // grid
    bool getGridActive();
    void setGridActive(bool v);
    
    vector<ofVec3f> getGridPoints();
    void            setGridPoints(vector<ofVec3f> v);
    
    // camera
    void    setCameraTransform();
    
    ofVec3f getCameraPosition();
    void    setCameraPosition(float azi, float ele, float dis);
    
    ofVec3f getCameraOrientation();
    void    setCameraOrientation(float roll, float tilt, float pan);
    
    float   getCameraFov();
    void    setCameraFov(float v);
    
    ofVec2f getCameraOffset();
    void    setCameraOffset(float x, float y);
    
    ofVec2f getCameraScale();
    void    setCameraScale(float x, float y);
    
    vector<float>   getCameraShear();
    void            setCameraShear(vector<float>);
    ofTexture&      getTextureReference();

    int     index;
    bool    keyboard;
    bool    mouse;
    bool    active;
    bool    enable;
    int     editMode;
    bool    mod;
    bool    all;
    void    setValue(float v);
    Mask    mask;
    ofFbo   renderFbo;
    
    ofVboMesh renderPlane;

    // intensity
    float   brightness;
    float   contrast;

    // color
    float   hue;
    float   saturation;
    float   lightness;

    Plane   plane;
    
    vector<ofPoint> lastKey;
    vector<ofVec3f> lastGrid;
    
    int     width, height;

	// color curves
	Curves  curves;
    
    int     projectorStartingIndex;
    
    enum    editModes{
                BRIGHTNESS,CONTRAST,SATURATION,
                CURVES,
                CORNERPIN, GRID,
                AZIMUTH, ELEVATION, DISTANCE,
                ROLL, TILT, PAN,
                FOV, OFFSET_X, OFFSET_Y,
                NONE, BRUSH_SCALE, BRUSH_OPACITY, WHITE, BLACK, ENABLE};

private:
    ofVec3f     sphToCar(ofVec3f t);
    float       round(float d);
    float       roundTo(float val, float n);
    
    ofCamera    camera;
    ofFbo       fbo;
    ofRectangle view;

    float       value;
    int         fboSample;
    string      xmlPrefix;

    // plane
    ofVec2f     planePosition;

    // camera
    ofVec3f         cameraPosition;
    ofVec3f         cameraOrientation;
    float           cameraFov;
    ofVec2f         cameraOffset;
    ofVec2f         cameraScale;
    vector<float>   cameraShear;

    ofMatrix4x4     transform;
};



}
