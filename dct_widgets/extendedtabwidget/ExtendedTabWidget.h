/******************************************************************************
 * Copyright (C) 2017 Dream Chip Technologies GmbH
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *****************************************************************************/
#pragma once

#include <QWidget>
#include <QIcon>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QButtonGroup>
#include <QPushButton>

class ExtendedTabWidget : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex STORED true NOTIFY currentIndexChanged)
    Q_PROPERTY(QStringList pageTitleList READ pageTitleList WRITE setPageTitleList STORED true)
    Q_PROPERTY(QString pageTitle READ pageTitle WRITE setPageTitle STORED false NOTIFY pageTitleChanged)
    Q_PROPERTY(QStringList pageIconList READ pageIconList WRITE setPageIconList STORED true)
    Q_PROPERTY(QIcon pageIcon READ pageIcon WRITE setPageIcon STORED false NOTIFY pageIconChanged)

public:
    ExtendedTabWidget(QWidget *parent = 0);

    QSize sizeHint() const;

    int count() const;
    int currentIndex() const;
    QWidget *widget(int index);
    int indexOf(QWidget* w);

    QStringList pageTitleList() const;
    QString pageTitle() const;

    QStringList pageIconList() const;
    QIcon pageIcon() const;

    bool setVisible(QWidget* w, bool b);
    bool setEnabled(QWidget* w, bool b);

    // For lazy programmers migrating from QTabWidget to this class
    void addTab( QWidget *page, const QString &title )
    {
        addPage( page, QIcon(), title );
    }

    void addTab( QWidget *page, const QIcon &icon=QIcon(), const QString &title=QString() )
    {
        addPage( page, icon, title );
    }

    void setTabText(int index, const QString &title)
    {
        setPageTitle( index, title );
    }

public slots:   
    void addPage( QWidget *page, const QIcon &icon=QIcon(), const QString &title=QString() );
    void insertPage( int index, QWidget *page, const QIcon &icon=QIcon(), const QString &title=QString() );
    void removePage( int index );
    void setCurrentIndex( int index );

    void setPageTitleList( QStringList const &newTitleList );
    void setPageTitle( QString const &newTitle );
    void setPageTitle( int index, QString const &newTitle );

    void setPageIconList( QStringList const &newIconList );
    void setPageIcon( QIcon const &newIcon );

signals:
    void currentIndexChanged(int index);
    void pageTitleChanged(const QString &title);
    void pageIconChanged(const QIcon &icon);

private:
    QStringList      m_titleList;
    QStringList      m_iconList;

    QStackedWidget * m_stackWidget;
    QButtonGroup *   m_buttonGroup;
    QVBoxLayout *    m_buttonLayout;
    
    QHBoxLayout *    m_layout;
};
