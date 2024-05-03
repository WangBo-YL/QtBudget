#include "item.h"

Item::Item(int itemID,int budgetID, const QString& name, double amount, int categoryID)
    : itemID(itemID), budgetID(budgetID),  categoryID(categoryID), name(name),amount(amount) {}

Item::Item(const QString& name, double amount, int categoryID):budgetID(budgetID),categoryID(categoryID),name(name),amount(amount)
{
}



QString Item::getName() const { return name; }
double Item::getAmount() const { return amount; }
int Item::getItemID() const { return itemID; }
int Item::getBudgetID() const { return budgetID; }
int Item::getCategoryID() const { return categoryID; }
void Item::setItemID(int id){itemID = id;}
void Item::setName(const QString& newName) { name = newName; }
void Item::setAmount(double newAmount) { amount = newAmount; }
void Item::setBudgetID(int id) {budgetID = id;}
void Item::setCategoryID(int id){categoryID = id;}
