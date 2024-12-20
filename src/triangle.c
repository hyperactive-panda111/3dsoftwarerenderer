#include "triangle.h"
#include "sort.h"
#include "vector.h"

vect3_t get_triangle_normal(vect4_t vertices[3]) {
            // Check backface culling to see if current face should be projected
            vect3_t vector_a = vect3_from_vect4(vertices[0]);
            vect3_t vector_b = vect3_from_vect4(vertices[1]);
            vect3_t vector_c = vect3_from_vect4(vertices[2]);

            // Get the vector subtraction of B - A and C - A
            vect3_t vector_ab = vect3_sub(vector_b, vector_a);
            vect3_t vector_ac = vect3_sub(vector_c, vector_a);

            vect3_normalize(&vector_ab);
            vect3_normalize(&vector_ac);

            // Compute the face normal (using cross product to find perpendicular)
            vect3_t normal = vect3_cross(vector_ab, vector_ac);
            vect3_normalize(&normal);

            return normal;
}

void fill_flat_bottom_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
    // Find the two slopes of a triangle (two triangle legs)
    float inv_slope_1 = (float)(x1 - x0) / (y1 - y0);
    float inv_slope_2 = (float)(x2 - x0) / (y2 - y0);

    // Start x_start and x_end from the top vertex (x0, y0)
    float x_start = x0;
    float x_end = x0;

    // Loop all the scanlines from top to bottom
     for (int y = y0; y <= y2; y++) {
        draw_line(x_start, y, x_end, y, color);
        x_start += inv_slope_1;
        x_end += inv_slope_2;
    }
}

void fill_flat_top_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
    // Find the two slopes of the triangle (two triangle legs)
    float inv_slope_1 = (float)(x2 - x0) / (y2 - y0);
    float inv_slope_2 = (float)(x2 - x1) / (y2 - y1);

    // Start x_start and x_end from the bottom vertex (x2, y2)
    float x_start = x2;
    float x_end = x2;

    // Loop all the scanlines from bottom to top
    for (int y = y2; y >= y0; y--) {
        draw_line(x_start, y, x_end, y, color);
        x_start -= inv_slope_1;
        x_end -= inv_slope_2;
    }
}

void draw_triangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
    draw_line(x0, y0, x1, y1, color);
    draw_line(x1, y1, x2, y2, color);
    draw_line(x2, y2, x0, y0, color);
}

/*
///////////////////////////////////////////////////////////////////////////////
// Return the barycentric weights alpha, beta, and gamma for point p
///////////////////////////////////////////////////////////////////////////////
//
//         (B)
//         /|\
//        / | \
//       /  |  \
//      /  (P)  \
//     /  /   \  \
//    / /       \ \
//   //           \\
//  (A)------------(C)
//
/////////////////////////////////////////////////////////////////////////////// */
vect3_t barycentric_weights(vect2_t a, vect2_t b, vect2_t c, vect2_t p) {
    // Find the vectors between the vertices ABC and point p
    vect2_t ac = vect2_sub(c, a);
    vect2_t ab = vect2_sub(b, a);
    vect2_t ap = vect2_sub(p, a);
    vect2_t pc = vect2_sub(c, p);
    vect2_t pb = vect2_sub(b, p);

    // Compute the area of the full parallegram/triangle ABC using 2D cross product
    float area_parallelogram_abc = (ac.x * ab.y - ac.y * ab.x); // || AC x AB ||

    // Alpha is the area of the small parallelogram/triangle PBC divided by the area of the full parallelogram/triangle ABC
    float alpha = (pc.x * pb.y - pc.y * pb.x) / area_parallelogram_abc;
    // Beta is the area of the small parallelogram/triangle APC divided by the area of the full parallelogram/triangle ABC
    float beta = (ac.x * ap.y - ac.y * ap.x) / area_parallelogram_abc;

    // Weight gamma is easily found since barycentric coordinates always add up to 1.0
    float gamma = 1 - alpha - beta;

    vect3_t weights = { alpha, beta, gamma };
    return weights;
}


// Function to draw the texel at position(x, y) using interpolation
void draw_triangle_texel(int x, int y, upng_t* texture,
          vect4_t point_a, vect4_t point_b, vect4_t point_c,
          tex2_t a_uv, tex2_t b_uv, tex2_t c_uv) {
    vect2_t p = { x, y };
    vect2_t a = vect2_from_vect4(point_a); 
    vect2_t b = vect2_from_vect4(point_b);
    vect2_t c = vect2_from_vect4(point_c);

    vect3_t weights = barycentric_weights(a, b, c, p);

    float alpha = weights.x;
    float beta = weights.y;
    float gamma = weights.z;

    // Variables to store the interpolated values of U, V and 1/w for the current pixel
    float interpolated_u;
    float interpolated_v;
    float interpolated_reciprocal_w;

    // Perform the interpolation of all U and V values using barycentric weights
    interpolated_u = (a_uv.u / point_a.w) * alpha + (b_uv.u / point_b.w) * beta + (c_uv.u / point_c.w) * gamma;
    interpolated_v = (a_uv.v / point_a.w) * alpha + (b_uv.v / point_b.w) * beta + (c_uv.v / point_c.w) * gamma;

    // Also interpolte the value of 1/w for the current pixel
    interpolated_reciprocal_w = (1 / point_a.w) * alpha + (1 / point_b.w) * beta + (1 / point_c.w) * gamma;

    interpolated_u /= interpolated_reciprocal_w;
    interpolated_v /= interpolated_reciprocal_w;

    // Get the mesh texture width and height dimensions
    int texture_width = upng_get_width(texture);
    int texture_height = upng_get_height(texture);

    // Map the UV coordinate to full texture width and height 
    int tex_x = abs((int)(interpolated_u * texture_width)) % texture_width;
    int tex_y = abs((int)(interpolated_v * texture_height)) % texture_height;

    // Adjust 1/w so the pixels that are closer to the camera have smaller values
    interpolated_reciprocal_w = 1.0 - interpolated_reciprocal_w;

    // Only draw the pixel if the depth value is less than the one previously stored int the z-buffer
    if (interpolated_reciprocal_w < get_zbuffer_at(x,y)) {
        // Get the buffer of colors from the texture
        uint32_t* texture_buffer = (uint32_t*)upng_get_buffer(texture);

        // Draw texel at pixel position(x, y) from mapped texture
        draw_pixel(x, y, texture_buffer[(texture_width * tex_y) + tex_x]);

        // Update the z-buffer value with the 1/w of this current pixel
        update_zbuffer_at(x, y, interpolated_reciprocal_w);
        }
}

void draw_triangle_pixel(
        int x, int y, uint32_t color, 
        vect4_t point_a, vect4_t point_b, vect4_t point_c) {

    vect2_t p = { x, y };
    vect2_t a = vect2_from_vect4(point_a);
    vect2_t b = vect2_from_vect4(point_b);
    vect2_t c = vect2_from_vect4(point_c);

    vect3_t weights = barycentric_weights(a, b, c, p);

    float alpha = weights.x;
    float beta = weights.y;
    float gamma = weights.z;
    float interpolated_reciprocal_w; 

    // Also interpolte the value of 1/w for the current pixel
    interpolated_reciprocal_w = (1 / point_a.w) * alpha + (1 / point_b.w) * beta + (1 / point_c.w) * gamma;

    // Adjust 1/w to reverse the order of the pixels
    interpolated_reciprocal_w = 1.0 - interpolated_reciprocal_w; 

    if (interpolated_reciprocal_w < get_zbuffer_at(x, y)) {
        // Draw a pixel at position(x, y) with a solid color
        draw_pixel(x, y, color);

        // Update the z-buffer value with the 1/w of this current pixel
        update_zbuffer_at(x, y, interpolated_reciprocal_w);
    }
}

void draw_filled_triangle(
        int x0, int y0, float z0, float w0,
        int x1, int y1, float z1, float w1,
        int x2, int y2, float z2, float w2,
        uint32_t color) {
    // Sort the vertices by y-coordinate ascending( y0 < y1 < y2)
    if (y0 > y1) {
        int_swap(&y0, &y1);
        int_swap(&x0, &x1);
        float_swap(&z0, &z1);
        float_swap(&w0, &w1);
    }
    if (y1 > y2) {
        int_swap(&y1, &y2);
        int_swap(&x1, &x2);
        float_swap(&z1, &z2);
        float_swap(&w1, &w2);
    }
    if (y0 > y1) {
        int_swap(&y0, &y1);
        int_swap(&x0, &x1);
        float_swap(&z0, &z1);
        float_swap(&w0, &w1);
    }

    // Create vector points and texture coordinates after sorting vertices
    vect4_t point_a = { x0, y0, z0, w0 };
    vect4_t point_b = { x1, y1, z1, w1 };
    vect4_t point_c = { x2, y2, z2, w2 };

    // Render the upper part of the triangle(flat-bottom triangle)
    //////////////////////////////////////////////////////////////
    float inv_slope_1 = 0;
    float inv_slope_2 = 0;

    if (y1 - y0 != 0) inv_slope_1 = (x1 - x0) / (float) (abs) (y1 - y0); 
    if (y2 - y0 != 0) inv_slope_2 = (x2 - x0) / (float) (abs) (y2 - y0);

    if (y1 - y0 != 0) {
        for (int y = y0; y <= y1; y++) {
            int x_start = x1 + (y - y1) * inv_slope_1;
            int x_end = x0 + (y - y0) * inv_slope_2;

            if (x_end < x_start)
                int_swap(&x_start, &x_end);

            for (int x = x_start; x < x_end; x++) {
                // Draw pixel with color from the texture
                 draw_triangle_pixel(x, y, color, point_a, point_b, point_c);
            }
        }
    }

    // Render the lower part of the triangle(flat-top triangle)
    //////////////////////////////////////////////////////////////
    inv_slope_1 = 0;
    inv_slope_2 = 0;

    if (y2 - y1 != 0) inv_slope_1 = (x2 - x1) / (float) (abs) (y2 - y1); 
    if (y2 - y0 != 0) inv_slope_2 = (x2 - x0) / (float) (abs) (y2 - y0);

    if (y2 - y1 != 0) {
        for (int y = y1; y <= y2; y++) {
            int x_start = x1 + (y - y1) * inv_slope_1;
            int x_end = x0 + (y - y0) * inv_slope_2;

            if (x_end < x_start)
                int_swap(&x_start, &x_end);

            for (int x = x_start; x < x_end; x++) {
                 draw_triangle_pixel(x, y, color, point_a, point_b, point_c);
            }
        }
    }
}

void draw_textured_triangle(
        int x0, int y0, float z0, float w0, float u0, float v0,
        int x1, int y1, float z1, float w1, float u1, float v1,
        int x2, int y2, float z2, float w2, float u2, float v2,
        upng_t* texture
) {
    // Sort the vertices by y-coordinate ascending( y0 < y1 < y2)

    if (y0 > y1) {
        int_swap(&y0, &y1);
        int_swap(&x0, &x1);
        float_swap(&z0, &z1);
        float_swap(&w0, &w1);
        float_swap(&u0, &u1);
        float_swap(&v0, &v1);
    }
    if (y1 > y2) {
        int_swap(&y1, &y2);
        int_swap(&x1, &x2);
        float_swap(&z1, &z2);
        float_swap(&w1, &w2);
        float_swap(&u1, &u2);
        float_swap(&v1, &v2);
    }
    if (y0 > y1) {
        int_swap(&y0, &y1);
        int_swap(&x0, &x1);
        float_swap(&z0, &z1);
        float_swap(&w0, &w1);
        float_swap(&u0, &u1);
        float_swap(&v0, &v1);
    }

    //Flip the V component to account for inverted UV-coordinates
    v0 = 1.0 - v0;
    v1 = 1.0 - v1;
    v2 = 1.0 - v2;
    

    // Create vector points and texture coordinates after sorting vertices
    vect4_t point_a = { x0, y0, z0, w0 };
    vect4_t point_b = { x1, y1, z1, w1 };
    vect4_t point_c = { x2, y2, z2, w2 };

    tex2_t a_uv = { u0, v0 };
    tex2_t b_uv = { u1, v1 };
    tex2_t c_uv = { u2, v2 };

    // Render the upper part of the triangle(flat-bottom triangle)
    //////////////////////////////////////////////////////////////
    float inv_slope_1 = 0;
    float inv_slope_2 = 0;

    if (y1 - y0 != 0) inv_slope_1 = (x1 - x0) / (float) (abs) (y1 - y0); 
    if (y2 - y0 != 0) inv_slope_2 = (x2 - x0) / (float) (abs) (y2 - y0);

    if (y1 - y0 != 0) {
        for (int y = y0; y <= y1; y++) {
            int x_start = x1 + (y - y1) * inv_slope_1;
            int x_end = x0 + (y - y0) * inv_slope_2;

            if (x_end < x_start)
                int_swap(&x_start, &x_end);

            for (int x = x_start; x < x_end; x++) {
                // Draw pixel with color from the texture
                 draw_triangle_texel(x, y, texture, point_a, point_b, point_c, a_uv, b_uv, c_uv);
            }
        }
    }


    // Render the lower part of the triangle(flat-top triangle)
    //////////////////////////////////////////////////////////////
    inv_slope_1 = 0;
    inv_slope_2 = 0;

    if (y2 - y1 != 0) inv_slope_1 = (x2 - x1) / (float) (abs) (y2 - y1); 
    if (y2 - y0 != 0) inv_slope_2 = (x2 - x0) / (float) (abs) (y2 - y0);

    if (y2 - y1 != 0) {
        for (int y = y1; y <= y2; y++) {
            int x_start = x1 + (y - y1) * inv_slope_1;
            int x_end = x0 + (y - y0) * inv_slope_2;

            if (x_end < x_start)
                int_swap(&x_start, &x_end);

            for (int x = x_start; x < x_end; x++) {
                 draw_triangle_texel(x, y, texture, point_a, point_b, point_c, a_uv, b_uv, c_uv);
            }
        }
    }
}
