#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QColorDialog>
#include "canvas.h"

class QSlider;
class QSpinBox;
class QComboBox;
class QPushButton;
class QCheckBox;
class QDoubleSpinBox;

class MainWindow : public QMainWindow {
    Q_OBJECT  // 添加这行

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void setBackgroundColor();
    void setBallColor();
    void setBallSize(int size);
    void setBallSpeed(double speed);
    void setMode(int index);
    void setInvisibleDuration(int duration);
    void setVariableSpeed(bool enabled);

private:
    void createControls();
    void setupLayout();

    Canvas *m_canvas;
    Ball *m_ball;

    QWidget *m_centralWidget;
    QWidget *m_controlPanel;
    QPushButton *m_bgColorButton;
    QPushButton *m_ballColorButton;
    QSpinBox *m_ballSizeSpin;
    QDoubleSpinBox *m_ballSpeedSpin;
    QComboBox *m_modeCombo;
    QSpinBox *m_durationSpin;
    QCheckBox *m_variableSpeedCheck;
};

#endif // MAINWINDOW_H