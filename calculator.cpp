#include "calculator.h"

Calculator::Calculator(QWidget *parent)
    : QMainWindow(parent)
    , centralWidget(new QWidget(this))
    , m_layout(new QGridLayout())
    , m_line(new QLineEdit("", centralWidget))
{
    this->setCentralWidget(centralWidget);
    centralWidget->setLayout(m_layout);

    design();
    connectNumberButtons();
    connectOperationButtons();
}

Calculator::~Calculator() = default;

void Calculator::onNumberClicked() {
    if (lastOperation == "%" || lastOperation == "+" || lastOperation == "-" || lastOperation == "x" || lastOperation == "/" || lastOperation == "=") {
        m_line->clear();
        lastOperation = "";
    }
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (button) {
        QString buttonText = button->text();
        m_line->setText(m_line->text() + buttonText);
    }
}

void Calculator::onOperationClicked() {
    if (m_line->text() != "") {
        QPushButton* button = qobject_cast<QPushButton*>(sender());
        if (button) {
            QString op = button->text();
            lastOperation = op;
            QString num = m_line->text();
            vec.push_back(num);
            vec.push_back(op);
        }
    }
}

void Calculator::onOperationKey() {
    if (m_line->text() != "") {
        QString num = m_line->text();
        vec.push_back(num);
        vec.push_back(lastOperation);
    }
}

void Calculator::onNegPosClicked()
{
    QString text = m_line->text();
    if (text[0] == '-') {
        if (!text.isEmpty()) {
            text.remove(0, 1);
            m_line->setText(text);
        }
    }
    else {
        m_line->setText('-' + m_line->text());
    }
}

void Calculator::onDeleteClicked() {
    QString text = m_line->text();
    if (!text.isEmpty()) {
        text.chop(1);
        m_line->setText(text);
    }
}

void Calculator::onEqualClicked() {
    if (m_line->text() != "") {
        lastOperation = "=";
        vec.push_back(m_line->text());
        QString result = calculate(vec);
        m_line->clear();
        m_line->setText(result);
    }
}

QString Calculator::calculate(QVector<QString>& expression) {
    QStack<float> operands;
    QChar lastOperator = '+';

    for (const QString& token : expression) {
        if (token[0].isDigit() || (token.size() != 1 && token[1].isDigit())) {
            float num = token.toFloat();
            operands.push(num);
        } else if (token == "+" || token == "-" || token == "x" || token == "/") {
            lastOperator = token[0];
            continue;
        }

        if (operands.size() < 2 || lastOperator == '+' || lastOperator == '-') {
            continue; // For addition and subtraction, no need to evaluate yet
        }

        // For multiplication and division, evaluate immediately
        double b = operands.top();
        operands.pop();
        double a = operands.top();
        operands.pop();

        if (lastOperator == 'x') {
            operands.push(a * b);
        } else if (lastOperator == '/') {
            if (b != 0) {
                operands.push(a / b);
            }
        }
    }

    // Final evaluation for remaining operations (addition and subtraction)
    double result = 0.0;
    while (!operands.empty()) {
        result += operands.top();
        operands.pop();
    }
    vec.clear();
    return QString::number(result);
}

void Calculator::onPercentClicked() {
    lastOperation = "%";
    QString text = m_line->text();
    bool ok;
    float number = text.toFloat(&ok);
    if (ok) {
        number = number / 100;
        text = QString::number(number);
        m_line->setText(text);
    }
}

void Calculator::onACClicked() {
    vec.clear();
    m_line->clear();
}

void Calculator::dotClicked() {
    m_line->setText(m_line->text() + '.');
}

void Calculator::keyPressEvent(QKeyEvent* event) {
    if (event->key() >= Qt::Key_0 && event->key() <= Qt::Key_9) {
        if (lastOperation == "%" || lastOperation == "+" || lastOperation == "-" || lastOperation == "x" || lastOperation == "/" || lastOperation == "=") {
            m_line->clear();
            lastOperation = "";
        }
        QString buttonText = QString(event->text());
        m_line->setText(m_line->text() + buttonText);
    }
    else if (event->key() == Qt::Key_Delete || event->key() == Qt::Key_Backspace) {
        onDeleteClicked();
    }
    else if (event->key() == Qt::Key_Enter || event->key() == Qt::Key_Equal) {
        onEqualClicked();
    }
    else if (event->key() == Qt::Key_Asterisk) {
        lastOperation = "x";
        onOperationKey();
    }
    else if (event->key() == Qt::Key_Plus) {
        lastOperation = "+";
        onOperationKey();
    }
    else if (event->key() == Qt::Key_Minus) {
        lastOperation = "-";
        onOperationKey();
    }
    else if (event->key() == Qt::Key_Slash) {
        lastOperation = "/";
        onOperationKey();
    }
    else if (event->key() == Qt::Key_Percent) {
        onPercentClicked();
    }
    else if (event->key() == Qt::Key_Period) {
        dotClicked();
    }
}

void Calculator::connectNumberButtons()
{
    for (int row = 2; row < m_layout->rowCount() - 1; ++row) {
        for (int col = 0; col < m_layout->columnCount() - 1; ++col) {
            QLayoutItem* item = m_layout->itemAtPosition(row, col);
            if (item) {
                QPushButton* button = qobject_cast<QPushButton*>(item->widget());
                if (button) {
                    connect(button, &QPushButton::clicked, this, &Calculator::onNumberClicked);
                }
            }
        }
    }
    QPushButton* button = qobject_cast<QPushButton*>(m_layout->itemAtPosition(5, 0)->widget());
    connect(button, &QPushButton::clicked, this, &Calculator::onNumberClicked);
}

void Calculator::connectOperationButtons()
{
    QPushButton* button = qobject_cast<QPushButton*>(m_layout->itemAtPosition(5, 2)->widget());
    connect(button, &QPushButton::clicked, this, &Calculator::onDeleteClicked);

    button = qobject_cast<QPushButton*>(m_layout->itemAtPosition(1, 0)->widget());
    connect(button, &QPushButton::clicked, this, &Calculator::onACClicked);

    button = qobject_cast<QPushButton*>(m_layout->itemAtPosition(1, 1)->widget());
    connect(button, &QPushButton::clicked, this, &Calculator::onNegPosClicked);

    button = qobject_cast<QPushButton*>(m_layout->itemAtPosition(1, 2)->widget());
    connect(button, &QPushButton::clicked, this, &Calculator::onPercentClicked);

    button = qobject_cast<QPushButton*>(m_layout->itemAtPosition(1, 3)->widget());
    connect(button, &QPushButton::clicked, this, &Calculator::onOperationClicked);

    button = qobject_cast<QPushButton*>(m_layout->itemAtPosition(2, 3)->widget());
    connect(button, &QPushButton::clicked, this, &Calculator::onOperationClicked);

    button = qobject_cast<QPushButton*>(m_layout->itemAtPosition(3, 3)->widget());
    connect(button, &QPushButton::clicked, this, &Calculator::onOperationClicked);

    button = qobject_cast<QPushButton*>(m_layout->itemAtPosition(4, 3)->widget());
    connect(button, &QPushButton::clicked, this, &Calculator::onOperationClicked);

    button = qobject_cast<QPushButton*>(m_layout->itemAtPosition(5, 3)->widget());
    connect(button, &QPushButton::clicked, this, &Calculator::onEqualClicked);

    button = qobject_cast<QPushButton*>(m_layout->itemAtPosition(5, 1)->widget());
    connect(button, &QPushButton::clicked, this, &Calculator::dotClicked);
}

void Calculator::design()
{
    m_layout->setAlignment(Qt::AlignCenter);

    m_layout->addWidget(m_line, 0, 0, 1, 4);
    m_line->setFixedHeight(100);
    m_line->setReadOnly(true);
    m_line->setStyleSheet("background-color: lightGray; color: gray; border-radius: 30px; font-size: 40px;");

    m_layout->addWidget(new QPushButton("AC", centralWidget), 1, 0);
    m_layout->addWidget(new QPushButton("+/-", centralWidget), 1, 1);
    m_layout->addWidget(new QPushButton("%", centralWidget), 1, 2);
    m_layout->addWidget(new QPushButton("/", centralWidget), 1, 3);

    m_layout->addWidget(new QPushButton("7", centralWidget), 2, 0);
    m_layout->addWidget(new QPushButton("8", centralWidget), 2, 1);
    m_layout->addWidget(new QPushButton("9", centralWidget), 2, 2);
    m_layout->addWidget(new QPushButton("x", centralWidget), 2, 3);

    m_layout->addWidget(new QPushButton("4", centralWidget), 3, 0);
    m_layout->addWidget(new QPushButton("5", centralWidget), 3, 1);
    m_layout->addWidget(new QPushButton("6", centralWidget), 3, 2);
    m_layout->addWidget(new QPushButton("-", centralWidget), 3, 3);

    m_layout->addWidget(new QPushButton("1", centralWidget), 4, 0);
    m_layout->addWidget(new QPushButton("2", centralWidget), 4, 1);
    m_layout->addWidget(new QPushButton("3", centralWidget), 4, 2);
    m_layout->addWidget(new QPushButton("+", centralWidget), 4, 3);

    m_layout->addWidget(new QPushButton("0", centralWidget), 5, 0);
    m_layout->addWidget(new QPushButton(".", centralWidget), 5, 1);
    m_layout->addWidget(new QPushButton("Del", centralWidget), 5, 2);
    m_layout->addWidget(new QPushButton("=", centralWidget), 5, 3);

    changeButtonStyle();
}

void Calculator::changeButtonStyle()
{
    for (int row = 1; row < m_layout->rowCount(); ++row) {
        for (int col = 0; col < m_layout->columnCount(); ++col) {
            QLayoutItem* item = m_layout->itemAtPosition(row, col);
            if (item) {
                QPushButton* button = qobject_cast<QPushButton*>(item->widget());
                if (button) {
                    button->setFixedSize(100, 100);
                    if (col != m_layout->columnCount() - 1) {
                        button->setStyleSheet("background-color: drakGray; color: white; border-radius: 50px; font-size: 30px;");
                    }
                    else {
                        button->setStyleSheet("background-color: orange; color: white; border-radius: 50px; font-size: 30px;");
                    }
                }
            }
        }
    }

    for (int col = 0; col < m_layout->columnCount() - 1; ++col) {
        QPushButton* button = qobject_cast<QPushButton*>(m_layout->itemAtPosition(1, col)->widget());
        button->setStyleSheet("background-color: gray; color: white; border-radius: 50px; font-size: 30px;");
    }
}


