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
#include "entropy.h"
#include "Mark.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT
  
public:
  explicit MainWindow(QWidget *parent = 0);
  Ui::MainWindow *ui;
  Form *ControlW;
public slots:
  void setMarksize(double d);
  void setLinesize(int i);
  void setAspect(double a = 1);
  void setBackground(bool b);
  void Notsave(bool b);
  void setZerolinex(double y);
  void setZeroliney(double x);
  void readDate();
  void loadOneCSVData();
  void loadMultiCSVData();
  void optRatio();
  void optMarker();//for one sequences
  void optMultiMarker();//for multi sequences
  double run(int &goodStep);
private:
  std::vector<std::pair<float,float>> loadData(std::string strFile);
  void clearData();
  void ratioData(const QVector<double>& OX, const QVector<double>& OY, double ratio = 1); //change y value with ratio
  void normalizeData(QVector<QVector2D> &data);
  void dataSelecting(int stride); //select 100 data points
  void dataSelecting(QVector<int>& indexes);
  void saveFigure(double ratio, double markersize, int sampleStep = 1);
  void saveMarks(const string filename, vector<Mark>& marks);

  int marginwidth;
  int plotwidth;
  int plotheight;
  int blank;
  double newwidth;
  double newheight;
  bool save = true;
  QVector<double> X,OX,PX,MX;
  QVector<double> Y,OY,PY,MY;
  circlearea cr;
  linearea lr;
  area m_area;

  QString fileName;
  Banking *m_pBank;
  double aspecRatio;
  std::vector<std::pair<float,float>> m_data;
  QVector<QVector<QVector2D>> m_multiData;
  std::vector<float> m_slopes;

  double init_markSize;
  double init_lineSize;
};

#endif // MAINWINDOW_H
