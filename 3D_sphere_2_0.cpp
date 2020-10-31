#define SizeX_of_Window 600
#define SizeY_of_Window 600
#define Radius 300

#include <stdio.h>
#include "TXLib.h"

class Vector {
    public: 
        double x, y, z;
        double length();
        void Turn_Vector(double degree);
        Vector normalization();
        Vector (double x_coord, double y_coord, double z_coord) {
            x = x_coord;
            y = y_coord;
            z = z_coord;
        }

};
    void draw_sphere(double coord_x, double coord_y, double coord_z, double lambert2, double lambert1, double fong2, double fong1);
    void draw_pixels(double x, double y, COLORREF color);
    double koef_I(Vector a, Vector b);
    double koef_J(Vector a, Vector b);
    Vector operator / (Vector a, double lambda);
    Vector operator - (Vector a,Vector b);
    Vector operator * (Vector a, double lambda);
    double operator ^ (Vector a, Vector b);

int main()
{
    txCreateWindow(SizeX_of_Window, SizeY_of_Window);
    txClear();
    
    double z = 0;
    
    double Static_X = -300;
    double Static_Y = 300;
        
    Vector radius_vector_to_source(90, -90, 100);
    
    while(1) {  
        radius_vector_to_source.Turn_Vector(txPI/7);
        for (int x = -SizeX_of_Window / 2; x <= SizeX_of_Window / 2; x++) {
            for (int y = -SizeY_of_Window / 2; y <= SizeY_of_Window / 2; y++) { 
                if (x * x + y * y <= Radius * Radius) {
                    
                    z = sqrt(Radius * Radius - x * x - y * y);
                    
                    Vector normal_vector(x, y, z);
                    Vector normal_static_vector(x,y,z);
                    Vector static_source_to_pixels(-Static_X, -Static_Y, z);
                    Vector source_to_pixels = radius_vector_to_source;
                    Vector eye_to_pixels(-x, -y, 2 * Radius);
                    
                    double lambert_dinamic = koef_I(normal_vector, source_to_pixels);
                    double lambert_static = koef_I(normal_static_vector, static_source_to_pixels);
                    
                    //double I = normal_vector ^ source_to_pixels;
                    //double I1 = normal_static_vector ^ static_source_to_pixels;
                    
                    eye_to_pixels = eye_to_pixels.normalization();
                    normal_vector = normal_vector.normalization() * 2 - source_to_pixels.normalization();
                    normal_vector = normal_vector.normalization();
                    normal_static_vector = normal_static_vector.normalization() * 2 - static_source_to_pixels.normalization();
                    normal_static_vector = normal_static_vector.normalization();
                    
                    
                    double fong_dinamic = pow(normal_vector ^ eye_to_pixels, 10) / 5;
                    double fong_static= pow(normal_static_vector ^ eye_to_pixels, 10) / 5;
                    
                    
                    draw_sphere(x, y, z, lambert_static, fong_static, lambert_dinamic, fong_dinamic);
                    
                }
                else {
                    
                    draw_pixels(x, y, RGB(0, 0, 0));
                
                }
        }
    }
               
    }
	return 0;
}
void draw_pixels(double x, double y, COLORREF color) {
    double x1 = SizeX_of_Window  / 2;
    double y1 = SizeY_of_Window / 2;
    txSetPixel(x + x1, y + y1, color);
}

void draw_sphere(double coord_x, double coord_y, double coord_z, double lambert2, double fong2, double lambert1, double fong1){
    double koef = lambert1 + fong1;
    double koef_static = lambert2 + fong2;
    double all_koef = koef + koef_static;
    if (lambert1 + lambert2 > 0) {
        if (koef + koef_static > 1 ) 
            all_koef = 1;
        draw_pixels(coord_x, coord_y, RGB (0.85 * coord_z * all_koef, 0, 0));
    }
    else {
        draw_pixels(coord_x, coord_y, RGB(0, 0, 0));
    }
}

double koef_I(Vector a, Vector b) {
    a = a.normalization();
    b = b.normalization();
    return a ^ b;
}

double Vector::length() {
	return sqrt(x * x + y * y + z * z);
}


void Vector::Turn_Vector(double degree) {
    double x0 = 0, z0 = 0;
	//plot new_coords = { 0, 0, 0 };
	x0 = x * cos(degree) - z * sin(degree);
	z0 = x * sin(degree) + z * cos(degree);
	//coords_ = new_coords;
    x = x0;
    z = z0;
}


Vector Vector::normalization() {
    Vector temp = *this;
	double length = temp.length();
	return temp * 1 / length;
}
Vector operator / (Vector a, double lambda)  {
    Vector result(a.x / lambda, a.y / lambda, a.z / lambda);
    return result;
}

Vector operator - (Vector a, Vector b) {
     Vector result_vector(a.x - b.x, a.y - b.y, a.z - b.z);
     return result_vector;
}

Vector operator * (Vector a, double lambda) {
    Vector result_vector(lambda * a.x, lambda * a.y, lambda * a.z);
    return result_vector;
}

double operator ^ (Vector a, Vector b) {
    return (a.x * b.x + a.y * b.y + a.z * b.z);
}
