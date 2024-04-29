#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , db("budget.db")
    ,currentBudget(nullptr)
{
    ui->setupUi(this);
    ui->itemListWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->budgetListWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    // connect(ui->saveItemButton, &QPushButton::clicked, this, &MainWindow::on_saveItemButton_clicked);
    connect(ui->cancelButton, &QPushButton::clicked,this, &MainWindow::on_cancelButton_clicked);
    connect(ui->exitButton, &QPushButton::clicked,this, &MainWindow::on_cancelButton_clicked);
    connect(ui->cancelButton_3, &QPushButton::clicked,this, &MainWindow::on_cancelButton_clicked);
    connect(ui->returnButton, &QPushButton::clicked,this, &MainWindow::on_returnButton_clicked);
    connect(ui->budgetListWidget, &QListWidget::itemClicked, this, &MainWindow::on_budgetListWidget_itemClicked);
    connect(ui->itemListWidget, &QListWidget::itemSelectionChanged, this, &MainWindow::on_itemSelected);
    connect(ui->editItemButton, &QPushButton::clicked, this, &MainWindow::on_editItemButton_clicked);
    connect(ui->budgetListWidget, &QListWidget::itemClicked, this, &MainWindow::on_budgetSelected);
    connect(ui->editBudgetButton, &QPushButton::clicked, this, &MainWindow::on_editBudgetButton_clicked);
    connect(ui->deleteItemButton, &QPushButton::clicked, this, &MainWindow::on_deleteItemButton_clicked);
}


MainWindow::~MainWindow()
{
    delete ui;
    delete currentBudget;
}


void MainWindow::on_itemSelected()
{
    if(!ui->itemListWidget->selectedItems().isEmpty())
    {
        ui->editItemButton->show();
    }else{
        ui->editItemButton->hide();
    }
}
void MainWindow::updateItemList(int currentBudgetID)
{
    QList<Item> items = db.getItemsByBudget(currentBudgetID);
    ui->itemListWidget->clear();
    for(const Item& item:items)
    {
        QListWidgetItem *itemWidget = new QListWidgetItem(item.getName()+ " - $" + QString::number(item.getAmount()));
        itemWidget->setData(Qt::UserRole, QVariant(item.getItemID()));
        ui->itemListWidget->addItem(itemWidget);
    }
    ui -> editItemButton->hide();
}


void MainWindow::updateBudgetList()
{
    QList<Budget> budgets = db.getAllBudgets();
    ui->budgetListWidget->clear();
    for (const Budget& budget : budgets) {
        QListWidgetItem* item = new QListWidgetItem(budget.getName());
        item->setData(Qt::UserRole, QVariant(budget.getBudgetID()));
        ui->budgetListWidget->addItem(item);
    }
    ui->editBudgetButton->hide();
}

void MainWindow::returnToHomepage()
{
    navigateToPage(0);
}

void MainWindow::navigateToPage(int pageIndex)
{
    ui->stackedWidget->setCurrentIndex(pageIndex);
}



void MainWindow::on_BudgetButton_clicked()
{
    navigateToPage(1);
}


void MainWindow::on_returnButton_clicked()
{
    navigateToPage(0);
}

void MainWindow::on_NewBudgetButton_clicked()
{
    navigateToPage(2);
}


void MainWindow::on_cancelButton_clicked()
{
    navigateToPage(1);
    ui->budgetNameTxt->clear();
    qDebug() << "Return to homepage. ";

}


void MainWindow::on_createButton_clicked()
{
    QString budgetName = ui->budgetNameTxt->text().trimmed();
    if (budgetName.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Budget name cannot be empty.");
        return;
    }

    int existingBudgetID = db.getBudgetIDByName(budgetName);
    if (existingBudgetID != -1) {
        QMessageBox::warning(this, "Warning", "A budget with this name already exists.");
        return;
    }

    Budget newBudget(budgetName, 0);
    if (db.addBudget(newBudget)) {
        currentBudgetID = db.getBudgetIDByName(budgetName);
        currentBudgetName = budgetName;
        navigateToPage(3);
        updateBudgetList();
        updateItemList(currentBudgetID);
    } else {
        QMessageBox::warning(this, "Database Error", "Failed to add budget.");
    }
}


void MainWindow::on_addItemButton_clicked()
{
    navigateToPage(4);
}



void MainWindow::on_saveItemButton_clicked() {

    QString itemName = ui->itemNameEdit->text();
    double itemAmount = ui->amountEdit->text().toDouble();
    int categoryID = ui->categoryBox->currentIndex();

    if(itemName.trimmed().isEmpty())
    {
        QMessageBox::warning(this, "Input Error", "Item name cannot be empty.");
        return;
    }

    if (itemAmount <= 0) {
        QMessageBox::warning(this, "Input Error", "Item amount must be greater than 0.");
        return;
    }

    Item newItem(currentBudgetID,itemName,itemAmount,categoryID);
    if(db.addItem(newItem))
    {
        QMessageBox::information(this, "Success", "Item added successfully.");
        navigateToPage(3);  // Assuming this is the confirmation or next logical page
        updateItemList(currentBudgetID);
    }else{
         QMessageBox::warning(this, "Database Error", "Failed to add item.");
    }

    ui->itemNameEdit->clear();
    ui->amountEdit->clear();
    ui->categoryBox->setCurrentIndex(0);
    navigateToPage(3);  // Assuming this is the confirmation or next logical page
}


void MainWindow::on_budgetSelected()
{
    if(ui->budgetListWidget->currentItem())
    {
        ui->editBudgetButton->show();
    }
}



void MainWindow::on_ExistingBudgetButton_clicked()
{
    navigateToPage(5);
    updateBudgetList();

}


void MainWindow::on_returnButton_2_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}



void MainWindow::on_budgetListWidget_itemClicked()
{
    ui->editBudgetButton->show();
}


void MainWindow::on_exitButton_clicked()
{
    navigateToPage(1);
    updateBudgetList();
}


void MainWindow::on_editItemButton_clicked()
{

    QListWidgetItem *selectedItem = ui->itemListWidget->currentItem();

    if (selectedItem) {
        int itemID = selectedItem->data(Qt::UserRole).toInt();
        try {
            Item item = db.getItem(itemID);
            if (item.getBudgetID() == currentBudgetID) {
                navigateToPage(6);
            } else {
                QMessageBox::warning(this, "Error", "Selected item does not belong to the current budget.");
            }
        } catch (const std::exception& e) {
            QMessageBox::warning(this, "Database Error", QString("Failed to retrieve item details: %1").arg(e.what()));
        }
    } else {
        QMessageBox::warning(this, "Selection Needed", "Please select an item to edit.");
    }
}


void MainWindow::on_editItemSaveButton_clicked()
{

    QString itemName = ui->itemNameEdit2->text();
    double itemAmount = ui->amountEdit2->text().toDouble();
    int categoryID = ui->categoryBox2->currentIndex();
    if(itemName.trimmed().isEmpty())
    {
        QMessageBox::warning(this, "Input Error", "Item name cannot be empty.");
        return;
    }

    if (itemAmount <= 0) {
        QMessageBox::warning(this, "Input Error", "Item amount must be greater than 0.");
        return;
    }

    Item newItem(currentBudgetID,itemName,categoryID,itemAmount);
    if(db.updateItem(newItem,currentBudgetID))
    {
        QMessageBox::information(this, "Success", "Item added successfully.");
        navigateToPage(3);  // Assuming this is the confirmation or next logical page
        updateItemList(currentBudgetID);
    }else{
        QMessageBox::warning(this, "Database Error", "Failed to add item.");
    }

    ui->itemNameEdit2->clear();
    ui->amountEdit2->clear();
    ui->categoryBox2->setCurrentIndex(0);
    navigateToPage(3);
}


void MainWindow::on_cancelEditItemButton_clicked()
{
    navigateToPage(3);
}


void MainWindow::on_editBudgetButton_clicked()
{
    if (ui->budgetListWidget->currentItem()) {
        int selectedBudgetID = ui->budgetListWidget->currentItem()->data(Qt::UserRole).toInt();
        currentBudgetID = selectedBudgetID;
        navigateToPage(6);
    }
}


void MainWindow::on_deleteItemButton_clicked()
{
    QListWidgetItem* selectedItem = ui->itemListWidget->currentItem();
    if (selectedItem) {
        int itemID = selectedItem->data(Qt::UserRole).toInt();
        if (QMessageBox::question(this, "Confirm Delete", "Are you sure you want to delete this item?") == QMessageBox::Yes) {
            if (db.deleteItem(itemID)) {
                QMessageBox::information(this, "Success", "Item deleted successfully.");
                updateItemList(currentBudgetID);
            } else {
                QMessageBox::warning(this, "Error", "Failed to delete item.");
            }
        }
    } else {
        QMessageBox::warning(this, "No Selection", "Please select an item to delete.");
    }
}

