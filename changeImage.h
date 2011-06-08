#ifndef CHANGEIMAGE_H
#define CHANGEIMAGE_H

namespace transmationImage
{

        QPixmap changeBrightness(const QPixmap& image, int brightness);
        QPixmap changeContrast(const QPixmap& image, int contrast);
        QPixmap changeGamma(const QPixmap& image, int gamma);
}

#endif // CHANGEIMAGE_H
