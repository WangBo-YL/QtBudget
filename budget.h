#ifndef BUDGET_H
#define BUDGET_H
#include<QVector>
class Budget
{
    public:
        Budget(const QString& name, double total);
        Budget(int id, const QString& name, double total);
        int getBudgetID() const;
        const QString getName() const;
        double getTotal() const;
        void setName(const QString& newName);
        void setBudgetID(int newID);
        void setTotal(double total);


    private:
    int budgetID;
    QString name;
    double totalAmount = 0;
};

#endif
