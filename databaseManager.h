#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QVariant>
#include <QDebug>
#include <QtSql/QSqlError>
#include <QStandardPaths>
#include "budget.h"
#include "item.h"
#include "expense.h"
#include "saving.h"
#include "expense.h"
#include "income.h"
class databaseManager
{
private:
    QSqlDatabase db;
public:
    databaseManager(const QString& path);
    ~databaseManager();
    void initializeDatabase();
    bool addItem(const Item& item);
    bool addBudget(const Budget& budget);
    Budget getBudget(const QString& name);
    Saving getSaving(int savingID);
    QList<Budget> getAllBudgets();
    QList<Item> getItemsByBudget(int budgetID);
    QList<Item> getAllItems();
    QList<QString> getItemNamesByBudget(int budgetID);
    QList<QString> getAllItemNames();
    QList<QString> getAllBudgetNames();
    QList<Saving> getAllSavingPlans();
    QList<Expense> getAllExpenses();
    QList<income> getAllIncomes();
    Item getItem(const QString& name);
    bool updateBudgetRemaining(const Item& item);
    bool updateItemTotal(const Expense& expense);
    bool updateBudget(const Budget& budget, int budgetID);
    bool updateItem(int id,const QString& name, double cap, double total);
    bool deleteBudget(int budgetID);
    bool deleteItem(int itemID);
    bool deleteSaving(int savingID);
    bool updateSaving(const Saving& saving, int savingID);
    QSqlDatabase& getDatabase(){ return db;}
    int getBudgetIdByItem(const Item& item);
    int getBudgetIDByName(const QString& name);
    int getItemID(const QString& name);
    int getSavingID(const QString& savingName);
    bool updateItemTotal(double Transaction, const Item& item);
    bool updateBudgetTotal(double spend, const Budget& budget);
    bool addExpense(const Expense& expense);
    bool addSavingPlan(const Saving& saving);
    bool addIncome(const income& income);
};

#endif // DATABASEMANAGEMENT_H
