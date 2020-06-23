void WaveWater::paintEvent(QPaintEvent *)
{
    if(image.isNull()) {
        return;
    }

    QPainter painter(this);
    painter.setRenderHints(QPainter::SmoothPixmapTransform);

    //如果停止阶段则绘制原图,否则绘制渲染图
    if (isStoped) {
        painter.drawPixmap(0, 0, image);
    } else {
        painter.drawPixmap(0, 0, pixmap);
    }
}

void WaveWater::drawImage()
{
    if(isStoped) {
        timer->stop();
    } else {
        this->waveRender();
        this->update();
    }
}

void WaveWater::waveSpread()
{
    quint32 counter = 0;
    // 波能渐变
    for(int h = 1; h < imageHeight - 1; h++) {
        for(int w = 1; w < imageWidth - 1; w++) {
            //波能扩散
            waveBuffer2[w][h] = ((waveBuffer1[w - 1][h] + waveBuffer1[w + 1][h] + waveBuffer1[w][h - 1] + waveBuffer1[w][ h + 1]) >> 1) - waveBuffer2[w][h];
            //波能衰减
            waveBuffer2[w][h] -= (waveBuffer2[w][h] >> damping);
            //判断波动释放结束
            if(waveBuffer1[w][h] - waveBuffer2[w][h]) {
                counter++;
            }
        }
    }

    //交换缓冲区
    int **temp = waveBuffer1;
    waveBuffer1 = waveBuffer2;
    waveBuffer2 = temp;

    //设置释放完成
    isStoped = counter ? false : true;
}

void WaveWater::waveRender()
{
    //先改变波纹振幅
    waveSpread();
    if(isStoped) {
        return;
    }

    int offsetX = 0;
    int offsetY = 0;

    //扫描位图
    for(int y = 1; y < imageHeight - 1; y++) {
        for(int x = 1; x < imageWidth - 1; x++) {
            //根据波幅计算位图数据偏移值,渲染点(x,y)对应原始图片(offsetX,offsetY)
            offsetX = x + ((waveBuffer1[x - 1][y] - waveBuffer1[x + 1][y]) >> refractive);
            offsetY = y + ((waveBuffer1[x][y - 1] - waveBuffer1[x][y + 1]) >> refractive);
            //复制象素
            if(0 <= offsetX && offsetX < imageWidth - 1 && 0 <= offsetY && offsetY < imageHeight - 1) {
                QRgb color = imageOrigin.pixel(offsetX, offsetY);
                imageRender.setPixel(x, y, color);
            }
        }
    }

    pixmap.convertFromImage(imageRender);
}

void WaveWater::dropStone(int x, int y, int stoneSize, int stoneWeight)
{
    isStoped = false;
    int posX = 0;
    int posY = 0;

    for(int h = -stoneSize; h < stoneSize; h++) {
        for(int w = -stoneSize; w < stoneSize; w++) {
            posX = x + w;
            posY = y + h;

            //控制范围,不能超出图片
            if(posX < 0 || posX >= imageWidth || posY < 0 || posY >= imageHeight) {
                continue;
            }

            //点(w,h)在一个圆形区域内,初始化波能缓冲区1
            if(h * h + w * w <= stoneSize * stoneSize) {
                waveBuffer1[posX][posY] = -stoneWeight;
            }
        }
    }
}

