#include"databaseManager.h"

databaseManager::databaseManager(const QString& path)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(path);
    db.open();
    if(!db.open())
    {
        qDebug() << "Error: Unable to open database" << path;
        qDebug() << "Error Message:" << db.lastError().text();
        throw std::runtime_error("Database connection failed");
    }
    else
    {
        qDebug() << "Database connected successfully!";
        initializeDatabase();
    }
}

databaseManager::~databaseManager()
{
    if(db.isOpen())
    {
        db.close();
    }
}


void databaseManager::initializeDatabase()
{
    QSqlQuery query;

    // create budget table
    bool isBuilt = query.exec("CREATE TABLE IF NOT EXISTS budget ("
                              "budgetID INTEGER PRIMARY KEY AUTOINCREMENT, "
                              "budgetName VARCHAR(100) NOT NULL, "
                              "total DOUBLE NOT NULL, "
                              "remainingAmount DOUBLE NOT NULL);");

    if (isBuilt) {
        qDebug() << "Budget table created or already exists";
    } else {
        qDebug() << "Failed to create budget table:" <<                 query.lastError().text();
    }

    // create item table
    isBuilt = query.exec("CREATE TABLE IF NOT EXISTS item ("
                         "itemID INTEGER PRIMARY KEY AUTOINCREMENT, "
                         "budgetID INTEGER NOT NULL, "
                         "itemName VARCHAR(100) NOT NULL, "
                         "cap DOUBLE(10,2) NOT NULL CHECK (cap <= 100), "
                         "itemTotal DOUBLE(10,2) NOT NULL, "
                         "FOREIGN KEY (budgetID) REFERENCES budget(budgetID))");

    if (isBuilt) {
        qDebug() << "Item table created or already exists";
    } else {
        qDebug() << "Failed to create item table:" << query.lastError().text();
    }

    // create category table
    isBuilt = query.exec("CREATE TABLE IF NOT EXISTS category ("
                         "categoryID INTEGER PRIMARY KEY AUTOINCREMENT, "
                         "budgetID INTEGER NOT NULL, "
                         "categoryName VARCHAR(100) NOT NULL, "
                         "totalSpent DOUBLE NOT NULL, "
                         "FOREIGN KEY (budgetID) REFERENCES budget(budgetID))");

    if (isBuilt) {
        qDebug() << "Category table created or already exists";
    } else {
        qDebug() << "Failed to create category table:" << query.lastError().text();
    }

}


bool databaseManager::addItem(const Item& item)
{
    if (!db.isOpen()) {
        qDebug() << "Database not open";
        return false;
    }

    QSqlQuery query(db);
    db.transaction();  // Start transaction

    // First, insert the new item
    query.prepare("INSERT INTO item (budgetID, itemName, cap, itemTotal) VALUES (?, ?, ?, ?)");
    query.addBindValue(item.getBudgetID());
    query.addBindValue(item.getName());
    query.addBindValue(item.getCap());
    query.addBindValue(item.getTotal());

    if (!query.exec()) {
        db.rollback();  // Rollback if execute failed
        qDebug() << "Failed to add item:" << query.lastError();
        return false;
    }

    // Second, update the total amount of the budget
    query.prepare("UPDATE budget SET remainingAmount = total - ? WHERE budgetID = ?");
    query.addBindValue(item.getTotal());
    query.addBindValue(item.getBudgetID());

    if (!query.exec()) {
        db.rollback();  // Rollback if update failed
        qDebug() << "Failed to update budget total:" << query.lastError();
        return false;
    }

    db.commit();  // Success and commit
    return true;
}



bool databaseManager::addBudget(const Budget& budget)
{
    if (!db.isOpen()) {
        qDebug() << "Database not open";
        return false;
    }

    QSqlQuery checkQuery(db);
    checkQuery.prepare("SELECT COUNT(*) FROM budget WHERE budgetName = ?");
    checkQuery.addBindValue(budget.getName());
    if (!checkQuery.exec()) {
        qDebug() << "Failed to check for existing budget:" << checkQuery.lastError();
        return false;
    }
    if (checkQuery.next() && checkQuery.value(0).toInt() > 0) {
        qDebug() << "A budget with the same name already exists.";
        return false;
    }

    QSqlQuery query(db);
    db.transaction();

    query.prepare("INSERT INTO budget (budgetName, total) VALUES (?, ?)");
    query.addBindValue(budget.getName());
    query.addBindValue(budget.getTotal());

    if (!query.exec()) {
        db.rollback();
        qDebug() << "Failed to add budget" << query.lastError();
        return false;
    } else {
        db.commit();
        return true;
    }
}

Budget databaseManager::getBudget (int budgetID)
{
    if (!db.isOpen()) {
        qDebug() << "Database not open";
        throw std::runtime_error("Database is not open.");
    }

    QSqlQuery query(db);
    query.prepare("SELECT budgetID, budgetName, total FROM budget WHERE budgetID = ?");
    query.addBindValue(budgetID);

    if (!query.exec()) {
        qDebug() << "Failed to retrieve budget:" << query.lastError();
        throw std::runtime_error("Query failed.");
    }

    if (query.next()) {
        int id = query.value(0).toInt();
        QString name = query.value(1).toString();
        double totalAmount = query.value(2).toDouble();
        return Budget(id, name, totalAmount);
    } else {
        throw std::runtime_error("Budget not found.");
    }
}

QList<Budget> databaseManager::getAllBudgets()
{
    QList<Budget> budgets;
    if(!db.isOpen())
    {
        qDebug() << "Database not open";
        throw std::runtime_error("Database is not open.");
    }

    QSqlQuery query(db);
    query.prepare("SELECT budgetID, budgetName, total FROM budget");

    if (!query.exec()) {
        qDebug() << "Failed to retrieve budgets:" << query.lastError();
        throw std::runtime_error("Query failed.");
    }

    while (query.next()) {
        int id = query.value(0).toInt();
        QString name = query.value(1).toString();
        double totalAmount = query.value(2).toDouble();
        budgets.append(Budget(id, name, totalAmount));
    }
    return budgets;
}

QList<Item> databaseManager::getItemsByBudget(int budgetID)
{
    QList<Item> items;
    if (!db.isOpen()) {
        qDebug() << "Database not open";
        throw std::runtime_error("Database is not open.");
    }

    QSqlQuery query(db);
    query.prepare("SELECT itemName, amount, categoryID FROM item WHERE budgetID = ?");
    query.addBindValue(budgetID);

    if (!query.exec()) {
        qDebug() << "Failed to retrieve items:" << query.lastError();
        throw std::runtime_error("Query failed.");
    }

    while (query.next()) {
        QString name = query.value(0).toString();
        double amount = query.value(1).toDouble();
        int categoryId = query.value(2).toInt();
        items.append(Item(name, amount, categoryId));
    }
    return items;
}



bool databaseManager::updateItem(const Item& item)
{
    if (!db.isOpen()) {
        qDebug() << "Database not open";
        return false;
    }

    // Get the current amount of the item
    double currentAmount = 0;
    QSqlQuery query(db);
    query.prepare("SELECT amount, budgetID FROM item WHERE itemID = ?");
    query.addBindValue(item.getItemID());
    if (!query.exec() || !query.next()) {
        qDebug() << "Failed to fetch current item amount:" << query.lastError();
        return false;
    }
    currentAmount = query.value(0).toDouble();
    int currentBudgetID = query.value(1).toInt();

    // Update the item
    db.transaction();
    query.prepare("UPDATE item SET itemName = ?, amount = ?, categoryID = ? WHERE itemID = ?");
    query.addBindValue(item.getName());
    query.addBindValue(item.getAmount());
    query.addBindValue(item.getCategoryID());
    query.addBindValue(item.getItemID());

    if (!query.exec()) {
        db.rollback();
        qDebug() << "Failed to update item:" << query.lastError();
        return false;
    }

    // Check if the amount has changed
    double amountDifference = item.getAmount() - currentAmount;
    if (amountDifference != 0) {
        // Update the total amount of the budget
        query.prepare("UPDATE budget SET total = total + ? WHERE budgetID = ?");
        query.addBindValue(amountDifference);
        query.addBindValue(currentBudgetID);

        if (!query.exec()) {
            db.rollback();  // Rollback if budget update failed
            qDebug() << "Failed to update budget total:" << query.lastError();
            return false;
        }
    }

    db.commit();
    return true;
}


bool databaseManager::updateBudget(const Budget& budget) {
    if (!db.isOpen()) {
        qDebug() << "Database not open";
        return false;
    }

    QSqlQuery query(db);
    db.transaction();  // Start transaction

    query.prepare("UPDATE budget SET budgetName = ?, total = ? WHERE budgetID = ?");
    query.addBindValue(budget.getName());
    query.addBindValue(budget.getTotal());
    query.addBindValue(budget.getBudgetID());

    if (!query.exec()) {
        qDebug() << "Failed to update budget:" << query.lastError();
        db.rollback();  // Rollback transaction if update failed
        return false;
    }

    db.commit();  // Commit the transaction
    return true;
}



bool databaseManager::deleteItem(int itemID) {
    if (!db.isOpen()) {
        qDebug() << "Database not open";
        return false;
    }

    QSqlQuery query(db);
    db.transaction();

    // Retrieve the item details needed for budget update
    int budgetID;
    double itemAmount;
    query.prepare("SELECT budgetID, amount FROM item WHERE itemID = ?");
    query.addBindValue(itemID);
    if (query.exec() && query.next()) {
        budgetID = query.value(0).toInt();
        itemAmount = query.value(1).toDouble();
    } else {
        qDebug() << "Failed to find item for deletion:" << query.lastError();
        db.rollback();
        return false;
    }

    // Delete the item
    query.prepare("DELETE FROM item WHERE itemID = ?");
    query.addBindValue(itemID);
    if (!query.exec()) {
        qDebug() << "Failed to delete item:" << query.lastError();
        db.rollback();
        return false;
    }

    // Update the total amount of the budget
    query.prepare("UPDATE budget SET total = total - ? WHERE budgetID = ?");
    query.addBindValue(itemAmount);
    query.addBindValue(budgetID);
    if (!query.exec()) {
        qDebug() << "Failed to update budget total after item deletion:" << query.lastError();
        db.rollback();
        return false;
    }

    db.commit();
    return true;
}


bool databaseManager::deleteBudget(const QString& budgetName) {
    if (!db.isOpen()) {
        qDebug() << "Database not open";
        return false;
    }

    QSqlQuery query(db);
    db.transaction();


    query.prepare("DELETE FROM item WHERE budgetName = ?");
    query.addBindValue(budgetName);
    if (!query.exec()) {
        qDebug() << "Failed to delete item for budget:" << query.lastError();
        db.rollback();
        return false;
    }


    query.prepare("DELETE FROM budget WHERE budgetName = ?");
    query.addBindValue(budgetName);
    if (!query.exec()) {
        qDebug() << "Failed to delete budget:" << query.lastError();
        db.rollback();
        return false;
    }

    db.commit();
    return true;
}


Item databaseManager::getItem(int itemID)
{
    if (!db.isOpen()) {
        qDebug() << "Database not open";
        throw std::runtime_error("Database is not open.");
    }

    QSqlQuery query(db);
    query.prepare("SELECT itemID, budgetID, itemName, amount, categoryID FROM item WHERE itemID = ?");
    query.addBindValue(itemID);

    if (!query.exec()) {
        qDebug() << "Failed to retrieve item:" << query.lastError().text();
        throw std::runtime_error("Query failed to execute.");
    }

    if (query.next()) {
        int itemID = query.value(0).toInt();
        int budgetID = query.value(1).toInt();
        QString name = query.value(2).toString();
        double amount = query.value(3).toDouble();
        int categoryID = query.value(4).toInt();

        // Assuming Item has a constructor that takes these parameters
        return Item(itemID, budgetID, name, amount, categoryID);
    } else {
        qDebug() << "Item not found with ID:" << itemID;
        throw std::runtime_error("Item not found.");
    }
}


int databaseManager::getBudgetIdByItem(int itemID)
{
    if (!db.isOpen()) {
        qDebug() << "Database not open";
        throw std::runtime_error("Database is not open.");
    }


    QSqlQuery query(db);
    query.prepare("SELECT budgetID FROM item WHERE itemID = ?");
    query.addBindValue(itemID);

    if (query.exec() && query.next()) {
        return query.value(0).toInt(); // 成功返回对应的 budgetID
    } else {
        qDebug() << "Failed to retrieve budget ID for item ID " << itemID << ": " << query.lastError();
        return -1; // 失败时返回 -1
    }
}
