#include "item.h"

Item::Item(int itemID, int budgetID,const QString& name, double cap, double total):itemID(itemID),budgetID(budgetID),name(name),cap(cap),itemTotal(total)
{
}
Item::Item(int budgetId, const QString& name, double cap, double total):budgetID(budgetId),name(name),cap(cap),itemTotal(total)
{
}


QString Item::getName() const { return name; }
double Item::getTotal() const { return itemTotal; }
int Item::getItemID() const { return itemID; }
int Item::getBudgetID() const { return budgetID; }
double Item::getCap() const {return cap;}

void Item::setName(const QString& newName) { name = newName; }
void Item::setCap(double cap) { this->cap = cap; }
void Item::setTotal(double total) {itemTotal = total;}
void Item::setBudgetID(int id) {budgetID = id;}

