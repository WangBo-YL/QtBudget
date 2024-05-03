#ifndef BUDGET_H
#define BUDGET_H
#include<QVector>
class Budget
{
    public:
        Budget(const QString& name);
        Budget(const QString& name, double total);
        Budget(int id, const QString& name, double total);
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
    double total = 0;
    void updateRemaining()
    {
        remainingAmount = total;
    }
};

#endif
