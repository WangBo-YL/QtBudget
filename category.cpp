#include "category.h"

Category::Category(const QString& name, int budgetID, double total)
    :budgetID(budgetID),name(name),totalSpent(total)
{}

Category::Category(int categoryID, int budgetID, const QString& name, double total)
    : categoryID(categoryID),budgetID(budgetID),name(name),totalSpent(total)
{}

int Category::getCategoryID() const
{
    return categoryID;
}

QString Category::getName() const
{
    return name;
}

void Category::setName(const QString& name)
{
    this->name = name;
}

void Category::setTotalSpent(double total)
{
    totalSpent = total;
}

double Category::getTotalSpent() const
{
    return totalSpent;
}

int Category::getBudgetID() const
{
    return budgetID;
}
