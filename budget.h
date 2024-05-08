#ifndef BUDGET_H
#define BUDGET_H
#include<QVector>
class Budget
{
    public:
    Budget(){this->budgetID = 0,this->name="", this->total = 0.0, this->remainingAmount = 0.0;}
        Budget(const QString& name);
        Budget(const QString& name, double total);
        Budget(int id, const QString& name, double total, double remain);
        int getBudgetID() const;
        QString getName() const;
        double getTotal() const;
        double getRemaining() const;
        void setName(const QString& newName);
        void setBudgetID(int newID);
        void setTotal(double total);
        void setRemaining(double spend);
        bool operator==(const Budget& other)
        {
            return this->getBudgetID() == other.getBudgetID();
        }

        bool operator != (const Budget& other)
        {
            return this->getBudgetID() != other.getBudgetID();
        }
    private:
    int budgetID;
    QString name;
    double remainingAmount;
    double total;
};

#endif
