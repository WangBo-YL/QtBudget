#include"budget.h"

Budget::Budget(const QString& name, double total = 0):name(name),total(total)
{
}

Budget::Budget(int id, const QString& name, double total)
    :budgetID(id),name(name),total(total)
{

}

int Budget::getBudgetID()const
{
    return budgetID;
}

QString Budget::getName() const
{
    return name;
}

void Budget::setName(const QString& name)
{
    this->name = name;
}

double Budget::getTotal() const
{
    return total;
}

void Budget::setBudgetID(int id)
{
    budgetID = id;
}

void Budget::setTotal(double total)
{
    this->total = total;
}
