#include "expense.h"

Expense::Expense(int itemID, double total, const QString& description, const QString& date):total(total),description(description),date(date), itemID(itemID){}

Expense::Expense(double total, const QString& description, const QString& date):total(total),description(description),date(date){}

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

void Expense::setDate(const QString& date) {this->date = date;}

QString Expense::getDate() const {return date;}

int Expense::getExpID() const
{
    return expenseID;
}
