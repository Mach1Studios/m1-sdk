float xRot, yRot ;
float prevMouseX, prevMouseY;

void setup()
{
  size(600, 600, P3D) ;
  colorMode (HSB, 360, 100, 100);
}

int[][] positions = {  
  {-1, -1, -1}, 
  {1, -1, -1}, 
  {-1, -1, 1}, 
  {1, -1, 1}, 
  {-1, 1, -1}, 
  {1, 1, -1}, 
  {-1, 1, 1}, 
  {1, 1, 1},
};

void draw()
{
  background(0);
  lights();

  pushMatrix() ;

  camera(width/2, height/2, (height/2) / tan(PI/6), width/2, height/2, 0, 0, 1, 0);
  translate(width/2, height/2);

  ArrayList<Float> volumes = eightChannelsAlgorithm(yRot , xRot, 0);//  eightChannelsIsotropicAlgorithm(yRot, xRot, 0);

  textSize(32);

  float size = 150;
  for (int i=0; i<8; i++)
  {
    pushMatrix() ;
    translate(size * positions[i][0], size * positions[i][1], size * positions[i][2]) ;
    fill(10, 100, 100); 
    noStroke();
    sphere(1 + 40 * volumes.get(i));
    fill(220, 255, 255); 
    text(i, 5, 5); 
    popMatrix();
  }

  if (mousePressed)
  {
    xRot += 2 * (prevMouseX - mouseX);
    yRot += 2 * (mouseY - prevMouseY);
  }
  prevMouseX = mouseX;
  prevMouseY = mouseY;

  translate(0, 0, 0);
  pushMatrix() ;
  rotateY(radians (xRot));
  rotateX(radians (yRot));
  pyramid(50, 200);
  popMatrix() ;

  stroke(255, 0, 255); // PURPLE GRID
  pushMatrix();
  drawGrid(0, -50, 0, 20);
  popMatrix();

  popMatrix();

  fill(0, 0, 255); 

  textSize(16);
  text("xRot " + xRot, 10, 30); 
  text("yRot " + yRot, 10, 50);

  for (int i=0; i<8; i++)
  {
    text( i + ": " + volumes.get(i), 10, 80 + 20 * i );
  }
}


void pyramid( float w, float h)
{
  noFill();
  strokeWeight(1);
  stroke(255);

  beginShape();
  vertex(-1*w, -1*w, -1*h);
  vertex( 1*w, -1*w, -1*h);
  vertex(0, 0, 0);

  vertex(1*w, -1*w, -1*h);
  vertex(1*w, 1*w, -1*h);
  vertex(0, 0, 0);

  vertex(1*w, 1*w, -1*h);
  vertex(-1*w, 1*w, -1*h);
  vertex(0, 0, 0);

  vertex(-1*w, 1*w, -1*h);
  vertex(-1*w, -1*w, -1*h);
  vertex(0, 0, 0);
  endShape();
}

void drawGrid(float xoffset, float yoffset, float zoffset, float gridSize) {
  for (int i = -width/2; i <width/2; i+=gridSize) {
    for (int j = -height/2; j < height/2; j+=gridSize) {
      int y = 200;
      line(xoffset + i, yoffset + y, zoffset + j, xoffset + i+gridSize, yoffset + y, zoffset + j          );
      line(xoffset + i+gridSize, yoffset + y, zoffset + j, xoffset + i+gridSize, yoffset + y, zoffset + j+gridSize );
      line(xoffset + i+gridSize, yoffset + y, zoffset + j+gridSize, xoffset + i, yoffset + y, zoffset + j+gridSize );
      line(xoffset + i, yoffset + y, zoffset + j, xoffset + i, yoffset + y, zoffset + j+gridSize );
    }
  }
}