/**
 * BLG 336E - Analysis of Algorithms II
 * Assignment 2
 * Student Name: Remzi Deniz Çaylak
 * Student ID: 150210081
*/

#include <iostream>   // For input/output
#include <cmath>      // For distance calculations
#include <vector>     // For vector operations
#include <algorithm>  // For sorting operations
#include <limits>     // For min(), max() functions
#include <fstream>    // To read/write files
#include <chrono>     // For time measurement

//(These libraries are sufficient for this assignment.)

using namespace std;

// Structure to represent a point in 2D space
struct Point {
    double x, y;
};

// Helper function to calculate distance between two points
double distance(Point p1, Point p2) {

    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2)); //Returns the distance between 2 points using pythagor's formula.

}

// Helper function to compare points based on x-coordinate, can use this for sorting.
bool compareX(Point p1, Point p2) {

    return p1.x < p2.x; //Returns true is x coordinate of p1 is smaller than x coordinate of p2.

}

// Helper function to compare points based on y-coordinate, can use this for sorting.
bool compareY(Point p1, Point p2) {

    return p1.y < p2.y; //Returns true is y coordinate of p1 is smaller than y coordinate of p2.

}

// Function to find the closest pair of points using brute force.
pair<Point, Point> bruteForceClosestPair(vector<Point>& points, int start, int end) {

    double minDistance = numeric_limits<double>::max(); //Create minDistance and assign it to infinity to compare later.
    pair<Point, Point> closestPair; //Initializing the closestPair variable.

    for (int i = start; i <= end; ++i) { //This loop iterates through every point in our "points" vector.
        for (int j = i + 1; j <= end; ++j) { //This second loop iterates through every element after chosen element in our "points" vector.
            double dist = distance(points[i], points[j]); //Take the distance between every pair of nodes.
            if (dist < minDistance) { //If new distance is smaller than the previous minimum distance, update it and also update closestPair.
                minDistance = dist; //Update minDistance.
                closestPair = {points[i], points[j]}; //Update closestPair.
            }
        }
    }
    return closestPair; //After all comparisons, we get real closest pair and we return it.

}

// The main objective of the assignment. This is the function to find the closest pair of points recursively using divide and conquer.
pair<Point, Point> closestPair(vector<Point>& points, int start, int end) {

    if (end - start <= 3) { //If there are less than 3 element in the vector, then do brute force directly.
        return bruteForceClosestPair(points, start, end);
    }

    int mid = (start + end) / 2; //Initialize an index for dividing the vector in half.
    Point midPoint = points[mid]; //Find a middle point in the vector using the index that we initialized.

    pair<Point, Point> leftPair = closestPair(points, start, mid); //Recursively find the pair in left half.
    pair<Point, Point> rightPair = closestPair(points, mid + 1, end); //Recursively find the pair in right half.


    pair<Point, Point> minPair; //Initialize the minimum pair variable.
    if (distance(leftPair.first, leftPair.second) < distance(rightPair.first, rightPair.second)) { //Find the minimum pair of left and right halves.
        minPair = leftPair;
    }

    else {
        minPair = rightPair;
    }

    
    vector<Point> splitted;//Initialize the vector for pairs that one point is in the left half and other point is in the right half.
    for (int i = start; i <= end; ++i) {
        if (abs(points[i].x - midPoint.x) < distance(minPair.first, minPair.second)) { //Check if the new distance is smaller than the distance that we find in one of the halves.
            splitted.push_back(points[i]); //If new distance is smaller, push that pair into the vector.
        }
    }

    
    sort(splitted.begin(), splitted.end(), compareY); //Sort the vector of splitted points based on y-coordinates.

    
    int splittedSize = splitted.size();
    for (int i = 0; i < splittedSize; ++i) { //This is for comparing points in the splitted points vector to find a closer pair.
        for (int j = i + 1; j < splittedSize; ++j) {
            
            if ((splitted[j].y - splitted[i].y) < distance(minPair.first, minPair.second)) { //Check if the y-coordinate difference is less than the minimum distance we found so far.
                double dist = distance(splitted[i], splitted[j]);//Initialize distance for minimum splitted pair.
                if (dist < distance(minPair.first, minPair.second)) { //Check the distance is smaller than previous minPair's distance:
                    minPair = {splitted[i], splitted[j]}; //Make that pait minimum pair.
                }
            }
            
            else {
                break;
            }
        }
    }

    return minPair; //Return the minimum pair found.
}

// Function to remove a pair of points from the point vector. Returns the newly formed vector.
vector<Point> removePairFromVector(vector<Point>& point_vector, pair<Point,Point> point_pair) {

    auto isPairPointMatches = [&](const Point& p) {//Creating a lambda function for checking if a point matches either point in the pair.
        return (p.x == point_pair.first.x && p.y == point_pair.first.y) ||
               (p.x == point_pair.second.x && p.y == point_pair.second.y);
    };

    //Remove all the elements matching the pair's points using lambda function that we created.
    point_vector.erase(remove_if(point_vector.begin(), point_vector.end(), isPairPointMatches), point_vector.end());

    return point_vector; //Return the vector after removing.
}

// This function should wrap up the entire algorithm. It should:
//    1- Find the closest pair
//    2- Add the pair to the "pairs" vector
//    3- Remove the paired result from the map
// at each iteration.
// Then it should set "unconnected" variable as the final, unconnected city (if it exists).
void findClosestPairOrder(vector<Point> points) {
    vector<pair<Point,Point>> pairs; 
    Point unconnected = {-1,-1};

    sort(points.begin(), points.end(), compareX); //Sort the points vector by their x coordinate.

    while (points.size() > 1) { //While the given vector's size is bigger than 1, this loop will be activated.
        pair<Point, Point> closest = closestPair(points, 0, points.size() - 1); //Find the closest pair using our function "closestPair".
        pairs.push_back(closest); //Push the pair into the vector.
        points = removePairFromVector(points, closest); //Remove the pair from main vector.
    }

    if (!points.empty()) { //After removing the pairs, if the vector is not empty assign the points as unconnected.
        unconnected = points[0];
    }


    for (unsigned int i = 0; i < pairs.size(); ++i) { //Iterate through all pairs in vector to sort them.
        pair<Point, Point>& pair = pairs[i]; //Create a referance to pair in the vector.
       
        if (pair.first.y == pair.second.y && pair.first.x > pair.second.x) {//If the Y coordinates are equal, sort based on the X coordinate
            swap(pair.first, pair.second);
        } 
      
        else if (pair.first.y > pair.second.y) {//Otherwise, sort based on the Y coordinate
            swap(pair.first, pair.second);
        }
    }


    //This part prints the output, don't modify.    
    for(size_t i = 0; i < pairs.size(); i++){
        cout << "Pair " << i+1 << ": " << pairs[i].first.x << ", " << pairs[i].first.y << " - " << pairs[i].second.x << ", " << pairs[i].second.y << endl;
    }
    if (unconnected.x != -1){
        cout << "Unconnected " << unconnected.x << ", " << unconnected.y;
    }


}

//Read the coordinates from the file and convert them to a vector. Return the vector as a "vector<Point>"
vector<Point> readCoordinatesFromFile(const string& filename) {
    vector<Point> points;

    Point point;
    ifstream inputFile(filename); //Take the given file.
    while (inputFile >> point.x >> point.y) { //Take the inputs as coordinates of point.
        points.push_back(point); //Push the point into vector.
    }

    inputFile.close(); //Close the file.

    return points; //Return the vector.
}

// Main function. Ideally, you don't need to modify this.
int main(int argc, char* argv[]) {
    vector<Point> points = readCoordinatesFromFile(argv[1]);
    findClosestPairOrder(points);
    return 0;
}
