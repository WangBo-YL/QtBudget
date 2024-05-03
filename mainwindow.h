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


//一些关于new item的思考， 设计思路， 如果Dropbox有内容，则可以新建item，并选择新建到哪个budget，
//如果没有budget， 在尝试保存的时候弹出提示框，并且失败。
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
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    void navigateToPage(int pageIndex);
    void returnToHomepage();
    void updateItemList(int currentBudgetID);
    void updateBudgetList();
    int getBudgetID(const Item& item);
    int findItemIndex(QListWidgetItem* clickedItem);

private slots:
    void on_itemSelected(QListWidgetItem* item);

    void on_BudgetButton_clicked();

    void on_returnButton_clicked();

    void on_NewBudgetButton_clicked();

    void on_cancelButton_clicked();

    void on_createButton_clicked();

    void on_addItemButton_clicked();

    void on_saveItemButton_clicked();

    void on_ExistingBudgetButton_clicked();

    void on_returnButton_2_clicked();

    void on_budgetListWidget_itemClicked();

    void on_exitButton_clicked();

    void on_editItemButton_clicked();

    void on_editItemSaveButton_clicked();

    void on_cancelEditItemButton_clicked();

    void on_budgetSelected(int budgetID);

    void on_editBudgetButton_clicked();

    void on_deleteItemButton_clicked();

private:
    Ui::MainWindow *ui;
    databaseManager db;
    int currentBudgetID;
    QList<Budget> budgets;
    QList<Item> items;
    QListWidget* listWidget;
    QListWidgetItem* itemListWidget;
    QListWidgetItem* budgetListWidget;
};
#endif // MAINWINDOW_H
