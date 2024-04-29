#include "category.h"

Category::Category(int id, const QString& name)
    :categoryID(id), name(name)
{}

int Category::getCategoryID() const
{
    return categoryID;
}

QString Category::getName() const
{
    return name;
}

void Category::setName(const QString& name)
{
    this->name = name;
}

void Category::setCategoryID(int id)
{
    categoryID = id;
}
