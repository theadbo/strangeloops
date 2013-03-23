//import napplet.test.*;
//import napplet.*;


/*
  fv
 Modified code originally developed by André Sier 2010
 
 -AH 01-20-11
 
 */

import processing.opengl.*;
import processing.video.*;
import s373.flob.*;
import controlP5.*;


ControlP5 controlP5;
Capture video;   
Flob flob;       
ArrayList blobs; 
PrintWriter output;
PGraphicsOpenGL pgl;
//GL gl;


PSys psys;
//generative_test = gen_test;

//NAppletManager AManager = new NAppletManager(this);

int tresh = 30;   // adjust treshold value here or keys t/T
int fade = 25;
int om = 1;
int videores=128;
String info="";
PFont font;
float fps = 60;
int videotex = 0; //case 0: videotex = videoimg;//case 1: videotex = videotexbin; 
//case 2: videotex = videotexmotion//case 3: videotex = videoteximgmotion;
float drag = 0.957;
int colour_r=20;
int colour_g=20;
int colour_b=226;
float fade_alpha = 12;
float vidAlpha; //used to fade in video
boolean show_video,fadeInVideo = false;
boolean keepDrawing = false;

void setup() {
  //store some blob velocities for testing purposes
  //output = createWriter("vxes.txt"); 
  
 
  
  //hide cursor
  noCursor();

  //set up gui components
  controlP5 = new ControlP5(this);
  //add a slider to control drag
//  controlP5.addSlider("Drag",0.9,1,(screen.width-200),(screen.height-200),10,100);
//  controlP5.addSlider("Colour_R",0,255,201,(screen.width-220),(screen.height-200),10,100);
//  controlP5.addSlider("Colour_G",0,255,175,(screen.width-240),(screen.height-200),10,100);
//  controlP5.addSlider("Colour_B",0,255,226,(screen.width-260),(screen.height-200),10,100);
//  controlP5.addSlider("Fade",0,100,10,(screen.width-280),(screen.height-200),10,100);
  
  // osx quicktime bug 882 processing 1.0.1
  try { 
    quicktime.QTSession.open();
  } 
  catch (quicktime.QTException qte) { 
    qte.printStackTrace();
  }

  size(screen.width,screen.height,OPENGL);
  //size(1400,1050,OPENGL); propeller dance projector
  background(0);
  frameRate(fps);
  
  /* The next two params are adapted from D.Shiffman's Nature of
  ** Code, Chapter 4, pg. 35. They are used for the additive blending
  ** applied to the sprites
  */
  //pgl = (PGraphicsOpenGL) g; 
  //gl = pgl.gl;
  
  //rectMode(CENTER);
  // init video data and stream
  String[] devices = Capture.list();
  println(devices); //to help find your device number
  
/*
 Use the appropriate version of the Capture instantiation
 COMMON SETTINGS:
 -> devices[2]: iSight on Modern MPB/usb webcam
 -> devices[4]: firewire
 */
 
  video = new Capture(this, videores, videores, devices[2],(int)fps);
  //video = new Capture(this, videores, videores, devices[4],(int)fps);  
  flob = new Flob(videores, videores, width, height);

  flob.setThresh(tresh).setSrcImage(videotex)
    .setBackground(video).setBlur(0).setOm(1).
      setFade(fade).setMirror(true,false).setTrackingMinDist(1000); //originally 500

    font = createFont("monaco",10);
  textFont(font);

  psys = new PSys(800);
  stroke(255,20);
  strokeWeight(1);
}
/* END OF SETUP */


void draw() {

  if(video.available()) {
    video.read();
    //blobs = flob.calc(flob.binarize(video));
    blobs = flob.track(flob.binarize(video));
  }

  if(fadeInVideo){
    //beginDraw;
    tint(#FFFFFF,vidAlpha); //hex for white
    vidAlpha = vidAlpha+1;
    image(flob.getSrcImage(), 0, 0, width, height);
    println(vidAlpha);
    //if(vidAlpha>800) fadeInVideo=false; //show_video=true;
    //endDraw;
  }

  if(show_video){
    image(flob.getSrcImage(), 0, 0, width, height);
  }

  rectMode(CENTER);

  int numblobs = blobs.size();
  //println(flob.getTrackingMinDist()); //testing
  //println(numblobs);

  for(int i = 0; i < numblobs; i++) {
    //ABlob ab = (ABlob)flob.getABlob(i);
    trackedBlob ab = (trackedBlob)flob.getTrackedBlob(i); 

    psys.touch(ab);
  }


  /* Comment the next two lines out to see what it looks like without */
  /* the fading effect */
  colorMode(HSB,255);
  //fill(199,4,51,20); //fade things out
  fill(0,fade_alpha); //fade things out
  rect(0,0,width*2,height*2);
  
  
  if(keepDrawing){
    psys.go(drag,output);
    psys.draw(colour_r, colour_g, colour_b);
  }
  
}

void Colour_R(int the_r_colour) {
  colour_r = the_r_colour;
}

void Colour_G(int the_g_colour) {
  colour_g = the_g_colour;
}

void Colour_B(int the_b_colour) {
  colour_b = the_b_colour;
}

void Drag(float theDrag) {
  drag = theDrag;
  println("A drag event. Drag set to :"+drag);
}

void Fade(float theFade) {
  fade_alpha = theFade;
}

void keyPressed() {

  if (key=='r'){
    psys = new PSys(800); 
  }
  if (key=='s')
    keepDrawing = !keepDrawing;
  if (key=='S'){
    fade_alpha = 12;
    keepDrawing = false;
    show_video = false; 
  } 
  if (key=='i') {  
    videotex = (videotex+1)%4;
    flob.setImage(videotex);
  }
  if(key=='t') {
    flob.setTresh(tresh--);
    println(tresh);
  }
  if(key=='E') {
    fade_alpha = 2;
    //keepDrawing = false;
  }   
  if(key=='v') {
    show_video = !show_video;
  }
//  if(key=='f') {
//    fadeInVideo=!fadeInVideo;
//    vidAlpha = 0;
//    //println("changed this: " + fadeInVideo);f
//  }   
  if(key=='o') {
    om=(om +1) % 3;
    flob.setOm(om);
  }   
  if (key=='x') {  
    output.flush(); // Writes the remaining data to the file
    output.close(); // Finishes the file
    exit(); // Stops the program
  }
  if(key==' ') //space clear flob.background
    flob.setBackground(video);
  if (key=='T') {  
    //AManager.createWindowedNApplet("generative_test",screen.width,screen.height);
  }
}



