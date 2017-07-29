#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool noPermissions(const QString file);

private slots:
    bool createRunnerScript();

    bool setFileParam(const QString file, const QString param, const QString replacement);

    bool checkDE();

    void on_nextButton_clicked();

    void on_backButton_1_clicked();

    void on_gnomeButton_1_toggled(bool checked);

    void on_xButton_1_toggled(bool checked);

    void on_otherButton_1_toggled(bool checked);

    void on_nextButton_1_clicked();

    void on_backButton_2_clicked();

    void on_favButton_2_clicked();

    void on_picsButton_2_clicked();

    void on_configButton_2_clicked();

    void on_nextButton_2_clicked();

    void on_backButton_3_clicked();

    void on_stackedWidget_currentChanged(int arg1);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
