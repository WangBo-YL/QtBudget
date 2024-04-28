#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , db("budget.db")
    , currentBudget(nullptr)
{
    ui->setupUi(this);
    // connect(ui->saveItemButton, &QPushButton::clicked, this, &MainWindow::on_saveItemButton_clicked);
    connect(ui->cancelButton, &QPushButton::clicked,this, &MainWindow::on_cancelButton_clicked);
    connect(ui->exitButton, &QPushButton::clicked,this, &MainWindow::on_cancelButton_clicked);
    connect(ui->cancelButton_3, &QPushButton::clicked,this, &MainWindow::on_cancelButton_clicked);
    connect(ui->returnButton, &QPushButton::clicked,this, &MainWindow::on_returnButton_clicked);
    connect(ui->budgetListWidget, &QListWidget::itemClicked, this, &MainWindow::on_budgetListWidget_itemClicked);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete currentBudget;
}


void MainWindow::updateItemList()
{

}

void MainWindow::updateBudgetList()
{

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

}

void MainWindow::on_NewBudgetButton_clicked()
{

}


void MainWindow::on_cancelButton_clicked()
{
    navigateToPage(1);
    ui->budgetNameTxt->clear();
    qDebug() << "Return to homepage. ";

}


void MainWindow::on_createButton_clicked()
{


    navigateToPage(3);  //
}


void MainWindow::on_addItemButton_clicked()
{

}



void MainWindow::on_saveItemButton_clicked() {

    navigateToPage(3);  // Assuming this is the confirmation or next logical page
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



void MainWindow::on_budgetListWidget_itemClicked(QListWidgetItem *item)
{

}


void MainWindow::on_exitButton_clicked()
{
    navigateToPage(1);
    updateBudgetList();
}

