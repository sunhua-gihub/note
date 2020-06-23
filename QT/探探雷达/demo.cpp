void ScanTanTan::paintEvent(QPaintEvent *)
{
    int width = this->width();
    int height = this->height();
    int side = qMin(width, height);

    //绘制准备工作,启用反锯齿,平移坐标轴中心,等比例缩放
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    painter.translate(width / 2, height / 2);
    painter.scale(side / 200.0, side / 200.0);

    //绘制扫描线
    drawScan(&painter);
    //绘制扩散圈
    drawRing(&painter);
    //绘制中间图片
    drawImage(&painter);
}

void ScanTanTan::drawScan(QPainter *painter)
{
    painter->save();

    //锥形渐变颜色,通过透明度控制形成扫描效果
    QConicalGradient conicalGradient(0, 0, scanDeg);
    QColor color = scanColor;
    color.setAlpha(50);
    conicalGradient.setColorAt(0, color);
    color.setAlpha(0);
    conicalGradient.setColorAt(1, color);

    //设置画笔画刷
    QPen pen;
    pen.setWidth(scanWidth);
    pen.setBrush(conicalGradient);
    painter->setPen(pen);
    painter->setBrush(conicalGradient);

    //绘制饼圆
    QRect rect(-scanRadius, -scanRadius, scanRadius * 2, scanRadius * 2);
    painter->drawPie(rect, scanDeg * 16, 360 * 16);

    painter->restore();
}

void ScanTanTan::drawRing(QPainter *painter)
{
    painter->save();
    painter->setBrush(Qt::NoBrush);

    //绘制所有扩散圈,扩散圈其实就是个没有背景颜色的圆形
    for (int i = 0; i < rings.count(); i++) {
        RingData ring = rings.at(i);
        int radius = ring.radius;
        float width = ring.width;
        int alpha = 255 - ring.alpha;
        QColor color = ringColor;
        color.setAlpha(alpha);

        QPen pen;
        pen.setWidthF(width);
        pen.setColor(color);
        painter->setPen(pen);
        painter->drawEllipse(-radius, -radius, radius * 2, radius * 2);
    }

    painter->restore();
}

void ScanTanTan::drawImage(QPainter *painter)
{
    painter->save();

    //设置圆形遮罩路径,产生圆形头像效果
    QPainterPath path;
    path.addEllipse(QPoint(0, 0), imageRadius, imageRadius);
    painter->setClipPath(path);

    //绘制图片
    QRect rect(-imageRadius, -imageRadius, imageRadius * 2, imageRadius * 2);
    painter->drawPixmap(rect, image);

    //绘制图片边缘圆形
    QPen pen;
    pen.setWidth(imageBorderWidth);
    pen.setColor(imageBorderColor);
    painter->setPen(pen);
    painter->setBrush(Qt::NoBrush);

    //以下两种方法二选一,其实绘制360度的圆弧=绘制无背景的圆形
    //painter->drawArc(rect, 0, 360 * 16);
    painter->drawEllipse(rect);

    painter->restore();
}

