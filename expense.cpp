#include "expense.h"

Expense::Expense(int itemID, double total, const QString& description):total(total),description(description),itemID(itemID){}


QString Expense::getDescription() const
{
    return description;
}

int Expense::getItemID() const
{
    return itemID;
}

double Expense::getTotal() const
{
    return total;
}

void Expense::setDescription(const QString& des)
{
    description = des;
}

void Expense::setTotal(double total)
{
    this->total = total;
}


int Expense::getExpID() const
{
    return expenseID;
}
