#include "ofMain.h"

#ifdef TARGET_OSX
	#include "ofxSyphon.h"
	#include "ofxHapPlayer.h"
#endif
#ifdef TARGET_WIN32
	#include "ofDirectShowPlayer.h"
#endif

#include "render.h"

class Input {
	
public:
    Input();
    void setup();
    void stop();
    void close();
  
    void keyPressed(int key);
    
    void loadXML(ofXml &xml);
    void saveXML(ofXml &xml);
    
    void update();
   
    void bind();
    void unbind();

    int source;
	int maxSource;
    string file;
    
    ofTexture texture;
    
    int frameRate;
    
    ofImage image;
    ofVideoPlayer video;
    ofVideoGrabber capture;
    
    int resolution;

    #ifdef TARGET_OSX
        ofxHapPlayer hap;
        ofxSyphonClient syphon;
    #endif
};

