import processing.core.*; 
import processing.xml.*; 

import processing.opengl.*; 
import processing.video.*; 
import s373.flob.*; 
import controlP5.*; 

import java.applet.*; 
import java.awt.Dimension; 
import java.awt.Frame; 
import java.awt.event.MouseEvent; 
import java.awt.event.KeyEvent; 
import java.awt.event.FocusEvent; 
import java.awt.Image; 
import java.io.*; 
import java.net.*; 
import java.text.*; 
import java.util.*; 
import java.util.zip.*; 
import java.util.regex.*; 

public class theatreArt extends PApplet {

/*
  
 Modified code originally developed by Andr\u00e9 Sier 2010
 
 -AH 01-20-11
 
 */






ControlP5 controlP5;
Capture video;   
Flob flob;       
ArrayList blobs; 
PrintWriter output;


PSys psys;

int tresh = 20;   // adjust treshold value here or keys t/T
int fade = 25;
int om = 1;
int videores=128;
String info="";
PFont font;
float fps = 60;
int videotex = 0; //case 0: videotex = videoimg;//case 1: videotex = videotexbin; 
//case 2: videotex = videotexmotion//case 3: videotex = videoteximgmotion;
float drag = 0.957f;
int colour_r,colour_g,colour_b = 0;


public void setup() {
  //store some blob velocities for testing purposes
  output = createWriter("vxes.txt"); 

  //set up gui components
  controlP5 = new ControlP5(this);
  //add a slider to control drag
  controlP5.addSlider("Drag",0.9f,1,(screen.width-200),(screen.height-200),10,100);
  controlP5.addSlider("Colour_R",0,255,56,(screen.width-220),(screen.height-200),10,100);
  controlP5.addSlider("Colour_G",0,255,68,(screen.width-240),(screen.height-200),10,100);
  controlP5.addSlider("Colour_B",0,255,84,(screen.width-260),(screen.height-200),10,100);
  
  // osx quicktime bug 882 processing 1.0.1
  try { 
    quicktime.QTSession.open();
  } 
  catch (quicktime.QTException qte) { 
    qte.printStackTrace();
  }

  size(screen.width,screen.height,OPENGL);
  background(0);
  frameRate(fps);
  //rectMode(CENTER);
  // init video data and stream
  String[] devices = Capture.list();
  println(devices);

  video = new Capture(this, videores, videores, devices[2],(int)fps);  
  flob = new Flob(videores, videores, width, height);

  flob.setThresh(tresh).setSrcImage(videotex)
    .setBackground(video).setBlur(0).setOm(1).
      setFade(fade).setMirror(true,false);

  font = createFont("monaco",10);
  textFont(font);

  psys = new PSys(500);
  stroke(255,200);
  strokeWeight(2);
}



public void draw() {

  if(video.available()) {
    video.read();
    //blobs = flob.calc(flob.binarize(video));
    blobs = flob.track(flob.binarize(video));
  }

  //image(flob.getSrcImage(), 0, 0, width, height);

  rectMode(CENTER);

  int numblobs = blobs.size();
  //println(numblobs);

  for(int i = 0; i < numblobs; i++) {
    //ABlob ab = (ABlob)flob.getABlob(i);
    trackedBlob ab = (trackedBlob)flob.getTrackedBlob(i); 
    psys.touch(ab);

    //box
    //fill(ab.prevelx,ab.prevely,ab.maxdist2,ab.lifetime);
    //rect(ab.cx,ab.cy,ab.dimx,ab.dimy);
    //centroid
    fill(234,12,12,200);
    rect(ab.cx,ab.cy, 5, 5);
    //info = ""+ab.lifetime;
    //text(info);
  }


  /* Comment the next two lines out to see what it looks like without */
  /* the fading effect */

  fill(0,5); //fade things out
  rect(0,0,width*2,height*2);

  psys.go(drag,output);
  psys.draw(colour_r, colour_g, colour_b);
}

public void Colour_R(int the_r_colour) {
  colour_r = the_r_colour;
}

public void Colour_G(int the_g_colour) {
  colour_g = the_g_colour;
}

public void Colour_B(int the_b_colour) {
  colour_b = the_b_colour;
}

public void Drag(float theDrag) {
  drag = theDrag;
  println("A drag event. Drag set to :"+drag);
}

public void keyPressed() {

  if (key=='S')
    video.settings();
  if (key=='i') {  
    videotex = (videotex+1)%4;
    flob.setImage(videotex);
  }
  if(key=='t') {
    flob.setTresh(tresh--);
    println(tresh);
  }
  if(key=='T') {
    flob.setTresh(tresh++);
    println(tresh);
  }   
  if(key=='f') {
    flob.setFade(fade--);
  }
  if(key=='F') {
    flob.setFade(fade++);
  }   
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
}

//float drag = 0.957;
class Part {
  float x, y, vx, vy, ax, ay;
  float px,py,force;

  Part() {
    x = random(width);
    y = random(height);
    vx = random(-2,2);
    vy = random(-2,2);
    force = random(-2,2);
    px = x;
    py = y;
  }
  public void go(float drag,PrintWriter output) {
    vx += ax;
    vy += ay;
    vx *= drag;
    vy *= drag;
    px = x;
    py = y;
    x+=vx;
    y+=vy;
    ax = 0;
    ay = 0;
    bounds();
    output.println(vx); //record some velocities for testing
  }
  
  public void bounds(){
    boolean c = false;
    if(x>width){
      x-=width;
      c = true;
    }
    if(x<0){
      c = true;
      x+=width;
    }
    if(y>height){
      c = true;
      y-=height;
    }
    if(y<0){
      y+=height;
      c = true;
    }
    if(c){
      px = x;
      py = y;
    }
  }

  public void draw(int colourR,int colourG,int colourB) {
    //println("vx: "+vx);
    colorMode(HSB,255);
    stroke(colourR,colourG,colourB,random(100-y,255));
    line(px,py,x,y);
  }
  public void touch(ABlob ab) {
    float dx = ab.cx - x;
    float dy = ab.cy - y;
    float d = sqrt(dx*dx+dy*dy);
    if(d > 0 && d < 200) {
      d = 1.0f/d * force;
      dx *= d;
      dy *= d;
      ax += dx;
      ay += dy;
    }
  }
}


class PSys {
  Part p[];

  PSys (int num) {
    p = new Part[num];
    for(int i=0;i<p.length;i++)
      p[i] = new Part();
  }

  public void go(float drag,PrintWriter output) {    
    for(int i=0; i<p.length;i++)
      p[i].go(drag,output);
  }
  public void draw(int r_colour,int g_colour,int b_colour) {
    for(int i=0; i<p.length;i++)
      p[i].draw(r_colour,g_colour,b_colour);
  }
  public void touch(ABlob ab) {
    for(int i=0; i<p.length;i++)
      p[i].touch(ab);
  }
}

  static public void main(String args[]) {
    PApplet.main(new String[] { "--bgcolor=#FFFFFF", "theatreArt" });
  }
}
