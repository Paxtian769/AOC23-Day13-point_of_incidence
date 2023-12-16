#include <iostream>
#include <fstream>
#include <string>
#include <list>
using namespace std;

class LavaGrid {
private:
    char** charGrid;
    int columns;
    int rows;
    bool horiz_reflection;
    bool vert_reflection;
    long horiz_dist_to_mirror;
    long vert_dist_to_mirror;

    void process_horiz_reflection() {
        bool columnMatchesGrid[columns][columns];

        for (int currColumn = 0; currColumn < columns; currColumn++) {
            for (int compareColumn = 0; compareColumn < columns ; compareColumn++) {
                bool isMatch = true;
                if (currColumn != compareColumn) {
                    for (int currRow = 0; currRow < rows && isMatch; currRow++) {
                        isMatch = (charGrid[currRow][currColumn] == charGrid[currRow][compareColumn]);
                    }
                    columnMatchesGrid[currColumn][compareColumn] = isMatch;
                }
                else {
                    columnMatchesGrid[currColumn][compareColumn] = false;
                }
            }
        }

        bool leftSideMatch = false;
        int leftColumn = 0;
        for (int compareColumn = 1; compareColumn < columns && !leftSideMatch; compareColumn++) {
            if (columnMatchesGrid[leftColumn][compareColumn]) {
                int leftPos = leftColumn;
                int rightPos = compareColumn;
                while (leftPos < rightPos && columnMatchesGrid[leftPos][rightPos]) {
                    leftPos++;
                    rightPos--;
                }
                if (leftPos > rightPos) {
                    horiz_dist_to_mirror = leftPos;
                    horiz_reflection = true;
                    leftSideMatch = true;
                }
            }
        }

        if (!leftSideMatch) {
            bool rightSideMatch=false;
            int rightColumn = columns-1;
            for (int compareColumn = 0; compareColumn < columns && !rightSideMatch; compareColumn++) {
                int sum_of_diffs = 0;
                if (columnMatchesGrid[rightColumn][compareColumn]) {
                    int leftPos = compareColumn;
                    int rightPos = rightColumn;
                    while (leftPos < rightPos && columnMatchesGrid[leftPos][rightPos]) {
                        leftPos++;
                        rightPos--;
                    }
                    if (leftPos > rightPos) {
                        horiz_dist_to_mirror = leftPos;
                        horiz_reflection = true;
                        rightSideMatch = true;
                    }
                }
            }
        }
    }

    void process_vert_reflection() {
        bool rowMatchesGrid[rows][rows];
        for (int currRow = 0; currRow < rows; currRow++) {
            for (int compareRow = 0; compareRow < rows; compareRow++) {
                bool isMatch = true;
                if (currRow != compareRow) {
                    for (int currColumn = 0; currColumn < columns && isMatch; currColumn++) {
                        isMatch = (charGrid[currRow][currColumn] == charGrid[compareRow][currColumn]);
                    }
                    rowMatchesGrid[currRow][compareRow] = isMatch;
                }

                else {
                    rowMatchesGrid[currRow][compareRow] = false;
                }
            }
        }

        bool topSideMatch = false;
        int topRow = 0;
        for (int compareRow = 1; compareRow < rows && !topSideMatch; compareRow++) {
            if (rowMatchesGrid[topRow][compareRow]) {
                int topPos = topRow;
                int bottomPos = compareRow;
                while (topPos < bottomPos && rowMatchesGrid[topPos][bottomPos]) {
                    topPos++;
                    bottomPos--;
                }
                if (topPos > bottomPos) { // changed to strictly greater
                    vert_dist_to_mirror = topPos;
                    vert_reflection = true;
                    topSideMatch = true;
                }
            }
        }

        if (!topSideMatch) {
            bool bottomSideMatch=false;
            int bottomRow = rows-1;
            for (int compareRow = 0; compareRow < rows && !bottomSideMatch; compareRow++) {
                if (rowMatchesGrid[bottomRow][compareRow]) {
                    int topPos = compareRow;
                    int bottomPos = bottomRow;
                    while (topPos < bottomPos && rowMatchesGrid[topPos][bottomPos]) {
                        topPos++;
                        bottomPos--;
                    }
                    if (topPos > bottomPos) {
                        vert_dist_to_mirror = topPos;
                        vert_reflection = true;
                        bottomSideMatch = true;
                    }
                }
            }
        }
    }

    void build_char_grid(list<string> grid) {
        list<string>::iterator currLine = grid.begin();

        charGrid = new char*[rows];

        for (int row=0; row < rows && currLine != grid.end(); row++, currLine++) {
            charGrid[row] = new char[columns];
            for (int col=0; col < columns; col++) {
                charGrid[row][col] = (*currLine)[col];
            }
        }
    }

    void process_reflection() {

        process_horiz_reflection();
        process_vert_reflection();
    }
public:
    LavaGrid(ifstream &input_file, string &line) {
        list<string> grid;
        while (line.length() > 0) {
            grid.push_back(line);
            getline(input_file, line);
        }

        horiz_reflection = false;
        vert_reflection = false;
        rows = grid.size();
        columns = grid.begin()->size();
        build_char_grid(grid);

        process_reflection();
    }

    ~LavaGrid() {
        for (int row = 0; row < rows; row++) {
            delete[] charGrid[row];
        }
        delete[] charGrid;
    }

    bool is_horiz_reflection() {
        return horiz_reflection;
    }

    bool is_vert_reflection() {
        return vert_reflection;
    }

    long get_horizontal_distance() {
        return horiz_dist_to_mirror;
    }

    long get_vertical_distance() {
        return vert_dist_to_mirror;
    }

    void print_grid() {
        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < columns; col++) {
                cout << charGrid[row][col];
            }
            cout << endl;
        }
        cout << endl;
    }
};

int main(int argc, char **argv) {
    ifstream input_file;
    string file_name = "input.txt";
    input_file.open(file_name);
    string line;
    long horizontal_distances = 0;
    long vertical_distances = 0;
    long sum_of_notes=0;
    while (getline(input_file, line)) {
        LavaGrid lavaGrid(input_file, line);
        if (lavaGrid.is_horiz_reflection()) {
            horizontal_distances += lavaGrid.get_horizontal_distance();
        }
        else if (lavaGrid.is_vert_reflection()) {
            vertical_distances += lavaGrid.get_vertical_distance();
        }
    }

    sum_of_notes = (100 * vertical_distances) + horizontal_distances;

    cout << "Sum of notes = " << sum_of_notes << endl;

    return 0;
}
