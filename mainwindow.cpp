#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , db("budget.db")
{
    ui->setupUi(this);
    navigateToPage(0);
    updateTransactionTableWidget();
    ui->newItemTotalInput->setText("0");
}


MainWindow::~MainWindow()
{
    delete ui;
    delete savingListWidget;
    delete savingListWidgetItem;
    delete budgetListWidget;
    delete budgetListWidgetItem;
    delete transactionListWidgetItem;
    delete transactionListWidget;
    delete itemListWidget;
    delete itemListWidgetItem;
}

void MainWindow::navigateToPage(int pageIndex)
{
    ui->stackedWidget->setCurrentIndex(pageIndex);
}

void MainWindow::updateBudgetBox()
{
    ui->budgetComboBox->clear();
    QList<Budget> budgets = db.getAllBudgets();
    if(budgets.isEmpty())
    {
        qDebug() << "No budgets found or database error.";
        return;
    }
    for(const Budget& budget: budgets)
    {
        ui->budgetComboBox->addItem(budget.getName());
    }

}

void MainWindow::updateItemBox()
{
    ui->ItemCombobox->clear();
    QList<QString> itemNameList = db.getAllItemNames();
    if(itemNameList.isEmpty())
    {
        qDebug() << "No item found or database error.";
        return;
    }
    for(const QString& itemName: itemNameList)
    {
        ui->ItemCombobox->addItem(itemName);
    }
}

void MainWindow::updateSavingBox()
{
    ui->savingCombobox->clear();
    QList<Saving> savings = db.getAllSavingPlans();
    if(savings.isEmpty())
    {
        qDebug() << "No saving plans or database error.";
        return;
    }

    for(const Saving& saving: savings)
    {
        ui->savingCombobox->addItem(saving.getName());
    }
}

void MainWindow::updateSavingListWidget()
{
    QList<Saving> savings = db.getAllSavingPlans();
    QListWidget* savingList = ui->savingsListWidget;
    savingList->clear();

    if(savings.isEmpty())
    {
        qDebug() << "No budgets found or database error.";
        return;
    }
    for(const Saving& saving: savings)
    {
        // create new QlistwidgetItem
        QListWidgetItem* newItem = new QListWidgetItem;

        // format the string that needs to display
        QString displayText = QString("%1 - Goal: %2, Remaining: %3, Comment: %4")
                                  .arg(saving.getName())
                                  .arg(saving.getSavingGoal())
                                  .arg(saving.getGoalRemaining())
                                  .arg(saving.getComment());
        // set up the content display
        newItem->setText(displayText);

        //set budgetID as data that saved in the options, using Qt::userrole to avoid conflict.
        newItem->setData(Qt::UserRole, QVariant(saving.getSavingID()));

        // add new item in to list
        savingList->addItem(newItem);
    }
}

void MainWindow::updateItemListWidget()
{
    int budgetID = budgetListWidgetItem->data(Qt::UserRole).toInt();
    QString budgetName = budgetListWidgetItem->text().split(" - ").first();
    qDebug() << "Selected Budget Name: " << budgetName << ", ID: " << budgetID;
    QList<Item> items = db.getItemsByBudget(budgetID);
    QListWidget* itemList = ui->itemListWidget;
    itemList->clear();

    if(items.isEmpty())
    {
        qDebug() << "No budgets found or database error.";
        return;
    }
    for(const Item& item:items)
    {
        // create new QlistwidgetItem
        QListWidgetItem* newItem = new QListWidgetItem;

        // format the string that needs to display
        QString displayText = QString("%1 - Limit: %2, Total: %3")
                                  .arg(item.getName())
                                  .arg(item.getCap())
                                  .arg(item.getTotal());
        // set up the content display
        newItem->setText(displayText);

        //set itemID as data that saved in the options, using Qt::userrole to avoid conflict.
        newItem->setData(Qt::UserRole, QVariant(item.getBudgetID()));
        int retrievedID = newItem->data(Qt::UserRole).toInt();  // Retrieve with toInt
        qDebug() << "Added item to list with ID: " << retrievedID;
        // add new item in to list
        itemList->addItem(newItem);
        qDebug() << "Adding item to list with ID = " << newItem->data(Qt::UserRole).toInt();
    }
}

void MainWindow::updateCurrentBudget(const QString& name)
{
    try{
        currentBudget = db.getBudget(name);
    }catch(const std::runtime_error&e)
    {
        qDebug() << "Error updating budget: " << e.what();
    }
}

void MainWindow::updateBudgetList()
{
    QList<Budget> budgets = db.getAllBudgets();
    QListWidget* budgetList = ui->budgetListWidget;
    budgetList->clear();

    if(budgets.isEmpty())
    {
        qDebug() << "No budgets found or database error.";
        return;
    }
    for(const Budget& budget: budgets)
    {
        // create new QlistwidgetItem
        QListWidgetItem* newItem = new QListWidgetItem;

        // format the string that needs to display
        QString displayText = QString("%1 - Total: %2, Remaining: %3")
                                  .arg(budget.getName())
                                  .arg(budget.getTotal())
                                  .arg(budget.getRemaining());
        // set up the content display
        newItem->setText(displayText);

        //set budgetID as data that saved in the options, using Qt::userrole to avoid conflict.
        newItem->setData(Qt::UserRole, QVariant(budget.getBudgetID()));
        qDebug() << "budgetID : " << budget.getBudgetID();
        // add new item in to list
        budgetList->addItem(newItem);
    }
}




void MainWindow::updateTransactionTableWidget()
{
    QList<income> incomes = db.getAllIncomes();
    QList<Expense> expenses = db.getAllExpenses();

    for(const income& income: incomes)
    {
        addIncomeToTable(income);
    }

    for(const Expense& expense: expenses)
    {
        addExpenseToTable(expense);
    }
}

void MainWindow::addIncomeToTable(const income& income)
{
    int currentRow = ui->transactionListTableWidget->rowCount();
    ui->transactionListTableWidget->insertRow(currentRow);
    ui->transactionListTableWidget->setItem(currentRow, 0, new QTableWidgetItem("Income"));
    ui->transactionListTableWidget->setItem(currentRow, 1, new QTableWidgetItem(QString::number(income.getAmount())));
    ui->transactionListTableWidget->setItem(currentRow, 2, new QTableWidgetItem(income.getDate()));
}


void MainWindow::addExpenseToTable(const Expense& expense)
{
        int currentRow = ui->transactionListTableWidget->rowCount();
        ui->transactionListTableWidget->insertRow(currentRow);
        ui->transactionListTableWidget->setItem(currentRow, 0, new QTableWidgetItem("Expense"));
        ui->transactionListTableWidget->setItem(currentRow, 1, new QTableWidgetItem(QString::number(expense.getTotal())));
        ui->transactionListTableWidget->setItem(currentRow, 2, new QTableWidgetItem(expense.getDate()));
        ui->transactionListTableWidget->setItem(currentRow, 3, new QTableWidgetItem(expense.getDescription()));

}

void MainWindow::on_budgetMenuButton_clicked()
{
    navigateToPage(1);
}


void MainWindow::on_expenseMenuButton_clicked()
{
    navigateToPage(6);

}


void MainWindow::on_savingsMenuButton_clicked()
{
    navigateToPage(10);

}


void MainWindow::on_transactionHistoryButton_clicked()
{
    updateTransactionTableWidget();
    navigateToPage(9);

}


void MainWindow::on_reportMenuButton_clicked()
{
    navigateToPage(8);
}


void MainWindow::on_returnFromBudgetMenuButton_clicked()
{
    navigateToPage(0);

}


void MainWindow::on_newBudgetButton_clicked()
{
    navigateToPage(2);
    previousPageIndex = 1;
}


void MainWindow::on_budgetListButton_clicked()
{
    updateBudgetList();
    ui->editBudgetButton->hide();
    ui->budgetDetaiButton->hide();
    ui->deleteBudgetButton->hide();
    navigateToPage(4);
}


void MainWindow::on_newItemButton_clicked()
{
    updateBudgetBox();
    navigateToPage(3);
    previousPageIndex = 1;
}


void MainWindow::on_cancelCreateBudgetButton_clicked()
{

    navigateToPage(1);
    ui->newBudgetNameInput->clear();
    ui->newBudgetTotalInput->clear();

}


void MainWindow::on_saveBudgetButton_clicked()
{
    QString text = ui->newBudgetNameInput->text();
    double total = ui->newBudgetTotalInput->text().toDouble();
    if (text.isEmpty() || total <= 0) {
        QMessageBox::warning(this, "Input Error", "Please enter valid budget name and total amount.");
        return;
    }
    if(previousPageIndex == 1)
    {
        try {
            Budget budget(text,total);

            if(db.addBudget(budget))
            {
                qDebug() << "Budget Name: " <<text << ", Total : " << total;
                QMessageBox::information(this,"Success", "Create Budget Successful!");
            }

        } catch (...) {
            qDebug() <<  "Failed to create a new budget.";
        }
    }else{
        try {
            QString budgetName = budgetListWidgetItem->text().split(" - ").first();
            int budgetID  = db.getBudget(budgetName).getBudgetID();

            Budget newBudget(text, total);

            if(db.updateBudget(newBudget, budgetID))
            {
                QMessageBox::information(this,"Success", "Update Budget Successful!");
                qDebug() << "Update Budget Successful!";
            }

        } catch (...) {
            qDebug() << "Failed to update budget.";
        }

    }
    updateBudgetBox();
    updateBudgetList();
    ui->newBudgetNameInput->clear();
    ui->newBudgetTotalInput->clear();
    navigateToPage(1);
    previousPageIndex = 2;
}




void MainWindow::on_returnFromBudgetListButton_clicked()
{
    navigateToPage(1);

}


void MainWindow::on_budgetDetaiButton_clicked()
{
    updateItemListWidget();
    updateBudgetBox();
    navigateToPage(5);
    previousPageIndex = 4;
}


void MainWindow::on_editBudgetButton_clicked()
{
    navigateToPage(2);
    previousPageIndex = 4;
}


void MainWindow::on_deleteBudgetButton_clicked()
{
    QListWidgetItem* selectedItem = ui->budgetListWidget->currentItem();
    if (selectedItem == nullptr) {
        QMessageBox::warning(this, "Selection Error", "Please select a saving to delete.");
        return;
    }

    int budgetID = selectedItem->data(Qt::UserRole).toInt();
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Delete budget", "Are you sure you want to delete this budget?", QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        if (db.deleteBudget(budgetID)) {
            delete selectedItem;
            QMessageBox::information(this, "Success", "Budget deleted successfully.");
        } else {
            QMessageBox::warning(this, "Error", "Failed to delete the budget from the database.");
        }
    }
}




void MainWindow::on_cancelAddItemButton_clicked()
{

    navigateToPage(1);
}


void MainWindow::on_addItemButton_clicked()
{
    QString budgetName = ui->budgetComboBox->currentText();
    QString name = ui->newItemNameInput->text().trimmed();
    double cap = ui->newItemLimitInput->text().toDouble();
    double total = ui->newItemTotalInput->text().toDouble();

    if (budgetName.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please select a budget.");
        return;
    }

    if (name.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter a valid item name.");
        return;
    }

    if (cap <= 0 || total < 0) {
        QMessageBox::warning(this, "Input Error", "Cap and Total must be greater than zero.");
        return;
    }

    int budgetID = db.getBudgetIDByName(budgetName);
    qDebug() << "Budget ID is " << budgetID;
    if (budgetID == -1) {
        QMessageBox::warning(this, "Error", "Invalid budget selected.");
        return;
    }

    Item item(budgetID, name, cap, total);
    if (db.addItem(item)) {
        qDebug() << "You have successfully added a new Item to " << budgetName;

        QMessageBox::information(this, "Success", "Add item to budget successful!");


        int itemID = db.getItemID(name);
        qDebug() << "New added item ID is " << itemID;
    } else {
        QMessageBox::warning(this, "Failed", "Failed adding new items to budget.");
    }

    if(db.updateBudgetRemaining(item))
    {
        qDebug() << "Update budget remaining successfully.";
    }else{
        qDebug() << "Failed update budget remaining.";
    }

    ui->newItemNameInput->clear();
    ui->newItemLimitInput->clear();
    ui->newItemTotalInput->clear();
    navigateToPage(1);
    previousPageIndex = 3;
}


void MainWindow::on_cancelExpenseButton_clicked()
{
    navigateToPage(6);
}


void MainWindow::on_confirmExpenseButton_clicked()
{
    double amount = ui->expenseAmountInput->text().toDouble();
    QString description = ui->expenseDescriptionInput->text();
    QString itemName = ui->ItemCombobox->currentText();
    QDate date = ui->expenseDateEdit->date();
    QString dateStr = date.toString("yyyy-MM-dd");

    if (itemName.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please select an item.");
        return;
    }

    if (amount <= 0) {
        QMessageBox::warning(this, "Input Error", "Expense amount must be greater than zero.");
        return;
    }

    int itemID = db.getItemID(itemName);
    if (itemID == -1) {
        QMessageBox::warning(this, "Error", "Invalid item selected.");
        return;
    }

    Expense expense(itemID, amount, description, dateStr);
    if (db.addExpense(expense)) {
        qDebug() << "Add new expense to " << itemName << " successful!";
        QMessageBox::information(this, "Success", "Add expense record successfully.");
    } else {
        QMessageBox::warning(this, "Failed", "Failed to add new expense.");
    }

    ui->expenseAmountInput->clear();
    ui->expenseDescriptionInput->clear();
    ui->ItemCombobox->setCurrentIndex(0);
    navigateToPage(6);
    previousPageIndex = 7;
}



void MainWindow::on_returnFromExpenseMenuButton_clicked()
{
    navigateToPage(0);
}


void MainWindow::on_addExpenditureButton_clicked()
{
    updateItemBox();
    navigateToPage(7);
    previousPageIndex = 6;
}


void MainWindow::on_returnFromItemListButton_clicked()
{
    navigateToPage(4);
}


void MainWindow::on_editItemButton_clicked()
{
    navigateToPage(14);
    previousPageIndex = 5;
}


void MainWindow::on_deleteItemButton_clicked()
{
    QListWidgetItem* selectedItem = ui->itemListWidget->currentItem();
    if (selectedItem == nullptr) {
        QMessageBox::warning(this, "Selection Error", "Please select an item to delete.");
        return;
    }

    int itemID = selectedItem->data(Qt::UserRole).toInt();
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Delete Item", "Are you sure you want to delete this item?", QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        if (db.deleteItem(itemID)) {
            delete selectedItem;
            QMessageBox::information(this, "Success", "Item deleted successfully.");
        } else {
            QMessageBox::warning(this, "Error", "Failed to delete the item from the database.");
        }
    }
}


void MainWindow::on_cancelAddIncomeButton_clicked()
{
    navigateToPage(10);
}


void MainWindow::on_finishAddIncomeButton_clicked()
{
    QString savingPlan = ui->savingCombobox->currentText();
    double incomeAmount = ui->newIncomeInput->text().toDouble();
    QString date = ui->incomeDateEdit->date().toString("yyyy-MM-dd");

    if (savingPlan.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please select a saving plan.");
        return;
    }

    if (incomeAmount <= 0) {
        QMessageBox::warning(this, "Input Error", "Income amount must be greater than zero.");
        return;
    }

    int savingID = db.getSavingID(savingPlan);
    if (savingID == -1) {
        QMessageBox::warning(this, "Error", "Invalid saving plan selected.");
        return;
    }

    income newIncome(savingID, incomeAmount, date);
    if (db.addIncome(newIncome)) {
        qDebug() << "Add income successfully!";
        QMessageBox::information(this, "Success", "Add New Income Successfully!");
    } else {
        QMessageBox::warning(this, "Failed", "Failed to add new income.");
    }

    navigateToPage(10);
    previousPageIndex = 13;
}



void MainWindow::on_cancelSavingPlanButton_clicked()
{

    navigateToPage(10);
    ui->savingGoalInput->clear();
    ui->savingNameInput->clear();
    ui->savingDescriptionInput->clear();
}


void MainWindow::on_finishAddSavingPlanButton_clicked()
{
    QString savingName = ui->savingNameInput->text().trimmed();
    double planAmount = ui->savingGoalInput->text().toDouble();
    QString description = ui->savingDescriptionInput->text().trimmed();


    if (savingName.isEmpty()) {
        QMessageBox::warning(this, "Input Error", "Please enter a valid saving plan name.");
        return;
    }

    if (planAmount <= 0) {
        QMessageBox::warning(this, "Input Error", "Please enter a valid amount for the saving plan. Amount must be greater than zero.");
        return;
    }

    Saving saving(savingName, planAmount, planAmount, description);

    if(previousPageIndex != 12)
    {
        if(db.addSavingPlan(saving))
        {
            qDebug() << "Add new saving plan successful!";
            QMessageBox::information(this, "Success!", "Add new Saving Plan Successful!");
        }
        else
        {
            QMessageBox::warning(this, "Failed!", "Add new Saving Plan Failed!");
        }
    }
    else
    {
        int savingID = savingListWidgetItem->data(Qt::UserRole).toInt();
        if(db.updateSaving(saving, savingID))
        {
            qDebug() << "Updating saving plan successful!";
            QMessageBox::information(this, "Success!", "Updating Saving Plan Successful!");
        }
        else
        {
            QMessageBox::warning(this, "Failed!", "Updating Saving Plan Failed!");
        }
    }

    updateSavingBox();
    updateSavingListWidget();
    navigateToPage(10);
    previousPageIndex = 11;
}



void MainWindow::on_returnFromReportPageButton_clicked()
{
    navigateToPage(0);
}


void MainWindow::on_returnFromSavingsListButton_clicked()
{
    navigateToPage(10);
}


void MainWindow::on_editSavingListButton_clicked()
{
    navigateToPage(11);
    previousPageIndex = 12;
}


void MainWindow::on_deleteSavingsListButton_clicked()
{
    QListWidgetItem* selectedItem = ui->savingsListWidget->currentItem();
    if (selectedItem == nullptr) {
        QMessageBox::warning(this, "Selection Error", "Please select a saving to delete.");
        return;
    }

    int savingID = selectedItem->data(Qt::UserRole).toInt();
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Delete saving", "Are you sure you want to delete this saving?", QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        if (db.deleteSaving(savingID)) {
            delete selectedItem;
            QMessageBox::information(this, "Success", "Saving deleted successfully.");
        } else {
            QMessageBox::warning(this, "Error", "Failed to delete the saving from the database.");
        }
    }
}


void MainWindow::on_returnFromSavingButton_clicked()
{
    navigateToPage(0);
}


void MainWindow::on_newSavingPlanButton_clicked()
{
    navigateToPage(11);
    previousPageIndex = 10;
}


void MainWindow::on_savingsListButton_clicked()
{
    updateSavingListWidget(); 
    ui->editSavingListButton->hide();
    ui->deleteSavingsListButton->hide();
    navigateToPage(12);
}


void MainWindow::on_addIncomeButton_clicked()
{
    updateSavingBox();
    navigateToPage(13);
}


void MainWindow::on_returnFromTransPage_clicked()
{
    navigateToPage(0);
}




void MainWindow::on_budgetListWidget_itemClicked(QListWidgetItem *item)
{
    bool hasSelection = ui->budgetListWidget->selectedItems().isEmpty();
    if(!hasSelection)
    {
        ui->editBudgetButton->show();
        ui->budgetDetaiButton->show();
        ui->deleteBudgetButton->show();
        budgetListWidgetItem = item;
        qDebug() << "Budget selected, ID is: " << item->data(Qt::UserRole).toInt();
    }else{
        ui->editBudgetButton->hide();
        ui->budgetDetaiButton->hide();
        ui->deleteBudgetButton->hide();
    }


}


// void MainWindow::on_budgetListWidget_itemSelectionChanged()
// {
//     bool hasSelection = ui->budgetListWidget->selectedItems().isEmpty();
//     if(!hasSelection)
//     {
//         ui->editBudgetButton->show();
//         ui->budgetDetaiButton->show();
//         ui->deleteBudgetButton->show();
//     }

//     budgetListWidgetItem = ui->budgetListWidget->currentItem();
// }


void MainWindow::on_itemListWidget_itemClicked(QListWidgetItem *it)
{
    bool hasSelection = ui->itemListWidget->selectedItems().isEmpty();
    if(!hasSelection)
    {
        ui->editItemButton->show();
        ui->deleteItemButton->show();
        itemListWidgetItem = it;
        qDebug() << "Item selected, ID is: " << it->data(Qt::UserRole).toInt();
    }else
    {
        ui->editItemButton->hide();
        ui->deleteItemButton->hide();
    }
}


void MainWindow::on_savingsListWidget_itemClicked(QListWidgetItem *item)
{
    bool hasSelection = ui->savingsListWidget->selectedItems().isEmpty();
    if(!hasSelection)
    {
        ui->editSavingListButton->show();
        ui->deleteSavingsListButton->show();
        savingListWidgetItem = item;
        qDebug() << "Saving selected, ID is: " << item->data(Qt::UserRole).toInt();
    }else
    {
        ui->editSavingListButton->hide();
        ui->deleteSavingsListButton->hide();
    }
}


void MainWindow::on_cancelEditItemButton_clicked()
{
    navigateToPage(5);
    ui->editItemNameInput->clear();
    ui->editItemLimitInput->clear();
    ui->editItemTotalInput->clear();
}


void MainWindow::on_editItemFinishButton_clicked()
{
    QString name = ui->editItemNameInput->text();
    double cap = ui->editItemLimitInput->text().toDouble();
    double total = ui->editItemTotalInput->text().toDouble();

    int itemID = itemListWidgetItem->data(Qt::UserRole).toInt();
    qDebug() << "current item ID is : " << itemID;

    if(db.updateItem(itemID,name,cap,total))
    {
        qDebug() << "You have successfully update Item to " ;
        QMessageBox::information(this,"Success","Update item to budget successful!");
    }else{
        QMessageBox::warning(this,"Failed","Failed updating  item to budget.");
    }

    updateItemBox();
    updateItemListWidget();
    ui->editItemNameInput->clear();
    ui->editItemLimitInput->clear();
    ui->editItemTotalInput->clear();
    navigateToPage(5);
}

