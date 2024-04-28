#include "category.h"

Category::Category(int id,std::string name)
    :categoryID(id), name(name)
{}

int Category::getCategoryID() const
{
    return categoryID;
}

std::string Category::getName() const
{
    return name;
}

void Category::setName(std::string name)
{
    this->name = name;
}

void Category::setCategoryID(int id)
{
    categoryID = id;
}
