#include"budget.h"

Budget::Budget(const QString& name):name(name)
{
}
Budget::Budget(const QString& name, double total):name(name),total(total)
{
}

Budget::Budget(int id, const QString& name, double total)
    :budgetID(id),name(name),total(total)
{

}

//getter
int Budget::getBudgetID()const
{
    return budgetID;
}

QString Budget::getName() const
{
    return name;
}

double Budget::getTotal() const
{
    return total;
}

//setter
void Budget::setName(const QString& name)
{
    this->name = name;
}

void Budget::setBudgetID(int id)
{
    budgetID = id;
}

void Budget::setTotal(double total)
{
    this->total = total;
}

void Budget::setRemaining(double spend)
{
    remainingAmount = total - spend;
}

double Budget::getRemaining() const
{
    return remainingAmount;
}
