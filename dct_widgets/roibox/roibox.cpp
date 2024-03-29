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
 * @file    roibox.cpp
 *
 * @brief   Implementation of ROI setting box
 *
 *****************************************************************************/
#include <QtDebug>

#include "roibox.h"
#include "ui_roibox.h"

/******************************************************************************
 * namespaces 
 *****************************************************************************/
namespace Ui {
    class UI_ROIBox;
}

/******************************************************************************
 * local definitions
 *****************************************************************************/
#define ROI_WIDTH_END_ID               ( 0 )
#define ROI_HEIGHT_END_ID              ( 1 )
#define ROI_WIDTH_START_ID             ( 2 )
#define ROI_HEIGHT_START_ID            ( 3 )
#define ROI_OFFSET_X_ID                ( 4 )
#define ROI_OFFSET_Y_ID                ( 5 )

/******************************************************************************
 * Settings
 *****************************************************************************/
#define ROI_SETTINGS_SECTION_NAME      ( "ROI" )
#define ROI_SETTINGS_WIDTH             ( "width" )
#define ROI_SETTINGS_HEIGHT            ( "height" )
#define ROI_SETTINGS_OFFSET_X          ( "offset_x" )
#define ROI_SETTINGS_OFFSET_Y          ( "offset_y" )

/******************************************************************************
 * ROIBox::PrivateData
 *****************************************************************************/
class ROIBox::PrivateData
{
public:
    PrivateData( QWidget * parent )
        : m_ui( new Ui::UI_ROIBox )
        , width_step ( 1 )
        , height_step ( 1 )
        , max_width ( 0 )
        , max_height ( 0 )
        , width ( 0 )
        , height ( 0 )
        , width_scaled ( 0 )
        , height_scaled ( 0 )
        , max_offset_x ( 0 )
        , max_offset_y ( 0 )
        , offset_x ( 0 )
        , offset_y ( 0 )
        , numKeyPressEvent(false)
    {
        // initialize UI
        m_ui->setupUi( parent );

        // ROI-plot
        initROIPlot( m_ui->ROIPlot );
        QLinearGradient axisRectGradient;
        axisRectGradient.setStart(0, 0);
        axisRectGradient.setFinalStop(0, 350);
        axisRectGradient.setColorAt(0, QColor(120, 120, 120));
        axisRectGradient.setColorAt(1, QColor(48, 48, 48));
        m_ui->ROIPlot->axisRect()->setBackground(axisRectGradient);
    }

    ~PrivateData()
    {
        delete m_ui;
    }

    void initROIPlot( QCustomPlot * plot )
    {
        // set axises' & grid colors. 'scale' makes the grid thicker
        int scale = 3;
        QColor color(250, 253, 50);

        // ROI width end point
        QPen penROIoffsetX_2( color , scale);
        plot->addGraph();
        plot->graph( ROI_WIDTH_END_ID )->setPen( penROIoffsetX_2 );

        // ROI height end point
        QPen penROIoffsetY_2( color , scale);
        plot->addGraph();
        plot->graph( ROI_HEIGHT_END_ID )->setPen( penROIoffsetY_2 );

        // ROI width start point
        QPen penROIwidth( color , scale);
        plot->addGraph();
        plot->graph( ROI_WIDTH_START_ID )->setPen( penROIwidth );

        // ROI height start point
        QPen penROIheight( color , scale);
        plot->addGraph();
        plot->graph( ROI_HEIGHT_START_ID )->setPen( penROIheight );
 
        // ROI offsetX
        QPen penROIoffsetX_1( Qt::white );
        plot->addGraph();
        plot->graph( ROI_OFFSET_X_ID )->setPen( penROIoffsetX_1 );

        // ROI offsetY
        QPen penROIoffsetY_1( Qt::white );
        plot->addGraph();
        plot->graph( ROI_OFFSET_Y_ID )->setPen( penROIoffsetY_1 );

        plot->setBackground( QBrush(QColor(48,47,47)) );
        //plot->setInteractions( QCP::iRangeDrag | QCP::iRangeZoom); // | QCP::iSelectPlottables
        plot->axisRect()->setupFullAxesBox(true);
        //plot->setNotAntialiasedElement(QCP::AntialiasedElement::aeFills, true);

        //plot->xAxis->setLabel( "" );
        //plot->xAxis2->setLabel( "" );
        //plot->yAxis->setLabel( "" );
        //plot->yAxis2->setLabel( "" );

        plot->xAxis->setLabelColor( QColor(177, 177, 177) );
        plot->xAxis2->setLabelColor( QColor(177, 177, 177) );
        plot->yAxis->setLabelColor( QColor(177, 177, 177) );
        plot->yAxis2->setLabelColor( QColor(177, 177, 177) );

        plot->xAxis->setRange( 0, 400 );
        plot->xAxis2->setRange( 0, 400 );
        plot->yAxis->setRange( 0, 400 );
        plot->yAxis2->setRange( 0, 400 );

        plot->xAxis->setBasePen( QPen(QColor(177, 177, 177), 1) );
        plot->xAxis2->setBasePen( QPen(QColor(177, 177, 177), 1) );
        plot->yAxis->setBasePen( QPen(QColor(177, 177, 177), 1) );
        plot->yAxis2->setBasePen( QPen(QColor(177, 177, 177), 1) );

        plot->xAxis->setTickPen( QPen(QColor(177, 177, 177), 1) );
        plot->xAxis2->setTickPen( QPen(QColor(177, 177, 177), 1) );
        plot->yAxis->setTickPen( QPen(QColor(177, 177, 177), 1) );
        plot->yAxis2->setTickPen( QPen(QColor(177, 177, 177), 1) );

        plot->xAxis->setSubTickPen( QPen( QColor(177, 177, 177), 1) );
        plot->xAxis2->setSubTickPen( QPen( QColor(177, 177, 177), 1) );
        plot->yAxis->setSubTickPen( QPen( QColor(177, 177, 177), 1) );
        plot->yAxis2->setSubTickPen( QPen( QColor(177, 177, 177), 1) );

        plot->xAxis->setTickLabelColor( QColor(177, 177, 177) );
        plot->xAxis2->setTickLabelColor( QColor(177, 177, 177) );
        plot->yAxis->setTickLabelColor( QColor(177, 177, 177) );
        plot->yAxis2->setTickLabelColor( QColor(177, 177, 177) );

        plot->xAxis->setVisible(true);
        plot->xAxis2->setVisible(true);
        plot->yAxis->setVisible(true);
        plot->yAxis2->setVisible(true);

        //plot->xAxis2->setTickLabel("test");
        //plot->xAxis2->orientation(QCP::msTop);
        //AxisType::atTop

        color.setAlpha(30);
        m_ui->ROIPlot->graph(ROI_HEIGHT_END_ID)->setBrush(QBrush(color));

        // make layers to show graph's bprdered rectangle
        m_ui->ROIPlot->graph(ROI_HEIGHT_END_ID)->setChannelFillGraph(m_ui->ROIPlot->graph(ROI_HEIGHT_START_ID));
        m_ui->ROIPlot->addLayer("abovemain", m_ui->ROIPlot->layer("main"), QCustomPlot::limAbove);
        m_ui->ROIPlot->addLayer("belowmain", m_ui->ROIPlot->layer("main"), QCustomPlot::limBelow);
        m_ui->ROIPlot->graph(ROI_HEIGHT_START_ID)->setLayer("belowmain");
        m_ui->ROIPlot->xAxis->grid()->setLayer("belowmain");
        m_ui->ROIPlot->yAxis->grid()->setLayer("belowmain");

        plot->yAxis->setRangeReversed(true);
        plot->yAxis2->setRangeReversed(true);

        m_ui->ROIPlot->replot();
    }

    void setPlotRange()
    {
        m_ui->ROIPlot->xAxis->setRange( 0, max_width );
        m_ui->ROIPlot->xAxis2->setRange( 0, max_width );
        m_ui->ROIPlot->yAxis->setRange( 0, max_height );
        m_ui->ROIPlot->yAxis2->setRange( 0, max_height );
    }

    void setROIConfig()
    {
        if(offset_x > width || offset_y > height)
        {
            //return;
        }

        m_ui->ROIPlot->graph( ROI_WIDTH_START_ID )->data()->clear();
        m_ui->ROIPlot->graph( ROI_HEIGHT_START_ID )->data()->clear();
        m_ui->ROIPlot->graph( ROI_WIDTH_END_ID )->data()->clear();
        m_ui->ROIPlot->graph( ROI_HEIGHT_END_ID )->data()->clear();
        m_ui->ROIPlot->graph( ROI_OFFSET_X_ID )->data()->clear();
        m_ui->ROIPlot->graph( ROI_OFFSET_Y_ID )->data()->clear();

        // height start point
        QVector<double> height_x_start( 2 );
        QVector<double> height_y_start( 2 );
        height_x_start[0] = height_x_start[1] = offset_x;
        height_y_start[0] = height_scaled;
        height_y_start[1] = offset_y;

        // width start point
        QVector<double> width_x_start( 2 );
        QVector<double> width_y_start( 2 );
        width_x_start[0] = offset_x;
        width_x_start[1] = width_scaled;
        width_y_start[0] = width_y_start[1] = offset_y;

        // height end point
        QVector<double> height_x_end( 2 );
        QVector<double> height_y_end( 2 );
        height_x_end[0] = height_x_end[1] = width_scaled;
        height_y_end[0] = height_scaled;
        height_y_end[1] = offset_y;

        // width end point
        QVector<double> width_x_end( 2 );
        QVector<double> width_y_end( 2 );
        width_x_end[0] = offset_x;
        width_x_end[1] = width_scaled;
        width_y_end[0] = width_y_end[1] = height_scaled;

        //offset X
        QVector<double> offset_x_coor_x( 2 );
        QVector<double> offset_x_coor_y( 2 );
        offset_x_coor_x[0] = 0.0;
        offset_x_coor_x[1] = offset_x;
        offset_x_coor_y[0] = offset_x_coor_y[1] = offset_y;

        // offset Y
        QVector<double> offset_y_coor_x( 2 );
        QVector<double> offset_y_coor_y( 2 );
        offset_y_coor_x[0] = offset_y_coor_x[1] = offset_x;
        offset_y_coor_y[0] = 0.0;
        offset_y_coor_y[1] = offset_y;

        m_ui->ROIPlot->graph( ROI_WIDTH_START_ID )->setData( height_x_start, height_y_start );
        m_ui->ROIPlot->graph( ROI_HEIGHT_START_ID )->setData( width_x_start, width_y_start );
        m_ui->ROIPlot->graph( ROI_WIDTH_END_ID )->setData( height_x_end, height_y_end );
        m_ui->ROIPlot->graph( ROI_HEIGHT_END_ID )->setData( width_x_end, width_y_end );
        m_ui->ROIPlot->graph( ROI_OFFSET_X_ID )->setData( offset_x_coor_x, offset_x_coor_y );
        m_ui->ROIPlot->graph( ROI_OFFSET_Y_ID )->setData( offset_y_coor_x, offset_y_coor_y );

        m_ui->ROIPlot->replot();
    }

    Ui::UI_ROIBox *        m_ui;

    int                 width_step;
    int                 height_step;
    int                 max_width;
    int                 max_height;
    int                 width;
    int                 height;
    int                 width_scaled;
    int                 height_scaled;
    int                 max_offset_x;
    int                 max_offset_y;
    int                 offset_x;
    int                 offset_y;
    bool                numKeyPressEvent;
};

bool ROIBox::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        d_data->numKeyPressEvent = true;

        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        bool res = QObject::eventFilter(obj, event);

        if (keyEvent->key() == Qt::Key_Return || keyEvent->key() == Qt::Key_Enter)
        {
            d_data->numKeyPressEvent = false;
            return QObject::eventFilter(obj, event);
            //return true; /* Always accept return */
        }
        else
        {
            event->ignore();
            return res;
        }
    }
    else
    {
        d_data->numKeyPressEvent = false;

        // standard event processing
        return QObject::eventFilter(obj, event);
    }
}

/******************************************************************************
 * ROIBox::ROIBox
 *****************************************************************************/
ROIBox::ROIBox( QWidget * parent ) : DctWidgetBox( parent )
{
    // create private data container
    d_data = new PrivateData( this );

    // initialize UI
    d_data->m_ui->sbxStatROIOffsetX->setRange( 0, 100 );
    d_data->m_ui->sldStatROIOffsetX->setRange( 0, 100 );
    d_data->m_ui->sbxStatROIOffsetY->setRange( 0, 100 );
    d_data->m_ui->sldStatROIOffsetY->setRange( 0, 100 );

    d_data->m_ui->sbxStatROIWidth->setRange( 0, 100 );
    d_data->m_ui->sldStatROIWidth->setRange( 0, 100 );
    d_data->m_ui->sbxStatROIHeight->setRange( 0, 100 );
    d_data->m_ui->sldStatROIHeight->setRange( 0, 100 );

    // signal transition
    connect( d_data->m_ui->sbxStatROIOffsetX, SIGNAL(valueChanged(int)), this, SLOT(onSbxStatROIOffsetXChange(int)) );
    connect( d_data->m_ui->sldStatROIOffsetX, SIGNAL(valueChanged(int)), this, SLOT(onSldStatROIOffsetXChange(int)) );
    connect( d_data->m_ui->sbxStatROIOffsetY, SIGNAL(valueChanged(int)), this, SLOT(onSbxStatROIOffsetYChange(int)) );
    connect( d_data->m_ui->sldStatROIOffsetY, SIGNAL(valueChanged(int)), this, SLOT(onSldStatROIOffsetYChange(int)) );

    connect( d_data->m_ui->sbxStatROIWidth, SIGNAL(valueChanged(int)), this, SLOT(onSbxStatROIWidthChange(int)) );
    connect( d_data->m_ui->sldStatROIWidth, SIGNAL(valueChanged(int)), this, SLOT(onSldStatROIWidthChange(int)) );
    connect( d_data->m_ui->sbxStatROIHeight, SIGNAL(valueChanged(int)), this, SLOT(onSbxStatROIHeightChange(int)) );
    connect( d_data->m_ui->sldStatROIHeight, SIGNAL(valueChanged(int)), this, SLOT(onSldStatROIHeightChange(int)) );

    connect( d_data->m_ui->btnReset, SIGNAL(clicked()), this, SLOT(onBtnResetClicked()) );

    // Install event filter on line edit to catch events
    d_data->m_ui->sbxStatROIOffsetX->installEventFilter(this);
    d_data->m_ui->sbxStatROIOffsetY->installEventFilter(this);
    d_data->m_ui->sbxStatROIWidth->installEventFilter(this);
    d_data->m_ui->sbxStatROIHeight->installEventFilter(this);
}

/******************************************************************************
 * ROIBox::~ROIBox
 *****************************************************************************/
ROIBox::~ROIBox()
{
    delete d_data;
}

/******************************************************************************
 * ROIBox::prepareMode
 *****************************************************************************/
void ROIBox::prepareMode( const Mode )
{
    // @TODO do nothing here
}

/******************************************************************************
 * ROIBox::loadSettings (no looping over channels)
 *****************************************************************************/
void ROIBox::loadSettings( QSettings & s )
{
    s.beginGroup( ROI_SETTINGS_SECTION_NAME );
    //setROIEnable( s.value( ROI_SETTINGS_ENABLE ).toBool() );
    s.endGroup();
}

/******************************************************************************
 * ROIBox::saveSettings (no looping over channels)
 *****************************************************************************/
void ROIBox::saveSettings( QSettings & s )
{
    s.beginGroup( ROI_SETTINGS_SECTION_NAME );
    //s.setValue( ROI_SETTINGS_ENABLE, ROIEnable() );
    s.endGroup();
}

/******************************************************************************
 * ROIBox::applySettings (no looping over channels)
 *****************************************************************************/
void ROIBox::applySettings( void )
{
    //emit ROIConfigChanged( (int)ROIEnable(), ROIPoint(), ROISlope(), WhiteClip() );
}

/******************************************************************************
 * ROIBox::SetROIUIs
 *****************************************************************************/
void ROIBox::SetROIUIs()
{
    d_data->m_ui->sbxStatROIWidth->blockSignals( true );
    d_data->m_ui->sbxStatROIWidth->setValue( d_data->width);
    d_data->m_ui->sbxStatROIWidth->blockSignals( false );

    d_data->m_ui->sldStatROIWidth->blockSignals( true );
    d_data->m_ui->sldStatROIWidth->setValue( d_data->width );
    d_data->m_ui->sldStatROIWidth->blockSignals( false );

    d_data->m_ui->sbxStatROIHeight->blockSignals( true );
    d_data->m_ui->sbxStatROIHeight->setValue( d_data->height);
    d_data->m_ui->sbxStatROIHeight->blockSignals( false );

    d_data->m_ui->sldStatROIHeight->blockSignals( true );
    d_data->m_ui->sldStatROIHeight->setValue( d_data->height );
    d_data->m_ui->sldStatROIHeight->blockSignals( false );

    d_data->m_ui->sbxStatROIOffsetX->blockSignals( true );
    d_data->m_ui->sbxStatROIOffsetX->setValue( d_data->offset_x );
    d_data->m_ui->sbxStatROIOffsetX->blockSignals( false );

    d_data->m_ui->sldStatROIOffsetX->blockSignals( true );
    d_data->m_ui->sldStatROIOffsetX->setValue( d_data->offset_x );
    d_data->m_ui->sldStatROIOffsetX->blockSignals( false );

    d_data->m_ui->sbxStatROIOffsetY->blockSignals( true );
    d_data->m_ui->sbxStatROIOffsetY->setValue( d_data->offset_y );
    d_data->m_ui->sbxStatROIOffsetY->blockSignals( false );

    d_data->m_ui->sldStatROIOffsetY->blockSignals( true );
    d_data->m_ui->sldStatROIOffsetY->setValue( d_data->offset_y );
    d_data->m_ui->sldStatROIOffsetY->blockSignals( false );

    d_data->setROIConfig();
}

/******************************************************************************
 * ROIBox::onStatRoiInfoChange
 *****************************************************************************/
void ROIBox::onStatROIInfoChange( int max_width, int max_height, int width_step, int height_step )
{
    d_data->max_width = max_width;
    d_data->max_height = max_height;
    d_data->width_step = width_step;
    d_data->height_step = height_step;

    d_data->setPlotRange();

    d_data->m_ui->sbxStatROIWidth->blockSignals( true );
    d_data->m_ui->sbxStatROIWidth->setMaximum( d_data->max_width );
    d_data->m_ui->sbxStatROIWidth->setSingleStep( d_data->width_step );
    d_data->m_ui->sbxStatROIWidth->blockSignals( false );

    d_data->m_ui->sldStatROIWidth->blockSignals( true );
    d_data->m_ui->sldStatROIWidth->setMaximum( d_data->max_width );
    d_data->m_ui->sldStatROIWidth->blockSignals( false );

    d_data->m_ui->sbxStatROIHeight->blockSignals( true );
    d_data->m_ui->sbxStatROIHeight->setMaximum( d_data->max_height );
    d_data->m_ui->sbxStatROIHeight->setSingleStep( d_data->height_step );
    d_data->m_ui->sbxStatROIHeight->blockSignals( false );

    d_data->m_ui->sldStatROIHeight->blockSignals( true );
    d_data->m_ui->sldStatROIHeight->setMaximum( d_data->max_height );
    d_data->m_ui->sldStatROIHeight->blockSignals( false );

    d_data->m_ui->sbxStatROIOffsetX->blockSignals( true );
    d_data->m_ui->sbxStatROIOffsetX->setMaximum( d_data->max_width );
    d_data->m_ui->sbxStatROIOffsetX->setSingleStep( d_data->width_step );
    d_data->m_ui->sbxStatROIOffsetX->blockSignals( false );

    d_data->m_ui->sldStatROIOffsetX->blockSignals( true );
    d_data->m_ui->sldStatROIOffsetX->setMaximum( d_data->max_width );
    d_data->m_ui->sldStatROIOffsetX->blockSignals( false );

    d_data->m_ui->sbxStatROIOffsetY->blockSignals( true );
    d_data->m_ui->sbxStatROIOffsetY->setMaximum( d_data->max_height );
    d_data->m_ui->sbxStatROIOffsetY->setSingleStep( d_data->height_step );
    d_data->m_ui->sbxStatROIOffsetY->blockSignals( false );

    d_data->m_ui->sldStatROIOffsetY->blockSignals( true );
    d_data->m_ui->sldStatROIOffsetY->setMaximum( d_data->max_height );
    d_data->m_ui->sldStatROIOffsetY->blockSignals( false );
}

/******************************************************************************
 * ROIBox::onStatRoiChange
 *****************************************************************************/
void ROIBox::onStatROIChange( int width, int height, int offset_x, int offset_y )
{
    d_data->width = width;
    d_data->height = height;
    d_data->offset_x = offset_x;
    d_data->offset_y = offset_y;

    ConfigureMaxOffset();
    ConfigureHeightWidth();
    SetROIUIs();
}

/******************************************************************************
 * ROIBox::onSldRoiXChange
 *****************************************************************************/
void ROIBox::ConfigSldChange(int & data_ref, int index, int step)
{
    int temp_index = index / step * step;

    if(data_ref != temp_index)
    {
        data_ref = temp_index;
        StatROIChange();
    }
}

/******************************************************************************
 * ROIBox::ConfigSbxChange
 *****************************************************************************/
void ROIBox::ConfigSbxChange(int & data_ref, int index, int step)
{
    int temp_index = index / step * step;

    if(data_ref != temp_index || index != temp_index)
    {
        data_ref = temp_index;
        StatROIChange();
    }
}

/******************************************************************************
 * ROIBox::StatROIChange
 *****************************************************************************/
void ROIBox::StatROIChange()
{
    ConfigureMaxOffset();
    ConfigureHeightWidth();

    // Set all UIs
    SetROIUIs();

    // Emit ROI changed event
    //setWaitCursor();
    emit StatROIChanged(d_data->width, d_data->height, d_data->offset_x, d_data->offset_y);
    //setNormalCursor();

    d_data->setROIConfig();

   //emit ROIConfigChanged( (int)ROIEnable(), ROI_POINT_DEFAULT, ROI_SLOPE_DEFAULT, WHITE_CLIP_DEFAULT );
}

/******************************************************************************
 * ROIBox::onSbxStatROIOffsetXChange
 *****************************************************************************/
void ROIBox::onSbxStatROIOffsetXChange( int index )
{
    if(!d_data->numKeyPressEvent)
    {
        ConfigSbxChange( d_data->offset_x, index, d_data->width_step);
    }
}

/******************************************************************************
 * ROIBox::onSldStatROIOffsetXChange
 *****************************************************************************/
void ROIBox::onSldStatROIOffsetXChange( int index )
{
    ConfigSldChange( d_data->offset_x, index, d_data->width_step);
}

/******************************************************************************
 * ROIBox::onSbxStatROIOffsetYChange
 *****************************************************************************/
void ROIBox::onSbxStatROIOffsetYChange( int index )
{
    if(!d_data->numKeyPressEvent)
    {
        ConfigSbxChange( d_data->offset_y, index, d_data->height_step);
    }
}

/******************************************************************************
 * ROIBox::onSldStatROIOffsetYChange
 *****************************************************************************/
void ROIBox::onSldStatROIOffsetYChange( int index )
{
    ConfigSldChange( d_data->offset_y, index, d_data->height_step);
}

/******************************************************************************
 * ROIBox::onSbxStatROIWidthChange
 *****************************************************************************/
void ROIBox::onSbxStatROIWidthChange( int index )
{
    if(!d_data->numKeyPressEvent)
    {
        ConfigSbxChange( d_data->width, index, d_data->width_step);
    }
}

/******************************************************************************
 * ROIBox::onSldStatROIWidthChange
 *****************************************************************************/
void ROIBox::onSldStatROIWidthChange( int index )
{
    ConfigSldChange( d_data->width, index, d_data->width_step);
}

/******************************************************************************
 * ROIBox::onSldStatROIHeightChange
 *****************************************************************************/
void ROIBox::onSbxStatROIHeightChange( int index )
{
    if(!d_data->numKeyPressEvent)
    {
        ConfigSbxChange( d_data->height, index, d_data->height_step);
    }
}

/******************************************************************************
 * ROIBox::onSldStatROIHeightChange
 *****************************************************************************/
void ROIBox::onSldStatROIHeightChange( int index )
{
    ConfigSldChange( d_data->height, index, d_data->height_step);
}

/******************************************************************************
 * ROIBox::onBtnResetClicked
 *****************************************************************************/
void ROIBox::onBtnResetClicked()
{
    d_data->width = d_data->max_width;
    d_data->height = d_data->max_height;
    d_data->offset_x = 0;
    d_data->offset_y = 0;

    StatROIChange();
}


/******************************************************************************
 * ROIBox::ConfigureHeightWidth
 *****************************************************************************/
void ROIBox::ConfigureHeightWidth()
{
    d_data->width_scaled = d_data->width + d_data->offset_x;
    d_data->height_scaled = d_data->height + d_data->offset_y;
}

/******************************************************************************
 * ROIBox::SetMaxOffset
 *****************************************************************************/
void ROIBox::ConfigureMaxOffset()
{
    d_data->max_offset_x = d_data->max_width - d_data->width;
    d_data->max_offset_y = d_data->max_height - d_data->height;

    d_data->m_ui->sbxStatROIOffsetX->setRange( 0, d_data->max_offset_x );
    d_data->m_ui->sldStatROIOffsetX->setRange( 0, d_data->max_offset_x );

    d_data->m_ui->sbxStatROIOffsetY->setRange( 0, d_data->max_offset_y );
    d_data->m_ui->sldStatROIOffsetY->setRange( 0, d_data->max_offset_y );

    if( d_data->offset_x > d_data->max_offset_x )
    {
        d_data->offset_x = d_data->max_offset_x;
    }

    if( d_data->offset_y > d_data->max_offset_y )
    {
        d_data->offset_y = d_data->max_offset_y;
    }
}
