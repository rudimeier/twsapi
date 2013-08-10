/* Copyright (C) 2013 Interactive Brokers LLC. All rights reserved.  This code is subject to the terms
 * and conditions of the IB API Non-Commercial License or the IB API Commercial License, as applicable. */

package TestJavaClient;

import java.awt.Color;
import java.awt.event.ActionEvent;

import javax.swing.BorderFactory;
import javax.swing.JButton;
import javax.swing.JDialog;
import javax.swing.JLabel;
import javax.swing.JTextField;
import javax.swing.border.Border;
import javax.swing.border.TitledBorder;

public class FAAllocationInfoDlg extends JDialog {
    IBGridBagPanel mainJPanel = new IBGridBagPanel();
    IBGridBagPanel faGroupJPanel = new IBGridBagPanel();
    IBGridBagPanel faProfileJPanel = new IBGridBagPanel();

    JLabel m_groupLabel = new JLabel("Group");
    JLabel m_methodLabel = new JLabel("Method");
    JLabel m_percentageLabel = new JLabel("Percentage");
    JLabel m_profileLabel = new JLabel("Profile");

    JTextField m_groupTextField = new JTextField(20);
    JTextField m_methodTextField = new JTextField(20);
    JTextField m_percentageTextField = new JTextField(20);
    JTextField m_profileTextField = new JTextField(20);

    JButton m_okButton = new JButton("OK");
    JButton m_closeButton = new JButton("Close");

    private OrderDlg m_parent;

    public FAAllocationInfoDlg(OrderDlg dlg) {
        super(dlg, false);
        m_parent = dlg;
        try {
            jbInit();
            pack();
        } catch (Exception ex) {
            ex.printStackTrace();
        }
    }

    void jbInit() throws Exception {
        Color etchedColor = new Color(148, 145, 140);
        Border border1 = BorderFactory.createEtchedBorder(Color.white, etchedColor);
        Border border2 = BorderFactory.createEtchedBorder(Color.white, etchedColor);
        TitledBorder titledBorder1 = new TitledBorder(border1, "Group");
        TitledBorder titledBorder2 = new TitledBorder(border2, "Profile");

        faGroupJPanel.setBorder(titledBorder1);
        faProfileJPanel.setBorder(titledBorder2);

        faGroupJPanel.SetObjectPlacement(m_groupLabel,           0, 0);
        faGroupJPanel.SetObjectPlacement(m_groupTextField,       1, 0);
        faGroupJPanel.SetObjectPlacement(m_methodLabel,          0, 1);
        faGroupJPanel.SetObjectPlacement(m_methodTextField,      1, 1);
        faGroupJPanel.SetObjectPlacement(m_percentageLabel,      0, 2);
        faGroupJPanel.SetObjectPlacement(m_percentageTextField,  1, 2);

        faProfileJPanel.SetObjectPlacement(m_profileLabel,       0, 0);
        faProfileJPanel.SetObjectPlacement(m_profileTextField,   1, 0);

        mainJPanel.SetObjectPlacement(faProfileJPanel, 0, 0, 4, 1);
        mainJPanel.SetObjectPlacement(faGroupJPanel,   0, 1, 4, 1);
        mainJPanel.SetObjectPlacement(m_okButton,      1, 2, 1, 1);
        mainJPanel.SetObjectPlacement(m_closeButton,   2, 2, 1, 1);

        setTitle("FA Allocation Info");
        getContentPane().add(mainJPanel);
        setSize( 600, 300);


        m_okButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(ActionEvent e) {
                onOk();
            }
        });
        m_closeButton.addActionListener(new java.awt.event.ActionListener() {
            public void actionPerformed(ActionEvent e) {
                onClose();
            }
        });
    }

    void onOk() {
        m_parent.faGroup(m_groupTextField.getText().trim());
        m_parent.faMethod(m_methodTextField.getText().trim());
        m_parent.faPercentage(m_percentageTextField.getText().trim());
        m_parent.faProfile(m_profileTextField.getText().trim());
        dispose();
    }

    void onClose() {
        dispose();
    }
}
