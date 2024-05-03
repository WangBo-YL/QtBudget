#ifndef CATEGORY_H
#define CATEGORY_H
#include <QString>
class Category
{
    public:
    Category(const QString& name,int budgetID,double total);
        Category(int categoryID, int budgetID, const QString& name, double total);
    int getCategoryID() const;
    QString getName() const;
    int getBudgetID() const;
    void setName(const QString& name);
    void setTotalSpent(double total);
    double getTotalSpent() const;

    private:
        int categoryID;
        int budgetID;
        QString name;
        double totalSpent;
};

#endif
