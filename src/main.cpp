#include <algorithm>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

using namespace std;

bool findInARow(const std::vector<std::vector<std::string>> &v, int row,
                int value) {
  return std::find(v[row].begin(), v[row].end(), std::to_string(value)) !=
         v[row].end();
}

bool findInACol(const std::vector<std::vector<std::string>> &v, int col,
                int value) {
  for (int c = 0; c < 9; c++) {
    if (v[c][col] == std::to_string(value)) {
      return true;
    }
  }
  return false;
}

bool findInACube(const std::vector<std::vector<std::string>> &v, int row,
                 int col, int value) {
  int rowStart = row - (row % 3);
  int colStart = col - (col % 3);
  for (int r = rowStart; r < rowStart + 3; r++) {
    for (int c = colStart; c < colStart + 3; c++) {
      if (v[r][c] == std::to_string(value)) {
        return true;
      }
    }
  }
  return false;
}

std::vector<std::pair<int, int>>
getPossiblePositions(const std::vector<std::vector<std::string>> &v, int row,
                     int col, int value) {
  std::vector<std::pair<int, int>> positions;
  for (int i = row; i < row + 3; i++) {
    for (int j = col; j < col + 3; j++) {
      if (v[i][j] != "") {
        continue;
      }
      if (findInARow(v, i, value) == false &&
          findInACol(v, j, value) == false) {
        positions.push_back(std::make_pair(i, j));
      }
    }
  }
  return positions;
}

unsigned int countOfEmptyCells(std::vector<std::vector<std::string>> &v) {
  unsigned int count = 0;
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      if (v[i][j] == "") {
        count++;
      }
    }
  }
  return count;
}

int main(int argc, char const *argv[]) {

  std::vector<std::vector<std::string>> v = {
      {"", "3", "", "8", "", "", "", "", ""},
      {"", "", "", "", "7", "", "1", "", "2"},
      {"7", "", "", "1", "2", "", "", "", ""},
      {"", "", "", "2", "9", "", "3", "", ""},
      {"2", "", "6", "", "", "3", "", "", ""},
      {"", "4", "", "5", "", "", "", "", "6"},
      {"9", "", "5", "", "", "", "", "8", "7"},
      {"", "", "", "", "5", "", "9", "", "3"},
      {"6", "8", "3", "9", "", "", "2", "", "4"}};
  auto emptyCells = 81;
  while (countOfEmptyCells(v) != emptyCells) {
    emptyCells = countOfEmptyCells(v);
    for (int i = 0; i < 9; i++) {
      for (int j = 0; j < 9; j++) {
        if (v[i][j] != "")
          continue;
        std::vector<int> possibleValues;
        for (int k = 1; k <= 9; k++) {
          bool findInRow = findInARow(v, i, k);

          bool findInCol = findInACol(v, j, k);

          bool findInCube = findInACube(v, i, j, k);

          if (!findInCol && !findInRow && !findInCube) {
            if (std::find(possibleValues.begin(), possibleValues.end(), k) ==
                possibleValues.end()) {
              possibleValues.push_back(k);
            }
          }

          for (int r = 0; r < 9; r += 3) {
            for (int c = 0; c < 9; c += 3) {
              if (findInACube(v, r, c, k) == false) {
                auto pos = getPossiblePositions(v, r, c, k);
                if (pos.size() == 1) {
                  v[pos[0].first][pos[0].second] = std::to_string(k);
                }
              }
            }
          }
        }
        if (possibleValues.size() == 1) {
          v[i][j] = std::to_string(possibleValues[0]);
        }
      }
    }
  }
  for (int i = 0; i < 9; i++) {
    for (int j = 0; j < 9; j++) {
      std::cout << (v[i][j] == "" ? " " : v[i][j]) << " ";
    }
    std::cout << std::endl;
  }

  return 0;
}
