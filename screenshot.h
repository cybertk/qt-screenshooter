#ifndef SCREENSHOT_H
#define SCREENSHOT_H

#include "regionGrabber.h"

#include <QtGui>

#define SAVE_FILE_FORMAT "PNG"

QT_BEGIN_NAMESPACE
class QCheckBox;
class QGridLayout;
class QGroupBox;
class QHBoxLayout;
class QLabel;
class QPushButton;
class QSpinBox;
class QVBoxLayout;
QT_END_NAMESPACE

//! [0]
class Screenshot : public QWidget
{
        Q_OBJECT

public:
        Screenshot();
        void updateImageLabel();

protected:
        void resizeEvent(QResizeEvent *event);

private slots:
        void newScreenshot();
        void saveScreenshot();
        void dumpScreen();
        void captureScreenshot();
        void dumpFullScreen();
        void dumpPartialScreen();
        void zoomImage(int scaleFator);
        void changeBrightness(int brightness);
        void changeContrast(int contrast);
        void changeGamma(int gamma);
        void slotRegionGrabbed(const QPixmap &pix);

private:
        bool partialDump;
        int imageId;
        int scaleFator;
        QDir *dir;
        QString saveDir;
        QString savePath;
        QString userName;



        QString getSaveDir();
        QString analysisUserName();
        void createGroupBoxesLayout();
        void createButtonsLayout();

        QPushButton *createButton(const QString &text, QWidget *receiver,
                              const char *member);
        void updatePreview();

        QPixmap originalPixmap;
        QPixmap snapshot;
        RegionGrabber *rgnGrab;

        QLabel *imageLabel;
        QScrollArea *imageScrollArea;
        QPushButton *newScreenshotButton;
        QPushButton *saveScreenshotButton;
        QPushButton *captureScreenButton;
        QPushButton *zoomInButton;
        QPushButton *zoomOutButton;
        QPushButton *increaseBrightnessButton;
        QPushButton *decreaseBrightnessButton;

        QSpinBox *zoomSpinBox;
        QSpinBox *brightnessSpinBox;
        QSpinBox *contrastSpinBox;
        QSpinBox *gammaSpinBox;

        QGroupBox *zoomGroupBox;
        QGroupBox *brightnessGroupBox;
        QGroupBox *contrastGroupBox;
        QGroupBox *gammaGroupBox;

        QHBoxLayout *mainLayout;
        QVBoxLayout *buttonsLayout;
        QVBoxLayout *GroupBoxesLayoutLayout;
};
//! [0]

#endif
