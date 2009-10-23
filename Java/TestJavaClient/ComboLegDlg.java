/*
 *
 * ComboLegDlg.java
 *
 */
package TestJavaClient;

import java.awt.BorderLayout;
import java.awt.GridLayout;
import java.awt.Window;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.Vector;

import javax.swing.BorderFactory;
import javax.swing.BoxLayout;
import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTable;
import javax.swing.JTextField;
import javax.swing.table.AbstractTableModel;

import com.ib.client.ComboLeg;
import com.ib.client.Contract;

public class ComboLegDlg extends JDialog {
	
    //private static String 	BUY = "BUY";
    //private static String 	SELL = "SELL";
    //private static String 	SSHORT = "SSHORT";
	
	private Vector          m_comboLegs;

    private JTextField 		m_conId = new JTextField( "0");
    private JTextField 		m_ratio = new JTextField( "0");
    private JTextField 		m_action = new JTextField( "BUY");
    private JTextField 		m_exchange = new JTextField( "");
    private JTextField 		m_openClose = new JTextField( "0");
    private JTextField      m_shortSaleSlot = new JTextField( "0");
    private JTextField      m_designatedLocation = new JTextField( "");
    private JTextField      m_exemptCode = new JTextField( "-1");
    
    private JButton 		m_addLeg = new JButton( "Add");
    private JButton	 	    m_removeLeg = new JButton( "Remove");
    private JButton 		m_ok = new JButton( "OK");
    private JButton	 	    m_cancel = new JButton( "Cancel");
    
    private ComboLegModel 	m_comboLegsModel = new ComboLegModel();
    private JTable 		    m_comboTable = new JTable(m_comboLegsModel);
    private JScrollPane 	m_comboLegsPane = new JScrollPane(m_comboTable);
   
    public ComboLegModel comboLegModel() { return m_comboLegsModel; }

    public ComboLegDlg( Vector comboLegs, String orderExchange, JDialog owner) {
        super( owner, true);
        
        m_comboLegs = comboLegs;

        setTitle( "Combination Legs");

        // create combos list panel
        JPanel pLegList = new JPanel( new GridLayout( 0, 1, 10, 10) );
        pLegList.setBorder( BorderFactory.createTitledBorder( "Combination Order legs:") );
        m_comboLegsModel.comboLegModel().addAll(comboLegs);
        pLegList.add( m_comboLegsPane);
        
        if (orderExchange != null && orderExchange.length() > 0) {
        	m_exchange.setText(orderExchange);
        }

        // create combo details panel
        JPanel pComboDetails = new JPanel( new GridLayout( 0, 2, 10, 10) );
        pComboDetails.setBorder( BorderFactory.createTitledBorder( "Combo Leg Details:") );
        pComboDetails.add( new JLabel( "ConId:") );
        pComboDetails.add( m_conId);
        pComboDetails.add( new JLabel( "Ratio:") );
        pComboDetails.add( m_ratio);
        pComboDetails.add( new JLabel( "Side:") );
        pComboDetails.add( m_action);
        pComboDetails.add( new JLabel( "Exchange:") );
        pComboDetails.add( m_exchange);
        pComboDetails.add( new JLabel( "Open/Close:") );
        pComboDetails.add( m_openClose);
        pComboDetails.add( new JLabel( "Short Sale Slot:") );
        pComboDetails.add( m_shortSaleSlot);
        pComboDetails.add( new JLabel( "Designated Location:") );
        pComboDetails.add( m_designatedLocation);
        pComboDetails.add( new JLabel( "Exempt Code:") );
        pComboDetails.add( m_exemptCode);
        pComboDetails.add( m_addLeg);
        pComboDetails.add( m_removeLeg);

        // create button panel
        JPanel buttonPanel = new JPanel();
        buttonPanel.add( m_ok);
        buttonPanel.add( m_cancel);

        // create wrapper panel
        JPanel topPanel = new JPanel();
        topPanel.setLayout( new BoxLayout( topPanel, BoxLayout.Y_AXIS) );
        topPanel.add( pLegList);
        topPanel.add( pComboDetails);

        // create dlg box
        getContentPane().add( topPanel, BorderLayout.CENTER);
        getContentPane().add( buttonPanel, BorderLayout.SOUTH);

        // create action listeners
        m_addLeg.addActionListener( new ActionListener() {
            public void actionPerformed( ActionEvent e) {
                onAddLeg();
            }
        });
        m_removeLeg.addActionListener( new ActionListener() {
            public void actionPerformed( ActionEvent e) {
                onRemoveLeg();
            }
        });
        m_ok.addActionListener( new ActionListener() {
            public void actionPerformed( ActionEvent e) {
                onOk();
            }
        });
        m_cancel.addActionListener( new ActionListener() {
            public void actionPerformed( ActionEvent e) {
                onCancel();
            }
        });

        setSize(250, 600);
        centerOnOwner( this);
    }

     public void onAddLeg() {
        try {
            int conId = Integer.parseInt( m_conId.getText());
            int ratio = Integer.parseInt( m_ratio.getText());
            int openClose = Integer.parseInt( m_openClose.getText());
            int shortSaleSlot = Integer.parseInt(m_shortSaleSlot.getText());
            int exemptCode = Integer.parseInt(m_exemptCode.getText().length() != 0 ? m_exemptCode.getText() : "-1");
            m_comboLegsModel.addComboLeg( new ComboLeg(conId, ratio,
                            m_action.getText(), m_exchange.getText(), openClose,
                            shortSaleSlot, m_designatedLocation.getText(), exemptCode) );
        }
        catch( Exception e) {
            reportError( "Error - ", e);
            return;
        }
    }

    public void onRemoveLeg() {
        try {
            if ( m_comboTable.getSelectedRowCount() != 0 ) {
                int[] rows = m_comboTable.getSelectedRows();
                for ( int i=rows.length -1; i>=0 ; i-- ) {
                        m_comboLegsModel.removeComboLeg( rows[i]);
                }
            }
        }
        catch( Exception e) {
            reportError( "Error - ", e);
            return;
        }
    }

    void onOk() {
    	m_comboLegs.clear();
        m_comboLegs.addAll( m_comboLegsModel.comboLegModel());
        setVisible( false);
    }

    void onCancel() {
        setVisible( false);
    }


    void reportError( String msg, Exception e) {
        Main.inform( this, msg + " --" + e);
    }

    private void centerOnOwner( Window window) {
        Window owner = window.getOwner();
        if( owner == null) {
            return;
        }
        int x = owner.getX() + ((owner.getWidth()  - window.getWidth())  / 2);
        int y = owner.getY() + ((owner.getHeight() - window.getHeight()) / 2);
        if( x < 0) x = 0;
        if( y < 0) y = 0;
        window.setLocation( x, y);
    }
}

class ComboLegModel extends AbstractTableModel {
    private Vector  m_allData = new Vector();

    synchronized public void addComboLeg( ComboLeg leg)
    {
        m_allData.add( leg);
        fireTableDataChanged();
    }

    synchronized public void removeComboLeg( int index)
    {
        m_allData.remove(index);
        fireTableDataChanged();
    }

    synchronized public void removeComboLeg( ComboLeg leg)
    {
        for ( int i=0; i < m_allData.size(); i++ ) {
                if ( leg.equals( (ComboLeg)m_allData.get(i)) ) {
                        m_allData.remove(i);
                        break;
                }
        }
        fireTableDataChanged();
    }

    synchronized public void reset() {
        m_allData.removeAllElements();
		fireTableDataChanged();
    }

    synchronized public int getRowCount() {
        return m_allData.size();
    }

    synchronized public int getColumnCount() {
        return 8;
    }

    synchronized public Object getValueAt(int r, int c) {
        ComboLeg leg = (ComboLeg)m_allData.get(r);

        switch (c) {
            case 0:
                return Integer.toString(leg.m_conId);
            case 1:
                return Integer.toString(leg.m_ratio);
            case 2:
                return leg.m_action;
            case 3:
                return leg.m_exchange;
            case 4:
                return Integer.toString(leg.m_openClose);
            case 5:
            	return Integer.toString(leg.m_shortSaleSlot);
            case 6:
            	return leg.m_designatedLocation;
            case 7:
            	return Integer.toString(leg.m_exemptCode);
            default:
                return "";
        }

    }

    public boolean isCellEditable(int r, int c) {
        return false;
    }

    public String getColumnName(int c) {
        switch (c) {
            case 0:
                return "ConId";
            case 1:
                return "Ratio";
            case 2:
                return "Side";
            case 3:
                return "Exchange";
            case 4:
                return "Open/Close";
            case 5:
            	return "Short Sale Slot";
            case 6:
            	return "Designated Location";
            case 7:
            	return "Exempt Code";
            default:
                return null;
        }
    }

    public Vector comboLegModel() {
        return m_allData;
    }
}
