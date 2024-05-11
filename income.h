#ifndef INCOME_H
#define INCOME_H
#include <QString>
class income
{
private:
    int incomeID;
    int savingID;
    double amount;
    QString date;

public:
    income(){this->incomeID = 0, this->savingID = 0, this->amount = 0.0, this->date= "";}
    income(int savingID, double amount, const QString& date);
    income(double amount, const QString& date);
    int getIncomeId() const;
    int getSavingID() const;
    double getAmount() const;
    QString getDate() const;

    void setAmount(double newAmount);
    void setDate(const QString& newDate);
    void setSavingID(int savingID) {this->savingID = savingID;}

};

#endif // INCOME_H
