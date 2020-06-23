void WavePlot::paintEvent(QPaintEvent *)
{
    //绘制准备工作,启用反锯齿
    QPainter painter(this);
    painter.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);

    //绘制背景
    drawBg(&painter);

    //绘制波纹
    drawLine(&painter);
}

void WavePlot::drawBg(QPainter *painter)
{
    painter->save();
    painter->setBrush(bgColor);
    painter->drawRect(this->rect());
    painter->restore();
}

void WavePlot::drawLine(QPainter *painter)
{
    int height = this-> height();
    int width = this->width();

    //起始点坐标和结束点坐标
    int startX = 0;
    int startY = 0;
    int endX = width - 0;
    int endY = height - 0;

    //计算当前值所占百分比
    double percent = 0.5;

    //正弦曲线公式 y = A * qSin(ωx + φ) + k

    //w表示周期,可以理解为水波的密度,值越大密度越大(浪越密集 ^_^),取值 密度*M_PI/宽度
    double w = waterDensity * M_PI / endX;

    //A表示振幅,可以理解为水波的高度,值越大高度越高(越浪 ^_^),取值高度的百分比
    double A = endY * waterHeight;

    //k表示y轴偏移,可以理解为进度,取值高度的进度百分比
    double k = endY * percent;

    //第一条波浪路径集合
    QPainterPath waterPath1;
    //第二条波浪路径集合
    QPainterPath waterPath2;

    //移动到左上角起始点
    waterPath1.moveTo(startX, endY);
    waterPath2.moveTo(startX, endY);

    offset += 0.6;
    if (offset > (endX / 2)) {
        offset = 0;
    }

    for(int x = startX; x <= endX; x++) {
        //第一条波浪Y轴
        double waterY1 = (double)(A * qSin(w * x + offset)) + k;
        //第二条波浪Y轴
        double waterY2 = (double)(A * qSin(w * x + offset + (endX / 2 * w))) + k;

        //将两个点连接成线
        waterPath1.lineTo(x, waterY1);
        waterPath2.lineTo(x, waterY2);
    }

    //移动到右下角结束点,整体形成一个闭合路径
    waterPath1.lineTo(endX, endY);
    waterPath2.lineTo(endX, endY);

    QColor waterColor1 = lineColor;
    waterColor1.setAlpha(100);
    QColor waterColor2 = lineColor;
    waterColor2.setAlpha(200);

    painter->save();
    painter->setPen(Qt::NoPen);

    //第一条波浪路径
    painter->setBrush(waterColor1);
    painter->drawPath(waterPath1);

    //第二条波浪路径
    painter->setBrush(waterColor2);
    painter->drawPath(waterPath2);

    painter->restore();
}

