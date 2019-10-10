/*
 * Copyright (C) 2016 ~ 2018 Deepin Technology Co., Ltd.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef DBMANAGER_H
#define DBMANAGER_H

// BookMarkTable
///////////////////////////////////////////////////////
//FilePath           | FileName | PageNumber        | Time  //
//TEXT primari key   | TEXT     | TEXT       | TEXT  //
///////////////////////////////////////////////////////

#include <QSqlDatabase>
#include <QObject>
#include <QDateTime>
#include <QMutex>
#include <QDebug>

class DBManager : public QObject
{
    Q_OBJECT
public:
    static DBManager *instance();

private:
    explicit DBManager(QObject *parent = nullptr);

    const QSqlDatabase getDatabase() const;
    void checkDatabase();

    static DBManager *m_dbManager;
    class DBManagerRelease // 它的唯一工作就是在析构函数中删除实例
    {
    public:
        ~DBManagerRelease()
        {
            if (DBManager::m_dbManager) {
                delete DBManager::m_dbManager;
                DBManager::m_dbManager = nullptr;
            }
        }
        static DBManagerRelease release;
    };

public:
    QString getBookMarks();     //  获取给文件 所有标签的页码
    void insertBookMark(const QString &);
    void updateBookMark(const QString &);
    void deleteBookMark();

    void setStrFilePath(const QString &strFilePath);

private:
    QString m_connectionName = "";
    QString m_strFilePath = "";
    QString m_strFileName = "";
    mutable QMutex m_mutex;
};

#endif // DBMANAGER_H
