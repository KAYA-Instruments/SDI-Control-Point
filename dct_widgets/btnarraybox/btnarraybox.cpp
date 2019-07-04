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
/**
 * @file    btnarraybox.cpp
 *
 * @brief   Class implementation of a button array box
 *
 *****************************************************************************/
#include "btnarraybox.h"

#include <QVector>
#include <QToolButton>
#include <QGridLayout>
#include <QSignalMapper>

#include <QDebug>

/******************************************************************************
 * ButtonArrayBox::CustomToolButton
 *
 * A QToolButton with an additional ID field which is used to determine
 * which of the buttons in the button array has been pressed.
 *****************************************************************************/
class ButtonArrayBox::CustomToolButton : public QToolButton
{
public:
    CustomToolButton( QWidget *parent = nullptr ) : QToolButton( parent )
    {
        id = -1;
    }

    void setId( int id )
    {
        this->id = id;
    }

    int getId()
    {
        return id;
    }

private:
    int id;
};

/******************************************************************************
 * ButtonArrayBox::PrivateData
 *****************************************************************************/
class ButtonArrayBox::PrivateData
{
public:
    PrivateData( ButtonArrayBox * owner )
    {
        m_owner     = owner;
        m_layout    = new QGridLayout();
        m_max_width = 0;
        m_max_cols  = 0;
        m_x         = 0;
        m_y         = 0;
    }

    ~PrivateData()
    {
        deleteButtons();

        delete m_layout; 
    }

    void setPixmap( int id, QPixmap px )
    {
        if ( id < m_buttons.count() )
        {
            CustomToolButton * btn = m_buttons[id];
            btn->setIcon( QIcon(px) );
        }
    }

    // delete all buttons
    void deleteButtons()
    {
        // reset coordinates
        m_y = 0; m_x = 0;

        // delete button instances 
        foreach( CustomToolButton * b, m_buttons )
        {
            m_layout->removeWidget( b );
            delete b;
        }    
        
        // clear vector 
        m_buttons.clear();
    }

    // relayout all buttons
    void rearrangeButtons()
    {
        // remove all buttons from layout 
        foreach( CustomToolButton * b, m_buttons )
        {
            m_layout->removeWidget( b );
        }

        m_y = 0;
        m_x = 0;

        // TODO: 
        for ( int i = 0; i<m_buttons.size(); i++ )
        {
            m_layout->addWidget( m_buttons[i], m_y, m_x++ );
            if ( (m_x == m_max_cols) || m_new_row[i] )
            {
                m_x = 0;
                m_y++;
            }
        }
    }

    // add a button
    void addButton( QString label, bool new_row )
    {
        // create new button 
        CustomToolButton * btn = new CustomToolButton( m_owner );

        // compute button width to present label
        QFontMetrics  fm  = btn->fontMetrics();
        int width = fm.horizontalAdvance( label );
        if ( m_max_width < width )
        {
            m_max_width = width;
        
            // resize all buttons to have same width
            foreach( CustomToolButton * b, m_buttons )
            {
                b->setFixedWidth( m_max_width + 20 );
                b->setFixedHeight( m_max_width + 20 );
            }
        }

        // set button label 
        btn->setText( label );
        btn->setId( m_buttons.size() );
        btn->setFixedWidth( m_max_width + 20 );
        btn->setFixedHeight( m_max_width + 20 );

        // Connect button to button clicked event using lambda expression
        connect(btn, &CustomToolButton::clicked, [this, btn]()
        {
            emit m_owner->buttonClicked( btn->getId() );
        });

        // add button to vector
        m_buttons.append( btn );
        m_new_row.append( new_row );

        // add button to layout at position( m_x, m_y )
        m_layout->addWidget( btn, m_y, m_x++ );
        if ( (m_x == m_max_cols) || new_row )
        {
            m_x = 0;
            m_y++;
        }
    }

    // add a button
    void addButton( QString label, QPixmap px, bool new_row )
    {
        // create new button 
        CustomToolButton * btn = new CustomToolButton( m_owner );

        // compute button width to present label
        QFontMetrics  fm  = btn->fontMetrics();
        int width = fm.horizontalAdvance( label );
        if ( m_max_width < width )
        {
            m_max_width = width;
        
            // resize all buttons to have same width
            foreach( CustomToolButton * b, m_buttons )
            {
                b->setFixedWidth( m_max_width + 20 );
                b->setFixedHeight( m_max_width + 20 );
            }     
        }

        // set button label 
        btn->setToolButtonStyle( Qt::ToolButtonTextUnderIcon );
        btn->setText( label );
        btn->setId( m_buttons.size() );
        btn->setFixedWidth( m_max_width + 20 );
        btn->setFixedHeight( m_max_width + 20 );
        btn->setIcon( QIcon(px) );
        btn->setIconSize( QSize(60,60) );

        // Connect button to button clicked event using lambda expression
        connect(btn, &CustomToolButton::clicked, [this, btn]()
        {
            emit m_owner->buttonClicked( btn->getId() );
        });

        // add button to vector
        m_buttons.append( btn );
        m_new_row.append( new_row );

        // add button to layout at position( m_x, m_y )
        m_layout->addWidget( btn, m_y, m_x++ );
        if ( (m_x == m_max_cols) || new_row )
        {
            m_x = 0;
            m_y++;
        }
    }

    ButtonArrayBox *            m_owner;
    QGridLayout *               m_layout;
    QVector<CustomToolButton *> m_buttons;
    QVector<bool>               m_new_row;
    int                         m_max_width;
    int                         m_max_cols;
    int                         m_x;
    int                         m_y;
};

/******************************************************************************
 * ButtonArrayBox::ButtonArrayBox
 *****************************************************************************/
ButtonArrayBox::ButtonArrayBox( QWidget * parent )
    : QGroupBox( parent )
{
    // create private data container
    d_data = new PrivateData( this );

    setTitle( QString() );
    setLayout( d_data->m_layout );
}

/******************************************************************************
 * ButtonArrayBox::~ButtonArrayBox
 *****************************************************************************/
ButtonArrayBox::~ButtonArrayBox()
{
    delete d_data;
}

/******************************************************************************
 * ButtonArrayBox::maxColumns
 *****************************************************************************/
int ButtonArrayBox::maxColumns() const
{
    return ( d_data->m_max_cols );
}

/******************************************************************************
 * ButtonArrayBox::setMaxColumns
 *****************************************************************************/
void ButtonArrayBox::setMaxColumns( int max )
{
    if ( (d_data->m_max_cols != max) && (max > 0) )
    {
        d_data->m_max_cols = max;
        d_data->rearrangeButtons();
    }
}

/******************************************************************************
 * ButtonArrayBox::addButton
 *****************************************************************************/
void ButtonArrayBox::addButton( QString label, bool newRow )
{

    d_data->addButton( label, newRow );
}

/******************************************************************************
 * ButtonArrayBox::addButton
 *****************************************************************************/
void ButtonArrayBox::addButton( QString label, QPixmap px, bool newRow )
{

    d_data->addButton( label, px, newRow );
}

/******************************************************************************
 * ButtonArrayBox::deleteButtons
 *****************************************************************************/
void ButtonArrayBox::deleteButtons()
{
    d_data->deleteButtons();
}

/******************************************************************************
 * ButtonArrayBox::setButtonPixmap
 *****************************************************************************/
void ButtonArrayBox::setButtonPixmap( int id, QPixmap px )
{
    d_data->setPixmap( id, px );
}
