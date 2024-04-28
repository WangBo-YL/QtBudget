#ifndef CATEGORY_H
#define CATEGORY_H
#include <string>
class Category
{
    public:
    Category(int id,std::string name);
    int getCategoryID() const;
    std::string getName() const;
    void setName(std::string name);
    void setCategoryID(int id);

    private:
        int categoryID;
        std::string name;
};

#endif
