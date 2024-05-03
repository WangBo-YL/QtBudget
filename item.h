#ifndef ITEM_H
#define ITEM_H
#include <QString>

class Item
{
private:
    int itemID;
    int budgetID;
    QString name;
    double cap;
    double itemTotal;

public:
    Item(int itemID, int budgetID,const QString& name, double cap, double total);
    Item(int budgetID,const QString& name, double cap,double total);
    void setName(const QString& newName);
    void setCap(double cap);
    void setTotal(double total);
    void setBudgetID(int budgetID);
    QString getName() const;
    double getCap() const ;
    double getTotal() const;
    int getItemID() const;
    int getBudgetID() const;

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
