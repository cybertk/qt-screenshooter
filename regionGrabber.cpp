/*
  Copyright (C) 2009 seraph <cra4fizx@gmail.com>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public
  License as published by the Free Software Foundation; either
  version 2 of the License, or ( at your option ) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Library General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this library; see the file COPYING.  If not, write to
  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
  Boston, MA 02110-1301, USA.
*/

#include "regionGrabber.h"

#include <QtGui>
#include <qapplication.h>
#include <qpainter.h>
#include <qpalette.h>
#include <qstyle.h>
#include <qtimer.h>
#include <qtooltip.h>

SizeTip::SizeTip( QWidget *parent, const char *name )
    : QLabel()
{
  setMargin( 2 );
  setIndent( 0 );
  setFrameStyle( QFrame::Plain | QFrame::Box );

  setPalette( QToolTip::palette() );
}

void SizeTip::setTip( const QRect &rect )
{
  QString tip = QString( "%1x%2" ).arg( rect.width() )
      .arg( rect.height() );

  setText( tip );
  adjustSize();

  positionTip( rect );
}

void SizeTip::positionTip( const QRect &rect )
{
  QRect tipRect = geometry();
  tipRect.moveTopLeft( QPoint( 0, 0 ) );

  /*if ( rect.intersects( tipRect ) )
  {
    QRect deskR = KGlobalSettings::desktopGeometry( QPoint( 0, 0 ) );

    tipRect.moveCenter( QPoint( deskR.width()/2, deskR.height()/2 ) );
    if ( !rect.contains( tipRect, true ) && rect.intersects( tipRect ) )
      tipRect.moveBottomRight( geometry().bottomRight() );
  }*/

  move( tipRect.topLeft() );
}

RegionGrabber::RegionGrabber()
  : mouseDown( false ), sizeTip( 0L )
{
        cursor = QCursor(Qt::CrossCursor);
        setCursor(cursor);
  sizeTip = new SizeTip((QWidget *) 0L);

  tipTimer = new QTimer(this);
  connect(tipTimer, SIGNAL(timeout() ), SLOT( updateSizeTip() ) );

  QTimer::singleShot( 200, this, SLOT( initGrabber() ) );
}

RegionGrabber::~RegionGrabber()
{
  delete sizeTip;
}

void RegionGrabber::initGrabber()
{
        QDesktopWidget *desktop = QApplication::desktop();
        QRect r = desktop->rect();
        pixmap = QPixmap::grabWindow(desktop->winId(), r.x(), r.y(),
                                     r.width(), r.height());

  /*if ( desktopWidget.isVirtualDesktop() )
    r = desktop->geometry();
  else
    r = desktop->screenGeometry();*/

        QPalette palette;
        palette.setBrush(backgroundRole(), QBrush(pixmap));
        setPalette(palette);
        resize(r.size());
        move(0, 0);
        showFullScreen();

        //QApplication::setOverrideCursor( crossCursor );
}

void RegionGrabber::mousePressEvent( QMouseEvent *e )
{
        if (e->button() == Qt::LeftButton ) {
                mouseDown = true;
                grabRect = QRect( e->pos(), e->pos() );
        }
}

void RegionGrabber::mouseMoveEvent( QMouseEvent *e )
{
        if ( mouseDown ) {
                sizeTip->hide();
        tipTimer->start( 250);

  //      drawRubber();
        grabRect.setBottomRight( e->pos() );
  //      drawRubber();
        update();
        }
}

void RegionGrabber::mouseReleaseEvent( QMouseEvent *e )
{
        mouseDown = false;
//        drawRubber();
        sizeTip->hide();

        grabRect.setBottomRight( e->pos() );

        QPixmap region = QPixmap::grabWindow(winId(), grabRect.x(), grabRect.y(),
                                grabRect.width(), grabRect.height());

        //QApplication::restoreOverrideCursor();

        emit regionGrabbed( region );
}

void RegionGrabber::keyPressEvent( QKeyEvent *e )
{
  if ( e->key() == Qt::Key_Escape) {
        QApplication::restoreOverrideCursor();
        emit regionGrabbed( QPixmap() );
  } else
        e->ignore();
}

void RegionGrabber::updateSizeTip()
{
  QRect rect = grabRect;

  sizeTip->setTip( rect );
  sizeTip->show();
}

void RegionGrabber::paintEvent(QPaintEvent *)
{
        QPainter p(this);
        p.drawRect(grabRect);
/*QPainter painter(this);

     QStyleOptionFocusRect option;
     option.initFrom(this);
     option.rect = grabRect;
     //option.backgroundColor = palette().color(QPalette::Background);
     option.backgroundColor = QColor(Qt::red);

     style()->drawPrimitive(QStyle::PE_FrameFocusRect, &option, &painter, this);
*/
}

void RegionGrabber::drawRubber()
{
        QPainter p();
        //p.begin(this);
        //p.setPen(QPen());
        //p.setBrush(Qt::NoBrush);
        //p.drawRect(grabRect);

        /*style().drawPrimitive( QStyle::PE_FocusRect, &p, grabRect, colorGroup(),
      QStyle::Style_Default, QStyleOption( colorGroup().base() ) );*/

        //p.end();
}

//#include "regiongrabber.moc"
