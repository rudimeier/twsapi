/* Copyright (C) 2013 Interactive Brokers LLC. All rights reserved.  This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

package TestJavaClient;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.WindowAdapter;
import java.awt.event.WindowEvent;
import java.util.LinkedList;
import java.util.ListIterator;

import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JSplitPane;
import javax.swing.JTable;
import javax.swing.table.AbstractTableModel;

import com.ib.client.EClientSocket;

public class MktDepthDlg extends JDialog {
    final static int OPERATION_INSERT 		= 0;
    final static int OPERATION_UPDATE 		= 1;
    final static int OPERATION_DELETE 		= 2;

    final static int SIDE_ASK = 0;
    final static int SIDE_BID = 1;
    final static int MKT_DEPTH_DATA_RESET = 317;

    private JButton 		m_close = new JButton( "Close");
    private MktDepthModel 	m_bidModel = new MktDepthModel();
    private MktDepthModel 	m_askModel = new MktDepthModel();
    private EClientSocket 	m_client;
    private int			  	m_id;

    public MktDepthDlg(String title, JFrame parent) {
        super(parent, title, false);

        JScrollPane bidPane = new JScrollPane(new JTable(m_bidModel));
        JScrollPane askPane = new JScrollPane(new JTable(m_askModel));

        bidPane.setBorder(BorderFactory.createTitledBorder( "Bid") );
        askPane.setBorder(BorderFactory.createTitledBorder( "Ask") );

        JSplitPane splitPane = new JSplitPane(JSplitPane.HORIZONTAL_SPLIT, bidPane, askPane);
        splitPane.setOneTouchExpandable(true);
        splitPane.setDividerLocation(300);
        splitPane.setPreferredSize(new Dimension(600, 380));

        JPanel closePanel = new JPanel();
        closePanel.add(m_close);
        m_close.addActionListener( new ActionListener() {
            public void actionPerformed( ActionEvent e) {
                onClose();
            }
        });

        this.addWindowListener( new WindowAdapter() {
            public void windowClosing(WindowEvent e) {
                onClose();
            }
        });


        getContentPane().add( splitPane, BorderLayout.CENTER);
        getContentPane().add( closePanel, BorderLayout.SOUTH);
        setLocation(20, 20);
        pack();
        reset();
    }

    void setParams( EClientSocket client, int id) {
        m_client = client;
        m_id = id;
        reset();
    }

    void updateMktDepth( int tickerId, int position, String marketMaker,
        int operation, int side, double price, int size) {
        try {
            MktDepthModel.MktDepthTableRow tmpRow = null;

            if (operation == OPERATION_INSERT )
            {
                    if ( side == SIDE_BID ) {
                            m_bidModel.addOrderAt(position, marketMaker, price, size);
                    }
                    else {
                            m_askModel.addOrderAt(position, marketMaker, price, size);
                    }
            }
            else if (operation == OPERATION_UPDATE )
            {
                    if ( side == SIDE_BID ) {
                            tmpRow = m_bidModel.getOrderAt(position);
                            if ( tmpRow != null ) {
                                    tmpRow.m_marketMaker = marketMaker;
                                    tmpRow.m_price = price;
                                    tmpRow.m_size = size;
                            }
                            m_bidModel.fireTableRowsUpdated(position, position);
                    }
                    else {
                            tmpRow = m_askModel.getOrderAt(position);
                            if ( tmpRow != null ) {
                                    tmpRow.m_marketMaker = marketMaker;
                                    tmpRow.m_price = price;
                                    tmpRow.m_size = size;
                            }
                            m_askModel.fireTableRowsUpdated(position, position);
                    }

            }
            else if  (operation == OPERATION_DELETE)
            {
                    if ( side == SIDE_BID ) {
                            m_bidModel.removeOrderAt(position);
                    }
                    else {
                            m_askModel.removeOrderAt(position);
                    }
            }

            if ( side == SIDE_BID ) {
                    m_bidModel.updateCumSizesAndAvgPrices(position);
            }
            else {
                    m_askModel.updateCumSizesAndAvgPrices(position);
            }
        }
        catch( Exception e) {
                System.out.println("Exception: " + e.getMessage());
        }
    }

    void reset() {
        m_bidModel.reset();
        m_askModel.reset();
    }

    void onClose() {
		m_client.cancelMktDepth( m_id );
        setVisible( false);
    }
}

class MktDepthModel extends AbstractTableModel {
    private LinkedList<MktDepthTableRow>  m_allData = new LinkedList<MktDepthTableRow>();

    synchronized public void addOrderAt(int position, String marketMaker, double price, int size)
    {
        MktDepthTableRow newData = new MktDepthTableRow(marketMaker, price, size);
        m_allData.add(position, newData);
        fireTableRowsInserted(position, position);
    }

    synchronized public void removeOrderAt(int position)
    {
        m_allData.remove(position);
        fireTableRowsDeleted(position, position);
    }

    synchronized public MktDepthTableRow getOrderAt(int orderPosition) {
        return getIteratorAt(orderPosition).next();
    }

    synchronized public ListIterator<MktDepthTableRow> getIteratorAt(int orderPosition) {
        return m_allData.listIterator(orderPosition);
    }

    synchronized public void updateCumSizesAndAvgPrices(int baseRow)
    {
        int     cumSize = 0;
        double  totalPrice = 0.0;
        MktDepthTableRow	tmpRow = null;

        if (baseRow > 0) {
            tmpRow = m_allData.get(baseRow - 1);
            cumSize = tmpRow.m_cumSize;
            totalPrice = tmpRow.m_price * cumSize;
        }

        for (int ctr = baseRow ; ctr < m_allData.size() ; ctr++)
        {
            tmpRow = m_allData.get(ctr);
            cumSize += tmpRow.m_size;
            totalPrice += (tmpRow.m_price * tmpRow.m_size);
            tmpRow.m_cumSize = cumSize;
            tmpRow.m_avgPrice = (totalPrice / cumSize);
            fireTableCellUpdated(ctr, 3);
            fireTableCellUpdated(ctr, 4);
        }
    }

    synchronized public void reset() {
        m_allData.clear();
        fireTableDataChanged();
    }

    synchronized public int getRowCount() {
        return m_allData.size();
    }

    synchronized public int getColumnCount() {
        return 5;
    }

    synchronized public Object getValueAt(int r, int c) {
        if (r >= m_allData.size()) {
            return null;
        }
        return m_allData.get(r).getValue(c);
    }

    public boolean isCellEditable(int r, int c) {
        return false;
    }

    public String getColumnName(int c) {
        switch (c) {
            case 0:
                return "MM";
            case 1:
                return "Price";
            case 2:
                return "Size";
            case 3:
                return "Cum Size";
            case 4:
                return "Avg Price";
            default:
                return null;
        }
    }

    class MktDepthTableRow {
        public String 	m_marketMaker;
        public double 	m_price;
        public int 		m_size;
        public int 		m_cumSize;
        public double	m_avgPrice;

        MktDepthTableRow(String marketMaker, double price,int size) {
            m_marketMaker = marketMaker;
            m_price = price;
            m_size = size;
            m_cumSize = 0;
            m_avgPrice = 0.0;
        }

        Object getValue(int c) {
            switch (c)
            {
                case 0:
                        return m_marketMaker;
                case 1:
                        return "" + m_price;
                case 2:
                        return "" + m_size;
                case 3:
                        return "" + m_cumSize;
                case 4:
                        return "" + m_avgPrice;
                default:
                        return null;
            }
        }
    }
}
