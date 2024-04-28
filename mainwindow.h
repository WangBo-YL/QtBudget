#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QString>
#include <QVector>
#include <QDoubleValidator>
#include <QIntValidator>
#include <QComboBox>
#include <QTableView>
#include <QListWidget>
#include <QListWidgetItem>
#include <QSqlTableModel>
#include <QStandardItemModel>
#include <QInputDialog>
#include "databaseManager.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
     MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    void navigateToPage(int pageIndex);
    void returnToHomepage();
    void updateItemList();
    void updateBudgetList();

private slots:

    void on_BudgetButton_clicked();

    void on_returnButton_clicked();

    void on_NewBudgetButton_clicked();

    void on_cancelButton_clicked();

    void on_createButton_clicked();

    void on_addItemButton_clicked();

    void on_saveItemButton_clicked();

    void on_ExistingBudgetButton_clicked();

    void on_returnButton_2_clicked();


    void on_budgetListWidget_itemClicked(QListWidgetItem *item);

    void on_exitButton_clicked();

private:
    Ui::MainWindow *ui;
    databaseManager db;
    Budget* currentBudget;
};
#endif // MAINWINDOW_H
