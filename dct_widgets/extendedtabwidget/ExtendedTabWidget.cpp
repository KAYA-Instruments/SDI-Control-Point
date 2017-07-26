/******************************************************************************
 *
 * Copyright 2016, Dream Chip Technologies GmbH. All rights reserved.
 * No part of this work may be reproduced, modified, distributed, transmitted,
 * transcribed, or translated into any language or computer format, in any form
 * or by any means without written permission of:
 * Dream Chip Technologies GmbH, Steinriede 10, 30827 Garbsen / Berenbostel,
 * Germany
 *
 *****************************************************************************/
#include <QDebug>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QButtonGroup>
#include <QPushButton>

#include <QApplication>
#include <QStyle>

#include "ExtendedTabWidget.h"

/******************************************************************************
 * ExtendedTabWidget::ExtendedTabWidget
 *****************************************************************************/
ExtendedTabWidget::ExtendedTabWidget( QWidget * parent) : QWidget( parent )
{
    m_buttonGroup = new QButtonGroup;
    
    m_stackWidget = new QStackedWidget;
    m_stackWidget->setFrameShape(QFrame::StyledPanel);

    connect( m_buttonGroup, SIGNAL(buttonClicked(int)), this, SLOT(setCurrentIndex(int)) );

    m_buttonLayout = new QVBoxLayout();
    m_buttonLayout->setSpacing(0);

    QVBoxLayout* buttonStretchLayout = new QVBoxLayout();
    buttonStretchLayout->setSpacing( 0 );
    buttonStretchLayout->addLayout( m_buttonLayout );
    buttonStretchLayout->addStretch();

    m_layout = new QHBoxLayout;
    m_layout->setSpacing( 0 );
    m_layout->setContentsMargins( 0, 0, 0, 0 );
    m_layout->addWidget( m_stackWidget );
    m_layout->addLayout( buttonStretchLayout );
    setLayout( m_layout );
}

/******************************************************************************
 * ExtendedTabWidget::sizeHint
 *****************************************************************************/
QSize ExtendedTabWidget::sizeHint() const
{
    int xMax=0, yMax=0;
    foreach( QAbstractButton * button, m_buttonGroup->buttons() )
    {
        xMax = qMax( xMax, button->sizeHint().width() );
        yMax = qMax( yMax, button->sizeHint().height() );
    }
    return ( QSize( xMax, yMax ) );
}

/******************************************************************************
 * ExtendedTabWidget::addPage
 *****************************************************************************/
void ExtendedTabWidget::addPage( QWidget * page, const QIcon& icon, const QString& title )
{
    insertPage( count(), page, icon, title );
}

/******************************************************************************
 * ExtendedTabWidget::removePage
 *****************************************************************************/
void ExtendedTabWidget::removePage( int index )
{
    QWidget * widget = m_stackWidget->widget( index );

    m_stackWidget->removeWidget( widget );
   
    QPushButton * button = (QPushButton *)m_buttonGroup->button( index );
    m_buttonLayout->removeWidget( button );
    m_buttonGroup->removeButton( button );
    delete ( button );
    
    setCurrentIndex( 0 );
}

/******************************************************************************
 * ExtendedTabWidget::count
 *****************************************************************************/
int ExtendedTabWidget::count() const
{
    return ( m_stackWidget->count() );
}

/******************************************************************************
 * ExtendedTabWidget::currentIndex
 *****************************************************************************/
int ExtendedTabWidget::currentIndex() const
{
    return ( m_stackWidget->currentIndex() );
}

/******************************************************************************
 * ExtendedTabWidget::insertPage
 *****************************************************************************/
void ExtendedTabWidget::insertPage( int index, QWidget *page, const QIcon &icon, const QString &title )
{
    page->setParent( m_stackWidget );

    m_stackWidget->insertWidget( index, page );

    // set label
    QString label = title;
    if ( label.isEmpty() )
    {
        if ( m_titleList.size() > index )
        {
            label = QApplication::translate(
                        ((QObject*)parent())->objectName().toLatin1().constData(),
                        m_titleList.value(index).toLatin1().constData() );
        }

        if ( label.isEmpty() )
        {
            label = tr("Page %1").arg(index);
        }
    }
    
    page->setWindowTitle( label );

    // set icon
    QIcon pix = icon;
    if ( pix.isNull() )
    {
        pix = QIcon( m_iconList.value(index) );
        if( pix.isNull() )
        {
            pix = QApplication::style()->standardIcon( QStyle::SP_ArrowUp );
            page->setWindowIcon( pix );
        }
    }
    else
    {
        page->setWindowIcon( pix );
    }

    // add QPushButton
    QPushButton* button = new QPushButton(pix, label);
    button->setObjectName( "__qt__passive_pushButton" );    // required for interaction within Designer
    button->setCheckable( true );
    if( count()==1 )
    {
        button->setChecked( true );
    }
    m_buttonGroup->addButton( button, index );
    m_buttonLayout->addWidget( button );
}

/******************************************************************************
 * ExtendedTabWidget::setCurrentIndex
 *****************************************************************************/
void ExtendedTabWidget::setCurrentIndex( int index )
{
    if ( index<0 || index>=count() )
    {
        index = 0;
    }

    if ( index != currentIndex() )
    {
        m_stackWidget->setCurrentIndex(index);
        m_buttonGroup->button(index)->setChecked( true );
        emit currentIndexChanged( index );
    }
}

QWidget * ExtendedTabWidget::widget( int index )
{
    return ( m_stackWidget->widget(index) );
}

int ExtendedTabWidget::indexOf( QWidget * widget )
{
    for( int i=0; i<m_stackWidget->count(); i++ )
    {
        if ( m_stackWidget->widget(i) == widget )
        {
            return ( i );
        }
    }

    return ( -1 );
}

bool ExtendedTabWidget::setVisible( QWidget * w, bool b )
{
    int index = indexOf( w );
    
    if ( index == -1 )
    {
        return false;
    }

    if( currentIndex() == index )
    {
        setCurrentIndex(0);
    }

    m_buttonGroup->button(index)->setVisible(b);
    
    return ( true );
}

bool ExtendedTabWidget::setEnabled( QWidget* w, bool b )
{
    int index = indexOf( w );

    if( index == -1 )
    {
        return false;
    }

    if( currentIndex() == index )
    {
        setCurrentIndex(0);
    }

    m_buttonGroup->button(index)->setEnabled(b);
    
    return ( true );
}

QStringList ExtendedTabWidget::pageTitleList() const
{
    QStringList m_titleList;

    for( int i=0; i<m_stackWidget->count(); i++ )
    {
        m_titleList << m_stackWidget->widget(i)->windowTitle();
    }

    return ( m_titleList );
}

QString ExtendedTabWidget::pageTitle() const
{
    if ( const QWidget *currentWidget = m_stackWidget->currentWidget() )
    {
        return ( currentWidget->windowTitle() );
    }
    return ( QString() );
}

QStringList ExtendedTabWidget::pageIconList() const
{
    QStringList iconList;

    for( int i=0; i<m_stackWidget->count(); i++ )
    {
        iconList << m_stackWidget->widget(i)->windowIcon().name();
    }

    return ( iconList );
}

QIcon ExtendedTabWidget::pageIcon() const
{
    if (const QWidget *currentWidget = m_stackWidget->currentWidget())
        return currentWidget->windowIcon();
    return QIcon();
}

void ExtendedTabWidget::setPageTitleList(QStringList const &newTitleList)
{
    m_titleList = newTitleList;

    //we have to force translation here
    for ( int i=0; i<m_titleList.count(); ++i )
    {
        m_titleList[i] = tr( m_titleList[i].toLatin1() );
    }

    if ( !count() )
    {
        return;
    }

    for( int i=0; i<m_stackWidget->count() && i<m_titleList.count(); i++ )
    {
        m_buttonGroup->button(i)->setText( m_titleList.at(i) );
        m_stackWidget->widget(i)->setWindowTitle( m_titleList.at(i) );
    }
}

void ExtendedTabWidget::setPageTitle(QString const &newTitle)
{
    if( !count() ) return;
    m_buttonGroup->button(currentIndex())->setText(newTitle);
    if (QWidget *currentWidget = m_stackWidget->currentWidget())
        currentWidget->setWindowTitle(newTitle);

    emit pageTitleChanged(newTitle);
}

void ExtendedTabWidget::setPageTitle(int index, QString const &newTitle)
{
    if( index<0 || index>=count() ) return;
    m_buttonGroup->button(index)->setText(newTitle);
    if (QWidget *currentWidget = m_stackWidget->widget(index))
        currentWidget->setWindowTitle(newTitle);

    emit pageTitleChanged(newTitle);
}

void ExtendedTabWidget::setPageIconList(QStringList const &newIconList)
{
    m_iconList = newIconList;

    if ( !count() )
    {
        return;
    }

    for( int i=0; i<m_stackWidget->count() && i<newIconList.count(); i++ )
    {
        m_buttonGroup->button(i)->setIcon( QIcon(newIconList.at(i)) );
        m_stackWidget->widget(i)->setWindowIcon( QIcon(newIconList.at(i)) );
    }
}

void ExtendedTabWidget::setPageIcon(QIcon const &newIcon)
{
    m_buttonGroup->button(currentIndex())->setIcon(newIcon);
    if (QWidget *currentWidget = m_stackWidget->currentWidget())
        currentWidget->setWindowIcon(newIcon);
    emit pageIconChanged(newIcon);
}
