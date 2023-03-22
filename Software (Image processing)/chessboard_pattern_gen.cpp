#include <opencv2/opencv.hpp>
#include <fstream>
#include <iostream>
#include <cmath>

using namespace cv;
using namespace std;

int main()
{
    // Define the dimensions of the A4 format
    const double A4_WIDTH = 210; // in millimeters
    const double A4_HEIGHT = 297; // in millimeters

    // Calculate the number of rows and columns of chessboard tiles that fit on an A4 page
    const int TILE_SIZE = 40; // in pixels
    const int MARGIN_SIZE = 20; // in pixels
    int num_rows = floor((A4_HEIGHT - 2 * MARGIN_SIZE) / TILE_SIZE);
    int num_cols = floor((A4_WIDTH - 2 * MARGIN_SIZE) / TILE_SIZE);

    // Define the margins around the edges of the A4 page
    const double LEFT_MARGIN = 20; // in millimeters
    const double TOP_MARGIN = 20; // in millimeters
    const double RIGHT_MARGIN = 20; // in millimeters
    const double BOTTOM_MARGIN = 20; // in millimeters

    // Calculate the size of the chessboard area, excluding margins
    const double CHESSBOARD_WIDTH = (num_cols * TILE_SIZE);
    const double CHESSBOARD_HEIGHT = (num_rows * TILE_SIZE);

    // Calculate the position of the top-left corner of the chessboard area, relative to the top-left corner of the SVG file
    const double CHESSBOARD_X = LEFT_MARGIN;
    const double CHESSBOARD_Y = TOP_MARGIN;

    // Calculate the size of the SVG file, including margins
    const double SVG_WIDTH = LEFT_MARGIN + CHESSBOARD_WIDTH + RIGHT_MARGIN;
    const double SVG_HEIGHT = TOP_MARGIN + CHESSBOARD_HEIGHT + BOTTOM_MARGIN;

    // Set the size of the SVG file
    double svg_width = SVG_WIDTH;
    double svg_height = SVG_HEIGHT;

    // Generate the chessboard pattern
    Mat chessboard(num_rows * TILE_SIZE, num_cols * TILE_SIZE, CV_8UC3, Scalar(255, 255, 255));
    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < num_cols; j++) {
            if ((i + j) % 2 == 0) {
                rectangle(chessboard, Point(j * TILE_SIZE, i * TILE_SIZE), Point((j + 1) * TILE_SIZE, (i + 1) * TILE_SIZE), Scalar(0, 0, 0), -1);
            }
        }
    }

    // Convert the OpenCV image to an SVG string
    string svg_image = "<svg width=\"" + to_string(svg_width) + "mm\" height=\"" + to_string(svg_height) + "mm\" viewBox=\"0 0 " + to_string(svg_width) + " " + to_string(svg_height) + "\" xmlns=\"http://www.w3.org/2000/svg\">\n";
    svg_image += "<rect x=\"0\" y=\"0\" width=\"" + to_string(svg_width) + "\" height=\"" + to_string(svg_height) + "\" fill=\"#fff\" />\n";
    svg_image += "<g transform=\"translate(" + to_string(CHESSBOARD_X) + " " + to_string(CHESSBOARD_Y) + ")\">\n";
    svg_image += "<rect x=\"0\" y=\"0\" width=\"" + to_string(num_cols * TILE_SIZE) + "\" height=\"" + to_string(num_rows * TILE_SIZE) + "\" fill=\"#000\" />\n";
    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < num_cols; j++) {
            if ((i + j) % 2 == 0) {
                svg_image += "<rect x=\"" + to_string(j * TILE_SIZE) + "\" y=\"" + to_string(i * TILE_SIZE) + "\" width=\"" + to_string(TILE_SIZE) + "\" height=\"" + to_string(TILE_SIZE) + "\" fill=\"#fff\" />\n";
            }
        }
    }
    svg_image += "</g>\n";
    svg_image += "</svg>";

    // Save the SVG string to a file
    ofstream file("chessboardq.svg");
    file << svg_image;
    file.close();

    return 0;
}