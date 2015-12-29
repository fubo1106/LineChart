/********************************************************************************
** Form generated from reading UI file 'form.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FORM_H
#define UI_FORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QDoubleSpinBox *MarkSize;
    QLabel *label;
    QLabel *label_2;
    QLabel *label_3;
    QDoubleSpinBox *Aspect;
    QSpinBox *LineSize;
    QRadioButton *save;
    QLabel *label_x;
    QLabel *label_y;
    QPushButton *btn_read;
    QLabel *label_rangeX;
    QLabel *label_rangeY;
    QLabel *label_4;
    QDoubleSpinBox *zerolinex;
    QDoubleSpinBox *zeroliney;
    QLabel *label_5;
    QLabel *label_6;
    QLabel *label_overlapC;
    QLabel *label_overlapL;
    QLabel *label_overlapR;
    QPushButton *btn_opt_marker;
    QRadioButton *background;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName(QStringLiteral("Form"));
        Form->resize(400, 366);
        MarkSize = new QDoubleSpinBox(Form);
        MarkSize->setObjectName(QStringLiteral("MarkSize"));
        MarkSize->setGeometry(QRect(10, 240, 58, 20));
        MarkSize->setDecimals(1);
        MarkSize->setSingleStep(0.5);
        MarkSize->setValue(7);
        label = new QLabel(Form);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 212, 71, 16));
        label_2 = new QLabel(Form);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(90, 210, 71, 16));
        label_3 = new QLabel(Form);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(160, 210, 81, 16));
        Aspect = new QDoubleSpinBox(Form);
        Aspect->setObjectName(QStringLiteral("Aspect"));
        Aspect->setGeometry(QRect(160, 240, 76, 20));
        Aspect->setDecimals(5);
        Aspect->setSingleStep(0.1);
        Aspect->setValue(1);
        LineSize = new QSpinBox(Form);
        LineSize->setObjectName(QStringLiteral("LineSize"));
        LineSize->setGeometry(QRect(90, 240, 40, 20));
        LineSize->setValue(1);
        save = new QRadioButton(Form);
        save->setObjectName(QStringLiteral("save"));
        save->setEnabled(true);
        save->setGeometry(QRect(20, 270, 89, 16));
        save->setChecked(false);
        label_x = new QLabel(Form);
        label_x->setObjectName(QStringLiteral("label_x"));
        label_x->setGeometry(QRect(150, 280, 54, 12));
        label_y = new QLabel(Form);
        label_y->setObjectName(QStringLiteral("label_y"));
        label_y->setGeometry(QRect(220, 280, 54, 12));
        btn_read = new QPushButton(Form);
        btn_read->setObjectName(QStringLiteral("btn_read"));
        btn_read->setGeometry(QRect(30, 310, 75, 23));
        label_rangeX = new QLabel(Form);
        label_rangeX->setObjectName(QStringLiteral("label_rangeX"));
        label_rangeX->setGeometry(QRect(150, 310, 171, 16));
        label_rangeY = new QLabel(Form);
        label_rangeY->setObjectName(QStringLiteral("label_rangeY"));
        label_rangeY->setGeometry(QRect(150, 330, 171, 16));
        label_4 = new QLabel(Form);
        label_4->setObjectName(QStringLiteral("label_4"));
        label_4->setGeometry(QRect(290, 200, 60, 12));
        zerolinex = new QDoubleSpinBox(Form);
        zerolinex->setObjectName(QStringLiteral("zerolinex"));
        zerolinex->setGeometry(QRect(260, 240, 58, 20));
        zerolinex->setMinimum(-9999.99);
        zerolinex->setMaximum(9999.99);
        zerolinex->setValue(1999);
        zeroliney = new QDoubleSpinBox(Form);
        zeroliney->setObjectName(QStringLiteral("zeroliney"));
        zeroliney->setGeometry(QRect(320, 240, 58, 20));
        zeroliney->setMinimum(-9999.99);
        zeroliney->setMaximum(9999.99);
        zeroliney->setValue(350);
        label_5 = new QLabel(Form);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setGeometry(QRect(280, 220, 6, 12));
        label_6 = new QLabel(Form);
        label_6->setObjectName(QStringLiteral("label_6"));
        label_6->setGeometry(QRect(340, 220, 6, 12));
        label_overlapC = new QLabel(Form);
        label_overlapC->setObjectName(QStringLiteral("label_overlapC"));
        label_overlapC->setGeometry(QRect(10, 10, 54, 12));
        label_overlapL = new QLabel(Form);
        label_overlapL->setObjectName(QStringLiteral("label_overlapL"));
        label_overlapL->setGeometry(QRect(150, 10, 54, 12));
        label_overlapR = new QLabel(Form);
        label_overlapR->setObjectName(QStringLiteral("label_overlapR"));
        label_overlapR->setGeometry(QRect(280, 10, 54, 12));
        btn_opt_marker = new QPushButton(Form);
        btn_opt_marker->setObjectName(QStringLiteral("btn_opt_marker"));
        btn_opt_marker->setGeometry(QRect(30, 340, 75, 23));
        background = new QRadioButton(Form);
        background->setObjectName(QStringLiteral("background"));
        background->setGeometry(QRect(20, 290, 89, 16));
        background->setChecked(true);

        retranslateUi(Form);

        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        Form->setWindowTitle(QApplication::translate("Form", "Form", 0));
        label->setText(QApplication::translate("Form", "Mark Size:", 0));
        label_2->setText(QApplication::translate("Form", "Line Size:", 0));
        label_3->setText(QApplication::translate("Form", "Aspect Radio:", 0));
        save->setText(QApplication::translate("Form", "Not Save", 0));
        label_x->setText(QApplication::translate("Form", "label_x", 0));
        label_y->setText(QApplication::translate("Form", "label_y", 0));
        btn_read->setText(QApplication::translate("Form", "loadData", 0));
        label_rangeX->setText(QApplication::translate("Form", "rangeX", 0));
        label_rangeY->setText(QApplication::translate("Form", "rangeY", 0));
        label_4->setText(QApplication::translate("Form", "Zero Line:", 0));
        label_5->setText(QApplication::translate("Form", "X", 0));
        label_6->setText(QApplication::translate("Form", "Y", 0));
        label_overlapC->setText(QApplication::translate("Form", "OverlapC", 0));
        label_overlapL->setText(QApplication::translate("Form", "OverlapL", 0));
        label_overlapR->setText(QApplication::translate("Form", "OverlapR", 0));
        btn_opt_marker->setText(QApplication::translate("Form", "OptMarker", 0));
        background->setText(QApplication::translate("Form", "Background", 0));
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FORM_H
