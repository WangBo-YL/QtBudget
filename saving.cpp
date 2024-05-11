#include"saving.h"

Saving::Saving(int savingID, const QString& name, double goal,double goalRemaining, const QString& comment):savingID(savingID),name(name),savingGoal(goal),goalRemaining(goalRemaining),comment(comment){}

Saving::Saving(const QString& name, double goal, double goalRemaining, const QString& comment):name(name),savingGoal(goal),goalRemaining(goalRemaining),comment(comment){}

int Saving::getSavingID() const
{
    return savingID;
}

double Saving::getSavingGoal() const
{
    return savingGoal;
}

double Saving::getGoalRemaining() const{return goalRemaining;}

QString Saving::getComment() const
{
    return comment;
}

QString Saving::getName() const {return name;}

void Saving::setSavingComment(const QString& comment)
{
    this->comment = comment ;
}

void Saving::setSavingGoal(double goal)
{
    savingGoal = goal;
}

void Saving::setSavingName(const QString& name)
{
    this->name = name;
}
