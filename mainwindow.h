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

enum mode{NEW, EDIT};
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
    void updateItemList(int currentBudgetID);
    void updateBudgetList();
    void updateBudgetBox();

private slots:

    void on_budgetMenuButton_clicked();

    void on_expenseMenuButton_clicked();

    void on_savingsMenuButton_clicked();

    void on_transactionHistoryButton_clicked();

    void on_reportMenuButton_clicked();

    void on_returnFromBudgetMenuButton_clicked();

    void on_newBudgetButton_clicked();

    void on_budgetListButton_clicked();

    void on_newItemButton_clicked();

    void on_cancelCreateBudgetButton_clicked();

    void on_saveBudgetButton_clicked();

    void on_returnFromBudgetListButton_clicked();

    void on_budgetDetaiButton_clicked();

    void on_editBudgetButton_clicked();

    void on_deleteBudgetButton_clicked();

    void on_cancelAddItemButton_clicked();

    void on_addItemButton_clicked();

    void on_cancelExpenseButton_clicked();

    void on_confirmExpenseButton_clicked();

    void on_returnFromExpenseMenuButton_clicked();

    void on_addExpenditureButton_clicked();

    void on_returnFromItemListButton_clicked();

    void on_editItemButton_clicked();

    void on_deleteItemButton_clicked();

    void on_cancelAddIncomeButton_clicked();

    void on_finishAddIncomeButton_clicked();

    void on_cancelSavingPlanButton_clicked();

    void on_finishAddSavingPlanButton_clicked();

    void on_returnFromReportPageButton_clicked();

    void on_returnFromSavingsListButton_clicked();

    void on_editSavingListButton_clicked();

    void on_deleteSavingsListButton_clicked();

    void on_returnFromSavingButton_clicked();

    void on_newSavingPlanButton_clicked();

    void on_savingsListButton_clicked();

    void on_addIncomeButton_clicked();

    void on_returnFromTransPage_clicked();


private:
    Ui::MainWindow *ui;
    databaseManager db;
    Budget currentBudget;
    Item currentItem;
    int currentPageIndex;
    int previousPageIndex;
};
#endif // MAINWINDOW_H
