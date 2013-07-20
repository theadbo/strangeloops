import king.kinect.*;

PImage img, depth;
short[] rawDepth;

float depth_calc = 0.0;
float average = 0;


void setup()
{
    size(1280, 480);

    img = createImage(640,480,RGB);
    depth = createImage(640,480,RGB);

    NativeKinect.init();
    NativeKinect.setVideoIR(); // View the IR images, see also setVideoRGB()
//    NativeKinect.setVideoRGB();
    NativeKinect.start();
}

void draw()
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
    
    depth_calc = 1.0 / (average * -0.0030711016 + 3.3309495161);
    
    println(depth_calc);
//    println( rawDepth[0] );

    image(img,0,0,640,480);
    image(depth,640,0,640,480);
}

