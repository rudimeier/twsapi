/* Copyright (C) 2013 Interactive Brokers LLC. All rights reserved.  This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

package TestJavaClient;

import java.awt.BorderLayout;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;

import com.ib.client.ExecutionFilter;

public class ExecFilterDlg extends JDialog {

	public int m_reqId;
    public ExecutionFilter	m_execFilter = new ExecutionFilter();
    public boolean 		m_rc;

    private JTextField 	m_reqIdTxt = new JTextField("0");
    private JTextField 	m_clientID = new JTextField("0");
    private JTextField 	m_acctCode = new JTextField();
    private JTextField 	m_time = new JTextField();
    private JTextField 	m_symbol = new JTextField();
    private JTextField 	m_secType = new JTextField();
    private JTextField 	m_exchange = new JTextField();
    private JTextField 	m_action = new JTextField();

    private JButton 	m_ok = new JButton( "OK");
    private JButton 	m_cancel = new JButton( "Cancel");

    public ExecFilterDlg( JFrame owner) {
        super( owner, true);

        setTitle( "Execution Report Filter");

        // create extended order attributes panel
        JPanel execRptFilterPanel = new JPanel( new GridLayout( 0, 2, 7, 7) );
        execRptFilterPanel.setBorder( BorderFactory.createTitledBorder( "Filter Criteria") );
        execRptFilterPanel.add( new JLabel( "Request ID:") );
        execRptFilterPanel.add( m_reqIdTxt);
        execRptFilterPanel.add( new JLabel( "Client ID:") );
        execRptFilterPanel.add( m_clientID);
        execRptFilterPanel.add( new JLabel( "Account Code:") );
        execRptFilterPanel.add( m_acctCode);
        execRptFilterPanel.add( new JLabel( "Time :") );
        execRptFilterPanel.add( m_time);
        execRptFilterPanel.add( new JLabel( "Symbol :") );
        execRptFilterPanel.add( m_symbol);
        execRptFilterPanel.add( new JLabel( "SecType :") );
        execRptFilterPanel.add( m_secType);
        execRptFilterPanel.add( new JLabel( "Exchange :") );
        execRptFilterPanel.add( m_exchange);
        execRptFilterPanel.add( new JLabel( "Action :") );
        execRptFilterPanel.add( m_action);

        // create button panel
        JPanel buttonPanel = new JPanel();
        buttonPanel.add( m_ok);
        buttonPanel.add( m_cancel);

        // create action listeners
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

        // create dlg box
        getContentPane().add( execRptFilterPanel, BorderLayout.CENTER);
        getContentPane().add( buttonPanel, BorderLayout.SOUTH);
        pack();
    }

    void onOk() {
        m_rc = false;

        try {

            m_reqId = Integer.parseInt( m_reqIdTxt.getText());

            // set extended order fields
            String clientId = m_clientID.getText().toString();
            if(clientId.equals("")){
            	m_execFilter.m_clientId = 0;
            }else{
            	m_execFilter.m_clientId = Integer.parseInt(clientId);
            }
            m_execFilter.m_acctCode = m_acctCode.getText();
            m_execFilter.m_time = m_time.getText();
            m_execFilter.m_symbol = m_symbol.getText();
            m_execFilter.m_secType = m_secType.getText();
            m_execFilter.m_exchange = m_exchange.getText();
            m_execFilter.m_side = m_action.getText();
        }
        catch( Exception e) {
            Main.inform( this, "Error - " + e);
            return;
        }

        m_rc = true;
        setVisible( false);
    }

    void onCancel() {
        m_rc = false;
        setVisible( false);
    }
}
