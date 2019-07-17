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
 * @file    LensDriverBox.cpp
 *
 * @brief   Implementation of lens driver box
 *
 *****************************************************************************/
#include <QtDebug>
#include <QProxyStyle>

#include <QModelIndex>
#include <QStandardItemModel>
#include <QLineEdit>
#include <QItemDelegate>
#include <QStyledItemDelegate>
#include <QTextEdit>

#include "lensdriverbox.h"
#include "ui_lensdriverbox.h"
#include "defines.h"

/******************************************************************************
 * namespaces 
 *****************************************************************************/
namespace Ui {
    class UI_LensDriverBox;
}

#define AUTO_REPEAT_THRESHOLD    ( 5000 )
#define IRIS_APT_TABLE_MAX_NO_ROWS              ( 2 )
#define IRIS_APT_TABLE_MAX_NO_COLUMNS           ( 9 )

/******************************************************************************
 * Overrule unconfigureable timer implementation in qt, there's  no property
 * in spinbox class to do this => use proxy-style (Arrggghhh!!!)
 *
 * Problem: Due to a long processing time of some provideo commands the 
 *          internal repeat-timer of spin-box gets expired. This leads to an 
 *          repetation of setValue event in the spin-box instance by button
 *          releases and finally the spinbox-value is decreased or increased 
 *          twice by clicking up- or down-button once.
 *
 * Hope my qt-patch will be accepted to make it configurable. 
 *****************************************************************************/
class SpinBoxStyle : public QProxyStyle
{
public:
    SpinBoxStyle( QStyle *style = nullptr ) : QProxyStyle(style) { }

    int styleHint
    (
        StyleHint hint,
        const QStyleOption * option = nullptr,
        const QWidget * widget = nullptr,
        QStyleHintReturn * returnData = nullptr
    ) const Q_DECL_OVERRIDE
    {
        if ( hint == QStyle::SH_SpinBox_ClickAutoRepeatThreshold )
        {
            return ( AUTO_REPEAT_THRESHOLD );
        }

        return ( QProxyStyle::styleHint(hint, option, widget, returnData) );
    }
};

/******************************************************************************
 * local definitions
 *****************************************************************************/
#define LENSDRIVER_SETTINGS_SECTION_NAME            ( "LENSDRIVER" )

#define LENSDRIVER_SETTINGS_SETTINGS                ( "settings" )

#define LENSDRIVER_SETTINGS_FOCUS_POSITION          ( "focus_position" )
#define LENSDRIVER_SETTINGS_ZOOM_POSITION           ( "zoom_position" )
#define LENSDRIVER_SETTINGS_IRIS_POSITION           ( "iris_position" )
#define LENSDRIVER_SETTINGS_FILTER_POSITION         ( "filter_position" )

#define LENSDRIVER_SETTINGS_FOCUS_SETTINGS          ( "focus_settings" )
#define LENSDRIVER_SETTINGS_ZOOM_SETTINGS           ( "zoom_settings" )
#define LENSDRIVER_SETTINGS_IRIS_SETTINGS           ( "iris_settings" )
#define LENSDRIVER_SETTINGS_FILTER_SETTINGS         ( "filter_settings" )

const lens_settings_t settingsUnkown {
    /* .address =             */ 0,
    /* .chipID =              */ 0,
    /* .controllerFeatures =  */ 0,
    /* .focusMotorNr =        */ 0,
    /* .zoomMotorNr =         */ 0,
    /* .irisMotorNr =         */ 0,
    /* .filterMotorNr =       */ 0,
    /* .focusMotorFeatures =  */ 0,
    /* .zoomMotorFeatures =   */ 0,
    /* .irisMotorFeatures =   */ 0,
    /* .filterMotorFeatures = */ 0
};

const lens_settings_t settingsICS {
    /* .address =             */ 32,
    /* .chipID =              */ 13,
    /* .controllerFeatures =  */ 7,
    /* .focusMotorNr =        */ 0,
    /* .zoomMotorNr =         */ 1,
    /* .irisMotorNr =         */ 2,
    /* .filterMotorNr =       */ 3,
    /* .focusMotorFeatures =  */ 3,
    /* .zoomMotorFeatures =   */ 3,
    /* .irisMotorFeatures =   */ 3,
    /* .filterMotorFeatures = */ 0
};

const lens_settings_t settingsDctKit_iris_1 {
    /* .address =             */ 34,
    /* .chipID =              */ 102,
    /* .controllerFeatures =  */ 1,
    /* .focusMotorNr =        */ 1,
    /* .zoomMotorNr =         */ 2,
    /* .irisMotorNr =         */ 0,
    /* .filterMotorNr =       */ 3,
    /* .focusMotorFeatures =  */ 11,
    /* .zoomMotorFeatures =   */ 0,
    /* .irisMotorFeatures =   */ 11,
    /* .filterMotorFeatures = */ 0
};

const lens_settings_t settingsDctKit_iris_2 {
    /* .address =             */ 34,
    /* .chipID =              */ 102,
    /* .controllerFeatures =  */ 2,
    /* .focusMotorNr =        */ 0,
    /* .zoomMotorNr =         */ 2,
    /* .irisMotorNr =         */ 1,
    /* .filterMotorNr =       */ 3,
    /* .focusMotorFeatures =  */ 11,
    /* .zoomMotorFeatures =   */ 0,
    /* .irisMotorFeatures =   */ 11,
    /* .filterMotorFeatures = */ 0
};

const lens_settings_t settingsDctKit_focus_1 {
    /* .address =             */ 34,
    /* .chipID =              */ 102,
    /* .controllerFeatures =  */ 1,
    /* .focusMotorNr =        */ 0,
    /* .zoomMotorNr =         */ 2,
    /* .irisMotorNr =         */ 1,
    /* .filterMotorNr =       */ 3,
    /* .focusMotorFeatures =  */ 11,
    /* .zoomMotorFeatures =   */ 0,
    /* .irisMotorFeatures =   */ 11,
    /* .filterMotorFeatures = */ 0
};

const lens_settings_t settingsDctKit_focus_2 {
    /* .address =             */ 34,
    /* .chipID =              */ 102,
    /* .controllerFeatures =  */ 2,
    /* .focusMotorNr =        */ 1,
    /* .zoomMotorNr =         */ 2,
    /* .irisMotorNr =         */ 0,
    /* .filterMotorNr =       */ 3,
    /* .focusMotorFeatures =  */ 11,
    /* .zoomMotorFeatures =   */ 0,
    /* .irisMotorFeatures =   */ 11,
    /* .filterMotorFeatures = */ 0
};

const lens_settings_t settingsDctKit_focus_iris {
    /* .address =             */ 34,
    /* .chipID =              */ 102,
    /* .controllerFeatures =  */ 3,
    /* .focusMotorNr =        */ 0,
    /* .zoomMotorNr =         */ 2,
    /* .irisMotorNr =         */ 1,
    /* .filterMotorNr =       */ 3,
    /* .focusMotorFeatures =  */ 11,
    /* .zoomMotorFeatures =   */ 0,
    /* .irisMotorFeatures =   */ 11,
    /* .filterMotorFeatures = */ 0
};

const lens_settings_t settingsDctKit_iris_focus {
    /* .address =             */ 34,
    /* .chipID =              */ 102,
    /* .controllerFeatures =  */ 3,
    /* .focusMotorNr =        */ 1,
    /* .zoomMotorNr =         */ 2,
    /* .irisMotorNr =         */ 0,
    /* .filterMotorNr =       */ 3,
    /* .focusMotorFeatures =  */ 11,
    /* .zoomMotorFeatures =   */ 0,
    /* .irisMotorFeatures =   */ 11,
    /* .filterMotorFeatures = */ 0
};




const lens_settings_t lensSettingProfiles[LensProfileMax] =
{
    settingsUnkown,
    settingsDctKit_iris_1,
    settingsDctKit_iris_2,
    settingsDctKit_focus_1,
    settingsDctKit_focus_2,
    settingsDctKit_focus_iris,
    settingsDctKit_iris_focus,
    settingsICS
};


const lens_iris_position_template_t computarM0824MPW2 {
    /* .fstops =            */ {160, 80, 40, 24, 0, 0, 0, 0, 0},
    /* .fstopPos =          */ {94, 60, 31, 0, 0, 0, 0, 0, 0},
    /* .lensName =          */ "Computar M0824-MPW2",
    /* .compatibleID =      */ {102}
};

const lens_iris_position_template_t computarV0828MPY {
    /* .fstops =            */ {160, 110, 80, 56, 40, 28, 0, 0, 0},
    /* .fstopPos =          */ {94, 78, 63, 45, 28, 0, 0, 0, 0},
    /* .lensName =          */ "Computar V0828-MPY",
    /* .compatibleID =      */ {102}
};

const lens_iris_position_template_t computarEG6Z0915TCSMPWIR {
    /* .fstops =            */ {160, 110, 80, 56, 40, 28, 20, 14, 0},
    /* .fstopPos =          */ {90, 84, 79, 72, 62, 49, 29, 0, 0},
    /* .lensName =          */ "Computar i-CS Lens EG6Z0915TCS-MPWIR",
    /* .compatibleID =      */ {13}
};



/******************************************************************************
 * pairSort
 * @brief Sorts a List of pairs given by a and b components first by a and then
 *        by b value. Duplicates are removed and the sorted / cleaned vectors
 *        are returned. Both vectors must have the same lenght!
 * @param a Vector of the first component of the pairs
 * @param b Vector of the second component of the paris
 * @returns False if an error occured (e.g. length missmatch), true otherwise
 *****************************************************************************/
static bool pairSort( QVector<int> &a, QVector<int> &b)
{
    if ( a.length() != b.length())
    {
        return false;
    }

    int size  = a.length();

    QList<QPair<int,int>> pointList;
    for ( int  i= 0; i < a.length(); i++ )
    {
        // Do not add duplicates to the List
        QPair<int,int> newPair(a.at(i), b.at(i));
        if (!pointList.contains(newPair))
        {
            pointList.append(newPair);
        }
    }



    // Sort it (will sort first for x, than for y, see QPair
    std::sort(pointList.begin(), pointList.end());

    // Write the results back into the table
    a.clear();
    b.clear();
    for ( int i = ( pointList.length() -1 ); i >= 0; i--)
    {
        a.append(pointList.at(i).first);
        b.append(pointList.at(i).second);
    }

    if(pointList.length() < size )
    {
        for(int i = (size - pointList.length() ); i > 0; i--)
        {
            a.append(0);
            b.append(0);
        }
    }



    return true;
}

/******************************************************************************
 * Delegate
 *****************************************************************************/
class LensDriverIrisTabStyledDelegat : public  QStyledItemDelegate
{

signals:
    void modelDataChanged(void) const;

public:
    // create a single editable table-cell
    QWidget* createEditor( QWidget * parent, const QStyleOptionViewItem &, const QModelIndex & index) const Q_DECL_OVERRIDE
    {
        QLineEdit * edit = new QLineEdit( parent );

        // set validator
        int upperBound;
        if (index.column() == 0)    // Set the upper bound of the first column
        {
            upperBound = m_firstColBound;
        }
        else                        // Set the upper bound of the second (and all other) columns
        {
            upperBound = m_secondColBound;
        }


        QValidator *valid;

        if (index.row() == 1)
        {
             valid = new QIntValidator( 0, upperBound, edit );
        }
        else
        {
            valid = new QDoubleValidator( 0, upperBound,1, edit );
        }
        edit->setValidator( valid );

        return ( edit );
    }

    // transfer value from data-model into line-edit
    void setEditorData( QWidget * editor, const QModelIndex & idx ) const Q_DECL_OVERRIDE
    {
        if( idx.model()->data( idx, Qt::EditRole ).userType() == QVariant::Int)
        {
            int value = idx.model()->data( idx, Qt::EditRole ).toInt();
            QLineEdit * edt = static_cast< QLineEdit * >( editor );
            edt->setText( QString().setNum(value) );
        }
        else
        {
            double value = idx.model()->data( idx, Qt::EditRole ).toDouble();
            QLineEdit * edt = static_cast< QLineEdit * >( editor );
            edt->setText( QString().setNum(value) );
        }
    }

    // transfer value from line_edit into data-model
    void setModelData( QWidget * editor, QAbstractItemModel * model, const QModelIndex &idx ) const Q_DECL_OVERRIDE
    {
        QLineEdit * edt = static_cast< QLineEdit * >( editor );
        QString value = edt->text();
        if( idx.row() == 1)
        {
            model->setData( idx, value.toInt(), Qt::EditRole );
        }
        else
        {
            model->setData( idx, value.toDouble(), Qt::EditRole );
        }


    }

    // set geometry of line-edit
    void updateEditorGeometry( QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & ) const Q_DECL_OVERRIDE
    {
        editor->setGeometry( option.rect );
    }

    QString displayText(const QVariant &value, const QLocale& locale) const Q_DECL_OVERRIDE
    {
        QString text;
        switch (value.userType()) {
           case QMetaType::Float:
           case QVariant::Double:
               text = QString("%1").arg( value.toDouble());
               break;
           case QVariant::Int:
           case QVariant::LongLong:
               text = locale.toString(value.toLongLong());
               break;
        default:
                text = locale.toString(value.toLongLong());
                break;
        }

        return text;
    }


    // Set the upper bound for first end second column
    void setBounds(int firstColBound, int secondColBound)
    {
        m_firstColBound = firstColBound;
        m_secondColBound = secondColBound;
    }

    // Get the upper bound of the first column
    int getFirstColBound()
    {
        return m_firstColBound;
    }

    // Get the upper bound of the second column
    int getSecondColBound()
    {
        return m_secondColBound;
    }



private:
    int m_firstColBound;
    int m_secondColBound;



};

/******************************************************************************
 * LensDriverBox::PrivateData
 *****************************************************************************/
class LensDriverBox::PrivateData
{
public:
    PrivateData(QWidget * parent)
        : m_ui( new Ui::UI_LensDriverBox )
        , m_cntEvents( 0 )
        , m_maxEvents( 5 )
        , m_sbxStyle( new SpinBoxStyle() )
        , m_delegate( new LensDriverIrisTabStyledDelegat() )
        , m_LensSettings{}
        , m_LensIrisTable{}


    {
        // initialize UI
        m_ui->setupUi( parent );
        initDataModel();
        m_ui->tblIrisAptPosition->setItemDelegate( m_delegate );
        m_ui->tblIrisAptPosition->horizontalHeader()->setSectionResizeMode( QHeaderView::Stretch );
        m_ui->tblIrisAptPosition->setSelectionBehavior( QAbstractItemView::SelectItems );
        m_ui->tblIrisAptPosition->clearFocus();
        setDataModel();

        m_delegate->setBounds( 500, 500 );
    }

    ~PrivateData()
    {
        delete m_ui;
        delete m_sbxStyle;
        delete m_model;
    }

    /******************************************************************************
     * initDataModel()
     *****************************************************************************/
    // initialize data model
    void initDataModel()
    {
        m_model = new QStandardItemModel( 0, IRIS_APT_TABLE_MAX_NO_COLUMNS, nullptr );
        m_model->setVerticalHeaderItem( 0, new QStandardItem(QString("F-Stop")) );
        m_model->setVerticalHeaderItem( 1, new QStandardItem(QString("Position")) );
    }


    /******************************************************************************
     * setDataModel()
     *****************************************************************************/
    // replace data model of the table view widget with our custom model
    void setDataModel()
    {
        // see http://doc.qt.io/qt-5.7/qabstractitemview.html#setModel
        // get pointer to old selection model
        QItemSelectionModel *m = m_ui->tblIrisAptPosition->selectionModel();
        // set new data model
        m_ui->tblIrisAptPosition->setModel( m_model );
        // delete old selection model
        delete m;
    }

    /******************************************************************************
     * fillTable( QVector<double> &Fstop, QVector<int> &FstopPos )
     *****************************************************************************/
    // set positions in tableview widget
    void fillTable( QVector<double> &Fstop, QVector<int> &FstopPos )
    {
        Q_ASSERT( Fstop.count() == FstopPos.count() );
        Q_ASSERT( m_model != NULL );

        // update data model (add or remove rows)
        int numColModel = m_model->columnCount();
        int numColInput = Fstop.count();
        if ( numColModel < numColInput )
        {
            m_model->insertColumns( 0, numColInput - numColModel );
        }
        else
        {
            m_model->removeColumns( 0, numColInput - numColModel );
        }

        // set values
        for ( int i=0; i < Fstop.count(); ++i )
        {
            QModelIndex Fstop_i    = m_model->index( 0, i, QModelIndex() );
            QModelIndex FstopPos_i = m_model->index( 1, i, QModelIndex() );


            // set alignment
            m_model->setData( Fstop_i, QVariant(Qt::AlignVCenter | Qt::AlignRight), Qt::TextAlignmentRole );
            m_model->setData( FstopPos_i, QVariant(Qt::AlignVCenter | Qt::AlignRight), Qt::TextAlignmentRole );

            // set used or clear unsed sample values
            m_model->setData( Fstop_i, Fstop[i] );
            m_model->setData( FstopPos_i, FstopPos[i] );

        }
    }

    /******************************************************************************
     * getDataFromModel( QVector<double> &Fstop, QVector<int> &FstopPos  )
     *****************************************************************************/
    void getDataFromModel( QVector<double> &Fstop, QVector<int> &FstopPos  )
    {
        Fstop.clear();
        FstopPos.clear();

        for ( int i=0; i<m_model->columnCount(); i ++ )
        {
            Fstop.append( m_model->data( m_model->index(0, i), Qt::DisplayRole ).toDouble() );
            FstopPos.append( m_model->data( m_model->index(1, i), Qt::DisplayRole ).toInt() );
        }
    }


    /******************************************************************************
     * GetLensProfileName( LensProfile profile )
     *****************************************************************************/
    QString GetLensProfileName( enum LensProfile profile )
    {
        switch ( profile )
        {
            case LensProfileUnknown:
                return ( QString(LENS_PROFILE_NAME_UNKOWN) );
            case LensProfileDctKit_iris_1:
                return ( QString(LENS_PROFILE_NAME_DCT_KIT_IRIS_1) );
            case LensProfileDctKit_iris_2:
                return ( QString(LENS_PROFILE_NAME_DCT_KIT_IRIS_2) );
            case LensProfileDctKit_focus_1:
                return ( QString(LENS_PROFILE_NAME_DCT_KIT_FOCUS_1) );
            case LensProfileDctKit_focus_2:
                return ( QString(LENS_PROFILE_NAME_DCT_KIT_FOCUS_2) );
            case LensProfileDctKit_focus_iris:
                return ( QString(LENS_PROFILE_NAME_DCT_KIT_FOCUS_IRIS) );
            case LensProfileDctKit_iris_focus:
                return ( QString(LENS_PROFILE_NAME_DCT_KIT_IRIS_FOCUS) );
            case LensProfileICS:
                return ( QString(LENS_PROFILE_NAME_I_CS) );
            default:
                return ( QString() );
        }
    }

    /******************************************************************************
     * GetLensProfileByName( LensProfile profile )
     *****************************************************************************/
    enum LensProfile GetLensProfileByName( QString profile )
    {
        if( profile.compare(QString(LENS_PROFILE_NAME_UNKOWN)) == 0  )
        {
            return LensProfileUnknown;
        }
        if( profile.compare(QString(LENS_PROFILE_NAME_DCT_KIT_IRIS_1)) == 0 )
        {
            return LensProfileDctKit_iris_1;
        }
        if( profile.compare(QString(LENS_PROFILE_NAME_DCT_KIT_IRIS_2)) == 0  )
        {
            return LensProfileDctKit_iris_2;
        }
        if( profile.compare(QString(LENS_PROFILE_NAME_DCT_KIT_FOCUS_1)) == 0 )
        {
            return LensProfileDctKit_focus_1;
        }
        if( profile.compare(QString(LENS_PROFILE_NAME_DCT_KIT_FOCUS_2)) == 0 )
        {
            return LensProfileDctKit_focus_2;
        }
        if( profile.compare(QString(LENS_PROFILE_NAME_DCT_KIT_FOCUS_IRIS)) == 0 )
        {
            return LensProfileDctKit_focus_iris;
        }
        if( profile.compare(QString(LENS_PROFILE_NAME_DCT_KIT_IRIS_FOCUS)) == 0 )
        {
            return LensProfileDctKit_iris_focus;
        }
        if( profile.compare(QString(LENS_PROFILE_NAME_I_CS)) == 0 )
        {
            return LensProfileICS;
        }

        return LensProfileUnknown;
    }

    Ui::UI_LensDriverBox *  m_ui;           /**< ui handle */
    int                     m_cntEvents;    /**< ignore move-events if slider moving */
    int                     m_maxEvents;    /**< number of ignored move-events */
    SpinBoxStyle *          m_sbxStyle;     /**< proxy style to overrule not implemented spinbox properties */

    LensDriverIrisTabStyledDelegat * m_delegate;                /**< delegation class */
    QStandardItemModel *    m_model;                        /**< data model */
    lens_settings_t         m_LensSettings;
    lens_iris_position_t    m_LensIrisTable;
    QVector<lens_iris_position_template_t> m_LensIrisTemplates;
};

/******************************************************************************
 * LensDriverBox::LensDriverBox
 *****************************************************************************/
LensDriverBox::LensDriverBox( QWidget * parent ) : DctWidgetBox( parent )
{
    // create private data container
    d_data = new PrivateData( this );



    // overrule auto-repeat threshold
    d_data->m_ui->sbxFocusPosition->setStyle( d_data->m_sbxStyle );
    d_data->m_ui->sbxFocusSpeed->setStyle( d_data->m_sbxStyle );
    d_data->m_ui->sbxFocusStepMode->setStyle( d_data->m_sbxStyle );
    d_data->m_ui->sbxFocusTorque->setStyle( d_data->m_sbxStyle );

    d_data->m_ui->sbxZoomPosition->setStyle( d_data->m_sbxStyle );
    d_data->m_ui->sbxZoomSpeed->setStyle( d_data->m_sbxStyle );
    d_data->m_ui->sbxZoomStepMode->setStyle( d_data->m_sbxStyle );
    d_data->m_ui->sbxZoomTorque->setStyle( d_data->m_sbxStyle );

    d_data->m_ui->sbxIrisPosition->setStyle( d_data->m_sbxStyle );
    d_data->m_ui->sbxIrisSpeed->setStyle( d_data->m_sbxStyle );
    d_data->m_ui->sbxIrisStepMode->setStyle( d_data->m_sbxStyle );
    d_data->m_ui->sbxIrisTorque->setStyle( d_data->m_sbxStyle );

    d_data->m_ui->sbxFilterPosition->setStyle( d_data->m_sbxStyle );
    d_data->m_ui->sbxFilterSpeed->setStyle( d_data->m_sbxStyle );
    d_data->m_ui->sbxFilterStepMode->setStyle( d_data->m_sbxStyle );
    d_data->m_ui->sbxFilterTorque->setStyle( d_data->m_sbxStyle );

    d_data->m_ui->sbxFocusPosition->setRange(0,100);
    d_data->m_ui->sbxFocusSpeed->setRange(0,100);
    d_data->m_ui->sbxFocusStepMode->setRange(0,100);
    d_data->m_ui->sbxFocusTorque->setRange(0,100);

    d_data->m_ui->sbxZoomPosition->setRange(0,100);
    d_data->m_ui->sbxZoomSpeed->setRange(0,100);
    d_data->m_ui->sbxZoomStepMode->setRange(0,100);
    d_data->m_ui->sbxZoomTorque->setRange(0,100);

    d_data->m_ui->sbxIrisPosition->setRange(0,100);
    d_data->m_ui->sbxIrisSpeed->setRange(0,100);
    d_data->m_ui->sbxIrisStepMode->setRange(0,100);
    d_data->m_ui->sbxIrisTorque->setRange(0,100);

    d_data->m_ui->sbxFilterPosition->setRange(0,100);
    d_data->m_ui->sbxFilterSpeed->setRange(0,100);
    d_data->m_ui->sbxFilterStepMode->setRange(0,100);
    d_data->m_ui->sbxFilterTorque->setRange(0,100);

    d_data->m_ui->sldFocusPosition->setRange(0,100);
    d_data->m_ui->sldZoomPosition->setRange(0,100);
    d_data->m_ui->sldIrisPosition->setRange(0,100);
    d_data->m_ui->sldFilterPosition->setRange(0,100);

    d_data->m_ui->btnActivateLens->clearFocus();
    d_data->m_ui->cbxEnableAdvancedSettings->setChecked(false);

    d_data->m_ui->lblFocusSpeed->setVisible(false);
    d_data->m_ui->lblFocusStepMode->setVisible(false);
    d_data->m_ui->lblFocusTorque->setVisible(false);

    d_data->m_ui->sbxFocusSpeed->setVisible(false);
    d_data->m_ui->sbxFocusStepMode->setVisible(false);
    d_data->m_ui->sbxFocusTorque->setVisible(false);

    d_data->m_ui->lblZoomSpeed->setVisible(false);
    d_data->m_ui->lblZoomStepMode->setVisible(false);
    d_data->m_ui->lblZoomTorque->setVisible(false);

    d_data->m_ui->sbxZoomSpeed->setVisible(false);
    d_data->m_ui->sbxZoomStepMode->setVisible(false);
    d_data->m_ui->sbxZoomTorque->setVisible(false);

    d_data->m_ui->lblIrisSpeed->setVisible(false);
    d_data->m_ui->lblIrisStepMode->setVisible(false);
    d_data->m_ui->lblIrisTorque->setVisible(false);

    d_data->m_ui->sbxIrisSpeed->setVisible(false);
    d_data->m_ui->sbxIrisStepMode->setVisible(false);
    d_data->m_ui->sbxIrisTorque->setVisible(false);

    d_data->m_ui->lblFilterSpeed->setVisible(false);
    d_data->m_ui->lblFilterStepMode->setVisible(false);
    d_data->m_ui->lblFilterTorque->setVisible(false);

    d_data->m_ui->sbxFilterSpeed->setVisible(false);
    d_data->m_ui->sbxFilterStepMode->setVisible(false);
    d_data->m_ui->sbxFilterTorque->setVisible(false);

    d_data->m_ui->cbxFocusFineEnable->setVisible(false);
    d_data->m_ui->cbxFocusInvertEnable->setVisible(false);
    d_data->m_ui->cbxZoomInvertEnable->setVisible(false);
    d_data->m_ui->cbxIrisInvertEnable->setVisible(false);
    d_data->m_ui->cbxFilterInvertEnable->setVisible(false);

    d_data->m_ui->lblIrisFStopTable->setVisible(false);
    d_data->m_ui->tblIrisAptPosition->setVisible(false);
    d_data->m_ui->cbxIrisTableTemplate->setVisible(false);
    d_data->m_ui->lblIrisTableUseTemplate->setVisible(false);
    d_data->m_ui->btnIrisTransmitTable->setVisible(false);


    d_data->m_LensIrisTemplates.append( computarM0824MPW2 );
    d_data->m_LensIrisTemplates.append( computarV0828MPY );
    d_data->m_LensIrisTemplates.append( computarEG6Z0915TCSMPWIR );

    // fill settings combo box
    for ( int i = LensProfileFirst; i < LensProfileMax; i++ )
    {
        addLensProfile( d_data->GetLensProfileName( static_cast<enum LensProfile>(i) ), i );
    }

    addLensIrisAperture( "Chose Apeture",0);

    // connect internal signals

    connect( d_data->m_ui->cbxLensProfile, SIGNAL(currentIndexChanged(int)), this, SLOT(onCbxLensProfileChange(int)) );
    connect( d_data->m_ui->btnActivateLens , SIGNAL(clicked()), this, SLOT(onBtnLensActiveChange()) );
    connect( d_data->m_ui->cbxEnableAdvancedSettings , SIGNAL(stateChanged(int)), this, SLOT(onCbxLensEnableAdvancedSettings(int)) );

    connect( this , SIGNAL(LensSettingsChanged(bool)), this, SLOT(onLensActiveChange(bool)) );


    // Focus Elements
    connect( d_data->m_ui->sbxFocusPosition, SIGNAL(valueChanged(int)), this, SLOT(onSbxLensFocusPositionChanged(int)));
    connect( d_data->m_ui->sldFocusPosition,SIGNAL(valueChanged(int)), d_data->m_ui->sbxFocusPosition, SLOT(setValue(int)));
    connect( d_data->m_ui->sbxFocusPosition, SIGNAL(valueChanged(int)),d_data->m_ui->sldFocusPosition,SLOT(setValue(int)));
    connect( d_data->m_ui->sbxFocusSpeed, SIGNAL(valueChanged(int)), this, SLOT(onSbxLensFocusSpeedChanged(int)));
    connect( d_data->m_ui->sbxFocusStepMode, SIGNAL(valueChanged(int)), this, SLOT(onSbxLensFocusStepModeChanged(int)));
    connect( d_data->m_ui->sbxFocusTorque, SIGNAL(valueChanged(int)), this, SLOT(onSbxLensFocusTorqueChanged(int)));
    connect( d_data->m_ui->cbxFocusFineEnable, SIGNAL(stateChanged(int)), this, SLOT(onCbxLensFocusFineChanged(int)) );
    connect( d_data->m_ui->cbxFocusInvertEnable, SIGNAL(stateChanged(int)), this, SLOT(onCbxLensFocusInvertChanged(int)) );

    // Zoom Elements
    connect( d_data->m_ui->sbxZoomPosition, SIGNAL(valueChanged(int)), this, SLOT(onSbxLensZoomPositionChanged(int)));
    connect( d_data->m_ui->sldZoomPosition,SIGNAL(valueChanged(int)), d_data->m_ui->sbxZoomPosition, SLOT(setValue(int)));
    connect( d_data->m_ui->sbxZoomPosition, SIGNAL(valueChanged(int)),d_data->m_ui->sldZoomPosition,SLOT(setValue(int)));
    connect( d_data->m_ui->sbxZoomSpeed, SIGNAL(valueChanged(int)), this, SLOT(onSbxLensZoomSpeedChanged(int)));
    connect( d_data->m_ui->sbxZoomStepMode, SIGNAL(valueChanged(int)), this, SLOT(onSbxLensZoomStepModeChanged(int)));
    connect( d_data->m_ui->sbxZoomTorque, SIGNAL(valueChanged(int)), this, SLOT(onSbxLensZoomTorqueChanged(int)));
    connect( d_data->m_ui->cbxZoomInvertEnable, SIGNAL(stateChanged(int)), this, SLOT(onCbxLensZoomInvertChanged(int)) );

    // Iris Elements
    connect( d_data->m_ui->sbxIrisPosition, SIGNAL(valueChanged(int)), this, SLOT(onSbxLensIrisPositionChanged(int)));
    connect( d_data->m_ui->sldIrisPosition,SIGNAL(valueChanged(int)), d_data->m_ui->sbxIrisPosition, SLOT(setValue(int)));
    connect( d_data->m_ui->sbxIrisPosition, SIGNAL(valueChanged(int)),d_data->m_ui->sldIrisPosition,SLOT(setValue(int)));
    connect( d_data->m_ui->sbxIrisSpeed, SIGNAL(valueChanged(int)), this, SLOT(onSbxLensIrisSpeedChanged(int)));
    connect( d_data->m_ui->sbxIrisStepMode, SIGNAL(valueChanged(int)), this, SLOT(onSbxLensIrisStepModeChanged(int)));
    connect( d_data->m_ui->sbxIrisTorque, SIGNAL(valueChanged(int)), this, SLOT(onSbxLensIrisTorqueChanged(int)));
    connect( d_data->m_ui->cbxIrisInvertEnable, SIGNAL(stateChanged(int)), this, SLOT(onCbxLensIrisInvertChanged(int)) );
    connect( d_data->m_ui->cbxLensIrisAperture, SIGNAL(currentIndexChanged(int)), this, SLOT(onCbxLensIrisApertureChanged ( int )));

    connect( this, SIGNAL(LensIrisAperturePosChanged( int )), this, SLOT(onLensIrisPositionChange(int )));
    connect( this, SIGNAL(LensIrisAperturePosChanged( int )), this, SLOT( onLensIrisAperturePosChange( int )));

    connect( d_data->m_ui->btnIrisAptPlus, SIGNAL(clicked()), this, SLOT(onBtnLensIrisAperturePlusChanged( void ) ));
    connect( d_data->m_ui->btnIrisAptMinus, SIGNAL(clicked()), this, SLOT(onBtnLensIrisApertureMinusChanged( void ) ));
    connect( d_data->m_ui->btnIrisTransmitTable,SIGNAL(clicked()), this, SLOT( onBtnLensIrisTableTransmitChanged( void) ));
    connect( this, SIGNAL(LensIrisSetupChanged( QVector<int> )), this, SLOT( onLensIrisSetupChange( QVector<int> ) ));
    connect( d_data->m_ui->cbxIrisTableTemplate, SIGNAL( currentIndexChanged(int)), this, SLOT( onCbxLensIrisTemplateChanged( int )) );


    // Filter Elements
    connect( d_data->m_ui->sbxFilterPosition, SIGNAL(valueChanged(int)), this, SLOT(onSbxLensFilterPositionChanged(int)));
    connect( d_data->m_ui->sldFilterPosition,SIGNAL(valueChanged(int)), d_data->m_ui->sbxFilterPosition, SLOT(setValue(int)));
    connect( d_data->m_ui->sbxFilterPosition, SIGNAL(valueChanged(int)),d_data->m_ui->sldFilterPosition,SLOT(setValue(int)));
    connect( d_data->m_ui->sbxFilterSpeed, SIGNAL(valueChanged(int)), this, SLOT(onSbxLensFilterSpeedChanged(int)));
    connect( d_data->m_ui->sbxFilterStepMode, SIGNAL(valueChanged(int)), this, SLOT(onSbxLensFilterStepModeChanged(int)));
    connect( d_data->m_ui->sbxFilterTorque, SIGNAL(valueChanged(int)), this, SLOT(onSbxLensFilterTorqueChanged(int)));
    connect( d_data->m_ui->cbxFilterInvertEnable, SIGNAL(stateChanged(int)), this, SLOT(onCbxLensFilterInvertChanged(int)) );




    ////////////////////
    // operation mode
    ////////////////////
    prepareMode( mode() ); 
}

/******************************************************************************
 * LensDriverBox::~LensDriverBox
 *****************************************************************************/
LensDriverBox::~LensDriverBox()
{
    delete d_data;
}

/******************************************************************************
 * LensDriverBox::prepareMode
 *****************************************************************************/
void LensDriverBox::prepareMode( const Mode mode )
{
    (void)mode;
}

/******************************************************************************
 * LensDriverBox::loadSettings
 *****************************************************************************/
void LensDriverBox::loadSettings( QSettings & s )
{
    s.beginGroup( LENSDRIVER_SETTINGS_SECTION_NAME );
    setLensProfile(s.value(LENSDRIVER_SETTINGS_SETTINGS).toString() );
    setLensFocusPosition(s.value(LENSDRIVER_SETTINGS_FOCUS_POSITION).toInt());
    setLensFocusSettings(s.value(LENSDRIVER_SETTINGS_FOCUS_SETTINGS).toString());
    setLensZoomPosition(s.value(LENSDRIVER_SETTINGS_ZOOM_POSITION).toInt());
    setLensZoomSettings(s.value(LENSDRIVER_SETTINGS_ZOOM_SETTINGS).toString());
    setLensIrisPosition(s.value(LENSDRIVER_SETTINGS_IRIS_POSITION).toInt());
    setLensIrisSettings(s.value(LENSDRIVER_SETTINGS_IRIS_SETTINGS).toString());
    setLensFilterPosition(s.value(LENSDRIVER_SETTINGS_FILTER_POSITION).toInt());
    setLensFilterSettings(s.value(LENSDRIVER_SETTINGS_FILTER_SETTINGS).toString());
    s.endGroup();
}

/******************************************************************************
 * LensDriverBox::saveSettings
 *****************************************************************************/
void LensDriverBox::saveSettings( QSettings & s )
{
    s.beginGroup( LENSDRIVER_SETTINGS_SECTION_NAME );
    s.setValue(LENSDRIVER_SETTINGS_SETTINGS, LensProfile());
    s.setValue(LENSDRIVER_SETTINGS_FOCUS_POSITION, LensFocusPosition());
    s.setValue(LENSDRIVER_SETTINGS_FOCUS_SETTINGS, LensFocusSettings());
    s.setValue(LENSDRIVER_SETTINGS_ZOOM_POSITION, LensZoomPosition());
    s.setValue(LENSDRIVER_SETTINGS_ZOOM_SETTINGS, LensZoomSettings());
    s.setValue(LENSDRIVER_SETTINGS_IRIS_POSITION, LensIrisPosition());
    s.setValue(LENSDRIVER_SETTINGS_IRIS_SETTINGS, LensIrisSettings());
    s.setValue(LENSDRIVER_SETTINGS_FILTER_POSITION, LensFilterPosition());
    s.setValue(LENSDRIVER_SETTINGS_FILTER_SETTINGS, LensFilterSettings());
    s.endGroup();
}

/******************************************************************************
 * LensDriverBox::applySettings
 *****************************************************************************/
void LensDriverBox::applySettings( void )
{
}

/******************************************************************************
 * LensDriverBox::profileToSettings
 *****************************************************************************/
lens_settings_t LensDriverBox::profileToSettings( enum LensProfile profile )
{
    return lensSettingProfiles[profile];
}

/******************************************************************************
 * LensDriverBox::settingsToProfile
 *****************************************************************************/
enum LensProfile LensDriverBox::settingsToProfile( lens_settings_t settings )
{
    for ( int i = 0; i < LensProfileMax; i++ )
    {
        if ( settings.address == lensSettingProfiles[i].address &&
             settings.chipID == lensSettingProfiles[i].chipID &&
             settings.controllerFeatures == lensSettingProfiles[i].controllerFeatures &&
             settings.focusMotorNr == lensSettingProfiles[i].focusMotorNr &&
             settings.zoomMotorNr == lensSettingProfiles[i].zoomMotorNr &&
             settings.irisMotorNr == lensSettingProfiles[i].irisMotorNr &&
             settings.filterMotorNr == lensSettingProfiles[i].filterMotorNr &&
             settings.focusMotorFeatures == lensSettingProfiles[i].focusMotorFeatures &&
             settings.zoomMotorFeatures == lensSettingProfiles[i].zoomMotorFeatures &&
             settings.irisMotorFeatures == lensSettingProfiles[i].irisMotorFeatures &&
             settings.filterMotorFeatures == lensSettingProfiles[i].filterMotorFeatures )
        {
            return static_cast<enum LensProfile>(i);
        }
    }

    return LensProfileUnknown;
}

/******************************************************************************
 * LensDriverBox::addLensProfile
 *****************************************************************************/
void LensDriverBox::addLensProfile( QString name, int id )
{
    d_data->m_ui->cbxLensProfile->blockSignals( true );
    d_data->m_ui->cbxLensProfile->addItem( name, id );
    d_data->m_ui->cbxLensProfile->blockSignals( false );
}

/******************************************************************************
 * LensDriverBox::addLensIrisAperture
 *****************************************************************************/
void LensDriverBox::addLensIrisAperture( QString name, int id )
{
    d_data->m_ui->cbxLensIrisAperture->blockSignals( true );
    d_data->m_ui->cbxLensIrisAperture->addItem( name, id );
    d_data->m_ui->cbxLensIrisAperture->blockSignals( false );
}

/******************************************************************************
 * LensDriverBox::addLensIrisTemplate
 *****************************************************************************/
void LensDriverBox::addLensIrisTemplate( QString name, int id )
{
    d_data->m_ui->cbxIrisTableTemplate->blockSignals( true );
    d_data->m_ui->cbxIrisTableTemplate->addItem( name, id );
    d_data->m_ui->cbxIrisTableTemplate->blockSignals( false );
}
/******************************************************************************
 * LensDriverBox::LensProfile
 *****************************************************************************/
QString LensDriverBox::LensProfile() const
{
    return ( d_data->m_ui->cbxLensProfile->currentText() );
}

/******************************************************************************
 * LensDriverBox::LensFocusPosition
 *****************************************************************************/
int LensDriverBox::LensFocusPosition() const
{
    return ( d_data->m_ui->sbxFocusPosition->value() );
}

/******************************************************************************
 * LensDriverBox::LensFocusSettings
 *****************************************************************************/
QString LensDriverBox::LensFocusSettings() const
{
    QString settings;
    settings.append(d_data->m_ui->sbxFocusSpeed->cleanText() );
    settings.append(" ");
    settings.append(d_data->m_ui->sbxFocusStepMode->cleanText() );
    settings.append(" ");
    settings.append(d_data->m_ui->sbxFocusTorque->cleanText() );
    return ( settings );
}

/******************************************************************************
 * LensDriverBox::LensZoomPosition
 *****************************************************************************/
int LensDriverBox::LensZoomPosition() const
{
    return ( d_data->m_ui->sbxZoomPosition->value() );
}

/******************************************************************************
 * LensDriverBox::LensZoomSettings
 *****************************************************************************/
QString LensDriverBox::LensZoomSettings() const
{
    QString settings;
    settings.append(d_data->m_ui->sbxZoomSpeed->cleanText() );
    settings.append(" ");
    settings.append(d_data->m_ui->sbxZoomStepMode->cleanText() );
    settings.append(" ");
    settings.append(d_data->m_ui->sbxZoomTorque->cleanText() );
    return ( settings );
}

/******************************************************************************
 * LensDriverBox::LensIrisPosition
 *****************************************************************************/
int LensDriverBox::LensIrisPosition() const
{
    return ( d_data->m_ui->sbxIrisPosition->value() );
}

/******************************************************************************
 * LensDriverBox::LensIrisSettings
 *****************************************************************************/
QString LensDriverBox::LensIrisSettings() const
{
    QString settings;
    settings.append(d_data->m_ui->sbxIrisSpeed->cleanText() );
    settings.append(" ");
    settings.append(d_data->m_ui->sbxIrisStepMode->cleanText() );
    settings.append(" ");
    settings.append(d_data->m_ui->sbxIrisTorque->cleanText() );
    return ( settings );
}

/******************************************************************************
 * LensDriverBox::LensFilterPosition
 *****************************************************************************/
int LensDriverBox::LensFilterPosition() const
{
    return ( d_data->m_ui->sbxFilterPosition->value() );
}

/******************************************************************************
 * LensDriverBox::LensFilterSettings
 *****************************************************************************/
QString LensDriverBox::LensFilterSettings() const
{
    QString settings;
    settings.append(d_data->m_ui->sbxFilterSpeed->cleanText() );
    settings.append(" ");
    settings.append(d_data->m_ui->sbxFilterStepMode->cleanText() );
    settings.append(" ");
    settings.append(d_data->m_ui->sbxFilterTorque->cleanText() );
    return ( settings );
}

/******************************************************************************
 * LensDriverBox::setLensProfile
 *****************************************************************************/
void LensDriverBox::setLensProfile( const QString mode )
{
    int index = d_data->m_ui->cbxLensProfile->findText( mode );
    if ( index != -1 )
    {
        d_data->m_ui->cbxLensProfile->blockSignals( true );
        d_data->m_ui->cbxLensProfile->setCurrentIndex( index );
        d_data->m_ui->cbxLensProfile->blockSignals( false );

        QVector<int> values;

        lens_settings_t settings = profileToSettings(static_cast<enum LensProfile>(index));

        values.append(settings.address);
        values.append(settings.chipID);
        values.append(settings.controllerFeatures);
        values.append(settings.focusMotorNr);
        values.append(settings.zoomMotorNr);
        values.append(settings.irisMotorNr);
        values.append(settings.filterMotorNr);
        values.append(settings.focusMotorFeatures);
        values.append(settings.zoomMotorFeatures);
        values.append(settings.irisMotorFeatures);
        values.append(settings.filterMotorFeatures);

        emit LensSettingsChanged( values );
    }
}

/******************************************************************************
 * LensDriverBox::setLensFocusPosition
 *****************************************************************************/
void LensDriverBox::setLensFocusPosition( int focus )
{

        d_data->m_ui->sbxFocusPosition->blockSignals( true );
        d_data->m_ui->sbxFocusPosition->setValue(focus);
        d_data->m_ui->sbxFocusPosition->blockSignals( false );

        emit LensFocusPositionChanged( focus );
}

/******************************************************************************
 * LensDriverBox::setLensZoomPosition
 *****************************************************************************/
void LensDriverBox::setLensZoomPosition( int zoom )
{

        d_data->m_ui->sbxZoomPosition->blockSignals( true );
        d_data->m_ui->sbxZoomPosition->setValue(zoom);
        d_data->m_ui->sbxZoomPosition->blockSignals( false );

        emit LensZoomPositionChanged( zoom );
}

/******************************************************************************
 * LensDriverBox::setLensIrisPosition
 *****************************************************************************/
void LensDriverBox::setLensIrisPosition( int iris )
{

        d_data->m_ui->sbxIrisPosition->blockSignals( true );
        d_data->m_ui->sbxIrisPosition->setValue(iris);
        d_data->m_ui->sbxIrisPosition->blockSignals( false );

        d_data->m_ui->sldIrisPosition->blockSignals( true );
        d_data->m_ui->sldIrisPosition->setValue(iris);
        d_data->m_ui->sldIrisPosition->blockSignals( false );

        emit LensIrisPositionChanged( iris );
}

/******************************************************************************
 * LensDriverBox::setLensFilterPosition
 *****************************************************************************/
void LensDriverBox::setLensFilterPosition( int filter )
{

        d_data->m_ui->sbxFilterPosition->blockSignals( true );
        d_data->m_ui->sbxFilterPosition->setValue(filter);
        d_data->m_ui->sbxFilterPosition->blockSignals( false );

        emit LensFilterPositionChanged( filter );
}

/******************************************************************************
 * LensDriverBox::setLensFocusSettings
 *****************************************************************************/
void LensDriverBox::setLensFocusSettings( const QString settings )
{
        QStringList values;
        values = settings.split(QRegularExpression("\\s+"));

        int speed = values.first().toInt();
        values.removeFirst();
        int stepMode = values.first().toInt();
        values.removeFirst();
        int torque = values.first().toInt();

        d_data->m_ui->sbxFocusSpeed->blockSignals( true );
        d_data->m_ui->sbxFocusSpeed->setValue(speed);
        d_data->m_ui->sbxFocusSpeed->blockSignals( false );

        d_data->m_ui->sbxFocusStepMode->blockSignals( true );
        d_data->m_ui->sbxFocusStepMode->setValue(stepMode);
        d_data->m_ui->sbxFocusStepMode->blockSignals( false );

        d_data->m_ui->sbxFocusTorque->blockSignals( true );
        d_data->m_ui->sbxFocusTorque->setValue(torque);
        d_data->m_ui->sbxFocusTorque->blockSignals( false );

        QVector<int> valueVector;

        valueVector.append(speed);
        valueVector.append(stepMode);
        valueVector.append(torque);

        emit LensFocusSettingsChanged(valueVector);


}

/******************************************************************************
 * LensDriverBox::setLensZoomSettings
 *****************************************************************************/
void LensDriverBox::setLensZoomSettings( const QString settings )
{
        QStringList values;
        values = settings.split(QRegularExpression("\\s+"));

        int speed = values.first().toInt();
        values.removeFirst();
        int stepMode = values.first().toInt();
        values.removeFirst();
        int torque = values.first().toInt();

        d_data->m_ui->sbxZoomSpeed->blockSignals( true );
        d_data->m_ui->sbxZoomSpeed->setValue(speed);
        d_data->m_ui->sbxZoomSpeed->blockSignals( false );

        d_data->m_ui->sbxZoomStepMode->blockSignals( true );
        d_data->m_ui->sbxZoomStepMode->setValue(stepMode);
        d_data->m_ui->sbxZoomStepMode->blockSignals( false );

        d_data->m_ui->sbxZoomTorque->blockSignals( true );
        d_data->m_ui->sbxZoomTorque->setValue(torque);
        d_data->m_ui->sbxZoomTorque->blockSignals( false );

        QVector<int> valueVector;

        valueVector.append(speed);
        valueVector.append(stepMode);
        valueVector.append(torque);

        emit LensZoomSettingsChanged(valueVector);


}

/******************************************************************************
 * LensDriverBox::setLensIrisSettings
 *****************************************************************************/
void LensDriverBox::setLensIrisSettings( const QString settings )
{
        QStringList values;
        values = settings.split(QRegularExpression("\\s+"));

        int speed = values.first().toInt();
        values.removeFirst();
        int stepMode = values.first().toInt();
        values.removeFirst();
        int torque = values.first().toInt();

        d_data->m_ui->sbxIrisSpeed->blockSignals( true );
        d_data->m_ui->sbxIrisSpeed->setValue(speed);
        d_data->m_ui->sbxIrisSpeed->blockSignals( false );

        d_data->m_ui->sbxIrisStepMode->blockSignals( true );
        d_data->m_ui->sbxIrisStepMode->setValue(stepMode);
        d_data->m_ui->sbxIrisStepMode->blockSignals( false );

        d_data->m_ui->sbxIrisTorque->blockSignals( true );
        d_data->m_ui->sbxIrisTorque->setValue(torque);
        d_data->m_ui->sbxIrisTorque->blockSignals( false );

        QVector<int> valueVector;

        valueVector.append(speed);
        valueVector.append(stepMode);
        valueVector.append(torque);

        emit LensIrisSettingsChanged(valueVector);


}

/******************************************************************************
 * LensDriverBox::setLensFilterSettings
 *****************************************************************************/
void LensDriverBox::setLensFilterSettings( const QString settings )
{
        QStringList values;
        values = settings.split(QRegularExpression("\\s+"));

        int speed = values.first().toInt();
        values.removeFirst();
        int stepMode = values.first().toInt();
        values.removeFirst();
        int torque = values.first().toInt();

        d_data->m_ui->sbxFilterSpeed->blockSignals( true );
        d_data->m_ui->sbxFilterSpeed->setValue(speed);
        d_data->m_ui->sbxFilterSpeed->blockSignals( false );

        d_data->m_ui->sbxFilterStepMode->blockSignals( true );
        d_data->m_ui->sbxFilterStepMode->setValue(stepMode);
        d_data->m_ui->sbxFilterStepMode->blockSignals( false );

        d_data->m_ui->sbxFilterTorque->blockSignals( true );
        d_data->m_ui->sbxFilterTorque->setValue(torque);
        d_data->m_ui->sbxFilterTorque->blockSignals( false );

        QVector<int> valueVector;

        valueVector.append(speed);
        valueVector.append(stepMode);
        valueVector.append(torque);

        emit LensFilterSettingsChanged(valueVector);


}

/******************************************************************************
 * LensDriverBox::showLensProfilBoxes
 *****************************************************************************/
int LensDriverBox::showLensProfilBoxes(enum LensProfile currentProfile)
{
    int res = 0;
    bool focusAvailable = false;
    bool zoomAvailable = false;
    bool irisAvailable = false;
    bool filterAvailable = false;

    lens_settings_t lensfeatures;
    lensfeatures  = profileToSettings( currentProfile );

    ( ( lensfeatures.controllerFeatures & ( 1 << lensfeatures.focusMotorNr ) ) > 0) ? focusAvailable = true : focusAvailable = false;
    ( ( lensfeatures.controllerFeatures & ( 1 << lensfeatures.zoomMotorNr ) ) > 0) ? zoomAvailable = true : zoomAvailable = false;
    ( ( lensfeatures.controllerFeatures & ( 1 << lensfeatures.irisMotorNr ) ) > 0) ? irisAvailable = true : irisAvailable = false;
    ( ( lensfeatures.controllerFeatures & ( 1 << lensfeatures.filterMotorNr ) ) > 0) ? filterAvailable = true : filterAvailable = false;

    d_data->m_ui->gbxFocusControl->setVisible(focusAvailable);
    d_data->m_ui->gbxZoomControl->setVisible(zoomAvailable);
    d_data->m_ui->gbxIrisControl->setVisible(irisAvailable);
    d_data->m_ui->gbxFilterControl->setVisible(filterAvailable);

    enableLensMotorSettings(LensFeatuesFocus,lensfeatures.focusMotorFeatures);
    enableLensMotorSettings(LensFeatuesZoom,lensfeatures.zoomMotorFeatures);
    enableLensMotorSettings(LensFeatuesIris,lensfeatures.irisMotorFeatures);
    enableLensMotorSettings(LensFeatuesFilter,lensfeatures.filterMotorFeatures);


   return res;
}

/******************************************************************************
 * LensDriverBox::enableLensMotorSettings
 *****************************************************************************/
int LensDriverBox::enableLensMotorSettings(enum LensFeatues features,int motorSettings)
{
    int res = 0;
    bool speedEnable = false;
    bool stepModeEnable = false;
    bool torqueEnable = false;

    ( ( motorSettings & ( 1 << 1 ) ) > 0 ) ? speedEnable = true : speedEnable = false;
    ( ( motorSettings & ( 1 << 2 ) ) > 0 ) ? stepModeEnable = true : stepModeEnable = false;
    ( ( motorSettings & ( 1 << 3 ) ) > 0 ) ? torqueEnable = true : torqueEnable = false;

    switch ( static_cast<int>(features) )
    {
        case static_cast<int>( LensFeatuesFocus):
        {
            d_data->m_ui->sbxFocusSpeed->setEnabled(speedEnable);
            d_data->m_ui->sbxFocusStepMode->setEnabled(stepModeEnable);
            d_data->m_ui->sbxFocusTorque->setEnabled(torqueEnable);
            break;
        }
        case static_cast<int>( LensFeatuesZoom ):
        {
            d_data->m_ui->sbxZoomSpeed->setEnabled(speedEnable);
            d_data->m_ui->sbxZoomStepMode->setEnabled(stepModeEnable);
            d_data->m_ui->sbxZoomTorque->setEnabled(torqueEnable);
            break;
        }
        case static_cast<int>( LensFeatuesIris ):
        {
            d_data->m_ui->sbxIrisSpeed->setEnabled(speedEnable);
            d_data->m_ui->sbxIrisStepMode->setEnabled(stepModeEnable);
            d_data->m_ui->sbxIrisTorque->setEnabled(torqueEnable);
            break;
        }
        case static_cast<int>( LensFeatuesFilter ):
        {
            d_data->m_ui->sbxFilterSpeed->setEnabled(speedEnable);
            d_data->m_ui->sbxFilterStepMode->setEnabled(stepModeEnable);
            d_data->m_ui->sbxFilterTorque->setEnabled(torqueEnable);
            break;
        }
    }

    return res;
}

/******************************************************************************
 * LensDriverBox::onLensSettingsChange
 *****************************************************************************/
void LensDriverBox::onLensSettingsChange( QVector<int> values )
{
    lens_settings_t settings;
    settings.address             = values[0];
    settings.chipID              = values[1];
    settings.controllerFeatures  = values[2];
    settings.focusMotorNr        = values[3];
    settings.zoomMotorNr         = values[4];
    settings.irisMotorNr         = values[5];
    settings.filterMotorNr       = values[6];
    settings.focusMotorFeatures  = values[7];
    settings.zoomMotorFeatures   = values[8];
    settings.irisMotorFeatures   = values[9];
    settings.filterMotorFeatures = values[10];

    showLensProfilBoxes(settingsToProfile(settings));
    d_data->m_LensSettings = settings;

    d_data->m_ui->cbxLensProfile->blockSignals( true );
    d_data->m_ui->cbxLensProfile->setCurrentText(d_data->GetLensProfileName(settingsToProfile(settings)) );
    d_data->m_ui->cbxLensProfile->blockSignals( false );


}



/******************************************************************************
 * LensDriverBox::onLensActiveChange
 *****************************************************************************/
void LensDriverBox::onLensActiveChange( bool active )
{
    static bool last_state = false;

    if( active == true)
    {
        d_data->m_ui->lblLensActivated->setText("Active");
    }
    else
    {
         d_data->m_ui->lblLensActivated->setText("not Active");
    }

    d_data->m_ui->gbxFocusControl->setEnabled(active);
    d_data->m_ui->gbxZoomControl->setEnabled(active);
    d_data->m_ui->gbxIrisControl->setEnabled(active);
    d_data->m_ui->gbxFilterControl->setEnabled(active);

    if(active == true && last_state == false)
    {
        emit SmallResyncRequest();
    }
    last_state = active;

    d_data->m_ui->cbxLensProfile->blockSignals(true);
    d_data->m_ui->cbxLensProfile->clear();
    d_data->m_ui->cbxLensProfile->blockSignals(true);

    if( active == true && ( d_data->m_LensSettings.chipID >= 100) )
    {
        for ( int i = LensProfileFirst; i < LensProfileMax; i++ )
        {
            // Add only Devices with normal register interface --> chip Id < 100
            lens_settings_t tempSettings = profileToSettings(static_cast< enum LensProfile>( i) );
            if( ( tempSettings.chipID > 100 ) && ( tempSettings.controllerFeatures == d_data->m_LensSettings.controllerFeatures ) )
            {
                addLensProfile( d_data->GetLensProfileName( static_cast<enum LensProfile>(i) ), i );
            }
        }

    }
    else
    {
        for ( int i = LensProfileFirst; i < LensProfileMax; i++ )
        {
            // Add only Devices with normal register interface --> chip Id < 100
            lens_settings_t tempSettings = profileToSettings(static_cast< enum LensProfile>( i) );
            if( tempSettings.chipID < 100)
            {
                addLensProfile( d_data->GetLensProfileName( static_cast<enum LensProfile>(i) ), i );
            }
            if( active == false)
            {
                addLensProfile( d_data->GetLensProfileName( settingsToProfile(d_data->m_LensSettings) ), i+1 );
            }
        }
    }
    d_data->m_ui->cbxLensProfile->blockSignals( true );
    d_data->m_ui->cbxLensProfile->setCurrentText( d_data->GetLensProfileName(settingsToProfile(d_data->m_LensSettings )) );
    d_data->m_ui->cbxLensProfile->blockSignals( false );


    d_data->m_ui->cbxIrisTableTemplate->blockSignals(true);
    d_data->m_ui->cbxIrisTableTemplate->clear();
    d_data->m_ui->cbxIrisTableTemplate->blockSignals(true);

    addLensIrisTemplate("Choose a Template",1);
    for( int i = 0; i < d_data->m_LensIrisTemplates.length(); i++)
    {
        if( d_data->m_LensIrisTemplates.value(i).compatibleID.contains( d_data->m_LensSettings.chipID))
        {
            addLensIrisTemplate(d_data->m_LensIrisTemplates.value(i).lensName,i);
        }
    }
}

/******************************************************************************
 * LensDriverBox::onLensInvertChange
 *****************************************************************************/
void LensDriverBox::onLensInvertChange( QVector<int> values )
{
    d_data->m_ui->cbxFocusInvertEnable->blockSignals(true);
    d_data->m_ui->cbxFocusInvertEnable->setChecked( bool( values.value(3)) );
    d_data->m_ui->cbxFocusInvertEnable->blockSignals(false);

    d_data->m_ui->cbxZoomInvertEnable->blockSignals(true);
    d_data->m_ui->cbxZoomInvertEnable->setChecked( bool( values.value(2)) );
    d_data->m_ui->cbxZoomInvertEnable->blockSignals(false);

    d_data->m_ui->cbxIrisInvertEnable->blockSignals(true);
    d_data->m_ui->cbxIrisInvertEnable->setChecked( bool( values.value(1)) );
    d_data->m_ui->cbxIrisInvertEnable->blockSignals(false);

    d_data->m_ui->cbxFilterInvertEnable->blockSignals(true);
    d_data->m_ui->cbxFilterInvertEnable->setChecked( bool( values.value(0)) );
    d_data->m_ui->cbxFilterInvertEnable->blockSignals(false);
}

/******************************************************************************
 * LensDriverBox::onLensFocusPositionChange
 *****************************************************************************/
void LensDriverBox::onLensFocusPositionChange( int pos )
{
    d_data->m_ui->sbxFocusPosition->blockSignals( true );
    d_data->m_ui->sbxFocusPosition->setValue(pos);
    d_data->m_ui->sbxFocusPosition->blockSignals( false );

    d_data->m_ui->sldFocusPosition->blockSignals( true );
    d_data->m_ui->sldFocusPosition->setValue(pos);
    d_data->m_ui->sldFocusPosition->blockSignals( false );
}

/******************************************************************************
 * LensDriverBox::onLensFocusFineChange
 *****************************************************************************/
void LensDriverBox::onLensFocusFineChange( bool en )
{
    d_data->m_ui->cbxFocusFineEnable->blockSignals( true );
    d_data->m_ui->cbxFocusFineEnable->setChecked( en);
    d_data->m_ui->cbxFocusFineEnable->blockSignals( false );
}


/******************************************************************************
 * LensDriverBox::onLensZoomPositionChange
 *****************************************************************************/
void LensDriverBox::onLensZoomPositionChange( int pos )
{
    d_data->m_ui->sbxZoomPosition->blockSignals( true );
    d_data->m_ui->sbxZoomPosition->setValue(pos);
    d_data->m_ui->sbxZoomPosition->blockSignals( false );

    d_data->m_ui->sldZoomPosition->blockSignals( true );
    d_data->m_ui->sldZoomPosition->setValue(pos);
    d_data->m_ui->sldZoomPosition->blockSignals( false );
}

/******************************************************************************
 * LensDriverBox::onLensZoomInvertChange
 *****************************************************************************/
void LensDriverBox::onLensZoomInvertChange( bool en )
{
    d_data->m_ui->cbxZoomInvertEnable->blockSignals( true );
    d_data->m_ui->cbxZoomInvertEnable->setChecked( en);
    d_data->m_ui->cbxZoomInvertEnable->blockSignals( false );
}

/******************************************************************************
 * LensDriverBox::onLensIrisPositionChange
 *****************************************************************************/
void LensDriverBox::onLensIrisPositionChange( int pos )
{
    d_data->m_ui->sbxIrisPosition->blockSignals( true );
    d_data->m_ui->sbxIrisPosition->setValue(pos);
    d_data->m_ui->sbxIrisPosition->blockSignals( false );

    d_data->m_ui->sldIrisPosition->blockSignals( true );
    d_data->m_ui->sldIrisPosition->setValue(pos);
    d_data->m_ui->sldIrisPosition->blockSignals( false );
}

/******************************************************************************
 * LensDriverBox::onLensFocusInvertChange
 *****************************************************************************/
void LensDriverBox::onLensIrisInvertChange( bool en )
{
    d_data->m_ui->cbxIrisInvertEnable->blockSignals( true );
    d_data->m_ui->cbxIrisInvertEnable->setChecked( en);
    d_data->m_ui->cbxIrisInvertEnable->blockSignals( false );
}

/******************************************************************************
 * LensDriverBox::onLensSettingsChange
 *****************************************************************************/
void LensDriverBox::onLensIrisSetupChange( QVector<int> values )
{
    d_data->m_LensIrisTable.fStops.clear();
    d_data->m_LensIrisTable.fStopsPos.clear();

    for(int i = 0; i < 9; i++)
    {
        d_data->m_LensIrisTable.fStops.append( values.value(i*2));
        d_data->m_LensIrisTable.fStopsPos.append( values.value( ( i*2 ) +1));
    }

    d_data->m_ui->cbxLensIrisAperture->blockSignals(true);
    d_data->m_ui->cbxLensIrisAperture->setInsertPolicy( d_data->m_ui->cbxLensIrisAperture->InsertAtTop );
    d_data->m_ui->cbxLensIrisAperture->clear();
    d_data->m_ui->cbxLensIrisAperture->blockSignals(false);

    addLensIrisAperture( "Chose Apeture",0);

    QString temp;
    QVector<double> tempFStops;
    for( int i = 0; i < d_data->m_LensIrisTable.fStops.size() ; i++ )
    {
        if( d_data->m_LensIrisTable.fStops.value(i) != 0)
        {
            temp = QString("%1").arg( double(d_data->m_LensIrisTable.fStops.value(i)) /10 );


            addLensIrisAperture( temp ,i+1);
        }
    }
    for( int i = 0; i < d_data->m_LensIrisTable.fStops.size(); i++ )
    {
        tempFStops.append( double( d_data->m_LensIrisTable.fStops.value(i) ) / 10);
    }

    d_data->fillTable(tempFStops,d_data->m_LensIrisTable.fStopsPos);

}



/******************************************************************************
 * LensDriverBox::onLensIrisAperturePosChange
 *****************************************************************************/
void LensDriverBox::onLensIrisAperturePosChange( int pos )
{
   if(d_data->m_LensIrisTable.fStopsPos.contains( pos ) )
   {
       for(int  i = 0; i < d_data->m_LensIrisTable.fStopsPos.size(); i++)
       {
           if( ( pos == d_data->m_LensIrisTable.fStopsPos.value( i ) ) && ( d_data->m_LensIrisTable.fStops.value( i ) != 0 ) )
           {
               d_data->m_ui->cbxLensIrisAperture->blockSignals(true);
               d_data->m_ui->cbxLensIrisAperture->setCurrentIndex(i + 1);
               d_data->m_ui->cbxLensIrisAperture->blockSignals(false);
           }
       }
   }
   else
   {
        d_data->m_ui->cbxLensIrisAperture->blockSignals(true);
        d_data->m_ui->cbxLensIrisAperture->setCurrentIndex(0);
        d_data->m_ui->cbxLensIrisAperture->blockSignals(false);
   }

}


/******************************************************************************
 * LensDriverBox::onLensFilterPositionChange
 *****************************************************************************/
void LensDriverBox::onLensFilterPositionChange( int pos )
{
    d_data->m_ui->sbxFilterPosition->blockSignals( true );
    d_data->m_ui->sbxFilterPosition->setValue(pos);
    d_data->m_ui->sbxFilterPosition->blockSignals( false );

    d_data->m_ui->sldFilterPosition->blockSignals( true );
    d_data->m_ui->sldFilterPosition->setValue(pos);
    d_data->m_ui->sldFilterPosition->blockSignals( false );
}

/******************************************************************************
 * LensDriverBox::onLensFilterInvertChange
 *****************************************************************************/
void LensDriverBox::onLensFilterInvertChange( bool en )
{
    d_data->m_ui->cbxFilterInvertEnable->blockSignals( true );
    d_data->m_ui->cbxFilterInvertEnable->setChecked( en);
    d_data->m_ui->cbxFilterInvertEnable->blockSignals( false );
}

/******************************************************************************
 * LensDriverBox::onLensFocusSettingsChange
 *****************************************************************************/
void LensDriverBox::onLensFocusSettingsChange( QVector<int> values  )
{
    d_data->m_ui->sbxFocusSpeed->blockSignals( true );
    d_data->m_ui->sbxFocusSpeed->setValue(values[0]);
    d_data->m_ui->sbxFocusSpeed->blockSignals( false );

    d_data->m_ui->sbxFocusStepMode->blockSignals( true );
    d_data->m_ui->sbxFocusStepMode->setValue(values[1]);
    d_data->m_ui->sbxFocusStepMode->blockSignals( false );

    d_data->m_ui->sbxFocusTorque->blockSignals( true );
    d_data->m_ui->sbxFocusTorque->setValue(values[2]);
    d_data->m_ui->sbxFocusTorque->blockSignals( false );

}

/******************************************************************************
 * LensDriverBox::onLensZoomSettingsChange
 *****************************************************************************/
void LensDriverBox::onLensZoomSettingsChange( QVector<int> values )
{
    d_data->m_ui->sbxZoomSpeed->blockSignals( true );
    d_data->m_ui->sbxZoomSpeed->setValue(values[0]);
    d_data->m_ui->sbxZoomSpeed->blockSignals( false );

    d_data->m_ui->sbxZoomStepMode->blockSignals( true );
    d_data->m_ui->sbxZoomStepMode->setValue(values[1]);
    d_data->m_ui->sbxZoomStepMode->blockSignals( false );

    d_data->m_ui->sbxZoomTorque->blockSignals( true );
    d_data->m_ui->sbxZoomTorque->setValue(values[2]);
    d_data->m_ui->sbxZoomTorque->blockSignals( false );

}

/******************************************************************************
 * LensDriverBox::onLensIrisSettingsChange
 *****************************************************************************/
void LensDriverBox::onLensIrisSettingsChange( QVector<int> values  )
{
    d_data->m_ui->sbxIrisSpeed->blockSignals( true );
    d_data->m_ui->sbxIrisSpeed->setValue(values[0]);
    d_data->m_ui->sbxIrisSpeed->blockSignals( false );

    d_data->m_ui->sbxIrisStepMode->blockSignals( true );
    d_data->m_ui->sbxIrisStepMode->setValue(values[1]);
    d_data->m_ui->sbxIrisStepMode->blockSignals( false );

    d_data->m_ui->sbxIrisTorque->blockSignals( true );
    d_data->m_ui->sbxIrisTorque->setValue(values[2]);
    d_data->m_ui->sbxIrisTorque->blockSignals( false );

}

/******************************************************************************
 * LensDriverBox::onLensFilterSettingsChange
 *****************************************************************************/
void LensDriverBox::onLensFilterSettingsChange(  QVector<int> values  )
{
    d_data->m_ui->sbxFilterSpeed->blockSignals( true );
    d_data->m_ui->sbxFilterSpeed->setValue(values[0]);
    d_data->m_ui->sbxFilterSpeed->blockSignals( false );

    d_data->m_ui->sbxFilterStepMode->blockSignals( true );
    d_data->m_ui->sbxFilterStepMode->setValue(values[1]);
    d_data->m_ui->sbxFilterStepMode->blockSignals( false );

    d_data->m_ui->sbxFilterTorque->blockSignals( true );
    d_data->m_ui->sbxFilterTorque->setValue(values[2]);
    d_data->m_ui->sbxFilterTorque->blockSignals( false );

}




/******************************************************************************
 * LensDriverBox::onCbxLensSettingsChange
 *****************************************************************************/
void LensDriverBox::onCbxLensProfileChange( int index )
{

    index  = d_data->GetLensProfileByName(d_data->m_ui->cbxLensProfile->itemText(index) );
    if (index != LensProfileUnknown)
    {

        QVector<int> values;

        lens_settings_t settings = profileToSettings(static_cast<enum LensProfile>(index));

        values.append(settings.address);
        values.append(settings.chipID);
        values.append(settings.controllerFeatures);
        values.append(settings.focusMotorNr);
        values.append(settings.zoomMotorNr);
        values.append(settings.irisMotorNr);
        values.append(settings.filterMotorNr);
        values.append(settings.focusMotorFeatures);
        values.append(settings.zoomMotorFeatures);
        values.append(settings.irisMotorFeatures);
        values.append(settings.filterMotorFeatures);

        d_data->m_LensSettings = settings;
        setWaitCursor();
        showLensProfilBoxes(static_cast<enum LensProfile>(index));
        emit LensSettingsChanged( values );
        emit LensSettingsChanged( false );

        setNormalCursor();
    }
}

/******************************************************************************
 * LensDriverBox::onBtnLensActiveChange
 *****************************************************************************/
void LensDriverBox::onBtnLensActiveChange( void )
{
        d_data->m_ui->btnActivateLens->clearFocus();
        setWaitCursor();
        emit LensActiveChanged( true );
        setNormalCursor();
}

/******************************************************************************
 * LensDriverBox::onCbxLensActivateAdvancedSettings
 *****************************************************************************/
void LensDriverBox::onCbxLensEnableAdvancedSettings(int state)
{
    bool check = static_cast<bool>(state);
    d_data->m_ui->lblFocusSpeed->setVisible(check);
    d_data->m_ui->lblFocusStepMode->setVisible(check);
    d_data->m_ui->lblFocusTorque->setVisible(check);

    d_data->m_ui->sbxFocusSpeed->setVisible(check);
    d_data->m_ui->sbxFocusStepMode->setVisible(check);
    d_data->m_ui->sbxFocusTorque->setVisible(check);

    d_data->m_ui->lblZoomSpeed->setVisible(check);
    d_data->m_ui->lblZoomStepMode->setVisible(check);
    d_data->m_ui->lblZoomTorque->setVisible(check);

    d_data->m_ui->sbxZoomSpeed->setVisible(check);
    d_data->m_ui->sbxZoomStepMode->setVisible(check);
    d_data->m_ui->sbxZoomTorque->setVisible(check);

    d_data->m_ui->lblIrisSpeed->setVisible(check);
    d_data->m_ui->lblIrisStepMode->setVisible(check);
    d_data->m_ui->lblIrisTorque->setVisible(check);

    d_data->m_ui->sbxIrisSpeed->setVisible(check);
    d_data->m_ui->sbxIrisStepMode->setVisible(check);
    d_data->m_ui->sbxIrisTorque->setVisible(check);

    d_data->m_ui->lblFilterSpeed->setVisible(check);
    d_data->m_ui->lblFilterStepMode->setVisible(check);
    d_data->m_ui->lblFilterTorque->setVisible(check);

    d_data->m_ui->sbxFilterSpeed->setVisible(check);
    d_data->m_ui->sbxFilterStepMode->setVisible(check);
    d_data->m_ui->sbxFilterTorque->setVisible(check);

    d_data->m_ui->cbxFocusFineEnable->setVisible(check);
    d_data->m_ui->cbxFocusInvertEnable->setVisible(check);
    d_data->m_ui->cbxZoomInvertEnable->setVisible(check);
    d_data->m_ui->cbxIrisInvertEnable->setVisible(check);
    d_data->m_ui->cbxFilterInvertEnable->setVisible(check);

    d_data->m_ui->lblIrisFStopTable->setVisible(check);
    d_data->m_ui->tblIrisAptPosition->setVisible(check);
    d_data->m_ui->cbxIrisTableTemplate->setVisible(check);
    d_data->m_ui->lblIrisTableUseTemplate->setVisible(check);
    d_data->m_ui->btnIrisTransmitTable->setVisible(check);
}

/******************************************************************************
 * LensDriverBox::onSbxLensFocusPositionChanged
 *****************************************************************************/
void LensDriverBox::onSbxLensFocusPositionChanged( int pos )
{

        setWaitCursor();
        emit LensFocusPositionChanged( pos );
        setNormalCursor();
}

/******************************************************************************
 * LensDriverBox::onSbxLensFocusSpeedChanged
 *****************************************************************************/
void LensDriverBox::onSbxLensFocusSpeedChanged( int speed )
{
        QVector<int> values;

        values.append(speed);
        values.append(d_data->m_ui->sbxFocusStepMode->value());
        values.append(d_data->m_ui->sbxFocusTorque->value());

        setWaitCursor();
        emit LensFocusSettingsChanged( values );
        setNormalCursor();
}

/******************************************************************************
 * LensDriverBox::onSbxLensFocusStepModeChanged
 *****************************************************************************/
void LensDriverBox::onSbxLensFocusStepModeChanged( int stepMode )
{
        QVector<int> values;

        values.append(d_data->m_ui->sbxFocusSpeed->value());
        values.append(stepMode);
        values.append(d_data->m_ui->sbxFocusTorque->value());

        setWaitCursor();
        emit LensFocusSettingsChanged( values );
        setNormalCursor();
}

/******************************************************************************
 * LensDriverBox::onSbxLensFocusTorqueChanged
 *****************************************************************************/
void LensDriverBox::onSbxLensFocusTorqueChanged( int torque )
{
        QVector<int> values;

        values.append(d_data->m_ui->sbxFocusSpeed->value());
        values.append(d_data->m_ui->sbxFocusStepMode->value());
        values.append(torque);

        setWaitCursor();
        emit LensFocusSettingsChanged( values );
        setNormalCursor();
}


/******************************************************************************
 * LensDriverBox::onCbxLensFocusFineChanged
 *****************************************************************************/
void LensDriverBox::onCbxLensFocusFineChanged( int enable)
{
    setWaitCursor();
    if( static_cast<bool>(enable) == true )
    {

        d_data->m_ui->sbxFocusPosition->setRange(0,1000);
        d_data->m_ui->sldFocusPosition->setRange(0,1000);

    }
    else
    {

        d_data->m_ui->sbxFocusPosition->setValue(d_data->m_ui->sbxFocusPosition->value() /10 );
        d_data->m_ui->sbxFocusPosition->setRange(0,100);
        d_data->m_ui->sldFocusPosition->setRange(0,100);

    }


    emit LensFocusFineChanged( static_cast<bool>(enable) );
    setNormalCursor();
}

/******************************************************************************
 * LensDriverBox::onCbxLensFocusInvertChanged
 *****************************************************************************/
void LensDriverBox::onCbxLensFocusInvertChanged( int enable)
{
    QVector<int> values;

    values.append(static_cast<int>( static_cast<bool>( d_data->m_ui->cbxFilterInvertEnable->isChecked() ) ) );
    values.append(static_cast<int>( static_cast<bool>( d_data->m_ui->cbxIrisInvertEnable->isChecked() ) ) );
    values.append(static_cast<int>( static_cast<bool>( d_data->m_ui->cbxZoomInvertEnable->isChecked() ) ) );
    values.append(static_cast<int>( static_cast<bool>( enable ) ) );

    setWaitCursor();
    emit LensInvertChanged( values );
    setNormalCursor();
}


/******************************************************************************
 * LensDriverBox::onSbxLensZoomPositionChanged
 *****************************************************************************/
void LensDriverBox::onSbxLensZoomPositionChanged( int pos )
{

        setWaitCursor();
        emit LensZoomPositionChanged( pos );
        setNormalCursor();
}

/******************************************************************************
 * LensDriverBox::onSbxLensZoomSpeedChanged
 *****************************************************************************/
void LensDriverBox::onSbxLensZoomSpeedChanged( int speed )
{
        QVector<int> values;

        values.append(speed);
        values.append(d_data->m_ui->sbxZoomStepMode->value());
        values.append(d_data->m_ui->sbxZoomTorque->value());

        setWaitCursor();
        emit LensZoomSettingsChanged( values );
        setNormalCursor();
}

/******************************************************************************
 * LensDriverBox::onSbxLensZoomStepModeChanged
 *****************************************************************************/
void LensDriverBox::onSbxLensZoomStepModeChanged( int stepMode )
{
        QVector<int> values;

        values.append(d_data->m_ui->sbxZoomSpeed->value());
        values.append(stepMode);
        values.append(d_data->m_ui->sbxZoomTorque->value());

        setWaitCursor();
        emit LensZoomSettingsChanged( values );
        setNormalCursor();
}

/******************************************************************************
 * LensDriverBox::onSbxLensZoomTorqueChanged
 *****************************************************************************/
void LensDriverBox::onSbxLensZoomTorqueChanged( int torque )
{
        QVector<int> values;

        values.append(d_data->m_ui->sbxZoomSpeed->value());
        values.append(d_data->m_ui->sbxZoomStepMode->value());
        values.append(torque);

        setWaitCursor();
        emit LensZoomSettingsChanged( values );
        setNormalCursor();
}

/******************************************************************************
 * LensDriverBox::onCbxLensZoomInvertChanged
 *****************************************************************************/
void LensDriverBox::onCbxLensZoomInvertChanged( int enable)
{
    QVector<int> values;

    values.append(static_cast<int>( static_cast<bool>( d_data->m_ui->cbxFilterInvertEnable->isChecked() ) ) );
    values.append(static_cast<int>( static_cast<bool>( d_data->m_ui->cbxIrisInvertEnable->isChecked() ) ) );
    values.append(static_cast<int>( static_cast<bool>( enable ) ) );
    values.append(static_cast<int>( static_cast<bool>( d_data->m_ui->cbxFocusInvertEnable->isChecked() ) ) );

    setWaitCursor();
    emit LensInvertChanged( values );
    setNormalCursor();
}


/******************************************************************************
 * LensDriverBox::onSbxLensIrisPositionChanged
 *****************************************************************************/
void LensDriverBox::onSbxLensIrisPositionChanged( int pos )
{

        setWaitCursor();
        emit LensIrisPositionChanged( pos );
        emit LensIrisAperturePosChanged( pos );
        setNormalCursor();
}

/******************************************************************************
 * LensDriverBox::onSbxLensIrisSpeedChanged
 *****************************************************************************/
void LensDriverBox::onSbxLensIrisSpeedChanged( int speed )
{
        QVector<int> values;

        values.append(speed);
        values.append(d_data->m_ui->sbxIrisStepMode->value());
        values.append(d_data->m_ui->sbxIrisTorque->value());

        setWaitCursor();
        emit LensIrisSettingsChanged( values );
        setNormalCursor();
}

/******************************************************************************
 * LensDriverBox::onSbxLensIrisStepModeChanged
 *****************************************************************************/
void LensDriverBox::onSbxLensIrisStepModeChanged( int stepMode )
{
        QVector<int> values;

        values.append(d_data->m_ui->sbxIrisSpeed->value());
        values.append(stepMode);
        values.append(d_data->m_ui->sbxIrisTorque->value());

        setWaitCursor();
        emit LensIrisSettingsChanged( values );
        setNormalCursor();
}

/******************************************************************************
 * LensDriverBox::onSbxLensIrisTorqueChanged
 *****************************************************************************/
void LensDriverBox::onSbxLensIrisTorqueChanged( int torque )
{
        QVector<int> values;

        values.append(d_data->m_ui->sbxIrisSpeed->value());
        values.append(d_data->m_ui->sbxIrisStepMode->value());
        values.append(torque);

        setWaitCursor();
        emit LensIrisSettingsChanged( values );
        setNormalCursor();
}

/******************************************************************************
 * LensDriverBox::onCbxLensIrisInvertChanged
 *****************************************************************************/
void LensDriverBox::onCbxLensIrisInvertChanged( int enable)
{
    QVector<int> values;

    values.append(static_cast<int>( static_cast<bool>( d_data->m_ui->cbxFilterInvertEnable->isChecked() ) ) );
    values.append(static_cast<int>( static_cast<bool>( enable ) ) );
    values.append(static_cast<int>( static_cast<bool>( d_data->m_ui->cbxZoomInvertEnable->isChecked() ) ) );
    values.append(static_cast<int>( static_cast<bool>( d_data->m_ui->cbxFocusInvertEnable->isChecked() ) ) );

    setWaitCursor();
    emit LensInvertChanged( values );
    setNormalCursor();
}


/******************************************************************************
 * LensDriverBox::onCbxLensIrisApertureChanged
 *****************************************************************************/
void LensDriverBox::onCbxLensIrisApertureChanged( int index)
{

    if(index != 0)
    {
        setWaitCursor();
        emit LensIrisApertureChanged( d_data->m_LensIrisTable.fStops.value( ( index - 1 ) ) );
        emit LensIrisAperturePosChanged( d_data->m_LensIrisTable.fStopsPos.value( ( index - 1 ) ) );
        setNormalCursor();
    }
}

/******************************************************************************
 * LensDriverBox::onBtnLensIrisAperturePlusChanged
 *****************************************************************************/
void LensDriverBox::onBtnLensIrisAperturePlusChanged( void )
{

    d_data->m_ui->btnIrisAptPlus->clearFocus();
    int acPos = d_data->m_ui->sbxIrisPosition->value();
    int i = 0;
    int index = -1;
    while( (  i  < d_data->m_LensIrisTable.fStops.size()  ) && ( acPos < d_data->m_LensIrisTable.fStopsPos.value(i)))
    {
        index = i;
        i++;
    }



    if( ( index >= 0 ) && ( d_data->m_LensIrisTable.fStops.value(index ) != 0 ) )
    {

        setWaitCursor();
        emit LensIrisApertureChanged( d_data->m_LensIrisTable.fStops.value( ( index  ) ) );
        emit LensIrisAperturePosChanged( d_data->m_LensIrisTable.fStopsPos.value( ( index  ) ) );
        setNormalCursor();
    }

}



/******************************************************************************
 * LensDriverBox::onBtnLensIrisApertureMinusChanged
 *****************************************************************************/
void LensDriverBox::onBtnLensIrisApertureMinusChanged( void )
{

    d_data->m_ui->btnIrisAptMinus->clearFocus();
    int acPos = d_data->m_ui->sbxIrisPosition->value();
    int i = 0;
    int index = -1;
    while( (  i  < d_data->m_LensIrisTable.fStops.size()  ) && ( acPos <= d_data->m_LensIrisTable.fStopsPos.value(i)))
    {
        index = i;
        i++;
    }



    if( ( index >= -1 ) && ( d_data->m_LensIrisTable.fStops.value(index + 1) != 0 ) )
    {

        setWaitCursor();
        emit LensIrisApertureChanged( d_data->m_LensIrisTable.fStops.value( ( index + 1 ) ) );
        emit LensIrisAperturePosChanged( d_data->m_LensIrisTable.fStopsPos.value( ( index + 1 ) ) );
        setNormalCursor();
    }

}

/******************************************************************************
 * LensDriverBox::onBtnLensIrisTableTransmitChanged
 *****************************************************************************/
void LensDriverBox::onBtnLensIrisTableTransmitChanged( void )
{

    d_data->m_ui->btnIrisTransmitTable ->clearFocus();

    QVector<double> tempFstop;
    QVector<int> tempIntFstop;
    QVector<int> tempFstopPos;
    QVector<int> tempIrisSettings;

   d_data->getDataFromModel(tempFstop,tempFstopPos);
   // Sort
   for(int i = 0; i< tempFstop.size(); i++)
   {
       tempIntFstop.append( int( tempFstop.value(i) * 10 ) );
   }


   pairSort(tempIntFstop,tempFstopPos);

   for(int i = 0; i< tempFstop.size(); i++)
   {
       tempIrisSettings.append(tempIntFstop.value(i));
       tempIrisSettings.append(tempFstopPos.value(i));
   }

   emit LensIrisSetupChanged(tempIrisSettings);
}

/******************************************************************************
 * LensDriverBox::onCbxLensIrisTemplateChanged
 *****************************************************************************/
void LensDriverBox::onCbxLensIrisTemplateChanged( int index )
{
    int i = 0;
    QVector<double> tempFStops;
    QVector<int> tempFStopPos;

    while( d_data->m_LensIrisTemplates.value(i).lensName.compare( d_data->m_ui->cbxIrisTableTemplate->itemText(index)) != 0 && i < d_data->m_LensIrisTemplates.length() )
    {
        i++;
    }

    if( i < d_data->m_LensIrisTemplates.length() )
    {

        for( int a = 0; a < d_data->m_LensIrisTemplates.value(i).fStops.length(); a++ )
        {
            tempFStops.append( double( d_data->m_LensIrisTemplates.value(i).fStops.value(a) ) / 10);
            tempFStopPos.append( d_data->m_LensIrisTemplates.value(i).fStopPos.value(a) );
        }


        d_data->fillTable(tempFStops,tempFStopPos);
    }

}



/******************************************************************************
 * LensDriverBox::onSbxLensFilterPositionChanged
 *****************************************************************************/
void LensDriverBox::onSbxLensFilterPositionChanged( int pos )
{

        setWaitCursor();
        emit LensFilterPositionChanged( pos );
        setNormalCursor();
}

/******************************************************************************
 * LensDriverBox::onSbxLensFilterSpeedChanged
 *****************************************************************************/
void LensDriverBox::onSbxLensFilterSpeedChanged( int speed )
{
        QVector<int> values;

        values.append(speed);
        values.append(d_data->m_ui->sbxFilterStepMode->value());
        values.append(d_data->m_ui->sbxFilterTorque->value());

        setWaitCursor();
        emit LensFilterSettingsChanged( values );
        setNormalCursor();
}

/******************************************************************************
 * LensDriverBox::onSbxLensFilterStepModeChanged
 *****************************************************************************/
void LensDriverBox::onSbxLensFilterStepModeChanged( int stepMode )
{
        QVector<int> values;

        values.append(d_data->m_ui->sbxFilterSpeed->value());
        values.append(stepMode);
        values.append(d_data->m_ui->sbxFilterTorque->value());

        setWaitCursor();
        emit LensFilterSettingsChanged( values );
        setNormalCursor();
}

/******************************************************************************
 * LensDriverBox::onSbxLensFilterTorqueChanged
 *****************************************************************************/
void LensDriverBox::onSbxLensFilterTorqueChanged( int torque )
{
        QVector<int> values;

        values.append(d_data->m_ui->sbxFilterSpeed->value());
        values.append(d_data->m_ui->sbxFilterStepMode->value());
        values.append(torque);

        setWaitCursor();
        emit LensFilterSettingsChanged( values );
        setNormalCursor();
}

/******************************************************************************
 * LensDriverBox::onCbxLensFilterInvertChanged
 *****************************************************************************/
void LensDriverBox::onCbxLensFilterInvertChanged( int enable)
{
    QVector<int> values;

    values.append(static_cast<int>( static_cast<bool>( enable ) ) );
    values.append(static_cast<int>( static_cast<bool>( d_data->m_ui->cbxIrisInvertEnable->isChecked() ) ) );
    values.append(static_cast<int>( static_cast<bool>( d_data->m_ui->cbxZoomInvertEnable->isChecked() ) ) );
    values.append(static_cast<int>( static_cast<bool>( d_data->m_ui->cbxFocusInvertEnable->isChecked() ) ) );

    setWaitCursor();
    emit LensInvertChanged( values );
    setNormalCursor();
}

