/* Copyright (C) 2013 Interactive Brokers LLC. All rights reserved.  This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

package TestJavaClient;

import java.awt.BorderLayout;
import java.awt.Frame;
import java.awt.GridLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JComboBox;
import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JPanel;

public class LogConfigDlg extends JDialog {
    public static int SYSTEM_LOG = 1;
    public static int ERROR_LOG = 2;
    public static int WARN_LOG = 3;
    public static int INFO_LOG = 4;
    public static int DETAIL_LOG = 5;

    JComboBox 	m_cmbServerLogLevels = new JComboBox();
    JButton 	m_ok = new JButton( "OK");
    JButton 	m_cancel = new JButton( "Cancel");
    int 	m_serverLogLevel;
    boolean 	m_rc;

    public LogConfigDlg( Frame owner) {
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
        m_cmbServerLogLevels.addItem("System");
        m_cmbServerLogLevels.addItem("Error");
        m_cmbServerLogLevels.addItem("Warning");
        m_cmbServerLogLevels.addItem("Information");
        m_cmbServerLogLevels.addItem("Detail");

        JPanel midPanel = new JPanel();
        midPanel.setLayout( new GridLayout( 0, 2, 5, 5) );
        midPanel.add( new JLabel( "Log Level :") );
        midPanel.add( m_cmbServerLogLevels);

        // create dlg box
        getContentPane().add( midPanel, BorderLayout.NORTH);
        getContentPane().add( buttonPanel, BorderLayout.SOUTH);
        setTitle( "Log Configuration");
        pack();
    }

    void onOk() {
        // set server log Level
        m_serverLogLevel = m_cmbServerLogLevels.getSelectedIndex() + 1;
        m_rc = true;
        setVisible( false);
    }

    void onCancel() {
        m_rc = false;
        setVisible( false);
    }
}
