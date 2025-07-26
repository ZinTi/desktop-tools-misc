#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QFrame>
#include <QColorDialog>
#include <QPushButton>      
#include <QSpinBox>         
#include <QDoubleSpinBox>   
#include <QComboBox>        
#include <QCheckBox>        
#include <QGridLayout>      

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    setWindowTitle("Ball Simulation");
    resize(800, 600);
    
    m_ball = new Ball(this);
    m_canvas = new Canvas;
    m_canvas->setBall(m_ball);
    
    createControls();
    setupLayout();
}

MainWindow::~MainWindow() {}

void MainWindow::createControls() {
    m_centralWidget = new QWidget(this);
    m_controlPanel = new QWidget;
    
    // 背景颜色按钮
    m_bgColorButton = new QPushButton("Background Color");
    connect(m_bgColorButton, &QPushButton::clicked, this, &MainWindow::setBackgroundColor);
    
    // 小球颜色按钮
    m_ballColorButton = new QPushButton("Ball Color");
    connect(m_ballColorButton, &QPushButton::clicked, this, &MainWindow::setBallColor);
    
    // 小球大小
    QLabel *sizeLabel = new QLabel("Ball Size:");
    m_ballSizeSpin = new QSpinBox;
    m_ballSizeSpin->setRange(5, 100);
    m_ballSizeSpin->setValue(20);
    connect(m_ballSizeSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &MainWindow::setBallSize);
    
    // 小球速度
    QLabel *speedLabel = new QLabel("Ball Speed:");
    m_ballSpeedSpin = new QDoubleSpinBox;
    m_ballSpeedSpin->setRange(0.1, 10.0);
    m_ballSpeedSpin->setSingleStep(0.1);
    m_ballSpeedSpin->setValue(1.0);
    connect(m_ballSpeedSpin, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::setBallSpeed);
    
    // 模式选择
    QLabel *modeLabel = new QLabel("Mode:");
    m_modeCombo = new QComboBox;
    m_modeCombo->addItem("Floating");
    m_modeCombo->addItem("Random");
    connect(m_modeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::setMode);
    
    // 停留时间
    QLabel *durationLabel = new QLabel("Hide Duration (ms):");
    m_durationSpin = new QSpinBox;
    m_durationSpin->setRange(100, 5000);
    m_durationSpin->setValue(2000);
    m_durationSpin->setSingleStep(100);
    connect(m_durationSpin, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &MainWindow::setInvisibleDuration);
    
    // 变速模式
    m_variableSpeedCheck = new QCheckBox("Variable Speed");
    connect(m_variableSpeedCheck, &QCheckBox::toggled,
            this, &MainWindow::setVariableSpeed);
    
    // 布局控制面板
    QGridLayout *controlLayout = new QGridLayout(m_controlPanel);
    // 修改所有 addWidget() 调用
    controlLayout->addWidget(m_bgColorButton, 0, 0, 1, 2);
    controlLayout->addWidget(m_ballColorButton, 1, 0, 1, 2);
    
    // 添加尺寸标签和小球尺寸控件
    controlLayout->addWidget(sizeLabel, 2, 0);
    controlLayout->addWidget(m_ballSizeSpin, 2, 1);
    
    // 添加速度标签和小球速度控件
    controlLayout->addWidget(speedLabel, 3, 0);
    controlLayout->addWidget(m_ballSpeedSpin, 3, 1);
    
    // 添加模式标签和模式选择
    controlLayout->addWidget(modeLabel, 4, 0);
    controlLayout->addWidget(m_modeCombo, 4, 1);
    
    // 添加持续时间标签和控件
    controlLayout->addWidget(durationLabel, 5, 0);
    controlLayout->addWidget(m_durationSpin, 5, 1);
    
    // 添加变速模式复选框
    controlLayout->addWidget(m_variableSpeedCheck, 6, 0, 1, 2);
    
    controlLayout->setRowStretch(7, 1);
}

void MainWindow::setupLayout() {
    QHBoxLayout *mainLayout = new QHBoxLayout(m_centralWidget);
    mainLayout->addWidget(m_canvas, 1);
    
    QFrame *vline = new QFrame;
    vline->setFrameShape(QFrame::VLine);
    vline->setFrameShadow(QFrame::Sunken);
    
    mainLayout->addWidget(vline);
    mainLayout->addWidget(m_controlPanel);
    
    setCentralWidget(m_centralWidget);
}

void MainWindow::setBackgroundColor() {
    QColor color = QColorDialog::getColor(m_canvas->property("backgroundColor").value<QColor>(), 
                                         this, "Select Background Color");
    if (color.isValid()) {
        m_canvas->setBackgroundColor(color);
    }
}

void MainWindow::setBallColor() {
    QColor color = QColorDialog::getColor(m_ball->color(), this, "Select Ball Color");
    if (color.isValid()) {
        m_canvas->setBallColor(color);
    }
}

void MainWindow::setBallSize(int size) {
    m_canvas->setBallSize(size);
}

void MainWindow::setBallSpeed(double speed) {
    m_canvas->setBallSpeed(static_cast<float>(speed));
}

void MainWindow::setMode(int index) {
    m_canvas->setRandomMode(index == 1);
    // 确保 m_durationSpin 已完全定义
    if (m_durationSpin) {
        m_durationSpin->setEnabled(index == 1);
    }
}

void MainWindow::setInvisibleDuration(int duration) {
    m_canvas->setInvisibleDuration(duration);
}

void MainWindow::setVariableSpeed(bool enabled) {
    m_canvas->setVariableSpeed(enabled);
}

