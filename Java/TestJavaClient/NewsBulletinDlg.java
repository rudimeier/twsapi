/* Copyright (C) 2013 Interactive Brokers LLC. All rights reserved.  This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

package TestJavaClient;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.BorderFactory;
import javax.swing.ButtonGroup;
import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JRadioButton;

public class NewsBulletinDlg extends JDialog {
    private IBGridBagPanel        m_subscriptionTypePanel = new IBGridBagPanel();
    private IBGridBagPanel        m_mainPanel = new IBGridBagPanel();

    private JButton 		m_btnSubscribe = new JButton( "Subscribe");
    private JButton 		m_btnUnsubscribe = new JButton( "Unsubscribe");
    private JButton 		m_btnClose = new JButton( "Close");

    private ButtonGroup		m_btnGroup = new ButtonGroup();
    private JRadioButton 	m_btnNewMsgs = new JRadioButton( "receive new messages only.");
    private JRadioButton 	m_btnAllMsgs = new JRadioButton( "receive all the current day's messages and any new messages.");

    public boolean 		m_rc;
    public boolean 		m_subscribe;
    public boolean 		m_allMsgs;

    public NewsBulletinDlg(SampleFrame parent) {
        super(parent, "IB News Bulletin Subscription", true);

        m_btnGroup.add( m_btnNewMsgs);
        m_btnGroup.add( m_btnAllMsgs);
        m_btnNewMsgs.setSelected( true);

        // register button listeners
        m_btnSubscribe.addActionListener( new ActionListener() {
                public void actionPerformed( ActionEvent e) {
                        onSubscribe();
                }
        });
        m_btnUnsubscribe.addActionListener( new ActionListener() {
                public void actionPerformed( ActionEvent e) {
                        onUnSubscribe();
                }
        });
        m_btnClose.addActionListener( new ActionListener() {
                public void actionPerformed( ActionEvent e) {
                        onClose();
                }
        });

        m_subscriptionTypePanel.setBorder( BorderFactory.createLineBorder( Color.BLACK));
        JLabel optionTypeLabel = new JLabel("When subscribing to IB news bulletins you have 2 options:");
        m_subscriptionTypePanel.SetObjectPlacement(optionTypeLabel, 0, 0);
        m_subscriptionTypePanel.SetObjectPlacement(m_btnNewMsgs,    0, 1);
        m_subscriptionTypePanel.SetObjectPlacement(m_btnAllMsgs,    0, 2);

        m_mainPanel.SetObjectPlacement(m_subscriptionTypePanel,     0, 0, 4, 1);
        m_mainPanel.SetObjectPlacement(m_btnSubscribe,              0, 1);
        m_mainPanel.SetObjectPlacement(m_btnUnsubscribe,            1, 1);
        m_mainPanel.SetObjectPlacement(m_btnClose,                  3, 1);

        getContentPane().add(m_mainPanel, BorderLayout.CENTER);
        setSize( 460, 160);
    }

    private void onSubscribe() {
        m_rc = true;
        m_subscribe = true;
        m_allMsgs = m_btnAllMsgs.isSelected();
        setVisible( false);
    }

    private void onUnSubscribe() {
        m_rc = true;
        m_subscribe = false;
        m_allMsgs = false;
        setVisible( false);
    }

    private void onClose() {
        m_rc = false;
        setVisible( false);
    }
}
