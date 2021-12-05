#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;


// We use the following point coordinates formulas:
// for any initial point location (x0, y0) and initial speed (vx0, vx0)
// at t == 0, the point coordinates and speed at the moment t > 0
// can be calculated:
//      { x = x0 + vx0 * t
//      { y = y0 + vy0 * t - g * t * t / 2
//      { vx = vx0
//      { vy = vy0 - g * t
// In these formulas we omit the partitioning x_i


const double g = 9.81;

int main(int argc, const char * argv[]) {
    //---- Read cmd parameters ----//
    string filename;
    if (argc == 2) {
        filename = string(argv[1]);
    } else {
        cerr << "ERROR: wrong number of input parameters: " << (argc - 1) << endl;
        return 1;
    }
    
    //---- Init file input ----//
    ifstream input;
    input.open(filename);
    if (!input) {
        cerr << "ERROR: can not open the file: " << filename << endl;
        return 1;
    }
    
    //---- Read the file ----//
    double x0 = 0, y0, vx, vy;
    input >> y0 >> vx >> vy;
    if (y0 < 0) {
        cerr << "ERROR: initial y coordinate must be >= 0" << endl;
        return 1;
    }
    
    vector<double> X, H;
    double curx, curh;
    size_t currentSection = 0;
    while (input >> curx >> curh) {
        // we assume input x coordinates to be sorted ascending
        if (X.size() > 0 && X.back() >= curx) {
            cerr << "ERROR: input is not an ascending sequence" << endl;
            return 1;
        }
        if (curx == x0) {
            cerr << "ERROR: initial point is located at the border" << endl;
            return 1;
        }
        if (curh < 0 ) {
            cerr << "ERROR: negative height" << endl;
            return 1;
        }
        X.push_back(curx);
        H.push_back(curh);
        // try to define an initial section
        if (x0 > curx) {
            currentSection = X.size();
        }
    }
    const size_t N = X.size();
    
    //---- Iterations ----//
    double dx = 0, dt = 0, h0 = 0;
    size_t nextSection = -1, i = -1;
    if (vx < 0) {
        i = currentSection; // will be recomputed to currentSection - 1 (*)
        nextSection = currentSection + 1; // will be recomputed to currentSection
    } else {
        i = currentSection - 1; // will be recomputed to currentSection (**)
        nextSection = currentSection - 1; // will be recomputed to currentSection
    }
    while (y0 > 0 || (y0 == 0 && vy > 0)) {
        // detect a stop condition and find the next point
        if (vx < 0) {
            if (i <= 0) break;
            i--; // currentSection - 1 if iterations just begun (*)
            nextSection = currentSection - 1;
        } else if (vx > 0) {
            if (i >= N - 1) break;
            i++; // currentSection if iterations just begun (**)
            nextSection = currentSection + 1;
        } else {
            break;
        }

        dx = X[i] - x0;
        x0 = X[i];
        h0 = H[i];
        dt = dx / vx; // always nonnegative
        y0 += (vy * dt - g * dt * dt / 2);
        vy -= g * dt;
        if (y0 <= h0) {
            // collision; stay in the same section and reflect x speed
            vx = -vx;
        } else {
            currentSection = nextSection;
        }


        // if y0 <= 0, then the point has falled at currrent secion
        // and the loop terminates
    }
    
    cout << currentSection << endl;
    return 0;
}
