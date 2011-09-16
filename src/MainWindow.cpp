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
#include <QFont>

#include <fcitx/ui.h>
#include <fcitx/fcitx.h>
#include <fcitx-config/fcitx-config.h>

#include "MainWindow.h"
#include <qvarlengtharray.h>

#define MAIN_BAR_ICONS_NUMBER 14

MainWindow::MainWindow()
{
    this->setupUi ( this );
    connect(openButton, SIGNAL(clicked()), this, SLOT(openButtonPushed()));
    openButtonPushed();

}

void MainWindow::openButtonPushed()
{
    /* FIXME:
     * Will memory leaks when the dialog opened?
     */
    skinPath=QFileDialog::getOpenFileName(this, tr("Open Config File"), "/usr/share/fcitx/skin", tr("config file (*.conf)"));
    qDebug() << skinPath;
    if (skinPath!="") {
        skinPath.replace("fcitx_skin.conf", "");
        redrawButtonPushed();
    }
}

void MainWindow::redrawButtonPushed()
{
    MyLoadConfig *skinClass=new MyLoadConfig(skinPath);
    DrawAllThings(*skinClass, skinPath);
    delete skinClass;
}

void MainWindow::DrawAllThings(MyLoadConfig skinClass, QString skinPath)
{
    QPixmap inputDestPixmap (0, 0);  // The size of this map should be modified by DrawResizableBackground.
    QPixmap mainBarDestPixmap (0, 0); // The size of this map should be modified by DrawMainBar.
    QPixmap menuDestPixmap (0, 0); // The size of this map should be modified by DrawMenu.
    
    DrawInputBar(inputDestPixmap, skinClass.skin, skinPath);
    DrawMainBar(mainBarDestPixmap, skinClass.skin, skinPath);
    DrawMenu(menuDestPixmap, skinClass.skin, skinPath);
};
    

/* FIXME:
 * What is it?
QSize MainWindow::GetInputBarDemoStringSize()
{
}
*/

QColor MainWindow::GetIntColor(ConfigColor floatColor)
{
    short r=(int)(floatColor.r*256);
    short g=(int)(floatColor.g*256);
    short b=(int)(floatColor.b*256);
    switch (r) {
        case 256 : r=255; break;
    }
    switch (g) {
        case 256 : g=255; break;
    }
    switch (b) {
        case 256 : b=255; break;
    }
    
    QColor converted(r, g, b);
    return converted;
}

void MainWindow::DrawResizableBackground (
    QPixmap &destPixmap,
    QPixmap &backgroundPixmap,
    int marginLeft,
    int marginRight,
    int marginTop,
    int marginBottom,
    int resizeWidth,
    int resizeHeight
)
{
    /**
     * 把一个指定的九宫格图片以resizeWidth和resizeHeight为中央区大小画进destPixmap
     */
    
    int originWidth=backgroundPixmap.width() - marginLeft - marginRight;
    int originHeight=backgroundPixmap.height() - marginTop - marginBottom;
    
    if ( resizeWidth <= 0 )
        resizeWidth = 1;
    if ( resizeHeight <= 0 )
        resizeHeight = 1;

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
    painter.end();
}

void MainWindow::DrawWidget (
    QPixmap &destPixmap, QPixmap &widgetPixmap,
    int x, int y
)
{
    /**
     * 在指定位置(x,y)按照原大小画出一个小部件，
     */
    
    QPainter painter(&destPixmap);
    painter.drawPixmap( x, y, widgetPixmap );
    painter.end();
}

void MainWindow::DrawMainBar(QPixmap &destPixmap, FcitxSkin &skin, QString skinPath)
{
    QPixmap *mainBarIcons=new QPixmap[MAIN_BAR_ICONS_NUMBER];
    QPixmap mainBarPixmap( QString(skinPath + '/' + skin.skinMainBar.backImg) );
    mainBarIcons[0]=( QString(skinPath + '/' + skin.skinMainBar.backImg) );
    mainBarIcons[1]=( QString(skinPath + '/' + skin.skinMainBar.logo) );
    mainBarIcons[2]=( QString(skinPath + '/' + skin.skinMainBar.eng) );
    mainBarIcons[3]=( QString(skinPath + '/' + skin.skinMainBar.active) );
    mainBarIcons[4]=( QString(skinPath + "/chttrans_inactive.png") );
    mainBarIcons[5]=( QString(skinPath + "/chttrans_active.png") );
    mainBarIcons[6]=( QString(skinPath + "/vk_inactive.png") );
    mainBarIcons[7]=( QString(skinPath + "/vk_active.png") );
    mainBarIcons[8]=( QString(skinPath + "/punc_inactive.png") );
    mainBarIcons[9]=( QString(skinPath + "/punc_active.png") );
    mainBarIcons[10]=( QString(skinPath + "/fullwidth_inactive.png") );
    mainBarIcons[11]=( QString(skinPath + "/fullwidth_active.png") );
    mainBarIcons[12]=( QString(skinPath + "/remind_inactive.png") );
    mainBarIcons[13]=( QString(skinPath + "/remind_active.png") );
    
    int marginLeft=skin.skinMainBar.marginLeft;
    int marginRight=skin.skinMainBar.marginRight;
    int marginTop=skin.skinMainBar.marginTop;
    int marginBottom=skin.skinMainBar.marginBottom;
    
    int resizeWidth=0;
    int resizeHeight=0;
    
    for (int i=1; i<MAIN_BAR_ICONS_NUMBER; i++ ) {
        resizeWidth += mainBarIcons[i].width();
    }
    for (int i=1; i<MAIN_BAR_ICONS_NUMBER; i++ ) {
        if (resizeHeight < mainBarIcons[i].height()) {
            resizeHeight = mainBarIcons[i].height();
        }
    }
    
    int totalWidth=resizeWidth+marginTop+marginBottom;
    int totalHeight=resizeHeight+marginLeft+marginRight;
    
    destPixmap=QPixmap(totalWidth, totalHeight);
    
    DrawResizableBackground(destPixmap, mainBarPixmap,
                            marginLeft, marginRight, marginTop, marginBottom,
                            resizeWidth, resizeHeight
    );
    
    int offset=0;
    for (int i=1; i<MAIN_BAR_ICONS_NUMBER; i++) {
        DrawWidget(destPixmap, mainBarIcons[i], marginLeft + offset, marginTop);
        offset += mainBarIcons[i].width();
    }
    
    mainBarLabel->setPixmap(destPixmap);
    delete [] mainBarIcons;
}

/* TODO:
void DrawInputBarDemoString(FcitxSkin &skin, QPixmap &destPixmap, 
*/

void MainWindow::DrawInputBar(QPixmap &destPixmap, FcitxSkin& skin, QString skinPath)
{
    int marginLeft=skin.skinInputBar.marginLeft;
    int marginRight=skin.skinInputBar.marginRight;
    int marginTop=skin.skinInputBar.marginTop;
    int marginBottom=skin.skinInputBar.marginBottom;
    int resizeWidth=180;
    int resizeHeight=40;
    
    QPixmap inputBarPixmap( QString(skinPath + '/' + skin.skinInputBar.backImg) );
    int totalWidth=marginLeft + marginRight + resizeWidth;
    int totalHeight=marginTop + marginBottom + resizeHeight;
    destPixmap=QPixmap(totalWidth, totalHeight);
    destPixmap.fill(Qt::transparent);
    QPainter painter(&inputBarPixmap);
    DrawResizableBackground(destPixmap, inputBarPixmap,
                            marginLeft, marginRight, marginTop, marginBottom,
                            resizeWidth, resizeHeight
    );
    
    QPixmap backArrowPixmap( QString(skinPath + '/' + skin.skinInputBar.backArrow) );
    QPixmap forwardArrowPixmap( QString(skinPath + '/' + skin.skinInputBar.forwardArrow) );
    DrawWidget(destPixmap, backArrowPixmap,
               totalWidth - skin.skinInputBar.iBackArrowX, skin.skinInputBar.iBackArrowY
    );
    DrawWidget(destPixmap, forwardArrowPixmap,
               totalWidth - skin.skinInputBar.iForwardArrowX, skin.skinInputBar.iForwardArrowY
    );
    inputWindowLabel->setPixmap(destPixmap);
}

void MainWindow::DrawMenu(QPixmap &destPixmap, FcitxSkin &skin, QString skinPath)
{
#define SET_OFFSET offset+=(fontSize+3);
    int marginLeft=skin.skinMenu.marginLeft;
    int marginRight=skin.skinMenu.marginRight;
    int marginTop=skin.skinMenu.marginTop;
    int marginBottom=skin.skinMenu.marginBottom;
    int width=100;
    int height=100;
    int offset=marginTop;
    QPixmap backgroundPixmap( QString(skinPath + '/' + skin.skinMenu.backImg) );
    QColor menuFontColor0( GetIntColor(skin.skinFont.menuFontColor[0]) );
    QColor menuFontColor1( GetIntColor(skin.skinFont.menuFontColor[1]) );
    qDebug() << menuFontColor0;
    qDebug() << menuFontColor1;
    QColor activeColor( GetIntColor(skin.skinMenu.activeColor) );
    QColor lineColor( GetIntColor(skin.skinMenu.lineColor) );
    
    // FIXME:
    // Color is double type!
    
    DrawResizableBackground(destPixmap, backgroundPixmap,
                            marginLeft, marginRight, marginTop, marginBottom,
                            width, height
    );
    
    QPainter textPainter(&destPixmap);
    int fontSize=skin.skinFont.menuFontSize;
    QFont menuFont("");
    menuFont.setPixelSize(fontSize);
    
    textPainter.setPen(activeColor);
    textPainter.fillRect(marginLeft, marginTop, width, fontSize, activeColor );
    
    //Draw text.
    textPainter.setFont( menuFont );
    textPainter.setPen(menuFontColor0);
    textPainter.drawText(marginLeft, offset, width, fontSize, Qt::AlignCenter, QString::fromUtf8("选中的行") );
    SET_OFFSET
    
    textPainter.setPen(menuFontColor1);
    textPainter.drawText(marginLeft, offset, width, fontSize, Qt::AlignCenter, QString::fromUtf8("未选中的行") );
    SET_OFFSET
    
    //Draw a line.
    textPainter.setPen(lineColor);
    textPainter.fillRect(marginLeft+3, offset, width-6, 2, lineColor);
    offset+=5;
    
    textPainter.setPen(menuFontColor1);
    textPainter.drawText(marginLeft, offset, width, fontSize, Qt::AlignCenter, QString::fromUtf8("Fcitx皮肤查看器") );
    SET_OFFSET
    
    menuLabel->setPixmap(destPixmap);
    
}


#include "MainWindow.moc"
