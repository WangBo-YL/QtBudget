#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , db("budget.db")
{
    ui->setupUi(this);
    updateBudgetBox();
    updateItemBox();
    ui->newItemTotalInput->setText("0");
    // connect(ui->saveBudgetButton, &QPushButton::clicked, this, &MainWindow::on_saveBudgetButton_clicked);
}


MainWindow::~MainWindow()
{
    delete ui;
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

void MainWindow::updateCurrentBudget(const QString& name)
{
    try{
        currentBudget = db.getBudget(name);
    }catch(const std::runtime_error&e)
    {
        qDebug() << "Error updating budget: " << e.what();
    }
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
    navigateToPage(4);

}


void MainWindow::on_newItemButton_clicked()
{
    navigateToPage(3);
    previousPageIndex = 1;
}


void MainWindow::on_cancelCreateBudgetButton_clicked()
{
    navigateToPage(1);

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
            if(db.updateBudget(currentBudget))
            {
                QMessageBox::information(this,"Success", "Update Budget Successful!");
                qDebug() << "Update Budget Successful!";
            }

        } catch (...) {
            qDebug() << "Failed to update budget.";
        }

    }
    updateBudgetBox();
    navigateToPage(1);
    previousPageIndex = 2;
}


void MainWindow::on_returnFromBudgetListButton_clicked()
{
    navigateToPage(1);

}


void MainWindow::on_budgetDetaiButton_clicked()
{
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

}




void MainWindow::on_cancelAddItemButton_clicked()
{
    navigateToPage(1);
}


void MainWindow::on_addItemButton_clicked()
{
    QString budgetName = ui->budgetComboBox->currentText();
    updateCurrentBudget(budgetName);
    QString name = ui->newItemNameInput->text();
    double cap = ui->newItemLimitInput->text().toDouble();
    double total = ui->newItemTotalInput->text().toDouble();
    Item item(currentBudget.getBudgetID(),name,cap,total);
    db.addItem(item);
    qDebug() << "You have successfully add a new Item to " << budgetName;

    updateItemBox();
    ui->newBudgetNameInput->clear();
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
    int itemID = db.getItemID(itemName);
    Expense expense(itemID,amount,description);
    if(db.addExpense(expense))
    {
        qDebug() << "Add new expense to " << itemName << " successfull!" ;
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
    navigateToPage(7);
    previousPageIndex = 6;
}


void MainWindow::on_returnFromItemListButton_clicked()
{
    navigateToPage(1);
}


void MainWindow::on_editItemButton_clicked()
{
    navigateToPage(3);
    previousPageIndex = 5;
}


void MainWindow::on_deleteItemButton_clicked()
{

}


void MainWindow::on_cancelAddIncomeButton_clicked()
{
    navigateToPage(1);
}


void MainWindow::on_finishAddIncomeButton_clicked()
{
    navigateToPage(10);
    previousPageIndex = 13;
}


void MainWindow::on_cancelSavingPlanButton_clicked()
{
    navigateToPage(10);
}


void MainWindow::on_finishAddSavingPlanButton_clicked()
{
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
    navigateToPage(12);
}


void MainWindow::on_addIncomeButton_clicked()
{
    navigateToPage(13);
}


void MainWindow::on_returnFromTransPage_clicked()
{
    navigateToPage(0);
}


