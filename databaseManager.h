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
    Budget getBudget(const QString& name);
    QList<Budget> getAllBudgets();
    QList<Item> getItemsByBudget(int budgetID);
    Item getItem(const QString& name);
    bool updateBudget(const Budget& budget);
    bool updateItem(const Item& item);
    bool deleteBudget(const QString& budgetName);
    bool deleteItem(int itemID);
    QSqlDatabase& getDatabase(){ return db;}
    int getBudgetIdByItem(const Item& item);
    int getItemID(const QString& name);
    bool updateItemTotal(double Transaction, const Item& item);
    bool updateBudgetTotal(double newTotal, const Budget& budget);
};

#endif // DATABASEMANAGEMENT_H
