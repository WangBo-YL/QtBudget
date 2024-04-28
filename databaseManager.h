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
    bool updateBudget(const Budget& budget, const QList<Item>& items);
    bool updateItem(const Item& item, bool updateBudgetTotal);
    bool updateBudgetTotalAmount(int budgetID);
    bool deleteBudget(int budgetID);
    bool deleteItem(int itemID);
};

#endif // DATABASEMANAGEMENT_H
