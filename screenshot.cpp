#include <QtGui>

#include "screenshot.h"
#include "changeImage.h"

//! [0]
Screenshot::Screenshot()
{
        imageLabel = new QLabel;
        imageLabel->setSizePolicy(QSizePolicy::Expanding,
                                   QSizePolicy::Expanding);
        imageLabel->setAlignment(Qt::AlignCenter);

        imageScrollArea = new QScrollArea;
        imageScrollArea->setWidget(imageLabel);
        imageScrollArea->setMinimumSize(480,320);

        createButtonsLayout();

        createGroupBoxesLayout();

        buttonsLayout->addLayout(GroupBoxesLayoutLayout);

        mainLayout = new QHBoxLayout;
        mainLayout->addLayout(buttonsLayout);
        mainLayout->addWidget(imageScrollArea);
        setLayout(mainLayout);

        zoomSpinBox->setValue(100);
        brightnessSpinBox->setValue(100);
        contrastSpinBox->setValue(100);
        gammaSpinBox->setValue(0);

        saveDir = getSaveDir();
        dir = new QDir(saveDir);

        setWindowTitle(tr("Tiny Image Processing Software"));

}
//! [0]

//! [1]
void Screenshot::resizeEvent(QResizeEvent * /* event */)
{
        /*QSize scaledSize = originalPixmap.size();
        scaledSize.scale(imageLabel->size(), Qt::KeepAspectRatio);
        if (!imageLabel->pixmap()
              || scaledSize != imageLabel->pixmap()->size())
        updatePreview();*/
}

//! [1]

//! [2]
void Screenshot::newScreenshot()
{
        hide();
        imageId = 0;
        QTimer::singleShot(500, this, SLOT(dumpScreen()));
}

void Screenshot::captureScreenshot()
{
        hide();
        //dumpScreen();
        QTimer::singleShot(500, this, SLOT(dumpScreen()));
}

//! [2]

//! [3]
void Screenshot::saveScreenshot()
{
    /*
        QString format = SAVE_FILE_FORMAT;
        if (imageId ==  0) {
                if (!dir->mkdir(userName)) {
                        //error tackle
                        QMessageBox::warning(0, "", "Create Dir failed");
                        return;
                }
        }
        QString savePath = dir->filePath(QString("%1/%2.png")
                                        .arg(userName)
                                        .arg(imageId));

                if(snapshot.save(savePath, format.toAscii())) {
                        imageId++;
                } else {
                        QMessageBox *err = new QMessageBox;
                        err->setText("Save image failed");
                        err->show();
                        err->exec();
                }
*/
}
//! [3]

//! [4]
void Screenshot::dumpScreen()
{
        scaleFator = 100;
        if (imageId) {
                dumpPartialScreen();
        } else {
                dumpFullScreen();
                userName = analysisUserName();
                QTimer::singleShot(100, this, SLOT(saveScreenshot()));
                updatePreview();
                show();
        }
}

void Screenshot::dumpPartialScreen()
{
            rgnGrab = new RegionGrabber();
            connect( rgnGrab, SIGNAL( regionGrabbed( const QPixmap & ) ),
                     SLOT( slotRegionGrabbed( const QPixmap & ) ) );
}

void Screenshot::dumpFullScreen()
{
        QDesktopWidget *desktop = QApplication::desktop();
        originalPixmap = QPixmap();
        originalPixmap = QPixmap::grabWindow(desktop->winId(), 0, 0,
                                             desktop->width(), desktop->height());
        snapshot = originalPixmap;
}

//! [8]
void Screenshot::createButtonsLayout()
{
    newScreenshotButton = createButton(tr("New"),
                                        this, SLOT(newScreenshot()));
    saveScreenshotButton = createButton(tr("Save"),
                                        this, SLOT(saveScreenshot()));
    captureScreenButton = createButton(tr("Capture"),
                                        this, SLOT(captureScreenshot()));
    /*zoomInButton = createButton(tr("Zoom In"),
                                        this, SLOT(zoomInImage()));
    zoomOutButton = createButton(tr("Zoom Out"),
                                        this, SLOT(zoomOutImage()));
    increaseBrightnessButton = createButton(tr("Increase Brightness"),
                                        this, SLOT(increaseBrightness()));
    decreaseBrightnessButton = createButton(tr("Decrease Brightness"),
                                        this, SLOT(decreaseBrightness()));
    //quitScreenshotButton = createButton(tr("Quit"), this, SLOT(close()));*/

    buttonsLayout = new QVBoxLayout;
    buttonsLayout->setAlignment(Qt::AlignTop);
    buttonsLayout->addStretch();
    buttonsLayout->addWidget(newScreenshotButton);
    buttonsLayout->addWidget(saveScreenshotButton);
    buttonsLayout->addWidget(captureScreenButton);
    //buttonsLayout->addWidget(zoomInButton);
    //buttonsLayout->addWidget(zoomOutButton);
    //buttonsLayout->addWidget(increaseBrightnessButton);
    //buttonsLayout->addWidget(decreaseBrightnessButton);
    //buttonsLayout->addWidget(quitScreenshotButton);
}
//! [8]

void Screenshot::createGroupBoxesLayout()
{
    zoomGroupBox = new QGroupBox(tr("Zoom"));
    zoomSpinBox = new QSpinBox;
    zoomSpinBox->setSuffix(tr(" %"));
    zoomSpinBox->setMaximum(400);
    connect(zoomSpinBox, SIGNAL(valueChanged(int)), this, SLOT(zoomImage(int)));
    QHBoxLayout *layout = new QHBoxLayout();
    layout->addWidget(zoomSpinBox);
    zoomGroupBox->setLayout(layout);

    brightnessGroupBox = new QGroupBox(tr("Brightness"));
    brightnessSpinBox = new QSpinBox;
    brightnessSpinBox->setMaximum(100);
    connect(brightnessSpinBox, SIGNAL(valueChanged(int)), this, SLOT(changeBrightness(int)));
    layout = new QHBoxLayout();
    layout->addWidget(brightnessSpinBox);
    brightnessGroupBox->setLayout(layout);

    contrastGroupBox = new QGroupBox(tr("contrast"));
    contrastSpinBox = new QSpinBox;
    contrastSpinBox->setMaximum(100);
    connect(contrastSpinBox, SIGNAL(valueChanged(int)), this, SLOT(changeContrast(int)));
    layout = new QHBoxLayout();
    layout->addWidget(contrastSpinBox);
    contrastGroupBox->setLayout(layout);

    gammaGroupBox = new QGroupBox(tr("Gamma"));
    gammaSpinBox = new QSpinBox;
    gammaSpinBox->setMaximum(100);
    connect(gammaSpinBox, SIGNAL(valueChanged(int)), this, SLOT(changeGamma(int)));
    layout = new QHBoxLayout();
    layout->addWidget(gammaSpinBox);
    gammaGroupBox->setLayout(layout);

    GroupBoxesLayoutLayout = new QVBoxLayout;
    GroupBoxesLayoutLayout->addWidget(zoomGroupBox);
    GroupBoxesLayoutLayout->addWidget(brightnessGroupBox);
    GroupBoxesLayoutLayout->addWidget(contrastGroupBox);
    GroupBoxesLayoutLayout->addWidget(gammaGroupBox);
}

QPushButton *Screenshot::createButton(const QString &text, QWidget *receiver,
                                      const char *member)
{
        QPushButton *button = new QPushButton(text);
        button->connect(button, SIGNAL(clicked()), receiver, member);
        return button;
}

void Screenshot::slotRegionGrabbed( const QPixmap &pix )
{
        if (!pix.isNull()) {
                originalPixmap = pix;
                snapshot = pix;
        }
        updatePreview();
        show();
        delete rgnGrab;
}

void Screenshot::updatePreview()
{
        QSize imageSize = originalPixmap.size() * scaleFator / 100;
        imageLabel->resize(imageSize);
        imageLabel->setPixmap(snapshot.scaled(imageSize,
                                                Qt::KeepAspectRatio,
                                                Qt::SmoothTransformation));
}

void Screenshot::zoomImage(int scaleFator)
{
        this->scaleFator = scaleFator;
        updatePreview();
}


void Screenshot::changeBrightness(int brightness)
{
        snapshot = transmationImage::changeBrightness(originalPixmap, brightness);
        updatePreview();
}

void Screenshot::changeContrast(int contrast)
{
        snapshot = transmationImage::changeContrast(originalPixmap, contrast);
        updatePreview();
}

void Screenshot::changeGamma(int gamma)
{
        snapshot = transmationImage::changeGamma(originalPixmap, gamma);
        updatePreview();
}

QString Screenshot::getSaveDir()
{
/*        QFile f("./screenshot.conf");
        if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
                QMessageBox::warning(0, "", "no screenshot.conf");
                return "./";
        QTextStream in(&f);
        if (in.atEnd())
                return "./";

        QString line = in.readLine();
        return line;
*/
return "/";
    }

QString Screenshot::analysisUserName()
{
        return "snapshot";
}
