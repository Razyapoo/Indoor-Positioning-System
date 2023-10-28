#include <iostream>
#include <fstream>
#include <cstdlib>

int main()
{
    // Create a data file containing the scatter plot data
    std::ofstream dataFile("scatter_data.txt");
    if (!dataFile)
    {
        std::cerr << "Error: Unable to open data file." << std::endl;
        return 1;
    }

    // Generate random data for the scatter plot
    int numPoints = 100;
    for (int i = 0; i < numPoints; ++i)
    {
        double x = static_cast<double>(i);
        double y = static_cast<double>(rand()) / RAND_MAX;
        dataFile << x << " " << y << std::endl;
    }
    dataFile.close();

    // Execute GNU Plotutils' command directly using popen
    FILE *gnuplotPipe = popen("gnuplot", "w");
    if (gnuplotPipe)
    {
        // Plot the scatter plot
        fprintf(gnuplotPipe, "set term png\n");
        fprintf(gnuplotPipe, "set output 'scatter_plot.png'\n");
        fprintf(gnuplotPipe, "plot 'scatter_data.txt' with points\n");
        fprintf(gnuplotPipe, "exit\n");

        pclose(gnuplotPipe);
        std::cout << "Scatter plot created: scatter_plot.png" << std::endl;
    }
    else
    {
        std::cerr << "Error: Failed to execute GNU Plotutils' gnuplot command." << std::endl;
        return 1;
    }

    return 0;
}