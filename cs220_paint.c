#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "cs220_paint.h"

unsigned char *read_image(const char *filename, int *width, int *height) {
  // open file for reading?
  FILE *in = fopen(filename, "rb");
  if (in == NULL) {
    return NULL;
  }

  // read image dimensions
  if (fscanf(in, "%d %d", width, height) != 2) {
    fclose(in);
    return NULL;
  }

  // allocate buffer for RGBA triples
  int num_vals = *width * *height * 3;
  unsigned char *buf = (unsigned char *) malloc(num_vals * sizeof(unsigned char));
  if (buf == NULL) {
    fclose(in);
    return NULL;
  }

  // read RGBA triples
  int error = 0;
  for (int i = 0; i < num_vals; i++) {
    unsigned val;
    if (fscanf(in, "%x", &val) != 1) {
      error = 1;
    } else {
      buf[i] = (unsigned char) val;
    }
  }

  // now can close input
  fclose(in);

  // if error, delete buffer and return NULL
  if (error) {
    free(buf);
    return NULL;
  }

  // return buffer
  return buf;
}


int write_image(const char *filename, int width, int height, const unsigned char *buf) {

  // open file for writing
  FILE *out = fopen(filename, "w");
  if (out == NULL) {
    return 0;
  }

  // add the width and height values into file
  fprintf(out, " %d %d\n", width, height);

  // stores hexadecimal value from buf
  unsigned val = 0;

  // value will help determine the use of new line character after each triple
  int counter = 0; 

  // the number of values in buf
  int total_vals = width * height * 3;
  
  // add the color component values into file
  for (int i = 0; i < total_vals; i++) {

    val = buf[i];
    counter++;
    
    if (i == total_vals - 1) {
      // if hexadecimal is the last value in the triple, then add new line
      if (counter == 3) {
        fprintf(out, " %x\n", val);
      }
      else {
	      fprintf(out, " %x", val);
      }
    }

    // reached the last hexademical value in buffer and no need for new line
    else {
      fprintf(out, "%x\n", val);
    }
  }

  // error handling
  if (ferror(out)) {
    printf("Error: error indicator was set for output file/n");
    return 0;
  }
  
  // close the output file
  fclose(out);

  // successfuly writes the complete image data
  return 1;
  
}


double geom_dist(double x1, double y1, double x2, double y2) {
  return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));  
}

// finds the min of two numbers
int min(int num1, int num2) {
  if (num1 >= num2)
    return num2;
  else {
    return num1;
  }
}

void render_rectangle(unsigned char *pixel_data, int img_width, int img_height,
                      int rect_x, int rect_y, int rect_w, int rect_h,
                      int r, int g, int b) {

  // determines if maximum x and y positions exceed the image width and height
  int x_end = min(rect_x + rect_w, img_width);
  int y_end = min(rect_y + rect_h, img_height);

  for (int x = rect_x; x < x_end; x++) {
    for (int y = rect_y; y < y_end; y++) {
	    // determine current position in pixel_data array
	    int curr = 3 * (y * img_width + x);
	    // assign r, g, and b values
	    pixel_data[curr] = r;
	    pixel_data[curr + 1] = g;
	    pixel_data[curr + 2] = b;
     }
  }
}


void render_ellipse(unsigned char *pixel_data, int img_width, int img_height,
                      double x1, double y1, double x2, double y2, double len,
                      int r, int g, int b) {

  // iterates through each pixel in the image
  for (int x = 0; x < img_width; x++) {
    for (int y = 0; y < img_height; y++) {
      // calculate distances between foci to point and between focal points
      double focal_point_dist = geom_dist(x1, y1, x2, y2);
      double f1_to_point_dist = geom_dist(x1, y1, x, y);
      double f2_to_point_dist = geom_dist(x2, y2, x, y);
      // checks if point is on or within the ellipse
      if (focal_point_dist + f1_to_point_dist + f2_to_point_dist <= len) {
	      int curr = 3 * (y * img_width + x);
	      pixel_data[curr] = r;
	      pixel_data[curr + 1] = g;
	      pixel_data[curr + 2] = b;
      }
    }
  }
	
}



void flood_fill(unsigned char *pixel_data, int img_width, int img_height,
                int x, int y, int r, int g, int b) {

  // find r, g, and b values of start pixel
  int curr = 3 * (y * img_width + x);
  int orig_r = pixel_data[curr];
  int orig_g = pixel_data[curr + 1];
  int orig_b = pixel_data[curr + 2];
  
  // call rec_flood_fill function
  rec_flood_fill(pixel_data, img_width, img_height, x, y, orig_r, orig_g, orig_b, r, g, b, -1);
}



void rec_flood_fill(unsigned char *pixel_data, int img_width, int img_height,
                    int x, int y,
                    int orig_r, int orig_g, int orig_b,
                    int r, int g, int b,
                    int dir) {

  // find current color values of pixel                                              
  int curr_r = 3 * (y * img_width + x);
  int curr_g = 3 * (y * img_width + x) + 1;
  int curr_b = 3 * (y * img_width + x) + 2;

  // determine if current color value matches original color value of start pixel                   
  int same_color = (pixel_data[curr_r] == orig_r) &&
    (pixel_data[curr_g] == orig_g) && (pixel_data[curr_b] == orig_b);

  // start pixel
  if (dir == -1) {

    // check if pixel coordinates exceed bounds of image or are negative
    if (x > img_width || y > img_height || x < 0 || y < 0) {
      return;
    }

    pixel_data[curr_r] = r;
    pixel_data[curr_g] = g;
    pixel_data[curr_b] = b;

    // left
    rec_flood_fill(pixel_data, img_width, img_height, x - 1, y, orig_r, orig_g, orig_b, r, g, b, 0);

    // right
    rec_flood_fill(pixel_data, img_width, img_height, x + 1, y, orig_r, orig_g, orig_b, r, g, b, 1);

    // up
    rec_flood_fill(pixel_data, img_width, img_height, x, y - 1, orig_r, orig_g, orig_b, r, g, b, 2);

    // down
    rec_flood_fill(pixel_data, img_width, img_height, x, y + 1, orig_r, orig_g, orig_b, r, g, b, 3);
  }
  
  // left
  if (dir == 0) {
	
    // check if pixel coordinates exceed bounds of image or are negative
    if (x > img_width || y > img_height || x < 0 || y < 0) {
      return;
    }

    // pixel has been filled or doesn't have the same original r, g, b values
    if (!same_color) {
      return;
    }
    // otherwise, change pixel color
    else {
      pixel_data[curr_r] = r;
      pixel_data[curr_g] = g;
      pixel_data[curr_b] = b;
    }

    // left
    rec_flood_fill(pixel_data, img_width, img_height, x - 1, y, orig_r, orig_g, orig_b, r, g, b, 0);

    // up
    rec_flood_fill(pixel_data, img_width, img_height, x, y - 1, orig_r, orig_g, orig_b, r, g, b, 2);

    // down
    rec_flood_fill(pixel_data, img_width, img_height, x, y + 1, orig_r, orig_g, orig_b, r, g, b, 3);
  }

  // right
  if (dir == 1) {
    
    // check if pixel coordinates exceed bounds of image or are negative
    if (x > img_width || y > img_height || x < 0 || y < 0) {
      return;
    }

    // pixel has been filled or doesn't have the same original r,g,b, values
    if (!same_color) {
      return;
    }

    // otherwise, change pixel color
    else {
      pixel_data[curr_r] = r;
      pixel_data[curr_g] = g;
      pixel_data[curr_b] = b;
    }

    // right                                                                                                                                                                        
    rec_flood_fill(pixel_data, img_width, img_height, x + 1, y, orig_r, orig_g, orig_b, r, g, b, 1);

    // up                                                                                                                                                                          
    rec_flood_fill(pixel_data, img_width, img_height, x, y - 1, orig_r, orig_g, orig_b, r, g, b, 2);

    // down                                                                                                                                                                        
    rec_flood_fill(pixel_data, img_width, img_height, x, y + 1, orig_r, orig_g, orig_b, r, g, b, 3); 
  }

  // up
  if (dir == 2) {

    // check if pixel coordinates exceed bounds of image or are negative
    if (x > img_width || y > img_height || x < 0 || y < 0) {
      return;
    }

    // pixel has been filled or doesn't have the same original r,g,b, values
    if (!same_color) {
      return;
    }
    // otherwise, change pixel color                                                                                                                                                
    else {
      pixel_data[curr_r] = r;
      pixel_data[curr_g] = g;
      pixel_data[curr_b] = b;
    }
                                                                                                                                  
    // left                                                                                                                                                                       
    rec_flood_fill(pixel_data, img_width, img_height, x - 1, y, orig_r, orig_g, orig_b, r, g, b, 0);

    // up                                                                                                                                                                         
    rec_flood_fill(pixel_data, img_width, img_height, x, y - 1, orig_r, orig_g, orig_b, r, g, b, 2);

    // right                                                                                                                                                                       
    rec_flood_fill(pixel_data, img_width, img_height, x + 1, y, orig_r, orig_g, orig_b, r, g, b, 1);
  }

  // down
  if (dir == 3) {

    // check if pixel coordinates exceed bounds of image or are negative
    if (x > img_width || y > img_height || x < 0 || y < 0) {
      return;
    }

    // pixel has been filled or doesn't have the same original r,g,b, values
    if (!same_color) {
      return;
    }
    // otherwise, change pixel color                                                                                                                                                
    else {
      pixel_data[curr_r] = r;
      pixel_data[curr_g] = g;
      pixel_data[curr_b] = b;
    }
                                                                                                                                 
    // left                                                                                                                                                                       
    rec_flood_fill(pixel_data, img_width, img_height, x - 1, y, orig_r, orig_g, orig_b, r, g, b, 0);

    // right                                                                                                                                                                      
    rec_flood_fill(pixel_data, img_width, img_height, x + 1, y, orig_r, orig_g, orig_b, r, g, b, 1);

    // down                                                                                                                                                                       
    rec_flood_fill(pixel_data, img_width, img_height, x, y + 1, orig_r, orig_g, orig_b, r, g, b, 3);
  }
}

