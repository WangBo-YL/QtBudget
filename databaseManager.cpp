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
                              "total DOUBLE(10, 2) NOT NULL)");
    if (isBuilt) {
        qDebug() << "Budget table created or already exists";
    } else {
        qDebug() << "Failed to create budget table:" << query.lastError().text();
    }

    // create item table
    isBuilt = query.exec("CREATE TABLE IF NOT EXISTS item ("
                         "itemID INTEGER PRIMARY KEY AUTOINCREMENT, "
                         "itemName VARCHAR(100) NOT NULL, "
                         "amount DOUBLE(10, 2) NOT NULL, "
                         "budgetID INTEGER, "
                         "categoryID INTEGER, "
                         "FOREIGN KEY (budgetID) REFERENCES budget(budgetID), "
                         "FOREIGN KEY (categoryID) REFERENCES category(categoryID))");
    if (isBuilt) {
        qDebug() << "Item table created or already exists";
    } else {
        qDebug() << "Failed to create item table:" << query.lastError().text();
    }

    // create category table
    isBuilt = query.exec("CREATE TABLE IF NOT EXISTS category ("
                         "categoryID INTEGER PRIMARY KEY AUTOINCREMENT, "
                         "categoryName VARCHAR(100) NOT NULL)");
    if (isBuilt) {
        qDebug() << "Category table created or already exists";
    } else {
        qDebug() << "Failed to create category table:" << query.lastError().text();
    }

    if (isBuilt) {
        QSqlQuery checkQuery("SELECT COUNT(*) FROM category");
        if (checkQuery.next() && checkQuery.value(0).toInt() == 0) {
            // No categories exist, insert them
            bool inserted = query.exec("INSERT INTO category (categoryName) VALUES "
                                       "('Grocery'), ('Transportation'), ('Housing'), "
                                       "('Utilities'), ('Food'), ('Clothing'), "
                                       "('Vehicle'), ('Entertainment')");
            if (inserted) {
                qDebug() << "Categories initialized successfully";
            } else {
                qDebug() << "Failed to insert categories:" << query.lastError().text();
            }
        } else {
            qDebug() << "Categories already exist";
        }
    }

}


bool databaseManager::addItem(const Item& item)
{
    if (!db.isOpen()) {
        qDebug() << "Database not open";
        return false;
    }

    QSqlQuery query(db);
    db.transaction();  // start transaction

    query.prepare("INSERT INTO item (itemID, budgetID, name, amount, categoryID) VALUES (?, ?, ?, ?, ?)");
    query.addBindValue(item.getItemID());
    query.addBindValue(item.getBudgetID());
    query.addBindValue(item.getName());
    query.addBindValue(item.getAmount());
    query.addBindValue(item.getCategoryID());

    if (!query.exec()) {
        db.rollback();  //rollback if exectue failed
        qDebug() << "Failed to add item:" << query.lastError();
        return false;
    } else {
        db.commit();  // success and commit
        return true;
    }
}

bool databaseManager::addBudget(const Budget& budget)
{
    if (!db.isOpen()) {
        return false;
    }

    QSqlQuery query(db);
    db.transaction();

    query.prepare("INSERT INTO budget (budgetID, name, totalAmount) VALUES (?, ?, ?)");
    query.addBindValue(budget.getBudgetID());
    query.addBindValue(budget.getName());
    query.addBindValue(budget.getTotal());

    if (!query.exec()) {
        db.rollback();
        qDebug() << "Failed to add budget with ID:" <<  budget.getBudgetID() << query.lastError();
        return false;
    } else {
        db.commit();
        return true;
    }
}

Budget databaseManager::getBudget(int budgetID)
{
    if (!db.isOpen()) {
        qDebug() << "Database not open";
        throw std::runtime_error("Database is not open.");
    }

    QSqlQuery query(db);
    query.prepare("SELECT budgetID, name, totalAmount FROM budget WHERE budgetID = ?");
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
    query.prepare("SELECT budgetID, name, totalAmount FROM budget");

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
    query.prepare("SELECT name, amount, categoryID FROM item WHERE budgetID = ?");
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

bool databaseManager::updateBudget(const Budget& budget, const QList<Item>& items) {\

    if (!db.isOpen()) {
        qDebug() << "Database not open";
        return false;
    }

    QSqlQuery query(db);
    db.transaction();

    bool itemsUpdated = false;
    double newTotalAmount = 0;

    for (const Item& item : items) {
        query.prepare("UPDATE item SET name = ?, amount = ?, categoryID = ? WHERE itemID = ?");
        query.addBindValue(item.getName());
        query.addBindValue(item.getAmount());
        query.addBindValue(item.getCategoryID());
        query.addBindValue(item.getItemID());

        if (!query.exec()) {
            db.rollback();
            qDebug() << "Failed to update item:" << query.lastError();
            return false;
        }
        itemsUpdated = true;
        newTotalAmount += item.getAmount();
    }


    if (itemsUpdated) {
        query.prepare("UPDATE budget SET totalAmount = ? WHERE budgetID = ?");
        query.addBindValue(newTotalAmount);
        query.addBindValue(budget.getBudgetID());

        if (!query.exec()) {
            db.rollback();
            qDebug() << "Failed to update budget total amount:" << query.lastError();
            return false;
        }
    }


    if (!budget.getName().isEmpty()) {
        query.prepare("UPDATE budget SET name = ? WHERE budgetID = ?");
        query.addBindValue(budget.getName());
        query.addBindValue(budget.getBudgetID());

        if (!query.exec()) {
            db.rollback();
            qDebug() << "Failed to update budget name:" << query.lastError();
            return false;
        }
    }

    db.commit();
    return true;
}

bool databaseManager::updateItem(const Item& item, bool updateBudgetTotal)
{
    if (!db.isOpen()) {
        qDebug() << "Database not open";
        return false;
    }

    QSqlQuery query(db);
    db.transaction();

    query.prepare("UPDATE item SET name = ?, amount = ?, categoryID = ? WHERE itemID = ?");
    query.addBindValue(item.getName());
    query.addBindValue(item.getAmount());
    query.addBindValue(item.getCategoryID());
    query.addBindValue(item.getItemID());

    if (!query.exec()) {
        db.rollback();
        qDebug() << "Failed to update item:" << query.lastError();
        return false;
    }

    if (updateBudgetTotal) {
        if (!updateBudgetTotalAmount(item.getBudgetID())) {
            db.rollback();
            return false;
        }
    }

    db.commit();
    return true;
}

bool databaseManager::updateBudgetTotalAmount(int budgetID)
{
    QSqlQuery query(db);
    query.prepare("SELECT SUM(amount) FROM item WHERE budgetID = ?");
    query.addBindValue(budgetID);
    double newTotal = 0;

    if (query.exec() && query.next()) {
        newTotal = query.value(0).toDouble();
    } else {
        qDebug() << "Failed to calculate total amount:" << query.lastError();
        return false;
    }

    query.prepare("UPDATE budget SET totalAmount = ? WHERE budgetID = ?");
    query.addBindValue(newTotal);
    query.addBindValue(budgetID);

    if (!query.exec()) {
        qDebug() << "Failed to update budget total:" << query.lastError();
        return false;
    }
    return true;
}


bool databaseManager::deleteItem(int itemID) {
    if (!db.isOpen()) {
        qDebug() << "Database not open";
        return false;
    }

    QSqlQuery query(db);
    db.transaction();


    int budgetID = 0;
    query.prepare("SELECT budgetID FROM item WHERE itemID = ?");
    query.addBindValue(itemID);
    if (query.exec() && query.next()) {
        budgetID = query.value(0).toInt();
    } else {
        qDebug() << "Failed to find item:" << query.lastError();
        db.rollback();
        return false;
    }

    query.prepare("DELETE FROM item WHERE itemID = ?");
    query.addBindValue(itemID);
    if (!query.exec()) {
        qDebug() << "Failed to delete item:" << query.lastError();
        db.rollback();
        return false;
    }

    if (!updateBudgetTotalAmount(budgetID)) {
        db.rollback();
        return false;
    }

    db.commit();
    return true;
}

bool databaseManager::deleteBudget(int budgetID) {
    if (!db.isOpen()) {
        qDebug() << "Database not open";
        return false;
    }

    QSqlQuery query(db);
    db.transaction();


    query.prepare("DELETE FROM item WHERE budgetID = ?");
    query.addBindValue(budgetID);
    if (!query.exec()) {
        qDebug() << "Failed to delete item for budget:" << query.lastError();
        db.rollback();
        return false;
    }


    query.prepare("DELETE FROM budget WHERE budgetID = ?");
    query.addBindValue(budgetID);
    if (!query.exec()) {
        qDebug() << "Failed to delete budget:" << query.lastError();
        db.rollback();
        return false;
    }

    db.commit();
    return true;
}
