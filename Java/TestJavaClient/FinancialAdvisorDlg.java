/* Copyright (C) 2013 Interactive Brokers LLC. All rights reserved.  This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

package TestJavaClient;

import java.awt.BorderLayout;
import java.awt.Dimension;
import java.awt.Frame;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.JButton;
import javax.swing.JDialog;

public class FinancialAdvisorDlg extends JDialog {
    private int DIALOG_WIDTH = 500;
    private int EDITOR_HEIGHT = 240;
    private JButton 	m_ok = new JButton( "OK");
    private JButton 	m_cancel = new JButton( "Cancel");
    private IBTextPanel groupTextEditor = new IBTextPanel("Groups", true) ;
    private IBTextPanel profileTextEditor = new IBTextPanel("Allocation Profiles", true) ;
    private IBTextPanel aliasTextEditor = new IBTextPanel("Aliases", true) ;
    String      groupsXML ;
    String      profilesXML ;
    String      aliasesXML ;
    boolean 	m_rc = false;

    public FinancialAdvisorDlg( Frame owner) {
        super( owner, "Financial Advisor", true);

        IBGridBagPanel editPanel = new IBGridBagPanel();

        editPanel.SetObjectPlacement( groupTextEditor,      0, 0 ) ;
        editPanel.SetObjectPlacement( profileTextEditor,    0, 1 ) ;
        editPanel.SetObjectPlacement( aliasTextEditor,      0, 2 ) ;
        Dimension editPanelSizeDimension =
            new Dimension(DIALOG_WIDTH, 3 * EDITOR_HEIGHT);
        editPanel.setPreferredSize(editPanelSizeDimension) ;

        IBGridBagPanel buttonPanel = new IBGridBagPanel();
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

        //setTitle( "Financial Advisor");
        getContentPane().add( editPanel, BorderLayout.NORTH);
        getContentPane().add( buttonPanel, BorderLayout.CENTER);
        pack();
    }

    void receiveInitialXML(String p_groupsXML, String p_profilesXML, String p_aliasesXML) {
        groupTextEditor.setTextDetabbed(p_groupsXML);
        profileTextEditor.setTextDetabbed(p_profilesXML);
        aliasTextEditor.setTextDetabbed(p_aliasesXML);
    }

    void onOk() {
        m_rc = true;
        groupsXML = groupTextEditor.getText();
        profilesXML = profileTextEditor.getText();
        aliasesXML = aliasTextEditor.getText();
        setVisible( false);
    }

    void onCancel() {
        m_rc = false;
        setVisible( false);
    }
}
