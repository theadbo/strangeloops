import toxi.geom.*;
//float drag = 0.957;
class Part {
  Vec2D particle_pos;
  
  float x, y, vx, vy, ax, ay;
  float px,py,force;

  Part() {
    x = random(width);
    y = random(height);
    vx = random(-2,2);
    vy = random(-2,2);
    force = random(-2,4);
    //force = -2;
    px = x;
    py = y;
  }
  void go(float drag,PrintWriter output) {
    vx += ax*(0.4);
    vy += ay*(0.4);
    vx *= drag;
    vy *= drag;
    px = x;
    py = y;
    x+=vx;
    y+=vy;
    ax = 0;
    ay = 0;
    bounds();
    //output.println(vx); //record some velocities for testing
  }
  
  void bounds(){
    boolean c = false;
    if(x>width){
      //x-=width;
      vx = vx*(-1);
      c = true;
    }
    if(x<0){
      c = true;
      //x+=width;
      vx = vx*(-1);
    }
    if(y>height){
      c = true;
      //y-=height;
      vy = vy*(-1);
    }
    if(y<0){
      //y+=height;
      vy = vy*(-1);
      c = true;
    }
    if(c){
      px = x;
      py = y;
    }
  }

  void draw(int colourR,int colourG,int colourB) {
    //println("vx: "+vx);
    noStroke();
    colorMode(HSB,255);
    stroke(colourR,colourG,colourB,random(100-y,255));
    ellipse(x,y,3,2);
    //line(px,py,x,y);
  }
  void touch(ABlob ab) {
    float dx = ab.cx - x;
    float dy = ab.cy - y;
    float d = sqrt(dx*dx+dy*dy);
    if(d > 0 && d < 200) {
      d = 1.0f/d * force;
      dx *= d;
      dy *= d;
      ax += dx*.4; //magic number to tune down the speeds
      ay += dy*.4;
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

  void go(float drag,PrintWriter output) {    
    for(int i=0; i<p.length;i++)
      p[i].go(drag,output);
  }
  void draw(int r_colour,int g_colour,int b_colour) {
    for(int i=0; i<p.length;i++)
      p[i].draw(r_colour,g_colour,b_colour);
  }
  void touch(ABlob ab) {
    for(int i=0; i<p.length;i++)
      p[i].touch(ab);
  }
}

