//Written by JordanG13
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWidgets>
#include <QFile>

const QString runNitrogen = "nitrogen --set-zoom --restore";
const QString GNOMESettings = " --force-setter=gnome;";
const QString XWindowsSettings = " --force-setter=xwindows;";
const QString noSelect = "Nothing Selected";
const QString favParam = "WALLPAPERFAV=";
const QString picsParam = "WALLPAPERDIR=";
const QString configParam = "CONFIG=";
const QString gnomeParam = "GNOMEDIR=\"/usr/share/backgrounds/gnome\"";
const QString scriptLoc = "./dual_monitor_nitrogen";

bool failed = false;
bool GNOME = false;
bool XWindows = false;
bool Other = true;
QString favPath;
QString picsPath;
QString configPath;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->title_label_3->hide();
    ui->message_label_3->hide();
    ui->backButton_3->hide();
    ui->finishButton_3->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

//NOTE: true if error, false if not
bool MainWindow::noPermissions(const QString file)
{
    QFile inputFile(file), orig(":/script/dual_monitor_nitrogen");
    QMessageBox permissions;

    if (inputFile.exists())
    {
        if (!inputFile.setPermissions(QFileDevice::ReadOwner | QFileDevice::WriteOwner | QFileDevice::ExeOwner))
        {
            permissions.setWindowTitle(tr("Error"));
            permissions.setText(tr("Please set file permissions to R/W for: ") + file);
            permissions.exec();
            return true;
        }
        else
            return false;
    }
    else
    {
        if (!orig.copy(file))
        {
            if (!inputFile.exists())
            {
                permissions.setWindowTitle(tr("Error"));
                permissions.setText(tr("Could not copy script to current directory (permission issue?)"));
                permissions.exec();
                return true;
            }
            else
            {
                if (!inputFile.setPermissions(QFileDevice::ReadOwner | QFileDevice::WriteOwner | QFileDevice::ExeOwner))
                {
                    permissions.setWindowTitle(tr("Error"));
                    permissions.setText(tr("Permission denied to open: ") + file);
                    permissions.exec();
                    return true;
                }
                else
                    return false;
            }
        }
        else
        {
            if (!inputFile.setPermissions(QFileDevice::ReadOwner | QFileDevice::WriteOwner | QFileDevice::ExeOwner))
            {
                permissions.setWindowTitle(tr("Error"));
                permissions.setText(tr("Permission denied to open: ") + file);
                permissions.exec();
                return true;
            }
            else
                return false;
        }
    }
}

//NOTE: false if error, true if not
bool MainWindow::createRunnerScript()
{
    QFile orig(":/script/runner"), runner("./runner");
    QMessageBox permissions;

    if (!orig.copy("./runner"))
    {
        if (!runner.exists())
        {
            permissions.setWindowTitle(tr("Error"));
            permissions.setText(tr("Could not copy runner script to current directory (permission issue?)"));
            permissions.exec();
            return false;
        }
        else
        {
            if (!runner.setPermissions(QFileDevice::ReadOwner | QFileDevice::WriteOwner | QFileDevice::ExeOwner))
            {
                permissions.setWindowTitle(tr("Error"));
                permissions.setText(tr("Permission denied to change runner script permissions."));
                permissions.exec();
                return false;
            }
            else
                return true;
        }
    }
    else
    {
        if (!runner.setPermissions(QFileDevice::ReadOwner | QFileDevice::WriteOwner | QFileDevice::ExeOwner))
        {
            permissions.setWindowTitle(tr("Error"));
            permissions.setText(tr("Permission denied to change runner script permissions."));
            permissions.exec();
            return false;
        }
        else
            return true;
    }
}

bool MainWindow::setFileParam(const QString file, const QString param, const QString replacement)
{
    QFile inputFile(file);
    QTextStream inStream(&inputFile), outStream(&inputFile);
    QString line;

    if (!noPermissions(file))
    {
        inputFile.open(QFile::ReadWrite | QFile::Text);

        while (!inStream.atEnd())
        {
            line = inStream.readLine();

            if (line.contains(param))
                line.replace(0, replacement.length(), replacement);

            outStream << line << "\n";
        }

        line.remove(0, 19); //BUG: output file has 5 "done" statements at beginning, remove them

        inputFile.seek(0);
        inputFile.write(line.toUtf8());
        inputFile.close();
        return true;
    }
    else
        return false;
}

bool MainWindow::checkDE()
{
    if (Other)
    {
        if (setFileParam(scriptLoc, gnomeParam, "#" + gnomeParam))
            if (setFileParam(scriptLoc, runNitrogen, runNitrogen + ";"))
                return true;
            else
                return false;
        else
            return false;
    }
    else if (XWindows)
    {
        if (setFileParam(scriptLoc, gnomeParam, "#" + gnomeParam))
            if (setFileParam(scriptLoc, runNitrogen, runNitrogen + XWindowsSettings))
                return true;
            else
                return false;
        else
            return false;
    }
    else
    {
        if (setFileParam(scriptLoc, "#" + gnomeParam, gnomeParam))
            if (setFileParam(scriptLoc, runNitrogen, runNitrogen + GNOMESettings))
                return true;
            else
                return false;
        else
            return false;
    }
}

void MainWindow::on_nextButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_backButton_1_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_gnomeButton_1_toggled(bool checked)
{
    if (checked)
        GNOME = true;
    else
        GNOME = false;
}

void MainWindow::on_xButton_1_toggled(bool checked)
{
    if (checked)
        XWindows = true;
    else
        XWindows = false;
}

void MainWindow::on_otherButton_1_toggled(bool checked)
{
    if (checked)
        Other = true;
    else
        Other = false;
}

void MainWindow::on_nextButton_1_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_backButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_favButton_2_clicked()
{
    QString favFolderName = QFileDialog::getExistingDirectory(this, tr("Set Favorites Folder"), tr("/home/"), QFileDialog::ShowDirsOnly);

    if (!favFolderName.isNull() || !favFolderName.isEmpty())
    {
        favPath = favParam + "\"" + favFolderName + "\"";
        ui->favButton_2->setText(favFolderName);
    }
    else
    {
        ui->favButton_2->setText(noSelect);
    }
}

void MainWindow::on_picsButton_2_clicked()
{
    QString picsFolderName = QFileDialog::getExistingDirectory(this, tr("Set Pictures Folder"), tr("/home/"), QFileDialog::ShowDirsOnly);

    if (!picsFolderName.isNull() || !picsFolderName.isEmpty())
    {
        picsPath = picsParam + "\"" + picsFolderName + "\"";
        ui->picsButton_2->setText(picsFolderName);
    }
    else
    {
        ui->picsButton_2->setText(noSelect);
    }
}

void MainWindow::on_configButton_2_clicked()
{
    QString configFileName = QFileDialog::getOpenFileName(this, tr("Select the bg-saved config file"), tr("/home/"), tr("Config Files (*.cfg)"));

    if (!configFileName.isNull() || !configFileName.isEmpty())
    {
        configPath = configParam + "\"" + configFileName + "\"";
        ui->configButton_2->setText(configFileName);
    }
    else
    {
        ui->configButton_2->setText(noSelect);
    }
}

void MainWindow::on_nextButton_2_clicked()
{
    QMessageBox continueMessageBox;
    continueMessageBox.setWindowTitle(tr("Before continuing..."));

    if (ui->picsButton_2->text() == noSelect)
    {
        continueMessageBox.setText(tr("Please be sure to select the pictures folder!"));
        continueMessageBox.exec();
    }
    else if (ui->favButton_2->text() == noSelect)
    {
        continueMessageBox.setText(tr("Please be sure to select the favorites folder!"));
        continueMessageBox.exec();
    }
    else if (ui->configButton_2->text() == noSelect)
    {
        continueMessageBox.setText(tr("Please be sure to select the bg-saved config file!"));
        continueMessageBox.exec();
    }
    else
    {
        if (!checkDE())
            return;
        else if (!setFileParam(scriptLoc, favParam, favPath))
            return;
        else if (!setFileParam(scriptLoc, picsParam, picsPath))
            return;
        else if (!setFileParam(scriptLoc, configParam, configPath))
            return;
        else if (!createRunnerScript())
            return;
        else
            ui->stackedWidget->setCurrentIndex(3);
    }
}

void MainWindow::on_backButton_3_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
    ui->title_label_3->hide();
    ui->message_label_3->hide();
    ui->backButton_3->hide();
    ui->finishButton_3->hide();
    ui->progressBar_3->show();
    ui->progress_label_3->show();
}

void MainWindow::on_stackedWidget_currentChanged(int arg1)
{
    if (arg1 == 3)
    {
        for (int var = 0; var <= 4; ++var)
        {
            if (var > 3)
            {
                ui->title_label_3->show();
                ui->message_label_3->show();
                ui->progressBar_3->hide();
                ui->progress_label_3->hide();
                ui->finishButton_3->show();
                ui->backButton_3->show();

                break;
            }

            ui->progressBar_3->setValue(var);

            QThread::sleep(1);
        }
    }
}
