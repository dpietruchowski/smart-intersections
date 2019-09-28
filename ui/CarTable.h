#ifndef CARTABLE_H
#define CARTABLE_H

#include <QWidget>
#include <QTableWidget>

class CarAgent;

class CarTable: public QWidget
{
public:
    CarTable(QWidget* parent);

    enum Column {
        CarId,
        Distance,
        Velocity,
        MaxVelocity
    };

    void clear();
    void updateAgent(const CarAgent* agent);
    void insertAgent(const CarAgent* agent);

private:
    int getRow(const CarAgent* agent);

private:
    QTableWidget* tableWidget = nullptr;
};

#endif // CARTABLE_H
