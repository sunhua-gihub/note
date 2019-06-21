#include "pathsortfilter.h"

PathSortFilter::PathSortFilter()
{

}

PathSortFilter::~PathSortFilter()
{

}

bool PathSortFilter::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    if(!sourceModel())
        return false;

    QModelIndex index = sourceModel()->index(source_row,0,source_parent);
    QDirModel *model = static_cast<QDirModel *>(sourceModel());
    QString str = model->fileName(index);

    if(model->fileInfo(index).isDir())
        return true;

    return false;
}
