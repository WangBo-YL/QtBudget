#ifndef ITEM_H
#define ITEM_H
#include <QString>

class Item
{
private:
    int itemID;
    int budgetID;
    int categoryID;
    QString name;
    double amount;


public:
    Item(int itemID, int budgetID,const QString& name, double amount,int categoryID);
    Item(int budgetID,const QString& name, double amount, int categoryID);
    void setName(const QString& newName);
    void setAmount(double Amount);
    void setCategoryID(int categoryID);
    void setBudgetID(int budgetID);
    void setItemID(int itemID);
    QString getName() const;
    double getAmount() const ;
    int getItemID() const;
    int getBudgetID() const;
    int getCategoryID() const;

};

#endif
