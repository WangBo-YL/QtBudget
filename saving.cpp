#include"saving.h"

Saving::Saving(int savingID, double goal, const QString& comment):savingID(savingID),savingGoal(goal),comment(comment){}

Saving::Saving(double goal, const QString& comment):savingGoal(goal),comment(comment){}

int Saving::getSavingID() const
{
    return savingID;
}

double Saving::getSavingGoal() const
{
    return savingGoal;
}

QString Saving::getComment() const
{
    return comment;
}

void Saving::setSavingComment(const QString& comment)
{
    this->comment = comment ;
}

void Saving::setSavingGoal(double goal)
{
    savingGoal = goal;
}

void Saving::updateGoalRemaining(double income)
{
    goalRemaining = savingGoal - income;
}
