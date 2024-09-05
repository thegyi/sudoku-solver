#include "mainwindow.h"
#include "qapplication.h"
#include "qchar.h"
#include "qlineedit.h"
#include "qpalette.h"
#include "ui_mainwindow.h"
#include <QIntValidator>
#include <QWidget>
#include <QTextToSpeech>

#include <vector>
#include <string>
#include <algorithm>

std::vector<std::vector<std::string>> v = {
    {"", "", "", "", "", "", "", "", ""},
    {"", "", "", "", "", "", "", "", ""},
    {"", "", "", "", "", "", "", "", ""},
    {"", "", "", "", "", "", "", "", ""},
    {"", "", "", "", "", "", "", "", ""},
    {"", "", "", "", "", "", "", "", ""},
    {"", "", "", "", "", "", "", "", ""},
    {"", "", "", "", "", "", "", "", ""},
    {"", "", "", "", "", "", "", "", ""}};

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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(size());
    for (auto child : this->centralWidget()->children()) {
        QLineEdit *edit = dynamic_cast<QLineEdit *>(child);
        if (edit != nullptr) {
            edit->setValidator( new QIntValidator(1, 9, edit) );
        }
    }
    m_speech = new QTextToSpeech(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_speech;
}

void MainWindow::on_actionExit_triggered()
{
    QApplication::exit();
}

//clear button
void MainWindow::on_pushButton_2_clicked()
{
    QColor colorBlack(Qt::black);
    for (auto child : this->centralWidget()->children()) {
        QLineEdit *edit = dynamic_cast<QLineEdit *>(child);
        if (edit != nullptr) {
            QPalette palette = edit->palette();
            palette.setColor(QPalette::Text, colorBlack);
            edit->setPalette(palette);
            edit->setText("");
        }
    }
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            v[i][j] = "";
        }
    }
}

//solve button
void MainWindow::on_pushButton_clicked() {

    //fill v with linedit values
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            v[i][j] = "";
            for (auto child : this->centralWidget()->children()) {
                QLineEdit *edit = dynamic_cast<QLineEdit *>(child);
                if (edit != nullptr) {
                    if (edit->objectName() == QString(QString("lineEdit_") + QString::fromStdString(std::to_string(i * 9 + j + 1)))) {
                        v[i][j] = edit->text().toStdString();
                    }
                    edit->text();
                }
            }
        }
    }


    unsigned int emptyCells = 81;
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

    QColor colorBlack(Qt::black);
    QColor colorBlue(Qt::red);
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            for (auto child : this->centralWidget()->children()) {
                QLineEdit *edit = dynamic_cast<QLineEdit *>(child);
                if (edit != nullptr) {
                    if (edit->objectName() == QString(QString("lineEdit_") + QString::fromStdString(std::to_string(i * 9 + j + 1)))) {
                        QPalette palette = edit->palette();
                        if (edit->text() == "") {
                            palette.setColor(QPalette::Text, colorBlue);
                        } else {
                            palette.setColor(QPalette::Text, colorBlack);
                        }
                        edit->setPalette(palette);
                        edit->setText(QString::fromStdString(v[i][j]));

                    }

                }
            }
        }
    }

}

void MainWindow::on_actionClear_triggered()
{
    on_pushButton_2_clicked();
}

void MainWindow::on_actionSolve_triggered()
{
    on_pushButton_clicked();
}

//speak button
void MainWindow::on_pushButton_3_clicked() {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            for (auto child : this->centralWidget()->children()) {
                QLineEdit *edit = dynamic_cast<QLineEdit *>(child);
                if (edit != nullptr) {
                    if (edit->objectName() == QString(QString("lineEdit_") + QString::fromStdString(std::to_string(i * 9 + j + 1)))) {
                       m_speech->say(edit->text());
                    }

                }
            }
        }
    }
}
