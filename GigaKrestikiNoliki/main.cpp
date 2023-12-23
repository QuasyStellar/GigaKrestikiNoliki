#include <QApplication>
#include <QGridLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QWidget>
#include <QFont>

class TicTacToe : public QWidget {
    Q_OBJECT

public:
    TicTacToe(QWidget *parent = nullptr) : QWidget(parent) {
        QGridLayout *gridLayout = new QGridLayout(this);
        for (int row = 0; row < 9; ++row) {
            for (int col = 0; col < 9; ++col) {
                QPushButton *button = new QPushButton("");
                button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
                button->setFont(QFont("Arial", 24));
                button->setFixedSize(50, 50);
                button->setStyleSheet("background-color: white;");
                gridLayout->addWidget(button, row, col);
                connect(button, &QPushButton::clicked, this, [this, button, row, col]() {
                    handleButtonClick(button, row, col);
                });
                buttons[row][col] = button;
            }
        }

        QPushButton *resetButton = new QPushButton("Сбросить поле", this);
        connect(resetButton, &QPushButton::clicked, this, &TicTacToe::resetGame);
        resetButton->setStyleSheet("background-color: white; color: black;");
        resetButton->setFont(QFont("Arial", 24));
        gridLayout->addWidget(resetButton, 9, 0, 1, 9);
        resetButton->setFixedHeight(50);
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                segmentWinStatus[i][j] = ' ';
            }
        }
    }

    void handleButtonClick(QPushButton *button, int row, int col) {
        ispath(row%3 * 3, col%3 * 3);
        if (button->text().isEmpty() && !gameOver && isfull(row%3 * 3, col%3 * 3)) {
            for (int i = 0; i< 9; ++i) {
                for (int j = 0; j < 9; ++j) {
                    buttons[i][j]->setEnabled(false);
                }
            }
            highlightSegment(row, col, 0);
            if (turnX) {
                if (segmentWinStatus[row / 3][col / 3] == ' '){
                    button->setText("X");
                    button->setStyleSheet("background-color: white; color: blue;");
                }
                else if (segmentWinStatus[row / 3][col / 3] == 'X'){
                    highlightWinningSegments(row, col);
                    button->setText("X");
                    button->setStyleSheet("background-color: blue; color: white;");
                }
                else{
                    highlightWinningSegments(row, col);
                    button->setText("X");
                    button->setStyleSheet("background-color: red; color: white;");}

            } else {
                if (segmentWinStatus[row / 3][col / 3] == ' '){
                    button->setText("O");
                    button->setStyleSheet("background-color: white; color: red;");
                }
                else if (segmentWinStatus[row / 3][col / 3] == 'X'){
                    highlightWinningSegments(row, col);
                    button->setText("O");
                    button->setStyleSheet("background-color: blue; color: white;");
                }
                else{
                    highlightWinningSegments(row, col);
                    button->setText("O");
                    button->setStyleSheet("background-color: red; color: white;");}
            }
            if (checkWinInSegment(row, col)) {
                highlightWinningSegments(row, col);
                segmentWinStatus[row / 3][col / 3] = (turnX ? 'X' : 'O');
                turnX = !turnX;
            }else {
                turnX = !turnX;
            }
            ispath(row%3 * 3, col%3 * 3);
            highlightSegment(row%3 * 3, col%3 * 3, 1);
            if (checkwin(!turnX ? 'X' : 'O')){
                QString winner = !turnX ? "X" : "O";
                QMessageBox::information(this, "Победа!", "Игрок " + winner + " победил!");
                gameOver = true;
                resetGame();
            }
            else if (checkTie()) {
                QMessageBox::information(this, "Ничья", "Игра окончена, ничья!");
                gameOver = true;
                resetGame();
            }
            button->setEnabled(false);
        }

        if (!isfull(row%3 * 3, col%3 * 3)){
            if (turnX) {
                if (segmentWinStatus[row / 3][col / 3] == ' '){
                    button->setText("-");
                    button->setStyleSheet("background-color: white; color: blue;");
                }
                else if (segmentWinStatus[row / 3][col / 3] == 'X'){
                    highlightWinningSegments(row, col);
                    button->setText("-");
                    button->setStyleSheet("background-color: blue; color: white;");
                }
                else{
                    highlightWinningSegments(row, col);
                    button->setText("-");
                    button->setStyleSheet("background-color: red; color: white;");}

            } else {
                if (segmentWinStatus[row / 3][col / 3] == ' '){
                    button->setText("-");
                    button->setStyleSheet("background-color: white; color: red;");
                }
                else if (segmentWinStatus[row / 3][col / 3] == 'X'){
                    highlightWinningSegments(row, col);
                    button->setText("-");
                    button->setStyleSheet("background-color: blue; color: white;");
                }
                else{
                    highlightWinningSegments(row, col);
                    button->setText("-");
                    button->setStyleSheet("background-color: red; color: white;");}
            }
        }
    }
    bool checkwin(char player){
        for (int i = 0; i < 3; ++i) {
            if ((segmentWinStatus[i][0] == player && segmentWinStatus[i][1] == player && segmentWinStatus[i][2] == player) ||
                (segmentWinStatus[0][i] == player && segmentWinStatus[1][i] == player && segmentWinStatus[2][i] == player)) {
                return true;
            }
        }
        if ((segmentWinStatus[0][0] == player && segmentWinStatus[1][1] == player && segmentWinStatus[2][2] == player) ||
            (segmentWinStatus[0][2] == player && segmentWinStatus[1][1] == player && segmentWinStatus[2][0] == player)) {
            return true;
        }

        return false;
    }

    void ispath (int row, int col){
        int segmentRow = row / 3 * 3;
        int segmentCol = col / 3 * 3;

        for (int i = segmentRow; i < segmentRow + 3; ++i) {
            for (int j = segmentCol; j < segmentCol + 3; ++j) {
                if (!isfull(i%3 * 3, j%3 * 3)){
                    if (turnX) {
                        if (segmentWinStatus[row / 3][col / 3] == ' '){
                            buttons[i][j]->setText("-");
                            buttons[i][j]->setStyleSheet("background-color: white; color: blue;");
                        }
                        else if (segmentWinStatus[row / 3][col / 3] == 'X'){
                            highlightWinningSegments(row, col);
                            buttons[i][j]->setText("-");
                            buttons[i][j]->setStyleSheet("background-color: blue; color: white;");
                        }
                        else{
                            highlightWinningSegments(row, col);
                            buttons[i][j]->setText("-");
                            buttons[i][j]->setStyleSheet("background-color: red; color: white;");}

                    } else {
                        if (segmentWinStatus[row / 3][col / 3] == ' '){
                            buttons[i][j]->setText("-");
                            buttons[i][j]->setStyleSheet("background-color: white; color: red;");
                        }
                        else if (segmentWinStatus[row / 3][col / 3] == 'X'){
                            highlightWinningSegments(row, col);
                            buttons[i][j]->setText("-");
                            buttons[i][j]->setStyleSheet("background-color: blue; color: white;");
                        }
                        else{
                            highlightWinningSegments(row, col);
                            buttons[i][j]->setText("-");
                            buttons[i][j]->setStyleSheet("background-color: red; color: white;");}
                    }
                }
            }
        }
    }

    bool isfull(int row, int col) {
        int segmentRow = row / 3 * 3;
        int segmentCol = col / 3 * 3;

        for (int i = segmentRow; i < segmentRow + 3; ++i) {
            for (int j = segmentCol; j < segmentCol + 3; ++j) {
                if (buttons[i][j]->text().isEmpty() and !(i%3 * 3 == row and j%3 * 3 == col)){
                    return true;
                }
            }
        }
        return false;
    }
    bool checkWinInSegment(int row, int col) {
        int segmentRow = row / 3 * 3;
        int segmentCol = col / 3 * 3;
        if (segmentWinStatus[segmentRow / 3][segmentCol / 3] != ' ') {
            return false;
        }

        return checkHorizontalWinInSegment(segmentRow, segmentCol) ||
               checkVerticalWinInSegment(segmentRow, segmentCol) ||
               checkDiagonalWinInSegment(segmentRow, segmentCol);
    }

    void highlightSegment(int row, int col, bool f) {
        int segmentRow = row / 3 * 3;
        int segmentCol = col / 3 * 3;

        for (int i = segmentRow; i < segmentRow + 3; ++i) {
            for (int j = segmentCol; j < segmentCol + 3; ++j) {
                if (f){
                    if (buttons[i][j]->text() == "X"){
                        buttons[i][j]->setStyleSheet("background-color: lightgreen; color: blue;");
                    }
                    else{
                        buttons[i][j]->setStyleSheet("background-color: lightgreen; color: red;");

                    }
                    if (buttons[i][j]->text().isEmpty()){
                        buttons[i][j]->setEnabled(true);}
                }else{
                    if (buttons[i][j]->text() == "X"){
                        buttons[i][j]->setStyleSheet("background-color: white; color: blue;");
                    }
                    else{
                        buttons[i][j]->setStyleSheet("background-color: white; color: red;");

                    }

                }
            }
        }
    }
    void highlightWinningSegments(int row, int col) {
        int segmentRow = row / 3 * 3;
        int segmentCol = col / 3 * 3;

        for (int i = segmentRow; i < segmentRow + 3; ++i) {
            for (int j = segmentCol; j < segmentCol + 3; ++j) {
                if ((turnX && segmentWinStatus[row / 3][col / 3] != 'O') || segmentWinStatus[row / 3][col / 3] == 'X') {
                    buttons[i][j]->setStyleSheet("background-color: blue;color: white;");
                } else {
                    buttons[i][j]->setStyleSheet("background-color: red;color: white;");
                }
            }
        }
    }

    bool checkHorizontalWinInSegment(int startRow, int startCol) {
        for (int i = startRow; i < startRow + 3; ++i) {
            if (checkHorizontalWin(i, startCol, startCol + 2)) {
                return true;
            }
        }
        return false;
    }

    bool checkVerticalWinInSegment(int startRow, int startCol) {
        for (int i = startCol; i < startCol + 3; ++i) {
            if (checkVerticalWin(startRow, i, startRow + 2)) {
                return true;
            }
        }
        return false;
    }

    bool checkDiagonalWinInSegment(int startRow, int startCol) {
        return checkDiagonalWin(startRow, startCol, startRow + 2, startCol + 2) || revcheckDiagonalWin(startRow, startCol, startRow+2, startCol-2);
    }

    void resetGame() {
        for (int row = 0; row < 9; ++row) {
            for (int col = 0; col < 9; ++col) {
                buttons[row][col]->setText("");
                buttons[row][col]->setStyleSheet("background-color: white;");
                buttons[row][col]->setEnabled(true);
            }
        }
        turnX = true;
        gameOver = false;
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < 3; ++j) {
                segmentWinStatus[i][j] = ' ';
            }
        }
    }

    bool checkHorizontalWin(int row, int startCol, int endCol) {
        for (int i = startCol; i < endCol; ++i) {
            if (buttons[row][i]->text() != buttons[row][i + 1]->text() || buttons[row][i]->text().isEmpty()) {
                return false;
            }
        }
        return true;
    }

    bool checkVerticalWin(int startRow, int col, int endRow) {
        for (int i = startRow; i < endRow; ++i) {
            if (buttons[i][col]->text() != buttons[i + 1][col]->text() || buttons[i][col]->text().isEmpty()) {
                return false;
            }
        }
        return true;
    }

    bool checkDiagonalWin(int startRow, int startCol, int endRow, int endCol) {
        for (int i = startRow, j = startCol; i < endRow && j < endCol; ++i, ++j) {
            if (buttons[i][j]->text() != buttons[i + 1][j + 1]->text() || buttons[i][j]->text().isEmpty()) {
                return false;
            }
        }
        return true;
    }
    bool revcheckDiagonalWin(int startRow, int startCol, int endRow, int endCol) {
        for (int i = startRow, j = startCol; i < endRow && j >endCol; ++i, --j) {
            if (buttons[i][j+2]->text() != buttons[i+1][j+1]->text() || buttons[i][j+2]->text().isEmpty()) {
                return false;
            }
        }
        return true;
    }

    bool checkTie() {
        for (int row = 0; row < 9; ++row) {
            for (int col = 0; col < 9; ++col) {
                if (buttons[row][col]->isEnabled()) {
                    return false;
                }
            }
        }
        return true;
    }

    bool turnX = true;
    bool gameOver = false;
    QPushButton *buttons[9][9];
    char segmentWinStatus[3][3];
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    TicTacToe game;
    game.setWindowTitle("Гига-нолики");
    game.setGeometry(100, 100, 600, 600);
    game.show();

    return app.exec();
}

#include "main.moc"
