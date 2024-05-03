#ifndef ITEM_H
#define ITEM_H
#include <QString>

class Item
{
private:
    int itemID;
    int budgetID;
    double cap;
    QString name;
    double itemTotal;

public:
    Item(int itemID, int budgetID,const QString& name, double amount);
    Item(const QString& name, double amount, int budgetID);
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
    bool operator==(const Item& other)
    {
        return this->getItemID() == other.getItemID();
    }

    bool operator !=(const Item& other)
    {
        return this->getItemID()!= other.getItemID();
    }

};

#endif
