/* Copyright (C) 2013 Interactive Brokers LLC. All rights reserved.  This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

package TestJavaClient;

import java.awt.BorderLayout;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;


public class AccountSummary extends JDialog {

    public boolean m_rc;

    public int m_reqId;
    public String m_groupName;
    public String m_tags;

    private JTextField 	m_reqIdTxt = new JTextField("0");
    private JTextField 	m_groupNameTxt = new JTextField("All");
    private JTextField 	m_tagsTxt = new JTextField("AccruedCash,BuyingPower,NetLiquidation");

    private JButton 	m_ok = new JButton( "OK");
    private JButton 	m_cancel = new JButton( "Cancel");

    public AccountSummary( JFrame owner) {
        super( owner, true);

        setTitle( "Account Summary");

        // create account summary panel
        JPanel accountSummaryPanel = new JPanel( new GridLayout( 0, 2, 3, 3) );
        accountSummaryPanel.add( new JLabel( "Request ID:") );
        accountSummaryPanel.add( m_reqIdTxt);
        accountSummaryPanel.add( new JLabel( "Group Name:") );
        accountSummaryPanel.add( m_groupNameTxt);
        accountSummaryPanel.add( new JLabel( "Tags:") );
        accountSummaryPanel.add( m_tagsTxt);

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
        getContentPane().add( accountSummaryPanel, BorderLayout.CENTER);
        getContentPane().add( buttonPanel, BorderLayout.SOUTH);
        pack();
    }

    void onOk() {
        m_rc = false;

        try {
            m_reqId = Integer.parseInt( m_reqIdTxt.getText());
            m_groupName = m_groupNameTxt.getText();
            m_tags = m_tagsTxt.getText();
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
