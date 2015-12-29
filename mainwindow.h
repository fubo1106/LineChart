/***************************************************************************
**                                                                        **
**  QCustomPlot, an easy to use, modern plotting widget for Qt            **
**  Copyright (C) 2011-2015 Emanuel Eichhammer                            **
**                                                                        **
**  This program is free software: you can redistribute it and/or modify  **
**  it under the terms of the GNU General Public License as published by  **
**  the Free Software Foundation, either version 3 of the License, or     **
**  (at your option) any later version.                                   **
**                                                                        **
**  This program is distributed in the hope that it will be useful,       **
**  but WITHOUT ANY WARRANTY; without even the implied warranty of        **
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         **
**  GNU General Public License for more details.                          **
**                                                                        **
**  You should have received a copy of the GNU General Public License     **
**  along with this program.  If not, see http://www.gnu.org/licenses/.   **
**                                                                        **
****************************************************************************
**           Author: Emanuel Eichhammer                                   **
**  Website/Contact: http://www.qcustomplot.com/                          **
**             Date: 25.04.15                                             **
**          Version: 1.3.1                                                **
****************************************************************************/

/************************************************************************************************************
**                                                                                                         **
**  This is the example code for QCustomPlot.                                                              **
**                                                                                                         **
**  It demonstrates basic and some advanced capabilities of the widget. The interesting code is inside     **
**  the "setup(...)Demo" functions of MainWindow.                                                          **
**                                                                                                         **
**  In order to see a demo in action, call the respective "setup(...)Demo" function inside the             **
**  MainWindow constructor. Alternatively you may call setupDemo(i) where i is the index of the demo       **
**  you want (for those, see MainWindow constructor comments). All other functions here are merely a       **
**  way to easily create screenshots of all demos for the website. I.e. a timer is set to successively     **
**  setup all the demos and make a screenshot of the window area and save it in the ./screenshots          **
**  directory.                                                                                             **
**                                                                                                         **
*************************************************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "qcustomplot.h" // the header file of QCustomPlot. Don't forget to add it to your project, if you use an IDE, so it gets compiled.
#include <QVector>
#include <QWidget>
#include <form.h>
#include <vector>
#include "circlearea.h"
#include "linearea.h"
#include <Banking.h>
#include "area.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT
  
public:
  explicit MainWindow(QWidget *parent = 0);
  void dataProcessing(const QVector<double>& OX, const QVector<double>& OY);
  Ui::MainWindow *ui;
  Form *ControlW;
public slots:
  void setMarksize(double d);
  void setLinesize(int i);
  void setAspect(double a);
  void setBackground(bool b);
  void savePlot(bool b);
  void setZerolinex(double y);
  void setZeroliney(double x);
  void readDate();
  void loadCSVData();
  void optMarker();
  double run();
private:
  std::vector<std::pair<float,float>> loadData(std::string strFile);
  void clearData();

  int marginwidth;
  int plotwidth;
  int plotheight;
  int blank;
  double newwidth;
  double newheight;
  bool save = false;
  QVector<double> X,OX,PX,MX;
  QVector<double> Y,OY,PY,MY;
  circlearea cr;
  linearea lr;
  area m_area;

  QString demoName;
  Banking *m_pBank;
  std::vector<std::pair<float,float>> m_data;
  std::vector<float> m_slopes;

};

#endif // MAINWINDOW_H
