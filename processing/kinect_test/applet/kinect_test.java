import processing.core.*; 
import processing.xml.*; 

import king.kinect.*; 

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

public class kinect_test extends PApplet {



PImage img, depth;
short[] rawDepth;

float depth_calc = 0.0f;
float average = 0;


public void setup()
{
    size(1280, 480);

    img = createImage(640,480,RGB);
    depth = createImage(640,480,RGB);

    NativeKinect.init();
    NativeKinect.setVideoIR(); // View the IR images, see also setVideoRGB()
//    NativeKinect.setVideoRGB();
    NativeKinect.start();
}

public void draw()
{
    img.pixels = NativeKinect.getVideo();
    img.updatePixels();

    depth.pixels = NativeKinect.getDepthMap();
    depth.updatePixels();

    rawDepth = NativeKinect.getDepthMapRaw();
     
    for ( int i = 0; i < rawDepth.length; ++i ) 
    { 
     average += rawDepth[i]; 
    } 
    average /= (float)(rawDepth.length);
    
    depth_calc = 1.0f / (average * -0.0030711016f + 3.3309495161f);
    
    println(depth_calc);
//    println( rawDepth[0] );

    image(img,0,0,640,480);
    image(depth,640,0,640,480);
}

  static public void main(String args[]) {
    PApplet.main(new String[] { "--bgcolor=#FFFFFF", "kinect_test" });
  }
}
