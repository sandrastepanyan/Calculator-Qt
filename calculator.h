#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QMainWindow>
#include <QGridLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QKeyEvent>
#include <QVector>
#include <QStack>

QT_BEGIN_NAMESPACE
namespace Ui {
class Calculator;
}
QT_END_NAMESPACE

class Calculator : public QMainWindow
{
    Q_OBJECT

public:
    Calculator(QWidget *parent = nullptr);
    ~Calculator();

private:
    void keyPressEvent(QKeyEvent* event);
    void onNumberClicked();
    void onOperationClicked();
    void onEqualClicked();
    void onPercentClicked();
    void onACClicked();
    void onNegPosClicked();
    void onDeleteClicked();
    void onOperationKey();
    void dotClicked();
    QString calculate(QVector<QString>& vec);

    void connectNumberButtons();
    void connectOperationButtons();
    void design();
    void changeButtonStyle();

private:
    QWidget* centralWidget;
    QGridLayout* m_layout;
    QLineEdit* m_line;
    QString lastOperation;
    QVector<QString> vec;
};
#endif // CALCULATOR_H
