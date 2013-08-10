/* Copyright (C) 2013 Interactive Brokers LLC. All rights reserved.  This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

package TestJavaClient;

import java.awt.BorderLayout;
import java.awt.Frame;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JTextField;

public class ConnectDlg extends JDialog {
    public static int LAST_CLIENT_ID = 0;

    JTextField 	m_ipAddress = new JTextField();
    JTextField 	m_port = new JTextField( "7496");
    JTextField 	m_clientId = new JTextField();
    JButton 	m_ok = new JButton( "OK");
    JButton 	m_cancel = new JButton( "Cancel");
    String 	m_retIpAddress;
    int 	m_retPort;
    int 	m_retClientId;
    boolean 	m_rc;

    public ConnectDlg( Frame owner) {
        super( owner, true);

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

        // create mid panel
        JPanel midPanel = new JPanel();
        midPanel.setLayout( new GridLayout( 0, 1, 5, 5) );
        midPanel.add( new JLabel( "IP Address (leave blank for local host)") );
        midPanel.add( m_ipAddress);
        midPanel.add( new JLabel( "Port") );
        midPanel.add( m_port);
        midPanel.add( new JLabel( "Client ID") );
        midPanel.add( m_clientId);
        m_clientId.setText( Integer.toString(LAST_CLIENT_ID) );

        // create dlg box
        getContentPane().add( midPanel, BorderLayout.CENTER);
        getContentPane().add( buttonPanel, BorderLayout.SOUTH);
        setTitle( "Connect");
        pack();
    }

    void onOk() {
        m_rc = false;

        try {
            // set id
            m_retIpAddress = m_ipAddress.getText();
            m_retPort = Integer.parseInt( m_port.getText() );
            m_retClientId = Integer.parseInt( m_clientId.getText() );
            LAST_CLIENT_ID = m_retClientId;
        }
        catch( Exception e) {
            Main.inform( this, "Error - " + e);
            return;
        }

        m_rc = true;
        setVisible( false);
    }

    void onCancel() {
        LAST_CLIENT_ID = Integer.parseInt( m_clientId.getText() );

        m_rc = false;
        setVisible( false);
    }
}
