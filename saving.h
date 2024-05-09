#ifndef SAVING_H
#define SAVING_H
#include <QString>

class Saving
{
private:
    int savingID;
    double savingGoal;
    double goalRemaining;
    QString comment;

public:
    Saving(){this->savingID = 0, this->savingGoal = 0,this->goalRemaining = 0, this->comment = "";}
    Saving(int savingID, double goal,const QString& comment);
    Saving(double goal, const QString& comment);

    int getSavingID() const;
    double getSavingGoal() const;
    QString getComment() const;
    void updateGoalRemaining(double income);
    void setSavingGoal(double newGoal);
    void setSavingComment(const QString& newComment);
};

#endif // SAVING_H
