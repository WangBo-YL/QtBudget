#include"budget.h"

Budget::Budget(const QString& name, double total):name(name),totalAmount(total)
{
}

Budget::Budget(int id, const QString& name, double total)
    :budgetID(id),name(name),totalAmount(total)
{

}

int Budget::getBudgetID()const
{
    return budgetID;
}

const QString Budget::getName() const
{
    return name;
}

void Budget::setName(const QString& name)
{
    this->name = name;
}

double Budget::getTotal() const
{
    return totalAmount;
}

void Budget::setBudgetID(int id)
{
    budgetID = id;
}

void Budget::setTotal(double total)
{
    totalAmount = total;
}
