void ShadowCalendar::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing);

    int sw = 336;
    int sh = 336;
    qreal scaleX = this->width() * 1.0 / sw;
    qreal scaleY = this->height() * 1.0 / sh;

    painter.scale(scaleX, scaleY);
    painter.setPen(Qt::NoPen);
    painter.fillRect(0, 0, sw, sh, bgColor);

    qreal iw = sw / 7.0;
    qreal ih = sh / 7.0;

    //mask
    QPointF globalpoint = this->mapFromGlobal(QCursor::pos());
    const QPointF &point = QPointF(globalpoint.x() / scaleX, globalpoint.y() / scaleY);

    //绘制光晕背景
    if (this->underMouse()) {
        int effectradius = 58;
        painter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
        QRadialGradient radialGrad(point, effectradius);
        radialGrad.setColorAt(0, QColor(0, 0, 0, 120));
        radialGrad.setColorAt(1, QColor(0, 0, 0, 255));
        painter.setBrush(radialGrad);
        painter.drawEllipse(point, effectradius, effectradius);

        painter.setCompositionMode(QPainter::CompositionMode_DestinationOver);
        painter.setBrush(Qt::NoBrush);

        for (int row = 0; row < 6; row++) {
            for (int column = 0; column < 7; column++) {
                QRectF rect = QRectF(column * iw, (row + 1) * ih, iw, ih).adjusted(3, 3, -3, -3);
                if (rect.contains(point)) {
                    painter.save();
                    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
                    painter.setPen(QPen(QColor(220, 220, 220, 160), 2));
                    painter.drawRoundedRect(rect, 2, 2);
                    painter.restore();
                    continue;
                } else {
                    painter.setPen(QPen(shadowColor, 2));
                }

                painter.drawRoundedRect(rect, 2, 2);
            }
        }

        //绘制圆形的光晕底层背景
        painter.fillRect(0, 0, sw, sh, QColor(200, 200, 200, 50));
    }

    //绘制头部中文数字,先设置图像叠加模式为源在上面
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
    painter.setPen(textColor);
    QStringList listHead;
    listHead << "一" << "二" << "三" << "四" << "五" << "六" << "日";
    for (int i = 0; i < 7; i++) {
        painter.drawText(i * iw, 0, iw, ih, Qt::AlignCenter, listHead.at(i));
    }

    //绘制日期
    for (int row = 0; row < 6; row++) {
        for (int column = 0; column < 7; column++) {
            if (dateItem[row][column].day > 0) {
                QRectF rect = QRectF(column * iw, (row + 1) * ih, iw, ih).adjusted(3, 3, -3, -3);

                //如果是选中的日期则突出绘制背景
                if (QDate::currentDate() == QDate(dateItem[row][column].year, dateItem[row][column].month, dateItem[row][column].day)) {
                    painter.setPen(QPen(selectColor, 2));
                    painter.setBrush(Qt::NoBrush);

                    //如果和光晕效果重叠则边框高亮
                    if (rect.contains(point)) {
                        painter.setPen(QPen(selectColor.lighter(), 2));
                    }

                    //绘制圆角边框
                    painter.drawRoundedRect(rect, 2, 2);

                    //绘制里边背景
                    painter.setPen(Qt::NoPen);
                    painter.setBrush(selectColor);
                    painter.drawRoundedRect(rect.adjusted(4, 4, -4, -4), 2, 2);
                }

                painter.setPen(textColor);
                painter.drawText(rect, Qt::AlignCenter, QString::number(dateItem[row][column].day));
            }
        }
    }
}

