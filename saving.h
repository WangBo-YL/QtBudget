#ifndef SAVING_H
#define SAVING_H
#include <QString>

class Saving
{
private:
    int savingID;
    QString name;
    double savingGoal;
    double goalRemaining;
    QString comment;

public:
    Saving(){this->savingID = 0, this->name = "",this->savingGoal = 0,this->goalRemaining = 0, this->comment = "";}
    Saving(int savingID, const QString& name,double goal, double goalRemaining,const QString& comment);
    Saving(const QString& name,double goal, double goalRemaining, const QString& comment);

    QString getName() const;
    int getSavingID() const;
    double getSavingGoal() const;
    double getGoalRemaining() const;
    QString getComment() const;
    void setSavingGoal(double newGoal);
    void setSavingComment(const QString& newComment);
    void setSavingName(const QString& name);
};

#endif // SAVING_H
