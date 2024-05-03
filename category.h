#ifndef CATEGORY_H
#define CATEGORY_H
#include <QString>
class Category
{
    public:
    Category(int id,const QString& name);
    int getCategoryID() const;
    QString getName() const;
    void setName(const QString& name);
    void setCategoryID(int id);

    private:
        int categoryID;
        QString name;
        int budgetID;
        double totalSpent;
};

#endif
