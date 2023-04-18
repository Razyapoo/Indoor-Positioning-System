#include <iostream>
#include <fstream>
#include <string>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

const int TILE_SIZE = 40; // Size of each square tile in pixels
// const int MARGIN_SIZE = 1; // Size of the margin in millimeters
const int PAGE_WIDTH = 297; // Width of the paper in millimeters
const int PAGE_HEIGHT = 420; // Height of the paper in millimeters

int main() {
    // Calculate the size of the chessboard
    int num_cols = PAGE_WIDTH / TILE_SIZE;
    int num_rows = PAGE_HEIGHT / TILE_SIZE;
    
    // Calculate the size of the margin
    int margin_x = 0;
    int margin_y = 0;
    // if ((num_cols * TILE_SIZE) + (2 * margin_x) > PAGE_WIDTH) {
    margin_x = (PAGE_WIDTH - (num_cols * TILE_SIZE)) / 2;
    // }
    // if ((num_rows * TILE_SIZE) + (2 * margin_y) > PAGE_HEIGHT) {
    margin_y = (PAGE_HEIGHT - (num_rows * TILE_SIZE)) / 2;
    // }
    
    // Calculate the size of the SVG image
    int svg_width = (num_cols * TILE_SIZE) + (2 * margin_x);
    int svg_height = (num_rows * TILE_SIZE) + (2 * margin_y);
    
    // Calculate the position of the chessboard within the SVG image
    int chessboard_x = margin_x;
    int chessboard_y = margin_y;
    
    // Create the OpenCV image
    // Mat image(svg_height, svg_width, CV_8UC3, Scalar(255, 255, 255));
    
    // Draw the chessboard
    // for (int i = 0; i < num_rows; i++) {
    //     for (int j = 0; j < num_cols; j++) {
    //         if ((i + j) % 2 == 0) {
    //             rectangle(image, Point(chessboard_x + (j * TILE_SIZE), chessboard_y + (i * TILE_SIZE)), Point(chessboard_x + ((j + 1) * TILE_SIZE), chessboard_y + ((i + 1) * TILE_SIZE)), Scalar(0, 0, 0), -1);
    //         }
    //     }
    // }
    
    // Convert the OpenCV image to an SVG string
    string svg_image = "<svg width=\"" + to_string(svg_width) + "mm\" height=\"" + to_string(svg_height) + "mm\" viewBox=\"0 0 " + to_string(svg_width) + " " + to_string(svg_height) + "\" xmlns=\"http://www.w3.org/2000/svg\">\n";
    svg_image += "<rect x=\"0\" y=\"0\" width=\"" + to_string(svg_width) + "\" height=\"" + to_string(svg_height) + "\" fill=\"#fff\" />\n";
    svg_image += "<g transform=\"translate(" + to_string(chessboard_x) + " " + to_string(chessboard_y) + ")\">\n";
    svg_image += "<rect x=\"0\" y=\"0\" width=\"" + to_string(num_cols * TILE_SIZE) + "\" height=\"" + to_string(num_rows * TILE_SIZE) + "\" fill=\"#000\" />\n";
    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j < num_cols; j++) {
            if ((i + j) % 2 == 1) {
                svg_image += "<rect x=\"" + to_string(j * TILE_SIZE) + "\" y=\"" + to_string(i * TILE_SIZE) + "\" width=\"" + to_string(TILE_SIZE) + "\" height=\"" + to_string(TILE_SIZE) + "\" fill=\"#fff\" />\n";
            }
        }
    }
    svg_image += "</g>\n";
    svg_image += "</svg>";
    // Save the SVG image to a file
    ofstream svg_file("chessboard.svg");
    svg_file << svg_image;
    svg_file.close();

    cout << "Chessboard saved to chessboard.svg" << endl;

    return 0;
}