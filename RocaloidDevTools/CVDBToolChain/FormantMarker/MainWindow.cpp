#include "MainWindow.h"
#include <QGridLayout>
#include <QSlider>
#include <math.h>

#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include <QDebug>

#include "DSP.h"
#include "../../../RUtil/RUtil.h"
#include "../../../RUtil/IO/FileUtil.h"
#include "../../../RUtil/Misc/Converter.h"

MainWindow::MainWindow(QWidget*)
{
    QGridLayout* MainLayout = new QGridLayout(this);
    InnerWaveBox = new WaveBox(this);
    F1Slider = new QSlider(Qt::Horizontal, this);
    F2Slider = new QSlider(Qt::Horizontal, this);
    F3Slider = new QSlider(Qt::Horizontal, this);
    S1Slider = new QSlider(Qt::Horizontal, this);
    S2Slider = new QSlider(Qt::Horizontal, this);
    S3Slider = new QSlider(Qt::Horizontal, this);
    SSlider = new QSlider(Qt::Vertical, this);
    StatusLabel = new QLabel(this);

    MainLayout -> addWidget(InnerWaveBox, 0, 0, 5, 20);
    MainLayout -> addWidget(F1Slider, 5, 0, 1, 10);
    MainLayout -> addWidget(F2Slider, 6, 0, 1, 10);
    MainLayout -> addWidget(F3Slider, 7, 0, 1, 10);
    MainLayout -> addWidget(S1Slider, 5, 10, 1, 9);
    MainLayout -> addWidget(S2Slider, 6, 10, 1, 9);
    MainLayout -> addWidget(S3Slider, 7, 10, 1, 9);
    MainLayout -> addWidget(SSlider, 5, 19, 3, 1);
    MainLayout -> addWidget(StatusLabel, 8, 0, 1, 20);

    F1Slider -> setMaximum(5000);
    F2Slider -> setMaximum(5000);
    F3Slider -> setMaximum(5000);
    S1Slider -> setMaximum(200);
    S2Slider -> setMaximum(200);
    S3Slider -> setMaximum(200);
    SSlider -> setMaximum(300);

    F1Slider -> setValue(1000);
    F2Slider -> setValue(1500);
    F3Slider -> setValue(4000);
    S1Slider -> setValue(100);
    S2Slider -> setValue(100);
    S3Slider -> setValue(100);
    SSlider -> setValue(100);

    StatusLabel -> setText(tr("Wave not loaded."));

    connect(F1Slider, SIGNAL(valueChanged(int)), SLOT(UpdateFilter(int)));
    connect(F2Slider, SIGNAL(valueChanged(int)), SLOT(UpdateFilter(int)));
    connect(F3Slider, SIGNAL(valueChanged(int)), SLOT(UpdateFilter(int)));
    connect(S1Slider, SIGNAL(valueChanged(int)), SLOT(UpdateFilter(int)));
    connect(S2Slider, SIGNAL(valueChanged(int)), SLOT(UpdateFilter(int)));
    connect(S3Slider, SIGNAL(valueChanged(int)), SLOT(UpdateFilter(int)));
    connect(SSlider, SIGNAL(valueChanged(int)), SLOT(UpdateFilter(int)));

    setLayout(MainLayout);
    setAcceptDrops(true);
    this -> setMinimumSize(500, 400);

    this -> InnerWaveBox -> SetWaveSize(120);
    this -> InnerWaveBox -> SetWaveNum(2);

    MainWave = (float*)malloc(sizeof(float) * 44100 * 20);

    float TestWave[120];
    Boost_FloatSet(TestWave, 0, 120);
    this -> InnerWaveBox -> LoadWave(TestWave, 0);
    this -> InnerWaveBox -> LoadWave(TestWave, 1);
}

MainWindow::~MainWindow()
{
    free(MainWave);
}

void MainWindow::resizeEvent(QResizeEvent*)
{
    this -> InnerWaveBox -> Redraw();
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent *event)
{
    String Mime, Tmp, Path;
    String_Ctor(& Mime);
    String_Ctor(& Tmp);
    String_Ctor(& Path);
    String_SetChars(& Mime, event -> mimeData() -> text().toStdString().c_str());
    Mime.Data_Index -= 2;
    Left(& Tmp, & Mime, 7);
    if(String_EqualChars(& Tmp, "file://"))
    {
        MidFrom(& Path, & Mime, 7);
        Right(& Tmp, & Path, 4);
        if(String_EqualChars(& Tmp, ".wav") || String_EqualChars(& Tmp, ".wsp"))
        {
            MainWave = (float*)realloc(MainWave, sizeof(float) * GetFileLength(& Path) / 2);
            MainWaveLen = LoadWaveAll(MainWave, & Path);
            qDebug() << "Wave loaded. Length: " << MainWaveLen;
            GenerateSpectrum(this -> InnerWaveBox);
            UpdateFilter(1);
        }
    }
    String_Dtor(& Mime);
    String_Dtor(& Tmp);
    String_Dtor(& Path);
}

void MainWindow::UpdateFilter(int)
{
    float TmpWave[1024];
    float F1 = F1Slider -> value();
    float F2 = F2Slider -> value();
    float F3 = F3Slider -> value();
    float S1 = (float)S1Slider -> value() / 100;
    float S2 = (float)S2Slider -> value() / 100;
    float S3 = (float)S3Slider -> value() / 100;
    float S = (float)SSlider -> value() / 100;
    String StatusStr, TmpStr;
    String_Ctor(& StatusStr);
    String_Ctor(& TmpStr);
    if(MainWaveLen > 0)
    {
        CStrFloatF(& TmpStr, F0, "%.2f");
        String_SetChars(& StatusStr, "F0: ");
        String_Join(& StatusStr, & TmpStr);
        String_JoinChars(& StatusStr, ", F1: ");
        CStrFloatF(& TmpStr, F1, "%.2f");
        String_Join(& StatusStr, & TmpStr);
        String_JoinChars(& StatusStr, ", F2: ");
        CStrFloatF(& TmpStr, F2, "%.2f");
        String_Join(& StatusStr, & TmpStr);
        String_JoinChars(& StatusStr, ", F3: ");
        CStrFloatF(& TmpStr, F3, "%.2f");
        String_Join(& StatusStr, & TmpStr);
        String_JoinChars(& StatusStr, ", S0: ");
        CStrFloatF(& TmpStr, 1, "%.2f");
        String_Join(& StatusStr, & TmpStr);
        String_JoinChars(& StatusStr, ", S1: ");
        CStrFloatF(& TmpStr, S1, "%.2f");
        String_Join(& StatusStr, & TmpStr);
        String_JoinChars(& StatusStr, ", S2: ");
        CStrFloatF(& TmpStr, S2, "%.2f");
        String_Join(& StatusStr, & TmpStr);
        String_JoinChars(& StatusStr, ", S3: ");
        CStrFloatF(& TmpStr, S3, "%.2f");
        String_Join(& StatusStr, & TmpStr);
        StatusLabel -> setText(tr(String_GetChars(& StatusStr)));
        GenerateFilterStrength(TmpWave, F0, F1, F2, F3, S, S1 * S, S2 * S, S3 * S);
        InnerWaveBox -> LoadWave(TmpWave, 1);
        InnerWaveBox -> Redraw();
    }
    String_Dtor(& TmpStr);
    String_Dtor(& StatusStr);
}
