#include "model.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "commands.h"
namespace vd {

//--------------------------------------------------------------
extern int maxHistory;
extern CommandHistory history;

Model::Model(){
	radius = 1;
	N = 128;  // Mesh resolution, must be multiple of 4
    textureScale = 1;
    textureFlip = false;
    textureRotate = 0;
    textureTilt = 0;
	textureFlipInternal = false;
    textureScaleInternal = 1;
    textureTiltInternal = 0;
    textureScaleInternalW = 1;
    textureScaleInternalH = 1;
}

//--------------------------------------------------------------
void Model::setup(){
 	int i,j,index = 0;
	int i1,i2,i3,i4;
	double theta,phi,r;
	double len;
	double x;
	double y;
	double z;
	double nx;
	double ny;
	double nz;
	double u;
	double v;
    vbo.clear();
	vbo.enableTextures();
	vbo.enableNormals();

	vbo.addVertex(ofVec3f(0,0,0));
	vbo.addNormal(ofVec3f(0,0,0));
	vbo.addTexCoord(ofVec2f(0,0));

	for (j=0;j<=N/4;j++) {
		for (i=0;i<=N;i++) {
			theta = i * 2 * PI / N;
			phi = PI/2  - PI/2 * j / (N/4);
			x = radius * cos(phi) * cos(theta);
			y = radius * cos(phi) * sin(theta);
			z = radius * sin(phi);
			vbo.addVertex(ofVec3f(x,y,-1*z));

			len = sqrt(x*x + y*y + z*z);
			nx = x/len;
   			ny = y/len;
   			nz = z/len;
			vbo.addNormal(ofVec3f(nx,ny,nz));

			phi = atan2(sqrt(x*x+y*y),z); // 0 ... pi/2
			theta = atan2(y,x); // -pi ... pi
			r = phi / PI/2 * 4 / (textureScale*textureScaleInternal*textureScaleInternalW*textureScaleInternalH); // 0 ... 1 --->
			u = 0.5 * (r*textureScaleInternalH * cos(theta) + 1);
			v = 0.5 * (r*textureScaleInternalW * sin(theta) + 1);
            
			if (textureFlipInternal){
				if (textureFlip)
	                vbo.addTexCoord(ofVec2f(u,1-v));
				else 
					vbo.addTexCoord(ofVec2f(u,v));
			}
			else {
				if (textureFlip)
					vbo.addTexCoord(ofVec2f(u,v));
				else
					vbo.addTexCoord(ofVec2f(u,1-v)); // reverse
			}
			index++;
		}
    }

	for (j=0;j<N/4;j++) {
		for (i=0;i<N;i++) {
			i1 =  j    * (N+1) + i       + 1;
			i2 =  j    * (N+1) + (i + 1) + 1;
			i3 = (j+1) * (N+1) + (i + 1) + 1;
			i4 = (j+1) * (N+1) + i       + 1;
			vbo.addTriangle(i1, i2, i3);
			vbo.addTriangle(i1, i2, i3);
			vbo.addTriangle(i1, i2, i3);
			vbo.addTriangle(i1, i3, i4);
			vbo.addTriangle(i1, i3, i4);
			vbo.addTriangle(i1, i3, i4);
		}
	}
}

//--------------------------------------------------------------
void Model::draw(){
    ofPushMatrix();
	ofRotateX(textureTilt+textureTiltInternal);
    ofRotateZ(textureRotate*-1);
    glEnable(GL_CULL_FACE);
    glCullFace( GL_BACK );
	vbo.draw();
    glDisable(GL_CULL_FACE);
    ofPopMatrix();
}

/******************************************
 LOAD MODEL XML
 ********************************************/
void Model::loadXML(ofXml &xml) {
    if (xml.exists("input[@scale]"))
        textureScale = ofToFloat( xml.getAttribute("input[@scale]") );
    if (xml.exists("input[@rotate]"))
        textureRotate = ofToFloat( xml.getAttribute("input[@rotate]") );
    if (xml.exists("input[@tilt]"))
        textureTilt = ofToFloat( xml.getAttribute("input[@tilt]") );
	string v;
	if (xml.exists("input[@flip]")) {
        v = xml.getAttribute("input[@flip]");
		if (v == "on")  textureFlip = true;
        else            textureFlip = false;
    }

    setup();
}
    
//--------------------------------------------------------------
void Model::saveXML(ofXml &xml) {
    xml.setTo("input");
    xml.setAttribute("scale", ofToString(textureScale));
	xml.setAttribute("rotate", ofToString(textureRotate));
    xml.setAttribute("tilt", ofToString(textureTilt));
	if (textureFlip) xml.setAttribute("flip", "on");
    else			 xml.setAttribute("flip", "off");
    xml.setToParent();
}

//--------------------------------------------------------------
void Model::saveMesh(string file) {
    vbo.save(file);
}

//--------------------------------------------------------------
bool Model::getTextureFlip(){
	return textureFlip;
}
    
//--------------------------------------------------------------
void Model::setTextureFlip(bool b){
	textureFlip = b;
	setup();
}

//--------------------------------------------------------------
float Model::getTextureRotate(){
	return textureRotate;
}
    
//--------------------------------------------------------------
void Model::setTextureRotate(float f){
	textureRotate = f;
}

//--------------------------------------------------------------
float Model::getTextureTilt(){
	return textureTilt;
}
    
//--------------------------------------------------------------
void Model::setTextureTilt(float f){
	textureTilt = f;
}
    
//--------------------------------------------------------------
float Model::getTextureScale(){
	return textureScale;
}
    
//--------------------------------------------------------------
void Model::setTextureScale(float f){
	textureScale = f;
	setup();
}


}//////
