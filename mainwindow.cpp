#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , db("budget.db")
{
    ui->setupUi(this);

    ui->budgetListWidget->setSelectionMode(QAbstractItemView::SingleSelection);

    listWidget = new QListWidget(this);
    connect(listWidget, &QListWidget::itemClicked, this, &MainWindow::on_itemSelected);

    // connect(ui->saveItemButton, &QPushButton::clicked, this, &MainWindow::on_saveItemButton_clicked);
    connect(ui->cancelButton, &QPushButton::clicked,this, &MainWindow::on_cancelButton_clicked);
    connect(ui->exitButton, &QPushButton::clicked,this, &MainWindow::on_cancelButton_clicked);
    connect(ui->cancelButton_3, &QPushButton::clicked,this, &MainWindow::on_cancelButton_clicked);
    connect(ui->returnButton, &QPushButton::clicked,this, &MainWindow::on_returnButton_clicked);
    connect(ui->budgetListWidget, &QListWidget::itemClicked, this, &MainWindow::on_budgetListWidget_itemClicked);
    connect(ui->editItemButton, &QPushButton::clicked, this, &MainWindow::on_editItemButton_clicked);
    connect(ui->budgetListWidget, &QListWidget::itemClicked, this, &MainWindow::on_budgetSelected);
    connect(ui->editBudgetButton, &QPushButton::clicked, this, &MainWindow::on_editBudgetButton_clicked);
    connect(ui->deleteItemButton, &QPushButton::clicked, this, &MainWindow::on_deleteItemButton_clicked);
}


MainWindow::~MainWindow()
{
    delete ui;
}

int MainWindow::getBudgetID(const Item& item)
{

}

int MainWindow::findItemIndex(QListWidgetItem* clickedItem)
{
    for(int i = 0; i < items.size(); ++i)
    {
        if(items[i].getItemID() == clickedItem.
    }
}

void MainWindow::on_itemSelected(QListWidgetItem* item)
{
    int itemID = item->data(Qt::UserRole).toInt();
    qDebug() << "Clicked Item ID: " <<itemID << " from Budget ID: " << currentBudgetID;
    if(!ui->itemListWidget->selectedItems().isEmpty())
    {
        ui->editItemButton->show();
    }else{
        ui->editItemButton->hide();
    }
}
void MainWindow::updateItemList(int budgetID)
{
    listWidget->clear();
    QList<Item> items = db.getItemsByBudget(budgetID);
    ui->itemListWidget->clear();
    for (const Item& item : items) {
        ui->itemListWidget->addItem(item.getName() + " - " + QString::number(item.getAmount()));
    }
}


void MainWindow::updateBudgetList()
{
    QSqlQuery query(db.getDatabase());
    query.prepare("SELECT * FROM item WHERE budgetID = ?");
    query.addBindValue(currentBudgetID);
    if (!query.exec()) {
        qDebug() << "Error retrieving items: " << query.lastError().text();
        return;
    }

    ui->itemListWidget->clear();
    while (query.next()) {
        QString itemDesc = QString("%1 - $%2").arg(query.value("itemName").toString()).arg(query.value("amount").toDouble());
        QListWidgetItem *itemWidget = new QListWidgetItem(itemDesc);
        itemWidget->setData(Qt::UserRole, query.value("itemID"));
        ui->itemListWidget->addItem(itemWidget);
    }
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

    QString existBudget = db.getBudget(budgetName).getName();
    if (existBudget == budgetName) {
        QMessageBox::warning(this, "Warning", "A budget with this name already exists.");
        return;
    }

    Budget newBudget(budgetName, 0);
    int newBudgetID = db.addBudget(newBudget);
    if (newBudgetID!=1) {
        QMessageBox::information(this,"Success","Budget Created Successful!");
        navigateToPage(3);
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

    Item newItem(itemName,itemAmount, categoryID);
    int newItemID = db.addItem(newItem);
    if(newItemID !=1)
    {
        qDebug() << "Item has been added to budget with ID" << newItemID << " in budget " << currentBudgetID;
        QMessageBox::information(this,"Success", "Item added successfully!");
    }
    ui->itemNameEdit->clear();
    ui->amountEdit->clear();
    ui->categoryBox->setCurrentIndex(0);
    navigateToPage(3);  // Assuming this is the confirmation or next logical page
}


void MainWindow::on_budgetSelected(int budgetID)
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
        Item item = db.getItem(itemID);
        if (item.getBudgetID() == currentBudgetID) {
            navigateToPage(6);
        } else {
            QMessageBox::warning(this, "Error", "Selected item does not belong to the current budget.");
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

    Item newItem(itemName,categoryID,itemAmount);

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

