#ifndef PATHSORTFILTER_H
#define PATHSORTFILTER_H

#include <QSortFilterProxyModel>
#include <QDirModel>

class PathSortFilter : public QSortFilterProxyModel
{
public:
    PathSortFilter();
    ~PathSortFilter();

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const;
};

#endif // PATHSORTFILTER_H
