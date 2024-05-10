#ifndef EXPENSE_H
#define EXPENSE_H
#include <QString>
class Expense{
private:
    int expenseID;
    double total;
    QString description;
    QString date;
    int itemID;
public:
    Expense(){this->expenseID = 0, this->itemID = 0,this->total =0.0, this->description = "", this->date = "";}
    Expense(int itemID, double total,const QString& description, const QString& date);
    Expense(double total, const QString& description, const QString& date);
    void setTotal(double total);
    void setDescription(const QString& des);
    void setDate(const QString& date);
    int getItemID() const;
    double getTotal() const;
    QString getDescription() const;
    int getExpID() const;
    QString getDate() const;
};

#endif // EXPENSE_H
