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
    Budget getBudget(int budgetID);
    QList<Budget> getAllBudgets();
    QList<Item> getItemsByBudget(int budgetID);
    Item getItem(int itemID);
    bool updateBudget(const Budget& budget);
    bool updateItem(const Item& item, int currentBudgetID);
    bool deleteBudget(int budgetID);
    bool deleteItem(int itemID);
    int getBudgetIDByName(const QString& name);
};

#endif // DATABASEMANAGEMENT_H
