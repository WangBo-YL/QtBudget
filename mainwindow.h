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
#include <QDateEdit>
#include "databaseManager.h"

enum mode{NEW, EDIT};

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
    void updateCurrentBudget(const QString& budgetName);
    void updateItemBox();
    void updateItemListWidget();
    void updateTransactionTableWidget();
    void updateSavingBox();
    void updateSavingListWidget();
    void addIncomeToTable(const income& income);
    void addExpenseToTable(const Expense& expense);

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

    // void on_expenseDateEdit_userDateChanged(const QDate &date);

    void on_budgetListWidget_itemClicked(QListWidgetItem *item);

    // void on_budgetListWidget_itemSelectionChanged();

    void on_itemListWidget_itemClicked(QListWidgetItem *item);

    void on_savingsListWidget_itemClicked(QListWidgetItem *item);

private:
    Ui::MainWindow *ui;
    QListWidget* savingListWidget;
    QListWidget* budgetListWidget;
    QListWidget* transactionListWidget;
    QListWidget* itemListWidget;
    QListWidgetItem* budgetListWidgetItem;
    QListWidgetItem* itemListWidgetItem;
    QListWidgetItem* savingListWidgetItem;
    QListWidgetItem* transactionListWidgetItem;
    databaseManager db;
    Budget currentBudget;
    Item currentItem;
    Expense expense;
    int currentPageIndex;
    int previousPageIndex;
};
#endif // MAINWINDOW_H
