#ifndef EXPENSE_H
#define EXPENSE_H
#include <QString>
class Expense{
private:
    int expenseID;
    double total;
    QString description;
    int itemID;
public:
    Expense(){this->expenseID = 0, this->itemID = 0,this->total =0.0, this->description = "";}
    Expense(int itemID, double total,const QString& description);
    void setTotal(double total);
    void setDescription(const QString& des);
    int getItemID() const;
    double getTotal() const;
    QString getDescription() const;
    int getExpID() const;

};

#endif // EXPENSE_H
