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
        qDebug() << "Failed to create budget table:" << query.lastError().text();
    }

    // create item table
    isBuilt = query.exec("CREATE TABLE IF NOT EXISTS item ("
                         "itemID INTEGER PRIMARY KEY AUTOINCREMENT, "
                         "budgetID INTEGER NOT NULL, "
                         "itemName VARCHAR(100) NOT NULL, "
                         "cap DOUBLE(10,2) NOT NULL, "
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

    // create expense table
    isBuilt = query.exec("CREATE TABLE IF NOT EXISTS expense ("
                         "expenseID INTEGER PRIMARY KEY AUTOINCREMENT, "
                         "itemID INTEGER NOT NULL, "
                         "total DOUBLE NOT NULL, "
                         "description TEXT, "
                         "date TEXT NOT NULL,"
                         "FOREIGN KEY (itemID) REFERENCES item(itemID));");

    if (isBuilt) {
        qDebug() << "Expense table created or already exists";
    } else {
        qDebug() << "Failed to create expense table:" << query.lastError().text();
    }

    // Create saving table
    isBuilt = query.exec("CREATE TABLE IF NOT EXISTS saving ("
                         "savingID INTEGER PRIMARY KEY AUTOINCREMENT, "
                         "planName VARCHAR(100) NOT NULL,"
                         "savingGoal DOUBLE NOT NULL, "
                         "goalRemaining DOUBLE NOT NULL, "
                         "description TEXT);");

    if (isBuilt) {
        qDebug() << "Saving table created or already exists";
    } else {
        qDebug() << "Failed to create Saving table:" << query.lastError().text();
    }

    // Create income table
    isBuilt = query.exec("CREATE TABLE IF NOT EXISTS income ("
                         "incomeID INTEGER PRIMARY KEY AUTOINCREMENT, "
                         "savingID INTEGER NOT NULL, "
                         "incomeAmount DOUBLE NOT NULL, "
                         "date TEXT NOT NULL, "
                         "FOREIGN KEY (savingID) REFERENCES saving(savingID));");

    if (isBuilt) {
        qDebug() << "Income table created or already exists";
    } else {
        qDebug() << "Failed to create income table:" << query.lastError().text();
    }

}


bool databaseManager::addItem(const Item& item)
{
    if (!db.isOpen()) {
        qDebug() << "Database not open";
        return false;
    }

    QSqlQuery query(db);
    QString itemName = item.getName();
    int suffix = 1; // Start suffix from 1

    // Check for existing item names and adjust if necessary
    while (true) {
        query.prepare("SELECT COUNT(*) FROM item WHERE itemName = ?");
        query.addBindValue(itemName);
        if (!query.exec()) {
            qDebug() << "Failed to check item name:" << query.lastError();
            return false;
        }
        query.next();
        if (query.value(0).toInt() == 0) {
            break; // No existing item with this name, proceed with this name
        } else {
            // If name exists, append a suffix and check again
            itemName = item.getName() + " (" + QString::number(suffix++) + ")";
        }
    }

    db.transaction();  // Start transaction

    // Insert the new item with the adjusted name
    query.prepare("INSERT INTO item (budgetID, itemName, cap, itemTotal) VALUES (?, ?, ?, ?)");
    query.addBindValue(item.getBudgetID());
    query.addBindValue(itemName);  // Use the potentially modified name
    query.addBindValue(item.getCap());
    query.addBindValue(item.getTotal());

    if (!query.exec()) {
        db.rollback();  // Rollback if execute failed
        qDebug() << "Failed to add item:" << query.lastError();
        return false;
    }

    // Update the total amount of the budget
    query.prepare("UPDATE budget SET remainingAmount = remainingAmount - ? WHERE budgetID = ?");
    query.addBindValue(item.getTotal());
    query.addBindValue(item.getBudgetID());

    if (!query.exec()) {
        db.rollback();  // Rollback if update failed
        qDebug() << "Failed to update budget total:" << query.lastError();
        return false;
    }

    db.commit();  // Commit transaction
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

    query.prepare("INSERT INTO budget (budgetName, total, remainingAmount) VALUES (?, ?, ?)");
    query.addBindValue(budget.getName());
    query.addBindValue(budget.getTotal());
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

bool databaseManager::addSavingPlan(const Saving& saving)
{
    if (!db.isOpen()) {
        qDebug() << "Database not open";
        return false;
    }

    QSqlQuery query(db);
    db.transaction();

    query.prepare("INSERT INTO saving (planName, savingGoal, goalRemaining, description) VALUES (?, ?, ?, ?)");
    query.addBindValue(saving.getName());
    query.addBindValue(saving.getSavingGoal());
    query.addBindValue(saving.getSavingGoal());
    query.addBindValue(saving.getComment());

    if (!query.exec()) {
        db.rollback();
        qDebug() << "Failed to add saving plan." << query.lastError();
        return false;
    } else {
        db.commit();
        return true;
    }
}


Budget databaseManager::getBudget (const QString& name)
{
    if (!db.isOpen()) {
        qDebug() << "Database not open";
        throw std::runtime_error("Database is not open.");
    }

    QSqlQuery query(db);
    query.prepare("SELECT budgetID, budgetName, total, remainingAmount FROM budget WHERE budgetName = ?");
    query.addBindValue(name);

    if (!query.exec()) {
        qDebug() << "Failed to retrieve budget:" << query.lastError();
        throw std::runtime_error("Query failed.");
    }

    if (query.next()) {
        int id = query.value(0).toInt();
        QString name = query.value(1).toString();
        double total = query.value(2).toDouble();
        double remain = query.value(3).toDouble();
        return Budget(id, name, total, remain);
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
    query.prepare("SELECT budgetID, budgetName, total, remainingAmount FROM budget");

    if (!query.exec()) {
        qDebug() << "Failed to retrieve budgets:" << query.lastError();
        throw std::runtime_error("Query failed.");
    }

    while (query.next()) {
        int id = query.value(0).toInt();
        QString name = query.value(1).toString();
        double total = query.value(2).toDouble();
        double remain = query.value(3).toDouble();
        budgets.append(Budget(id, name, total, remain));
    }
    return budgets;
}

QList<Item> databaseManager::getAllItems()
{
    QList<Item> items;
    if(!db.isOpen())
    {
        qDebug() << "Database not open";
        throw std::runtime_error("Database is not open.");
    }
    QSqlQuery query(db);
    query.prepare("SELECT itemID, budgetID, itemName, cap, itemTotal FROM item");
    if (!query.exec()) {
        qDebug() << "Failed to retrieve items:" << query.lastError();
        throw std::runtime_error("Query failed.");
    }
    while (query.next()) {
        int itemID = query.value(0).toInt();
        int budgetID = query.value(1).toInt();
        QString name = query.value(2).toString();
        double cap = query.value(3).toDouble();
        double total = query.value(5).toDouble();
        items.append(Item(itemID,budgetID,name,cap,total));
    }
    return items;
}

QList<QString> databaseManager::getItemNamesByBudget(int budgetID)
{
    QList<QString> itemNames;
    if(!db.isOpen())
    {
        qDebug() << "Database not open";
        throw std::runtime_error("Database is not open.");
    }
    QSqlQuery query(db);
    query.prepare("SELECT itemName FROM item WHERE budgetID = ?");
    query.addBindValue(budgetID);

    if (!query.exec()) {
        qDebug() << "Failed to retrieve item names:" << query.lastError();
        throw std::runtime_error("Query failed.");
    }
    while (query.next()) {
        QString name = query.value(0).toString();
        itemNames.append(name);
    }
    return itemNames;
}

QList<QString> databaseManager::getAllItemNames()
{
    QList<QString> itemNames;
    if(!db.isOpen())
    {
        qDebug() << "Database not open";
        throw std::runtime_error("Database is not open.");
    }
    QSqlQuery query(db);
    query.prepare("SELECT itemName FROM item");

    if (!query.exec()) {
        qDebug() << "Failed to retrieve item names:" << query.lastError();
        throw std::runtime_error("Query failed.");
    }
    while (query.next()) {
        QString name = query.value(0).toString();
        itemNames.append(name);
    }
    return itemNames;
}

QList<QString> databaseManager::getAllBudgetNames()
{
    QList<QString> budgetNames;
    if(!db.isOpen())
    {
        qDebug() << "Database not open";
        throw std::runtime_error("Database is not open.");
    }
    QSqlQuery query(db);
    query.prepare("SELECT budgetName FROM budget");
    if (!query.exec()) {
        qDebug() << "Failed to retrieve item names:" << query.lastError();
        throw std::runtime_error("Query failed.");
    }
    while (query.next()) {
        QString name = query.value(0).toString();
        budgetNames.append(name);
    }
    return budgetNames;
}

QList<Saving> databaseManager::getAllSavingPlans()
{
    QList<Saving> savings;
    if (!db.isOpen()) {
        qDebug() << "Database not open";
        throw std::runtime_error("Database is not open.");
    }

    QSqlQuery query(db);
    query.prepare("SELECT savingID, planName, savingGoal, goalRemaining, description FROM saving ");
    if (!query.exec()) {
        qDebug() << "Failed to retrieve savings:" << query.lastError();
        throw std::runtime_error("Query failed.");
    }
    while (query.next()) {
        int savingID = query.value(0).toInt();
        QString name = query.value(1).toString();
        double savingGoal = query.value(2).toDouble();
        double remaining = query.value(3).toDouble();
        QString description = query.value(4).toString();
        savings.append(Saving(savingID,name,savingGoal,remaining,description));
    }
    return savings;
}

QList<Expense> databaseManager::getAllExpenses()
{
    QList<Expense> expenses;
    if (!db.isOpen()) {
        qDebug() << "Database not open";
        throw std::runtime_error("Database is not open.");
    }

    QSqlQuery query(db);
    query.prepare("SELECT total, description, date FROM expense ");
    if (!query.exec()) {
        qDebug() << "Failed to retrieve savings:" << query.lastError();
        throw std::runtime_error("Query failed.");
    }
    while (query.next()) {
        double total = query.value(0).toDouble();
        QString description = query.value(1).toString();
        QString date = query.value(2).toString();
        expenses.append(Expense(total, description, date));
    }
    return expenses;
}



QList<Item> databaseManager::getItemsByBudget(int budgetID)
{
    QList<Item> items;
    if (!db.isOpen()) {
        qDebug() << "Database not open";
        throw std::runtime_error("Database is not open.");
    }

    QSqlQuery query(db);
    query.prepare("SELECT itemID, itemName, cap, itemTotal FROM item WHERE budgetID = ?");
    query.addBindValue(budgetID);

    if (!query.exec()) {
        qDebug() << "Failed to retrieve items:" << query.lastError();
        throw std::runtime_error("Query failed.");
    }

    while (query.next()) {
        int itemID = query.value(0).toInt();
        QString name = query.value(1).toString();
        double cap = query.value(2).toDouble();
        double itemTotal = query.value(3).toInt();
        items.append(Item(itemID, name, cap, itemTotal));
    }
    return items;
}

QList<income> databaseManager::getAllIncomes()
{
    QList<income> incomes;
    if (!db.isOpen()) {
        qDebug() << "Database not open";
        throw std::runtime_error("Database is not open.");
    }

    QSqlQuery query(db);
    query.prepare("SELECT incomeID, incomeAmount, date FROM income");

    if (!query.exec()) {
        qDebug() << "Failed to retrieve incomes:" << query.lastError();
        throw std::runtime_error("Query failed.");
    }

    while (query.next()) {
        int incomeID = query.value(0).toInt();
        double amount = query.value(1).toDouble();
        QString date = query.value(2).toString();
        incomes.append(income(incomeID, amount, date));
    }
    return incomes;
}

bool databaseManager::updateItem(const Item& item)
{
    if (!db.isOpen()) {
        qDebug() << "Database not open";
        return false;
    }

    QSqlQuery query(db);
    db.transaction();

    // Update the item directly with new values
    query.prepare("UPDATE item SET itemName = ?, cap = ?, itemTotal = ? WHERE itemID = ?");
    query.addBindValue(item.getName());
    query.addBindValue(item.getCap());
    query.addBindValue(item.getTotal());
    query.addBindValue(item.getItemID());

    if (!query.exec()) {
        db.rollback();  // Rollback if item update failed
        qDebug() << "Failed to update item:" << query.lastError();
        return false;
    }
    qDebug() << "Update item successful.";
    db.commit();
    return true;
}


bool databaseManager::updateBudget(const Budget& budget,int budgetID) {
    if (!db.isOpen()) {
        qDebug() << "Database not open";
        return false;
    }

    QSqlQuery query(db);
    db.transaction();  // Start transaction

    query.prepare("UPDATE budget SET budgetName = ?, total = ? WHERE budgetID = ?");
    query.addBindValue(budget.getName());
    query.addBindValue(budget.getTotal());
    query.addBindValue(budgetID);

    if (!query.exec()) {
        qDebug() << "Failed to update budget:" << query.lastError();
        db.rollback();  // Rollback transaction if update failed
        return false;
    }

    db.commit();  // Commit the transaction
    return true;
}



bool databaseManager::updateSaving(const Saving& saving, int savingID)
{
    if (!db.isOpen()) {
        qDebug() << "Database not open";
        return false;
    }
    double oldGoal,oldremaining;
    QSqlQuery query(db);
    query.prepare("SELECT savingGoal, goalRemaining FROM saving WHERE savingID = ?");
    query.addBindValue(savingID);
    if (!query.exec()) {
        qDebug() << "Failed to retrieve items:" << query.lastError();
        throw std::runtime_error("Query failed.");
    }
    if(query.next())
    {
        oldGoal = query.value(0).toDouble();
        oldremaining = query.value(1).toDouble();
    }else{
        qDebug() << "No saving found with ID: " <<savingID;
        return false;
    }

    db.transaction();  // Start transaction

    query.prepare("UPDATE saving SET planName = ?, savingGoal = ?, goalRemaining = ? WHERE savingID = ?");
    query.addBindValue(saving.getName());
    query.addBindValue(saving.getSavingGoal());
    query.addBindValue(oldremaining + saving.getSavingGoal()-oldGoal); //calculate the new goal remaining
    query.addBindValue(savingID);

    if (!query.exec()) {
        qDebug() << "Failed to update saving plan:" << query.lastError();
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
    double cap;
    query.prepare("SELECT budgetID, cap FROM item WHERE itemID = ?");
    query.addBindValue(itemID);
    if (query.exec() && query.next()) {
        budgetID = query.value(0).toInt();
        cap = query.value(1).toDouble();
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
    query.addBindValue(cap);
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


Item databaseManager::getItem(const QString& name)
{
    if (!db.isOpen()) {
        qDebug() << "Database not open";
        throw std::runtime_error("Database is not open.");
    }

    QSqlQuery query(db);
    query.prepare("SELECT itemID, budgetID, itemName, cap, itemTotal FROM item WHERE itemName = ?");
    query.addBindValue(name);

    if (!query.exec()) {
        qDebug() << "Failed to retrieve item:" << query.lastError().text();
        throw std::runtime_error("Query failed to execute.");
    }

    if (query.next()) {
        int itemID = query.value(0).toInt();
        int budgetID = query.value(1).toInt();
        QString name = query.value(2).toString();
        double cap = query.value(3).toDouble();
        double total = query.value(4).toInt();

        // Assuming Item has a constructor that takes these parameters
        return Item(itemID, budgetID, name, cap, total);
    } else {
        qDebug() << "Item not found with ID:" << name;
        throw std::runtime_error("Item not found.");
    }
}

Saving databaseManager::getSaving(int savingID)
{
    if (!db.isOpen()) {
        qDebug() << "Database not open";
        throw std::runtime_error("Database is not open.");
    }

    QSqlQuery query(db);
    query.prepare("SELECT planName, savingGoal, goalRemaining, description FROM saving WHERE savingID = ?");
    query.addBindValue(savingID);

    if (!query.exec()) {
        qDebug() << "Failed to retrieve saving plan:" << query.lastError().text();
        throw std::runtime_error("Query failed to execute.");
    }

    if (query.next()) {
        QString planName = query.value(0).toString();
        double goal = query.value(1).toDouble();
        double remain = query.value(2).toDouble();
        QString description = query.value(3).toString();
        return Saving(planName, goal, remain, description);
    } else {
        qDebug() << "Saving plan not found with ID:" << savingID;
        throw std::runtime_error("Saving plan not found.");
    }
}


int databaseManager::getBudgetIdByItem(const Item& item)
{
    if (!db.isOpen()) {
        qDebug() << "Database not open";
        throw std::runtime_error("Database is not open.");
    }


    QSqlQuery query(db);
    query.prepare("SELECT budgetID FROM item WHERE itemID = ?");
    query.addBindValue(item.getItemID());

    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    } else {
        qDebug() << "Failed to retrieve budget ID for item ID " << item.getItemID() << ": " << query.lastError();
        return -1;
    }
}


int databaseManager::getItemID(const QString& name)
{
    if(!db.isOpen())
    {
        qDebug() << "Database not open";
        throw std::runtime_error("Database is not open.");
    }

    QSqlQuery query(db);
    query.prepare("SELECT itemID FROM item WHERE itemName = ?");
    query.addBindValue(name);

    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    } else {
        qDebug() << "Failed to retrieve item ID for " << name << ": " << query.lastError();
        return -1;
    }
}

bool databaseManager::updateItemTotal(double transaction, const Item& item)
{
    if(!db.isOpen())
    {
        qDebug() << "Database not open";
        throw std::runtime_error("Database is not open.");
        return false;
    }

    QSqlQuery query(db);
    db.transaction();
    query.prepare("UPDATE item SET itemTotal = itemTotal + ? WHERE itemID = ?");
    query.addBindValue(transaction);
    query.addBindValue(item.getItemID());

    if(!query.exec()){
        db.rollback();
        qDebug() << "Update Item total failed.";
        return false;
    }

    int budgetID = getBudgetIdByItem(item);
    if(budgetID != -1) {
        query.prepare("UPDATE budget SET amountRemaining = amountRemaining - ? WHERE budgetID = ?");
        query.addBindValue(transaction);
        query.addBindValue(budgetID);
        if (!query.exec()) {
            db.rollback();
            qDebug() << "Update budget remaining failed.";
            return false;
        }
    }

    db.commit();
    return true;

}

bool databaseManager::updateBudgetTotal(double spend, const Budget& budget)
{
    if(!db.isOpen())
    {
        qDebug() << "Database not open";
        throw std::runtime_error("Database is not open.");
        return false;
    }
    QSqlQuery query(db);
    db.transaction();
    query.prepare("UPDATE budget SET total = ? WHERE budgetID = ? ");
    query.addBindValue(budget.getTotal());
    query.addBindValue(budget.getBudgetID());

    if(!query.exec()){
        db.rollback();
        qDebug() << "Update budget total failed.";
        return false;
    }

    query.prepare("UPDATE budget SET remainingAmount = total - ? WHERE budgetID = ? ");
    query.addBindValue(spend);
    query.addBindValue(budget.getBudgetID());

    if(!query.exec()){
        db.rollback();
        qDebug() << "Update budget remaining failed.";
        return false;
    }

    db.commit();
    return true;
}

bool databaseManager::addExpense(const Expense& expense)
{
    if (!db.isOpen()) {
        qDebug() << "Database not open";
        throw std::runtime_error("Database is not open.");
    }

    QSqlQuery query(db);
    db.transaction();

    try {
        // Insert the new expense
        query.prepare("INSERT INTO expense (itemID, total, description, date) VALUES (?, ?, ?, ?)"); // Add date field here
        query.addBindValue(expense.getItemID());
        query.addBindValue(expense.getTotal());
        query.addBindValue(expense.getDescription());
        query.addBindValue(expense.getDate()); // Assume getDate returns the date in the correct format
        if (!query.exec()) {
            throw std::runtime_error("Failed to insert expense: " + query.lastError().text().toStdString());
        }

        // Update the item total
        query.prepare("UPDATE item SET itemTotal = itemTotal + ? WHERE itemID = ?");
        query.addBindValue(expense.getTotal());
        query.addBindValue(expense.getItemID());
        if (!query.exec()) {
            throw std::runtime_error("Failed to update item total: " + query.lastError().text().toStdString());
        }

        // Get the associated budgetID for the item
        int budgetID;
        query.prepare("SELECT budgetID FROM item WHERE itemID = ?");
        query.addBindValue(expense.getItemID());
        if (!query.exec() || !query.next()) {
            throw std::runtime_error("Failed to find budgetID for item: " + query.lastError().text().toStdString());
        }
        budgetID = query.value(0).toInt();

        // Update the budget remainingAmount
        query.prepare("UPDATE budget SET remainingAmount = remainingAmount - ? WHERE budgetID = ?");
        query.addBindValue(expense.getTotal());
        query.addBindValue(budgetID);
        if (!query.exec()) {
            throw std::runtime_error("Failed to update budget remaining amount: " + query.lastError().text().toStdString());
        }

        // Commit transaction
        db.commit();
        qDebug() << "Expense added successfully and related totals updated.";
        return true;
    } catch (const std::runtime_error& e) {
        db.rollback();
        qDebug() << "Transaction failed: " << e.what();
        return false;
    }
}

int databaseManager::getSavingID(const QString& savingName)
{
    if(!db.isOpen())
    {
        qDebug() << "Database not open";
        throw std::runtime_error("Database is not open.");
        return -1;
    }

    QSqlQuery query(db);
    query.prepare("SELECT savingID FROM saving WHERE planName = ?");
    query.addBindValue(savingName);

    if (query.exec() && query.next()) {
        return query.value(0).toInt();
    } else {
        qDebug() << "Failed to retrieve item ID for " << savingName << ": " << query.lastError();
        return -1;
    }
}


bool databaseManager::addIncome(const income& income)
{
    if(!db.isOpen())
    {
        qDebug() << "Database not open";
        throw std::runtime_error("Database is not open.");
        return false;
    }

    QSqlQuery query(db);
    db.transaction();

    try {
        // Insert the new income
        query.prepare("INSERT INTO income (savingID, incomeAmount, Date) VALUES (?, ?, ?)");
        query.addBindValue(income.getSavingID());
        query.addBindValue(income.getAmount());
        query.addBindValue(income.getDate());
        if (!query.exec()) {
            throw std::runtime_error("Failed to insert income: " + query.lastError().text().toStdString());
        }

        // Update the savings total
        query.prepare("UPDATE saving SET goalRemaining = goalRemaining - ? WHERE savingID = ?");
        query.addBindValue(income.getAmount());
        query.addBindValue(income.getSavingID());
        if (!query.exec()) {
            throw std::runtime_error("Failed to update item total: " + query.lastError().text().toStdString());
        }

        // Commit transaction
        db.commit();
        qDebug() << "Expense added successfully and related totals updated.";
        return true;
    } catch (const std::runtime_error& e) {
        db.rollback();
        qDebug() << "Transaction failed: " << e.what();
        return false;
    }
}
