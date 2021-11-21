#include <iostream>
#include <fstream>  // reading a file
#include <limits>   // std::numeric_limits<double>::max()

using namespace std;


double dotProd(const double v1 [2], const double v2 [2]) {
    return v1[0] * v2[0] + v1[1] * v2[1];
}

int main() {
    ifstream input;
    input.open("in.txt"); // input data stream
    if (!input) {
        cerr << "ERROR: can not open the file\n";
        return 0;
    }
    double dir[2] = {0, 0}; // input line direction
    input >> dir[0] >> dir[1];
    if (dir[0] == 0 && dir[1] == 0) {
        cerr << "ERROR: zero direction vector\n";
        return 0;
    }
    // rotate a vector by 90 degree to get a normal (left) direction
    double norm[2] = {-dir[1], dir[0]};
    // read points and dynamically compute extremals
    double point[2]; // input point
    double leftmost[2], rightmost[2];
    double leftMax  = -numeric_limits<double>::max(); // yes, exactly this way
    double rightMin = numeric_limits<double>::max(); // assign max to min and min to max to get right comparisons
    while (input >> point[0] >> point[1]) {
        // actually we don't have to normalize \norm
        double dist = dotProd(norm, point);
        if (dist > leftMax) {
            leftMax = dist;
            leftmost[0] = point[0];
            leftmost[1] = point[1];
        }
        if (dist < rightMin) {
            rightMin = dist;
            rightmost[0] = point[0];
            rightmost[1] = point[1];
        }
    }
    input.close();
    if (leftMax < rightMin) {
        // only possible when there were no points
        cout << "NO POINTS PASSED\n";
    } else {
        cout << "Leftmost: " << leftmost[0] << " " << leftmost[1] << endl;
        cout << "Rightmost: " << rightmost[0] << " " << rightmost[1] << endl;
    }
    return 0;
}
