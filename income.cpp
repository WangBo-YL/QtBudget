#include "income.h"

income::income(int savingID, double amount, const QString& date):savingID(savingID),amount(amount),date(date){}

income::income(double amount, const QString& date):amount(amount),date(date){}

double income::getAmount() const {return amount;}

int income::getIncomeId() const {return incomeID;}

int income::getSavingID() const {return savingID;}

QString income::getDate() const {return date;}

void income::setDate(const QString& newDate){date = newDate;}

void income::setAmount(double newAmount){amount = newAmount;}
