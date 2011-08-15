/***************************************************************************
 *   Copyright (C) 2011~2011 by CSSlayer, ukyoi                            *
 *   wengxt@gmail.com                                                      *
 *   ukyoi@msn.com                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDebug>
#include <QPainter>
#include <QString>

#include <fcitx/ui.h>
#include <fcitx/fcitx.h>
#include <fcitx-config/fcitx-config.h>

#include "MainWindow.h"

MainWindow::MainWindow()
{
    this->setupUi ( this );

    // QImage inputImage ( QString::fromUtf8 ( "/home/saber/.config/fcitx/skin/plasma/input.png" ) );
    // QImage mainBarImage (QString::fromUTf8 ( "/home/saber/.config/fcitx/skin/plasma/main.png" ) );
    // qDebug() << inputImage.isNull();

    // 下面是测试用的边框、字体颜色
    // QImage inputImage ( QString::fromUtf8 ( "/home/ukyoi/.config/fcitx/skin/dark/input.png" ) );
    // QImage mainBarImage ( QString::fromUtf8 ( "/home/ukyoi/.config/fcitx/skin/dark/bar.png" ) );
    skinPath="/usr/share/fcitx/skin/dark";
    MyLoadConfig skinClass(skinPath);
    qDebug() << skinClass.skin.skinInfo.skinName;
    qDebug() << skinClass.skin.skinInputBar.backImg;
    qDebug() << skinClass.skin.skinInputBar.marginBottom;

    // inputWindowLabel->resize( inputWidth, inputHeight );
    QPixmap inputDestPixmap (0, 0);  // The size of this map should be modified by DrawResizableBackground func.
    // QPixmap mainBarDestPixmap (0, 0); // The size of this map should be modified by DrawMainBar.
    DrawResizableBackground(skinClass.skin, inputDestPixmap, skinPath);
    inputWindowLabel->setPixmap(inputDestPixmap);
    // mainBarLabel->setPixmap(mainBarDestPixmap);
}

QSize MainWindow::GetInputBarDemoStringSize()
{
}

//TODO:
/*
void MainWindow::DrawInputBarDemoString(
    FcitxSkin skin,
    QPixmap &destPixmap
)
{
    QString engDemoString("shu ru shi li");
    QString chnDemoString( QString::fromUtf8("1.第一候选 2.用户自造 3.其他") );

    QPainter painter ( &destPixmap );
    painter.setPen( QColor(engColor.r, engColor.g, engColor.b) );
    painter.drawText(
        marginLeft,
        marginTop,
        engDemoString );
    painter.setPen( QColor(chnColor.r, chnColor.g, chnColor.b) );
    painter.drawText(
        marginLeft,
        // marginTop,
        marginTop + demoStringSize.height() + demoStringSize.height()/2,
        chnDemoString );

    painter.end();
}
*/

void MainWindow::DrawResizableBackground (
    FcitxSkin &skin,
    QPixmap &destPixmap,
    QString skinPath
)
{
    QString backgroundPixmapPath=skinPath + '/' + skin.skinInputBar.backImg;
    qDebug() << backgroundPixmapPath;
    QPixmap backgroundPixmap(backgroundPixmapPath);
    
    int marginLeft=skin.skinInputBar.marginLeft;
    int marginRight=skin.skinInputBar.marginRight;
    int marginTop=skin.skinInputBar.marginTop;
    int marginBottom=skin.skinInputBar.marginBottom;
    int originWidth=backgroundPixmap.width() - marginLeft - marginRight;
    int originHeight=backgroundPixmap.height() - marginTop - marginBottom;
    // int resizeWidth = backgroundPixmap.width () - marginLeft - marginRight;
    // int resizeHeight = backgroundPixmap.height() - marginTop - marginBottom;
    int resizeWidth=120;
    int resizeHeight=40;
    
    if ( resizeHeight <= 0 )
        resizeHeight = 1;
    if ( resizeWidth <= 0 )
        resizeWidth = 1;

    destPixmap=QPixmap(resizeWidth + marginLeft + marginRight, resizeHeight + marginTop + marginBottom);
    destPixmap.fill ( Qt::transparent );
    QPainter painter ( &destPixmap );
    

    /* 画背景 */
    
    /* 九宫格
     * 7 8 9
     * 4 5 6
     * 1 2 3
     */
    /* part 1 */
    painter.drawPixmap(
        QRect(0, marginTop + resizeHeight, marginLeft, marginBottom),
        backgroundPixmap,
        QRect(0, marginTop + originHeight, marginLeft, marginBottom)
    );

    /* part 3 */
    painter.drawPixmap(
        QRect(marginLeft + resizeWidth, marginTop + resizeHeight, marginRight, marginBottom),
        backgroundPixmap,
        QRect(marginLeft + originWidth, marginTop + originHeight, marginRight, marginBottom)
    );

    /* part 7 */
    painter.drawPixmap(
        QRect(0 , 0, marginLeft, marginTop),
        backgroundPixmap,
        QRect(0, 0, marginLeft, marginTop)
    );

    /* part 9 */
    painter.drawPixmap(
        QRect(marginLeft + resizeWidth, 0, marginRight, marginTop),
        backgroundPixmap,
        QRect(marginLeft + originWidth, 0, marginRight, marginTop)
    );

    /* part 8 & 2 */
    painter.drawPixmap(
        QRect(marginLeft, 0, resizeWidth, marginTop),
        backgroundPixmap,
        QRect(marginLeft, 0, originWidth, marginTop)
    );
    painter.drawPixmap(
        QRect(marginLeft, marginTop + resizeHeight, resizeWidth, marginBottom),
        backgroundPixmap,
        QRect(marginLeft, marginTop + originHeight, originWidth, marginBottom)
    );

    /* part 4 & 6 */
    painter.drawPixmap(
        QRect(0, marginTop , marginLeft, resizeHeight),
        backgroundPixmap,
        QRect(0, marginTop , marginLeft, originHeight)
    );

    painter.drawPixmap(
        QRect(marginLeft + resizeWidth, marginTop , marginRight, resizeHeight),
        backgroundPixmap,
        QRect(marginLeft + originWidth, marginTop , marginRight, originHeight)
    );

    /* part 5 */
    painter.drawPixmap(
        QRect(marginLeft, marginTop , resizeWidth, resizeHeight),
        backgroundPixmap,
        QRect(marginLeft, marginTop , originWidth, originHeight)
    );
    
    /* 画箭头 */
    QString backArrowPath=skinPath + '/' + skin.skinInputBar.backArrow;
    QPixmap backArrowPixmap(backArrowPath);
    qDebug() << backArrowPath;
    int iBackArrowX=skin.skinInputBar.iBackArrowX;
    int iBackArrowY=skin.skinInputBar.iBackArrowY;
    painter.drawPixmap(
        QRect(destPixmap.width() - iBackArrowX, iBackArrowY, backArrowPixmap.width(), backArrowPixmap.height()),
        backArrowPixmap,
        QRect(0, 0, backArrowPixmap.width(), backArrowPixmap.height())
    );
    QString forwardArrowPath=skinPath + '/' + skin.skinInputBar.forwardArrow;
    QPixmap forwardArrowPixmap(forwardArrowPath);
    int iForwardArrowX=skin.skinInputBar.iForwardArrowX;
    int iForwardArrowY=skin.skinInputBar.iForwardArrowY;
    painter.drawPixmap(
        QRect(destPixmap.width() - iForwardArrowX, iForwardArrowY, forwardArrowPixmap.width(), forwardArrowPixmap.height()),
        forwardArrowPixmap,
        QRect(0, 0, forwardArrowPixmap.width(), forwardArrowPixmap.height())
    );
    
    painter.end();
}

#include "MainWindow.moc"
